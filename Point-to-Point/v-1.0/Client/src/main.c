#include "headers.h"

int main(int argc, char **argv)
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    printf("How does this script works ?\n");
    printf("This script is used to send/receive messages from a computer to another in the same LAN or WAN such as you're business LAN or home LAN and the Internet WAN.\n");
    printf("All the data is not uncrypted, it's clear text.\n");
    printf("All you need to do is: enter the Server's IP and PORT (by default the port is \"8888\").\n");
    printf("\nthe script that you are using is a client socket, so you do not need to install any server! But the other computer need to use the ServerSocket.exe and open its ports (such as port 8888)\n");

    choose:
    printf("\nStill runing this client script ?\n");
    printf("y/n: ");
    char answer[2];
    scanf("%1s", answer);
    if(strcmp(answer, "y") == 0)
    {
        printf("You chose \"yes\", so let's do it!\n");
        Sleep(2000);
    }else if(strcmp(answer, "n") == 0)
    {
        printf("See you !\n");
        Sleep(1000);
        return 0;
    }else
    {
        printf("please try again..\n");
        goto choose;
    }

    system("cls");

    Initialising:
    printf("Initialising Winsock2...\n");
    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    if((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    printf("what's the destination ?:\n");
    char IP_Destination[17];
    scanf("%16s", IP_Destination);
    printf("The destination's ip will be: %s\nwhat's the server's port ?:\n", IP_Destination);
    int PORT_Destination;
    scanf("%d", &PORT_Destination);
    char Dest_PORT[10];
    sprintf(Dest_PORT, "%d", PORT_Destination);
    printf("The destination's port will be: %s\n", Dest_PORT);

    printf("Connecting to server...\n");
    Sleep(2000);

    server.sin_addr.s_addr = inet_addr(IP_Destination);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_Destination);

    // Connection to server and sending/receiving data
    if(connect(s, (struct sockaddr *)&server, sizeof(server)) != SOCKET_ERROR)
    {
        printf("Connected\n");
        // wait for the server's welcome message
        char serverWelcome[999];
        recv(s, serverWelcome, 998, 0);
        printf("%s\n", serverWelcome);
    }else
    {
        printf("Could not connect to server: %d\n", WSAGetLastError());
        return 1;
    }

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
        printf("Your message: ");
        char out[999];
        scanf("%s", out);
        if(strcmp(out, "quit") == 0)
        {
            return 0;
        }else
        {
            send(s, out, 998, 0);
            printf("Message sent !\n");
        }
    }

    return 0;
}