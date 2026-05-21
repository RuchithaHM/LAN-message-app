#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define closesocket close
#endif

#define PORT 8080
#define BUFFER_SIZE 4096

void send_response(SOCKET client_socket, const char *response) {
    send(client_socket, response, strlen(response), 0);
}

int main() {

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == -1) {
        printf("Socket creation failed\n");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Bind failed\n");
        return 1;
    }

    listen(server_fd, 5);

    printf("Server started at http://localhost:8080\n");

    while(1) {

        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        recv(client_socket, buffer, BUFFER_SIZE, 0);

        const char *html =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html>"
            "<html>"
            "<head>"
            "<title>LAN Messenger</title>"
            "<style>"
            "body{font-family:Arial;background:#0f172a;color:white;display:flex;justify-content:center;align-items:center;height:100vh;}"
            ".chat{width:400px;background:#1e293b;padding:20px;border-radius:20px;}"
            ".messages{height:300px;background:#111827;padding:10px;border-radius:10px;overflow:auto;margin-bottom:10px;}"
            "input{width:70%;padding:10px;border:none;border-radius:10px;}"
            "button{padding:10px 20px;border:none;border-radius:10px;background:#3b82f6;color:white;}"
            "</style>"
            "</head>"
            "<body>"
            "<div class='chat'>"
            "<h2>LAN Messenger</h2>"
            "<div class='messages' id='messages'></div>"
            "<input type='text' id='msg' placeholder='Type message'>"
            "<button onclick='sendMessage()'>Send</button>"
            "</div>"
            "<script>"
            "function sendMessage(){"
            "let msg=document.getElementById('msg').value;"
            "let div=document.getElementById('messages');"
            "div.innerHTML += '<p><b>You:</b> '+msg+'</p>';"
            "document.getElementById('msg').value='';"
            "}"
            "</script>"
            "</body>"
            "</html>";

        send_response(client_socket, html);

        memset(buffer, 0, BUFFER_SIZE);

        closesocket(client_socket);
    }

    closesocket(server_fd);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
