#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include <armadillo>

#include "bitmanip.h"
#include "spc2img.h"


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    printf("Started listening for client... ");

    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    printf("client connected.\n");

    // No longer need server socket
    closesocket(ListenSocket);

    printf("Listening for commands...\n");


    // Receive until the peer shuts down the connection
    do {

        memset(recvbuf, ' ', DEFAULT_BUFLEN);
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {

            // tokenize received text
            // stripping nullspace from each token
            std::vector<std::string> tokens;
            char* pch = strtok(recvbuf, " ");
            while (pch != nullptr)
            {
                std::string tok = std::string(pch);
                tok.erase(std::find(tok.begin(), tok.end(), '\0'), tok.end());
                tokens.push_back(tok);
                pch = strtok(nullptr, " ");
            }
            tokens.pop_back();

            std::cout << "args = ";
            for (int i = 0; i < tokens.size(); ++i)
            {
                std::cout << tokens.at(i) << " ";
            }
            std::cout << std::endl;

            int cmd_shutdown = tokens.front().compare("shutdown");
            
            if (cmd_shutdown == 0)
            {
                printf("Shutting down...\n");
                break;
            }
            else
            {
                printf("Processing... ");
                std::string fn_spc = tokens.at(0);
                bool build_img = (bool)std::stoi(tokens.at(1));
                bool build_flim = (bool)std::stoi(tokens.at(2));
                int img_x = std::stoi(tokens.at(3));
                int img_y = std::stoi(tokens.at(4));
                std::shared_ptr<spcimage> S = std::make_shared<spcimage>();
                int status = loadspc(fn_spc, S, build_img, build_flim, img_x, img_y);
                printf(" done.\n");
            }
        }
        else if (iResult == 0)
            printf("Closing connection...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("client socket shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}