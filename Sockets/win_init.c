#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600  // Target Windows Vista or later
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

// Link the necessary libraries
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

int main() {
    WSADATA d;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        printf("Failed to initialize.\n");
        return 1;
    }

    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

    if (!adapters) {
        printf("Couldn't allocate %ld bytes for the adapters.\n", asize);
        WSACleanup();
        return -1;
    }

    int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);

    if (r == ERROR_BUFFER_OVERFLOW) {
        printf("GetAdaptersAddress wants %ld bytes.\n", asize);
        free(adapters);
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
        if (!adapters) {
            printf("Failed to allocate memory.\n");
            WSACleanup();
            return -1;
        }
    } else if (r != ERROR_SUCCESS) {
        printf("Error from GetAdaptersAddress: %d\n", r);
        free(adapters);
        WSACleanup();
        return -1;
    }

    PIP_ADAPTER_ADDRESSES adapter = adapters;  // Initialize adapter here

    while (adapter) {
        printf("\nAdapter name: %S\n", adapter->FriendlyName);
        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;

        while (address) {
            printf("\t%s",
                   address->Address.lpSockaddr->sa_family == AF_INET ?
                   "IPv4" : "IPv6");
            char ap[100];
            getnameinfo(address->Address.lpSockaddr,
                        address->Address.iSockaddrLength,
                        ap, sizeof(ap), 0, 0, NI_NUMERICHOST);  // Add missing comma and arguments
            printf("\t%s\n", ap);
            address = address->Next;
        }

        adapter = adapter->Next;  // Correct the loop control
    }

    free(adapters);
    WSACleanup();
    printf("Ok.\n");
    return 0;
}
