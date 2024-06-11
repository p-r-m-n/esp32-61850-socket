# README esp32-61850-socket

This repo contains two esp32 projects, each working as an intelligent electronic device (IED server with GOOSE publisher or subscriber) based on the TCP non-blocking socket template from esp-idf, either receiving data from a TCP socket and publishing it (tcp_socket_61850_pub) or subscribing to the data and sending it through a TCP socket (tcp_socket_61850_sub). They both use a modified version (already included in each project) of libiec61850 (https://github.com/mz-automation/libiec61850), which is necessary to create a hardware abstraction layer (HAL) for the esp32. All credits should be given to the respective authors.

Each folder is an ESP-IDF project (version 5.1.2), created with VSCODE and Espressif extension for VSCODE. As the GOOSE protocol of IEC 61850 has some time requirements regarding the packets, i created the HAL to send or receive these packets through ethernet only. So, each project should be flashed on a esp32 with ethernet (ESP32-Ethernet-Kit, WT32-ETH01, T-ETH-Lite, etc..). Since each board has its own ethernet pin config, you should edit the project "sdkconfig" (through menuconfig or editing directly inside the project folder) to match your ethernet pin configuration.

![alt text](https://github.com/p-r-m-n/esp32-61850-socket/blob/main/images/application.jpg?raw=true)
