/* BSD non-blocking socket example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys/socket.h"
#include "netdb.h"
#include "errno.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"

// extra

#include <lwip/if_api.h>
#include <esp_netif.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/queue.h>
#include <esp_mac.h>
#include <esp_sntp.h>
#include <esp_vfs.h>
#include <esp_vfs_l2tap.h>

#include <iec61850_server.h>
#include <static_model.h>
#include <hal_thread.h>
#include "linked_list.h"
#include "goose_receiver.h"
#include "goose_subscriber.h"

static void initialize_sntp(void)
{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();
}

extern IedModel iedModel;

static int running = 0;

int eventCount = 0;

static IedServer iedServer = NULL;

void sigint_handler(int signalId) {
running = 0;}

static const char * MTAG = "TCP_TEST";

static void connectionHandler(IedServer self, ClientConnection connection, bool connected, void* parameter) {
if (connected) {
ESP_LOGI(MTAG, "Connection opened");}

else {
ESP_LOGI(MTAG, "Connection closed");}
}

int readfunc(float scl_value, int order)
{
    printf("valor recebido = %f\n", scl_value);
    uint64_t timeStamp = Hal_getTimeInMs();
    MmsValue* float_scl_value;
    float_scl_value = MmsValue_newFloat(scl_value);

    switch(order)
    {
    case 0:
    {
        IedServer_lockDataModel(iedServer);
        IedServer_updateQuality(iedServer, IEDMODEL_ANALOGRW_GGIO1_AnOut1_q, QUALITY_VALIDITY_GOOD);
        IedServer_updateUTCTimeAttributeValue(iedServer, IEDMODEL_ANALOGRW_GGIO1_AnOut1_t, timeStamp);
        IedServer_updateAttributeValue(iedServer, IEDMODEL_ANALOGRW_GGIO1_AnOut1_mxVal_f, float_scl_value);
        IedServer_unlockDataModel(iedServer);
        break;
    }
    }

    MmsValue_delete(float_scl_value);
    return 0;
}

// **********************

/**
 * @brief Indicates that the file descriptor represents an invalid (uninitialized or closed) socket
 *
 * Used in the TCP server structure `sock[]` which holds list of active clients we serve.
 */
#define INVALID_SOCK (-1)

/**
 * @brief Time in ms to yield to all tasks when a non-blocking socket would block
 *
 * Non-blocking socket operations are typically executed in a separate task validating
 * the socket status. Whenever the socket returns `EAGAIN` (idle status, i.e. would block)
 * we have to yield to all tasks to prevent lower priority tasks from starving.
 */
#define YIELD_TO_ALL_MS 50

// esp definitions

#define ESP_SERVER_IP       "0.0.0.0"
#define ESP_SERVER_PORT     "5000"

static int sock_num = 0;

/**
 * @brief Utility to log socket errors
 *
 * @param[in] tag Logging tag
 * @param[in] sock Socket number
 * @param[in] err Socket errno
 * @param[in] message Message to print
 */
static void log_socket_error(const char *tag, const int sock, const int err, const char *message)
{
    ESP_LOGE(tag, "[sock=%d]: %s\n"
                  "error=%d: %s", sock, message, err, strerror(err));
}

/**
 * @brief Tries to receive data from specified sockets in a non-blocking way,
 *        i.e. returns immediately if no data.
 *
 * @param[in] tag Logging tag
 * @param[in] sock Socket for reception
 * @param[out] data Data pointer to write the received data
 * @param[in] max_len Maximum size of the allocated space for receiving data
 * @return
 *          >0 : Size of received data
 *          =0 : No data available
 *          -1 : Error occurred during socket read operation
 *          -2 : Socket is not connected, to distinguish between an actual socket error and active disconnection
 */
