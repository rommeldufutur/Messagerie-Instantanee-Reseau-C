#include "headers.h"

int main(int argc, char *argv[])
{
    FreeConsole();
    AllocConsole();

    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    Initialising:
    printf("Initialising Winsock2...\n");
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(1);
    }
    printf("Initialised.\n");

    if((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        exit(1);
    }
    printf("Socket created.\n");

    char IP_Destination[17];
    strcpy(IP_Destination, argv[1]);
    printf("%s ", IP_Destination);

    int PORT_Destination = atoi(argv[2]);
    printf("%d\n", PORT_Destination);

    printf("Connecting to server...\n");
    Sleep(2000);

    server.sin_addr.s_addr = inet_addr(IP_Destination);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_Destination);

    // Connection to server
    if(connect(s, (struct sockaddr *)&server, sizeof(server)) != SOCKET_ERROR)
    {
        printf("Connected\n");
    }else
    {
        printf("Could not connect to server: %d\n", WSAGetLastError());
        Sleep(3000);
        exit(1);
    }


    Sleep(1500);
    system("cls");
    unsigned int running = 1;
    while(running == 1)
    {
        char in[999];
        if(recv(s, in, 998, 0) != SOCKET_ERROR)
        {
            printf("Message from the server: %s\n", in);
        }
    }

    exit(0);
}