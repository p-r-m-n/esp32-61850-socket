# README - esp32-61850-socket

This repository contains two ESP32 projects, each functioning as an Intelligent Electronic Device (IED) with support for the GOOSE protocol from the IEC 61850 standard. Both are based on a non-blocking TCP socket model from ESP-IDF and can be opened, configured, compiled, and flashed onto a microcontroller using VSCode or VSCodium with the Espressif extension.

## Project Overview

### 1. **tcp_socket_61850_pub**
This project receives data from a TCP socket and publishes it via the GOOSE protocol.

### 2. **tcp_socket_61850_sub**
This project subscribes to data from the GOOSE protocol and forwards it via a TCP socket.

Both projects use a modified version of the [libiec61850](https://github.com/mz-automation/libiec61850) library (already included in each project), which is necessary to create a hardware abstraction layer (HAL) for the ESP32. Full credit for the original library should be given to the respective authors.

## Setup and Requirements

- **Platform:** ESP-IDF (version 5.1.2)
- **Recommended IDE:** VSCode or VSCodium with the Espressif extension
- **Compatible Hardware:** ESP32 boards with Ethernet support (e.g., ESP32-Ethernet-Kit, WT32-ETH01, T-ETH-Lite)
- **Ethernet Configuration:**
  - Due to the time constraints of the GOOSE protocol, communication is exclusively performed via Ethernet.
  - Each board has a specific Ethernet pin configuration.
  - The `sdkconfig` file must be modified to match the appropriate Ethernet pin configuration.
  - Configuration can be adjusted using `menuconfig` or by directly editing the file within the project folder.

## Application Example

The image below illustrates an example application of the system:

![Application Example](https://github.com/p-r-m-n/esp32-61850-socket/blob/main/images/application.png?raw=true)

---

This repository aims to facilitate the implementation of solutions based on the IEC 61850 protocol using ESP32. For questions or suggestions, feel free to open an issue!