static int try_receive(const char *tag, const int sock, char * data /*float  data*/, size_t max_len)
{
    int len = recv(sock, data, max_len, 0);
    if (len < 0) {
        if (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;   // Not an error
        }
        if (errno == ENOTCONN) {
            ESP_LOGW(tag, "[sock=%d]: Connection closed", sock);
            return -2;  // Socket has been disconnected
        }
        log_socket_error(tag, sock, errno, "Error occurred during receiving");
        return -1;
    }

    return len;
}

/**
 * @brief Sends the specified data to the socket. This function blocks until all bytes got sent.
 *
 * @param[in] tag Logging tag
 * @param[in] sock Socket to write data
 * @param[in] data Data to be written
 * @param[in] len Length of the data
 * @return
 *          >0 : Size the written data
 *          -1 : Error occurred during socket write operation
 */
static int socket_send(const char *tag, const int sock, /*const char * data*/ float fdata, const size_t len)
{
    int to_write = len;
    char buffer1[sizeof(float)], buffer_send[sizeof(float)];
    memcpy(buffer1, &fdata, sizeof(fdata));
    for (int i = 0; i < sizeof(float); i++)
                    buffer_send[sizeof(float)-1-i] = buffer1[i];


    while (to_write > 0) {
        int written = send(sock, buffer_send + (len - to_write), to_write, 0);
        printf("valor = %f\n", fdata);
        ESP_LOGI(tag, "Enviando...");
        if (written < 0 && errno != EINPROGRESS && errno != EAGAIN && errno != EWOULDBLOCK) {
            log_socket_error(tag, sock, errno, "Error occurred during sending");
            return -1;
        }
        to_write -= written;
    }
    return len;
}


// ************************************************************************************** tcp client **************************************************************************************
/*
static struct addrinfo hints_client = { .ai_socktype = SOCK_STREAM };
static struct addrinfo *address_info_client;
static int sock_client = INVALID_SOCK;
//static char rx_buffer_clt[128];
static float rx_buffer_clt; 
static const char *CTAG = "nonblocking-socket-client";

static int keepalive = 1;
static int keepcnt = 5;
static int keepidle = 30;
static int keepintvl = 60;

static void tcp_client_task(void *pvParameters)
{

    int res = getaddrinfo(ESP_CLIENT_IP, ESP_CLIENT_PORT, &hints_client, &address_info_client);
    if (res != 0 || address_info_client == NULL) {
        ESP_LOGE(CTAG, "couldn't get hostname for `%s` "
                      "getaddrinfo() returns %d, addrinfo=%p", ESP_CLIENT_IP, res, address_info_client);
        goto error;
    }

    // Creating client's socket
    sock_client = socket(address_info_client->ai_family, address_info_client->ai_socktype, address_info_client->ai_protocol);
    if (sock_client < 0) {
        log_socket_error(CTAG, sock_client, errno, "Unable to create socket");
        goto error;
    }
    ESP_LOGI(CTAG, "Socket created, connecting to %s:%s", ESP_CLIENT_IP, ESP_CLIENT_PORT);

    // Marking the socket as non-blocking
    int flags = fcntl(sock_client, F_GETFL);
    if (fcntl(sock_client, F_SETFL, flags | O_NONBLOCK) == -1) {
        log_socket_error(CTAG, sock_client, errno, "Unable to set socket non blocking");
    }
    
    if (connect(sock_client, address_info_client->ai_addr, address_info_client->ai_addrlen) != 0) {
        if (errno == EINPROGRESS) {
            ESP_LOGD(CTAG, "connection in progress");
            fd_set fdset;
            FD_ZERO(&fdset);
            FD_SET(sock_client, &fdset);

            // Connection in progress -> have to wait until the connecting socket is marked as writable, i.e. connection completes
            res = select(sock_client+1, NULL, &fdset, NULL, NULL);
            if (res < 0) {
                log_socket_error(CTAG, sock_client, errno, "Error during connection: select for socket to be writable");
                goto error;
            } else if (res == 0) {
                log_socket_error(CTAG, sock_client, errno, "Connection timeout: select for socket to be writable");
                goto error;
            } else {
                int sockerr;
                socklen_t len = (socklen_t)sizeof(int);
                log_socket_error(CTAG, sock_client, errno, "chegou até aqui");
                setsockopt(sock_client, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(int));
                setsockopt(sock_client, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
                setsockopt(sock_client, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
                setsockopt(sock_client, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));

                if (getsockopt(sock_client, SOL_SOCKET, SO_ERROR, (void*)(&sockerr), &len) < 0) {
                    log_socket_error(CTAG, sock_client, errno, "Error when getting socket error using getsockopt()");
                    goto error;
                }
                if (sockerr) {
                    log_socket_error(CTAG, sock_client, sockerr, "Connection error");
                    goto error;
                }
            }
        } else {
            log_socket_error(CTAG, sock_client, errno, "Socket is unable to connect");
            goto error;
        }
    }
    

    error:
        if (sock_client != INVALID_SOCK) {
            close(sock_client);
        }
        free(address_info_client);
        vTaskDelete(NULL);
}
*/


