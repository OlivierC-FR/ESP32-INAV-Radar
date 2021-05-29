![Logo](https://github.com/mistyk/inavradar-ESP32/raw/master/docs/logo.png)

# LoRa based inter UAV communication

INAV-Radar is an addition to the [INAV](https://github.com/iNavFlight/inav) flight control software, it relays information about UAVs in the area to the flight controller for display on the OSD. INAV-Radar does this by using [LoRa](https://en.wikipedia.org/wiki/LoRa) radio to broadcast position, altitude, speed and plane name. It also listens for other UAVs so INAV OSD  can display this information as a HUD.

[![Video](https://github.com/mistyk/inavradar-ESP32/raw/master/docs/video.png)](https://www.youtube.com/watch?v=7ww0YOGN7F0)
[![Video](https://github.com/Pairan/ESP32-INAV-Radar/raw/master/assets/inav-radar-teaser.png)](https://youtu.be/7XJ1eJVgmeg)

## News

RCgroups thread: [INAV-Radar on RCgroups](https://www.rcgroups.com/forums/showthread.php?3304673-iNav-Radar-ESP32-LoRa-modems)

*** 2.0 (2021/05/07)
- New air protocol (single frame type, faster, simpler)
- Fixed a bug at boot where the second ESP32 would have trouble to sync on the first because of the "randomization-if-solo" feature
- Improved the smoothness of the OSD display in INAV, now it gets each node position at 4Hz instead of 1Hz
- Added movement compensation, to correct the position of a moving node
- Added a GCS host type for a ground station (MWP)
- Added basic BlueTooth support for debugging
- Network settings : Same as v1.6 : 4 nodes, cycle time 250ms (4x250ms)
- All nodes must run the same v2.0 firmware, because of the new air protocol

*** 1.6 (2021/01/31)
Improved range, and minor optimizations (require all nodes to have 1.6, because new LoRa settings)

*** 1.5.1 (2020/12/04)
Wait for the host to reply for 9 secondes instead of 4, to account for INAV 2.6 slower boot

*** 1.50 (2020/02/11)
Improved timings for better out-of-range resistance (4 slots of 150ms = total cycle 600ms)
Code cleanup

*** 1.30 (2019/05/18)
Radar logo at boot
Better timings, greatly reduced display latency
Many cosmetic tweaks and fixes
Newest inav 2.2.dev REQUIRED (built 2019/05/18 or newer)

*** 1.20 (2019/05/14)
Better timing for MSP and air packets
5 nodes capable, but locked at 4 nodes for now
Faster rate for MSP messages to improve tracking accuracy
in iNav 2.2, faster display to reduce tracking stuttering
Known issue : sometime the debug page with the timings
reboots the module

*** 1.01 (2019/05/06)
More detailled screens per nodes
Displays the local vbat and mAh. These datas are not yet transmitted to the other nodes.
Pressing the top button during the boot sequence will put the module in "silent" mode (ground-station), it will only receive, and won't transmit, thus freeing a slot. Button must be pressed at least once, between the time the module is plugged and the end of the SCAN progress bar.
No need to update iNav since 1.00, no changes.

*** 1.00 (2019/04/25)
Initial release
Require iNav 2.2-dev, including the latest version for the Hud branch (build date 2019/04/27 or newer)
Cycle time 500ms, slotspacing 125ms, LoRA SF9 bw250, maximum 4 nodes (you + 3 others)

## Index

[Hardware](#hardware)

[Development](#development)

[Testing](#testing)

[Wireing](#Wireing)

[FC settings](#FC-settings)

[Manual flashing ESP](#manual)

[Contact](#contact)

## Hardware

Current development is done using these cheap ESP32 LoRa modules.

There are different variants for 433MHz and 868/915MHz:

[Banggood: ESP32 Lora 868/915MHz (2 Pcs)](http://bit.ly/inav-radar-lora32)

[Banggood: ESP32 Lora 433MHz](http://bit.ly/inav-radar-lora32-433)

Other variants (e.g. Heltec) or without OLED display and different antenna connectors should also work.

Also please keep track of your countries regulations regarding radio transmissions.

## Development

The software is based on two components:
- ESP32 LoRa part is found in this repo.
It's developed using [PlatformIO](https://platformio.org/) plugin for [Atom](https://atom.io/) editor.
- INAV : https://github.com/iNavFlight/inav.

## ESP32 firmware flashing

Your system may needs the driver for the USB UART bridge:
[Windows+MacOS](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)
 or [Alternative MacOS](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)

You will need [Python 3.4 or newer](https://www.python.org/downloads/) installed on your system.

Be sure to check 'Add Python to PATH':

The latest stable esptool.py release can be installed via pip in your command prompt:

Windows:
```
c:\> pip install esptool
```

MacOS:
```
$ pip3 install esptool
```

Download the air-to-air test firmware from the [releases page](https://www.dropbox.com/sh/0740du809y4liqe/AAD-bmQUqqSrfKlIDKxQgIcSa?dl=0)
and extract it. Run this command to flash it onto your ESP32 Lora module (Windows and MacOS):

You may change the --port to match your operating system. If you are using Windows check the [device manager](https://github.com/mistyk/inavradar-ESP32/raw/master/docs/devManager.PNG).

Windows:
```
c:\> cd (your air-to-air directory here)
c:\> esptool.py --port COM11 write_flash -z --flash_mode dio 0x1000 1_bootloader.bin 0x8000 2_default.bin 0xe000 3_bootapp.bin 0x10000 4_firmware.bin 0x291000 5_fs.bin
```

MacOS:
```
$ cd (your air-to-air directory here)
$ esptool.py --port /dev/tty.SLAB_USBtoUART write_flash -z --flash_mode dio 0x1000 1_bootloader.bin 0x8000 2_default.bin 0xe000 3_bootapp.bin 0x10000 4_firmware.bin 0x291000 5_fs.bin

```

The output should look something like this:
![Windows CMD output](https://github.com/mistyk/inavradar-ESP32/raw/master/docs/cmd.PNG)


## Wireing

To connect the ESP32 to the FC:
- wire up +5V and GND
- TX from FC to ESP RX pin 17
- RX from FC to ESP TX pin 23

## FC settings (INAV configurator)

Activate MSP on the corresponding UART, the speed is 115200. This is important as the LoRa-modules need this speed for communicating with the fc at that speed for a usable response on changes.

Enable the crosshair.

The HUD has an entry in the stick menu (OSD->HUD) where you can change this configuration at runtime.

Optional OSD and HUD cli settings:
```
osd_layout 0 2 0 0 V       
osd_layout 0 43 0 0 H      
osd_layout 0 44 0 0 H      
osd_layout 0 45 0 0 H 
set osd_crosshairs_style = TYPE6
set osd_horizon_offset = 0
set osd_camera_uptilt = 0
set osd_camera_fov_h = 135
set osd_camera_fov_v = 85
set osd_hud_margin_h = 1
set osd_hud_margin_v = 3
set osd_hud_homing = ON
set osd_hud_homepoint = ON
set osd_hud_radar_disp = 4
set osd_hud_radar_range_min = 1
set osd_hud_radar_range_max = 4000
```

## Contact

[Facebook Group](https://www.facebook.com/groups/360607501179901/)

[INAV-Radar on RCgroups](https://www.rcgroups.com/forums/showthread.php?3304673-iNav-Radar-ESP32-LoRa-modems)




