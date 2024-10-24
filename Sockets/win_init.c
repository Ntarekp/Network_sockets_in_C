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

    //Allocate memory to adapters and request adapters addresses from windows using GetAdapterAdresses() function

    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);

    if (!adapters) {
        printf("Couldn't allocate %ld bytes for the adapters.\n", asize);
        WSACleanup();
        return -1;
    }

    // Get network adapter information
    int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);

    if (r == ERROR_BUFFER_OVERFLOW) {
        // Reallocate memory if buffer is too small
        printf("GetAdaptersAddress wants %ld bytes.\n", asize);
        free(adapters);
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
        if (!adapters) {
            printf("Failed to allocate memory.\n");
            WSACleanup();
            return -1;
        }
    } else if (r == ERROR_SUCCESS) {
        // Success, process the adapters here
        while (adapters) {
            // Loop through the adapter list and print information
            // Example:
            // printf("Adapter: %s\n", adapters->FriendlyName);
            adapters = adapters->Next;
        }
    } else {
        // Handle error
        printf("Error from GetAdaptersAddress: %d\n", r);
        free(adapters);
        WSACleanup();
        return -1;
    }

    // Cleanup
    WSACleanup();
    printf("Ok.\n");
    return 0;
}
