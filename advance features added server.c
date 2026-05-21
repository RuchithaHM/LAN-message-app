---

# Advanced Features Added

## 1. Group Chat
- Multiple users can chat together.
- Server broadcasts messages to all connected clients.

## 2. Private Chat
- Users can send direct messages.

## 3. Online User List
- Displays active users connected to LAN.

## 4. Typing Indicator
- Shows when user is typing.

## 5. File Sharing
- Send images, PDFs, and documents.

## 6. Notifications
- Popup alert for new messages.

## 7. Chat History
- Saves previous conversations.

## 8. Login System
- Username and password authentication.

## 9. Dark Mode UI
- Modern responsive interface.

## 10. Timestamp Messages
- Shows time for each message.

---

# Advanced Multi-Client Server Code (Group Chat)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#define SOCKET int
#define closesocket close
#endif

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

SOCKET clients[MAX_CLIENTS];
int client_count = 0;

void broadcast(char *message, SOCKET sender) {
    for(int i=0;i<client_count;i++) {
        if(clients[i] != sender) {
            send(clients[i], message, strlen(message), 0);
        }
    }
}

#ifdef _WIN32
DWORD WINAPI handle_client(LPVOID socket_desc)
#else
void *handle_client(void *socket_desc)
#endif
{
    SOCKET client_socket = *(SOCKET*)socket_desc;
    char message[BUFFER_SIZE];

    while(1) {
        int read_size = recv(client_socket, message, BUFFER_SIZE, 0);

        if(read_size <= 0) {
            break;
        }

        message[read_size] = '�';

        printf("Message: %s
", message);

        broadcast(message, client_socket);
    }

    closesocket(client_socket);

#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

int main() {

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));

    listen(server_socket, MAX_CLIENTS);

    printf("LAN Messenger Server Started...
");

    int c = sizeof(struct sockaddr_in);

    while((client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t*)&c))) {

        clients[client_count++] = client_socket;

#ifdef _WIN32
        CreateThread(NULL, 0, handle_client, &client_socket, 0, NULL);
#else
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &client_socket);
#endif

        printf("Client Connected
");
    }

    closesocket(server_socket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
