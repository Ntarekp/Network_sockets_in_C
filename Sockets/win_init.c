#include <stdio.h>
#include <winsock2.h> // Header responsible for windows sockets library

#pragma comment(lib, "ws2_32.lib") // to configure library in non MiNGW users


int main(){
    WSADATA d; //Structure for windows socket Api

    if (WSAStartup(MAKEWORD(2,2), &d)){
        printf("Failed to initialize.\n");
        return -1;

    }

    WSACleanup();
    printf("Ok.\n");
    return 0;
}
