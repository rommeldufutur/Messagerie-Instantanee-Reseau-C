#include "headers.h"

int main(int argc, char **argv)
{
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

    printf("What's your ip address ?\n");
    char IP_Destination[17];
    scanf("%s", IP_Destination);
    printf("your IP is: %s\n", IP_Destination);

    printf("What's the port you will use ? (default choose 8888)\n");
    int PORT_Destination;
    scanf("%d", &PORT_Destination);
    char Dest_PORT[10];
    sprintf(Dest_PORT, "%d", PORT_Destination);
    printf("The server's port will be: %s\n", Dest_PORT);

    server.sin_addr.s_addr = inet_addr(IP_Destination);
    server.sin_family = AF_INET;
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
    }

    //Reply a welcome message to the client to tell him that he is connected to the server
    char message[999] = "\n\nYou are connected to the Server, this message is from the Server\n\n";
    send(s2, message, 998, 0);

    // creating a process to listen if their is a message comming in
    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = { 0 };

    char Command[100] = "ListenMessage.exe ";
    strcat(Command, IP_Destination);
    strcat(Command, " ");
    strcat(Command, "8889");


    bool success;
    CreateProcess(NULL, Command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    /*if(success)
    {

        // wait for process exit
        WaitForSingleObject(pi.hProcess, INFINITE);

        // get the exit code of process
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);

        // Closing all the handles that the child process required
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }else
    {
        printf("failed while creating the process for messages: %d\n", GetLastError());
        Sleep(3000);
        return 1;
    }*/

    Sleep(1500);
    system("cls");

    unsigned int running = 1;
    while(running == 1)
    {
        char in[999];
        if(recv(s2, in, 998, 0) != SOCKET_ERROR)
        {
            printf("Message from the client: %s\n", in);
        }
    }

    closesocket(s2);
    closesocket(s);
    WSACleanup();

    return 0;
}