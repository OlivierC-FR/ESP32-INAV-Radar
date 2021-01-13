/*
  MSP.cpp

  Copyright (c) 2017, Fabrizio Di Vittorio (fdivitto2013@gmail.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>

#include "MSP.h"


void MSP::begin(Stream & stream, uint32_t timeout)
{
  _stream   = &stream;
  _timeout  = timeout;
}


void MSP::reset()
{
  _stream->flush();
  while (_stream->available() > 0)
    _stream->read();
}

void MSP::send(uint8_t messageID, void * payload, uint8_t size)
{
  _stream->write('$');
  _stream->write('M');
  _stream->write('<');
  _stream->write(size);
  _stream->write(messageID);
  uint8_t checksum = size ^ messageID;
  uint8_t * payloadPtr = (uint8_t*)payload;
  for (uint8_t i = 0; i < size; ++i) {
    uint8_t b = *(payloadPtr++);
    checksum ^= b;
    _stream->write(b);
  }
  _stream->write(checksum);
}

uint8_t MSP::crc8_dvb_s2(uint8_t crc, byte a)
{
    crc ^= a;
    for (int ii = 0; ii < 8; ++ii) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0xD5;
        } else {
            crc = crc << 1;
        }
    }
    return crc;
}

void MSP::send2(uint16_t messageID, void * payload, uint8_t size) // 255 chars max, out of V2 specs
{
  uint8_t _crc = 0;
  uint8_t message[size + 9];
  message[0] = '$';
  message[1] = 'X';
  message[2] = '<';
  message[3] = 0; //flag
  message[4] = messageID; //function
  message[5] = messageID >> 8;
  message[6] = size; //payload size
  message[7] = size >> 8;
  for(uint8_t i = 3; i < 8; i++) {
    _crc = crc8_dvb_s2(_crc, message[i]);
  }
  //Start of Payload
  uint8_t * payloadPtr = (uint8_t*)payload;
  for (uint16_t i = 0; i < size; ++i) {
    message[i + 8] = *(payloadPtr++);
    _crc = crc8_dvb_s2(_crc, message[i + 8]);
  }
  message[size + 8] = _crc;
  _stream->write(message, sizeof(message));
}

// timeout in milliseconds
bool MSP::recv(uint8_t * messageID, void * payload, uint8_t maxSize, uint8_t * recvSize)
{
  uint32_t t0 = millis();

  while (1) {

    // read header
    while (_stream->available() < 6)
      if (millis() - t0 >= _timeout)
        return false;
    char header[3];
    _stream->readBytes((char*)header, 3);

    // check header
    if (header[0] == '$' && header[1] == 'M' && header[2] == '>') {
      // header ok, read payload size
      *recvSize = _stream->read();

      // read message ID (type)
      *messageID = _stream->read();

      uint8_t checksumCalc = *recvSize ^ *messageID;

      // read payload
      uint8_t * payloadPtr = (uint8_t*)payload;
      uint8_t idx = 0;
      while (idx < *recvSize) {
        if (millis() - t0 >= _timeout)
          return false;
        if (_stream->available() > 0) {
          uint8_t b = _stream->read();
          checksumCalc ^= b;
          if (idx < maxSize)
            *(payloadPtr++) = b;
          ++idx;
        }
      }
      // zero remaining bytes if *size < maxSize
      for (; idx < maxSize; ++idx)
        *(payloadPtr++) = 0;

      // read and check checksum
      while (_stream->available() == 0)
        if (millis() - t0 >= _timeout)
          return false;
      uint8_t checksum = _stream->read();
      if (checksumCalc == checksum) {
        return true;
      }

    }
  }

}


bool MSP::recv2(uint16_t * messageID, void * payload, uint8_t maxSize, uint8_t * recvSize)
{
  uint32_t t0 = millis();

  while (1) {

    // read header
    while (_stream->available() < 6)
      if (millis() - t0 >= _timeout)
        return false;
    char header[4];
    _stream->readBytes((char*)header, 4);

    // check header
    if (header[0] == '$' && header[1] == 'X' && header[2] == '>') {

      // read message ID (type)
      *messageID = _stream->read();


    // header ok, read payload size
      *recvSize = _stream->read();



      // read payload
      uint8_t * payloadPtr = (uint8_t*)payload;
      uint8_t idx = 0;
      while (idx < *recvSize) {
        if (millis() - t0 >= _timeout)
          return false;
        if (_stream->available() > 0) {
          uint8_t b = _stream->read();

          if (idx < maxSize)
            *(payloadPtr++) = b;
          ++idx;
        }
      }
      // zero remaining bytes if *size < maxSize
      for (; idx < maxSize; ++idx)
        *(payloadPtr++) = 0;



        return true;



    }
  }

}


// wait for messageID
// recvSize can be NULL
bool MSP::waitFor(uint8_t messageID, void * payload, uint8_t maxSize, uint8_t * recvSize)
{
  uint8_t recvMessageID;
  uint8_t recvSizeValue;
  uint32_t t0 = millis();
  while (millis() - t0 < _timeout)
    if (recv(&recvMessageID, payload, maxSize, (recvSize ? recvSize : &recvSizeValue)) && messageID == recvMessageID)
      return true;

  // timeout
  return false;
}

bool MSP::waitFor2(uint16_t messageID, void * payload, uint8_t maxSize, uint8_t * recvSize)
{
  uint16_t recvMessageID;
  uint8_t recvSizeValue;
  uint32_t t0 = millis();
  while (millis() - t0 < _timeout)
    if (recv2(&recvMessageID, payload, maxSize, (recvSize ? recvSize : &recvSizeValue)) && messageID == recvMessageID)
      return true;

  return false;
}

// send a message and wait for the reply
// recvSize can be NULL
bool MSP::request(uint8_t messageID, void * payload, uint8_t maxSize, uint8_t * recvSize)
{
  send(messageID, NULL, 0);
  return waitFor(messageID, payload, maxSize, recvSize);
}


// send message and wait for ack
bool MSP::command(uint8_t messageID, void * payload, uint8_t size, bool waitACK)
{
  send(messageID, payload, size);

  // ack required
  if (waitACK)
    return waitFor(messageID, NULL, 0);

  return true;
}

bool MSP::command2(uint16_t messageID, void * payload, uint8_t size, bool waitACK)
{
  send2(messageID, payload, size);

  // ack required
  if (waitACK)
    return waitFor2(messageID, NULL, 0);

  return true;
}

// map MSP_MODE_xxx to box ids
// mixed values from cleanflight and inav
static const uint8_t BOXIDS[30] PROGMEM = {
  0,  //  0: MSP_MODE_ARM
  1,  //  1: MSP_MODE_ANGLE
  2,  //  2: MSP_MODE_HORIZON
  3,  //  3: MSP_MODE_NAVALTHOLD (cleanflight BARO)
  5,  //  4: MSP_MODE_MAG
  6,  //  5: MSP_MODE_HEADFREE
  7,  //  6: MSP_MODE_HEADADJ
  8,  //  7: MSP_MODE_CAMSTAB
  10, //  8: MSP_MODE_NAVRTH (cleanflight GPSHOME)
  11, //  9: MSP_MODE_NAVPOSHOLD (cleanflight GPSHOLD)
  12, // 10: MSP_MODE_PASSTHRU
  13, // 11: MSP_MODE_BEEPERON
  15, // 12: MSP_MODE_LEDLOW
  16, // 13: MSP_MODE_LLIGHTS
  19, // 14: MSP_MODE_OSD
  20, // 15: MSP_MODE_TELEMETRY
  21, // 16: MSP_MODE_GTUNE
  22, // 17: MSP_MODE_SONAR
  26, // 18: MSP_MODE_BLACKBOX
  27, // 19: MSP_MODE_FAILSAFE
  28, // 20: MSP_MODE_NAVWP (cleanflight AIRMODE)
  29, // 21: MSP_MODE_AIRMODE (cleanflight DISABLE3DSWITCH)
  30, // 22: MSP_MODE_HOMERESET (cleanflight FPVANGLEMIX)
  31, // 23: MSP_MODE_GCSNAV (cleanflight BLACKBOXERASE)
  32, // 24: MSP_MODE_HEADINGLOCK
  33, // 25: MSP_MODE_SURFACE
  34, // 26: MSP_MODE_FLAPERON
  35, // 27: MSP_MODE_TURNASSIST
  36, // 28: MSP_MODE_NAVLAUNCH
  37, // 29: MSP_MODE_AUTOTRIM
};


// returns active mode (using MSP_STATUS and MSP_BOXIDS messages)
// see MSP_MODE_... for bits inside activeModes
bool MSP::getActiveModes(uint32_t * activeModes)
{
  // request status ex
  msp_status_t status;
  if (request(MSP_STATUS, &status, sizeof(status))) {
    // request permanent ids associated to boxes
    uint8_t ids[sizeof(BOXIDS)];
    uint8_t recvSize;
    if (request(MSP_BOXIDS, ids, sizeof(ids), &recvSize)) {
      // compose activeModes, converting BOXIDS to bit map (setting 1 if related flag in flightModeFlags is set)
      *activeModes = 0;
      for (uint8_t i = 0; i < recvSize; ++i) {
        if (status.flightModeFlags & (1 << i)) {
          for (uint8_t j = 0; j < sizeof(BOXIDS); ++j) {
            if (pgm_read_byte(BOXIDS + j) == ids[i]) {
              *activeModes |= 1 << j;
              break;
            }
          }
        }
      }
      return true;
    }
  }

  return false;
}