// ************************************************************************************** tcp server **************************************************************************************

static char rx_buffer_srv[sizeof(float)];
//static float rx_buffer_srv = 0;
static const char *STAG = "nonblocking-socket-server";
static struct addrinfo hints_server = { .ai_socktype = SOCK_STREAM };
static struct addrinfo *address_info_server;
static int listen_sock = INVALID_SOCK;
static int sock_srv[CONFIG_LWIP_MAX_SOCKETS - 3];
static const size_t max_socks = CONFIG_LWIP_MAX_SOCKETS - 3;

/**
 * @brief Returns the string representation of client's address (accepted on this server)
 */
static inline char* get_clients_address(struct sockaddr_storage *source_addr)
{
    static char address_str[128];
    char *res = NULL;
    // Convert ip address to string
    if (source_addr->ss_family == PF_INET) {
        res = inet_ntoa_r(((struct sockaddr_in *)source_addr)->sin_addr, address_str, sizeof(address_str) - 1);
    }
    #ifdef CONFIG_LWIP_IPV6
        else if (source_addr->ss_family == PF_INET6) {
            res = inet6_ntoa_r(((struct sockaddr_in6 *)source_addr)->sin6_addr, address_str, sizeof(address_str) - 1);
        }
    #endif
    if (!res) {
        address_str[0] = '\0'; // Returns empty string if conversion didn't succeed
    }
    return address_str;
}

