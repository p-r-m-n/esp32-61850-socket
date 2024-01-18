/*
 *  ethernet_bsd.c
 *
 *  Copyright 2013 Michael Zillgith, contributed to the project by Michael Clausen (School of engineering Valais).
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <ifaddrs.h>
//#include <net/if_dl.h>

#include <if_dl.h>
#include <esp_netif.h>
#include "esp_vfs_l2tap.h"
#include "esp_vfs.h"
#include <sys/ioctl.h>
#include <lwip/sockets.h>

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>

#include "lib_memory.h"
#include "hal_ethernet.h"

#define IN_BUFFER_SIZE 1500

char *in_buffer;

struct sEthernetSocket {
    int esp_socket;
    uint8_t *espBuffer;
    int espBufferSize;
};

struct sEthernetHandleSet {
    struct pollfd *handles;
    int nhandles;
};

EthernetHandleSet
EthernetHandleSet_new(void)
{
    EthernetHandleSet result = (EthernetHandleSet) GLOBAL_MALLOC(sizeof(struct sEthernetHandleSet));

    if (result != NULL) {
        result->handles = NULL;
        result->nhandles = 0;
    }
    return result;
}

void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if (self != NULL && sock != NULL) {
        int i = self->nhandles++;
        self->handles = realloc(self->handles, self->nhandles * sizeof(struct pollfd));
            
        self->handles[i].fd = sock->esp_socket;
        self->handles[i].events = POLLIN;
    }
}

void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if ((self != NULL) && (sock != NULL)) {
        unsigned i;
        for (i = 0; i < self->nhandles; i++) {
            if (self->handles[i].fd == sock->esp_socket) {
                memmove(&self->handles[i], &self->handles[i+1], sizeof(struct pollfd) * (self->nhandles - i - 1));
                self->nhandles--;
                return;
            }
        }
    }
}

int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs)
{
    int result;
    
    if ((self != NULL) && (self->nhandles >= 0)) {
        result = poll(self->handles, self->nhandles, timeoutMs);
    }
    else {
       result = -1;
    }
    return result;
}

void
EthernetHandleSet_destroy(EthernetHandleSet self)
{
    if (self->nhandles)
        free(self->handles);

    GLOBAL_FREEMEM(self);
}

void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{       
        
    addr[0] = 0x90;
    addr[1] = 0x38;
    addr[2] = 0x0C;
    addr[3] = 0xB1;
    addr[4] = 0x70;
    addr[5] = 0x83;

}

EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{   
    //int eth_tap_fd = open("/dev/net/tap", 0);
    int eth_tap_fd = open("/dev/net/tap", O_NONBLOCK);
    //uint16_t eth_type_filter = 0x88B8;

    ioctl(eth_tap_fd, L2TAP_S_INTF_DEVICE, "ETH_DEF");
    //ioctl(eth_tap_fd, L2TAP_S_RCV_FILTER, &eth_type_filter);
    
    EthernetSocket ethernetSocket = GLOBAL_CALLOC(1, sizeof(struct sEthernetSocket));

    if (ethernetSocket) {
        ethernetSocket->esp_socket = eth_tap_fd;

        if (ethernetSocket->esp_socket == -1) {
            printf("Error creating esp socket!\n");
            GLOBAL_FREEMEM(ethernetSocket);
            return NULL;
        }

        /* Allocate a buffer for the message reception. */
        ethernetSocket->espBuffer = GLOBAL_CALLOC(1, IN_BUFFER_SIZE);
        if (!ethernetSocket->espBuffer)
        {
            printf("Unable to allocate esp RX buffer!\n");
            GLOBAL_FREEMEM(ethernetSocket);
            return NULL;
        }


    }

    return ethernetSocket;
}

void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType)
{
    uint16_t eth_type_filter = 0x88B8;
    printf("aplicou filtro goose\n");
    ioctl(ethSocket->esp_socket, L2TAP_S_RCV_FILTER, &eth_type_filter);
}

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    int size;
    in_buffer = GLOBAL_CALLOC(bufferSize, sizeof(*in_buffer));
    size = read(self->esp_socket, in_buffer, IN_BUFFER_SIZE);
    
    if(size > 0){
        memcpy(buffer, in_buffer, size);
        free(in_buffer);
        return size;
    }
    free(in_buffer);
    return recvfrom(self->esp_socket, buffer, bufferSize, MSG_DONTWAIT, 0, 0);
}

void
Ethernet_sendPacket(EthernetSocket self, uint8_t* buffer, int packetSize)
{
    close(self->esp_socket);
    int eth_tap_fd = open("/dev/net/tap", 0);
    ioctl(eth_tap_fd, L2TAP_S_INTF_DEVICE, "ETH_DEF");
    /* Just send the packet as it is. */
    int abc = write(self->esp_socket, buffer, packetSize);
    printf("Enviou pacote de tamanho %d\n", abc);
    close(self->esp_socket);
    eth_tap_fd = open("/dev/net/tap", O_NONBLOCK);
    ioctl(eth_tap_fd, L2TAP_S_INTF_DEVICE, "ETH_DEF");
}

void
Ethernet_destroySocket(EthernetSocket self)
{
    /* Close the esp device. */
    close(self->esp_socket);
    

    /* Free all dynamic resources used by the ethernet socket. */
    GLOBAL_FREEMEM(self->espBuffer);
    GLOBAL_FREEMEM(self);
}

bool
Ethernet_isSupported()
{
    return true;
}

