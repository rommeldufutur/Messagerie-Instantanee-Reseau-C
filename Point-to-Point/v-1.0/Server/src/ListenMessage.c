#include "headers.h"

int main(int argc, char *argv[])
{
    FreeConsole();
    AllocConsole();
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    struct sockaddr_in client;

    printf("Initialising Winsock2...\n");
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    if((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    int PORT_Destination = atoi(argv[2]);
    server.sin_port = htons(PORT_Destination);

    // Binding
    if(bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d\n", WSAGetLastError());
        return 1;
    }else
    {
        printf("Binded\n");
    }

    printf("Listening incomming connections..\n");
    listen(s, 3);

    // Accept connections
    int c = sizeof(struct sockaddr_in);
    SOCKET s2 = accept(s, (struct sockaddr *)&client, &c);
    if(s2 != SOCKET_ERROR || INVALID_SOCKET)
    {
        printf("Connection accepted with %s on port %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    }else
    {
        printf("Connection failed with %s on port %d reason : %s\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port), WSAGetLastError());
        Sleep(3000);
        return 1;
    }

    Sleep(1500);
    system("cls");
    unsigned int running = 1;
    while(running == 1)
    {
        printf("Your message: ");
        char out[999];
        scanf("%s", out);

        if(strcmp(out, "quit") == 0)
        {
            return 0;
        }else
        {
            send(s2, out, 998, 0);
            printf("message sent !\n");
        }
    }



    closesocket(s2);
    closesocket(s);
    WSACleanup();

    exit(0);
}