static void tcp_server_task(void *pvParameters)
{
    
    SemaphoreHandle_t *server_ready = pvParameters;
    
    
    union {
        char chars[sizeof(float)];
        float f;
    } u;
    

    // Prepare a list of file descriptors to hold client's sockets, mark all of them as invalid, i.e. available
    for (int i=0; i<max_socks; ++i) {
        sock_srv[i] = INVALID_SOCK;
    }

    // Translating the hostname or a string representation of an IP to address_info
    int res = getaddrinfo(ESP_SERVER_IP, ESP_SERVER_PORT, &hints_server, &address_info_server);
    if (res != 0 || address_info_server == NULL) {
        ESP_LOGE(STAG, "couldn't get hostname for `%s` "
                      "getaddrinfo() returns %d, addrinfo=%p", ESP_SERVER_IP, res, address_info_server);
        goto error;
    }

    // Creating a listener socket
    listen_sock = socket(address_info_server->ai_family, address_info_server->ai_socktype, address_info_server->ai_protocol);

    if (listen_sock < 0) {
        log_socket_error(STAG, listen_sock, errno, "Unable to create socket");
        goto error;
    }
    ESP_LOGI(STAG, "Listener socket created");

    // Marking the socket as non-blocking
    int flags = fcntl(listen_sock, F_GETFL);
    if (fcntl(listen_sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        log_socket_error(STAG, listen_sock, errno, "Unable to set socket non blocking");
        goto error;
    }
    ESP_LOGI(STAG, "Socket marked as non blocking");

    // Binding socket to the given address
    int err = bind(listen_sock, address_info_server->ai_addr, address_info_server->ai_addrlen);
    if (err != 0) {
        log_socket_error(STAG, listen_sock, errno, "Socket unable to bind");
        goto error;
    }
    ESP_LOGI(STAG, "Socket bound on %s:%s", ESP_SERVER_IP, ESP_SERVER_PORT);

    // Set queue (backlog) of pending connections to one (can be more)
    err = listen(listen_sock, 1);
    if (err != 0) {
        log_socket_error(STAG, listen_sock, errno, "Error occurred during listen");
        goto error;
    }
    ESP_LOGI(STAG, "Socket listening");
    xSemaphoreGive(*server_ready);

    // Main loop for accepting new connections and serving all connected clients
    while (1) {
        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);

        // Find a free socket
        int new_sock_index = 0;
        for (new_sock_index=0; new_sock_index<max_socks; ++new_sock_index) {
            if (sock_srv[new_sock_index] == INVALID_SOCK) {
                break;
            }
        }

        // We accept a new connection only if we have a free socket
        if (new_sock_index < max_socks) {
            // Try to accept a new connections
            sock_srv[new_sock_index] = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);

            if (sock_srv[new_sock_index] < 0) {
                if (errno == EWOULDBLOCK) { // The listener socket did not accepts any connection
                                            // continue to serve open connections and try to accept again upon the next iteration
                    ESP_LOGV(STAG, "No pending connections...");
                } else {
                    log_socket_error(STAG, listen_sock, errno, "Error when accepting connection");
                    goto error;
                }
            } else {
                // We have a new client connected -> print it's address
                ESP_LOGI(STAG, "[sock=%d]: Connection accepted from IP:%s", sock_srv[new_sock_index], get_clients_address(&source_addr));

                // ...and set the client's socket non-blocking
                flags = fcntl(sock_srv[new_sock_index], F_GETFL);
                if (fcntl(sock_srv[new_sock_index], F_SETFL, flags | O_NONBLOCK) == -1) {
                    log_socket_error(STAG, sock_srv[new_sock_index], errno, "Unable to set socket non blocking");
                    goto error;
                }
                ESP_LOGI(STAG, "[sock=%d]: Socket marked as non blocking", sock_srv[new_sock_index]);
            }
        }

        

        // We serve all the connected clients in this loop
        for (int i=0; i<max_socks; ++i) {
            if (sock_srv[i] != INVALID_SOCK) {

                // This is an open socket -> try to serve it
                int len = try_receive(STAG, sock_srv[i], rx_buffer_srv, sizeof(rx_buffer_srv));
                if (len < 0) {
                    // Error occurred within this client's socket -> close and mark invalid
                    ESP_LOGI(STAG, "[sock=%d]: try_receive() returned %d -> closing the socket", sock_srv[i], len);
                    close(sock_srv[i]);
                    sock_srv[i] = INVALID_SOCK;
                } else if (len > 0) {
                    // Received some data -> echo back
                    for (i = 0; i < sizeof(float); i++)
                    u.chars[sizeof(float)-1-i] = rx_buffer_srv[i];
                    float f1 = u.f;

                    //printf("valor 2 = %f\n", f1);
                    //ESP_LOGI(STAG, "[sock=%d]: Received %f", sock_srv[i], sizeof(f1), f1);


                    readfunc( f1, 0);


                    //len = socket_send(STAG, sock_srv[i], rx_buffer_srv, len);
                    /*if (len < 0) {
                        // Error occurred on write to this socket -> close it and mark invalid
                        ESP_LOGI(STAG, "[sock=%d]: socket_send() returned %d -> closing the socket", sock_srv[i], len);
                        close(sock_srv[i]);
                        sock_srv[i] = INVALID_SOCK;
                    } else {
                        // Successfully echoed to this socket
                        ESP_LOGI(STAG, "[sock=%d]: Written %.*f", sock_srv[i], len, rx_buffer_srv);
                    }*/
                }

            } // one client's socket
        } // for all sockets

        // Yield to other tasks
        vTaskDelay(pdMS_TO_TICKS(YIELD_TO_ALL_MS));
    }

    error:
    if (listen_sock != INVALID_SOCK) {
        close(listen_sock);
    }

    /*for (int i=0; i<max_socks; ++i) {
        if (sock_srv[i] != INVALID_SOCK) {
            close(sock_srv[i]);
        }
    }*/

    //free(address_info_server);
    //vTaskDelete(NULL);
}

