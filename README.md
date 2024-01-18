This repo contains two esp32 projects (GOOSE publisher and subscriber) based on the TCP non-blocking socket template from esp-idf, either receiving data from a TCP socket and publishing it (tcp_socket_61850_pub) or subscribing to the data and sending it through a TCP socket (tcp_socket_61850_sub). They both use a modified version of libiec61850 (https://github.com/mz-automation/libiec61850), which is necessary to create a hardware abstraction layer (HAL) for the esp32. I don't own any code from esp-idf or libiec61850, and all the credits should go to the respectful owners.