static void tcp_send(int order, float value)
{
    int len = 0;
    for (int i=0; i<max_socks; ++i) {
        if (sock_srv[i] != INVALID_SOCK) {

            ESP_LOGI(STAG, "Esp sends data to the client...");
            len = socket_send(STAG, sock_srv[i], value, sizeof(value));
            if (len < 0) {
                ESP_LOGE(STAG, "Error occurred during socket_send");
                //goto error;
            }
        }
    }
    ESP_LOGI(STAG, "Written: %.*f", len, value);

    //error:
        //if (listen_sock == INVALID_SOCK) {
        //    close(listen_sock);
        //}
        //free(address_info_client);
        //vTaskDelete(NULL);
}

// ************************************************************************************** 61850 **************************************************************************************



static ControlHandlerResult
controlHandlerForBinaryOutput(ControlAction action, void* parameter, MmsValue* value, bool test) {
    if (test)
    return CONTROL_RESULT_FAILED;
    int mms_val;
    if (MmsValue_getType(value) == MMS_BOOLEAN) {
    mms_val = MmsValue_getBoolean(value);}
    else return CONTROL_RESULT_FAILED;

    return CONTROL_RESULT_OK;
}

static ControlHandlerResult
controlHandlerForInt32Output(ControlAction action, void* parameter, MmsValue* value, bool test) {
    if (test)
    return CONTROL_RESULT_FAILED;
    int mms_val;
    if (MmsValue_getType(value) == MMS_INTEGER) {
    mms_val = MmsValue_toInt32(value);
    }
    else return CONTROL_RESULT_FAILED;

    return CONTROL_RESULT_OK;
}

static ControlHandlerResult
controlHandlerForFloatOutput(ControlAction action, void* parameter, MmsValue* value, bool test) {
    if (test)
    return CONTROL_RESULT_FAILED;
    float mms_val;
    if (MmsValue_getType(value) == MMS_FLOAT) {
    mms_val = MmsValue_toFloat(value);
    }
    else return CONTROL_RESULT_FAILED;

    if (parameter == IEDMODEL_ANALOGRW_GGIO1_AnOut1){
    tcp_send(0, mms_val);
    fflush(stdout);
    }
    return CONTROL_RESULT_OK;
}

static void
goCbEventHandler(MmsGooseControlBlock goCb, int event, void* parameter)
{
    printf("\nAccess to GoCB: %s\n", MmsGooseControlBlock_getName(goCb));
    printf("         GoEna: %i\n", MmsGooseControlBlock_getGoEna(goCb));
}

static void
valor_rec(MmsValue* rec)
{
    if(MmsValue_getType(rec) == MMS_FLOAT){

        //printf("valor float: %.4f", MmsValue_toFloat(rec));
        tcp_send(0, MmsValue_toFloat(rec));
    }
    else if(MmsValue_getType(rec) == MMS_BOOLEAN){
        tcp_send(0, MmsValue_getBoolean(rec));
    }
}

static void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);
    valor_rec(MmsValue_getElement(MmsValue_getElement(values, 0), 0));
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    

    // *********************************************************************************  61850

    ESP_ERROR_CHECK(esp_vfs_l2tap_intf_register(NULL));

    /* Create new server configuration object */
 
    IedServerConfig config = IedServerConfig_create();
    
    
    /* Set buffer size for buffered report control blocks to 200000 bytes */
    
    
    IedServerConfig_setReportBufferSize(config, 200000);
    
    
    /* Set stack compliance to a specific edition of the standard (WARNING: data model has also to be checked for compliance) */
    
    
    IedServerConfig_setEdition(config, IEC_61850_EDITION_2);
    
    
    /* Set the base path for the MMS file services */
    
    
    //IedServerConfig_setFileServiceBasePath(config, "./vmd-filestore/");
    
    
    /* disable MMS file service */
    
    
    IedServerConfig_enableFileService(config, false);
    
    
    /* enable dynamic data set service */
    
    
    IedServerConfig_enableDynamicDataSetService(config, true);
    
    
    /* disable log service */
    
    
    IedServerConfig_enableLogService(config, false);
    
    
    /* set maximum number of clients */
    
    
    IedServerConfig_setMaxMmsConnections(config, 1);
    
    
    /* Create a new IEC 61850 server instance */
    
    
    iedServer = IedServer_createWithConfig(&iedModel, NULL, config);
    
    
    /* configuration object is no longer required */
    
    
    IedServerConfig_destroy(config);
    
    
    /* set the identity values for MMS identify service */
    
    IedServer_setServerIdentity(iedServer, "UFPE", "esp32_61850", "1.5.1");

    /* Install handler for binary operate command */

    /* Install handler for analog operate command */
    IedServer_setControlHandler(iedServer, IEDMODEL_ANALOGRW_GGIO1_AnOut1, (ControlHandler) controlHandlerForFloatOutput, IEDMODEL_ANALOGRW_GGIO1_AnOut1);

 

    IedServer_setConnectionIndicationHandler(iedServer, (IedConnectionIndicationHandler) connectionHandler, NULL);
    IedServer_setGooseInterfaceId(iedServer, "2");
    IedServer_setGoCBHandler(iedServer, goCbEventHandler, NULL);/* MMS server will be instructed to start listening for client connections. */
    
    //GooseReceiver receiver = GooseReceiver_create();
    //GooseReceiver_setInterfaceId(receiver, "2");

    //GooseSubscriber subscriber = GooseSubscriber_create("subANALOGRW/LLN0$GO$gcbAnalogs", NULL);
    //GooseSubscriber subscriber = GooseSubscriber_create("pubANALOGRW/LLN0$GO$gcbAnalogs", NULL);

    //uint8_t dstMac[6] = {0x90, 0x38, 0x0c, 0xb1, 0x70, 0x83};
    //uint8_t dstMac[6] = {0x90, 0x38, 0x0c, 0xb1, 0x70, 0x83};

    //GooseSubscriber_setDstMac(subscriber, dstMac);
    //GooseSubscriber_setAppId(subscriber, 4096);
    //GooseSubscriber_setListener(subscriber, gooseListener, NULL);

    //GooseReceiver_addSubscriber(receiver, subscriber);
    //GooseReceiver_start(receiver);

    IedServer_start(iedServer, 102);

    if (!IedServer_isRunning(iedServer)) {
    printf("Starting server failed (maybe need root permissions or another server is already using the port)! Exit.");
    fflush(stdout);
    IedServer_destroy(iedServer);
    exit(-1);
    }
    
    /* Start GOOSE publishing */ 
    IedServer_enableGoosePublishing(iedServer); 

    running = 1;
    //signal(SIGINT, sigint_handler);

    // *****************************************************************************************************************

    // tcp server

    SemaphoreHandle_t server_ready = xSemaphoreCreateBinary();
    assert(server_ready);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, &server_ready, 5, NULL);
    xSemaphoreTake(server_ready, portMAX_DELAY);
    vSemaphoreDelete(server_ready);


    // tcp client

    //xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);

}
