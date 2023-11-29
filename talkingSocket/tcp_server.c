#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8081
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];
int client_count = 0;

void *client_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[1024];

    while((read_size = recv(sock, client_message, 1024, 0)) > 0) {
        // 클라이언트로부터 메시지를 받고 다른 모든 클라이언트에게 전달
        for (int i = 0; i < client_count; i++) {
            if (client_sockets[i] != sock) { // 메시지를 보낸 클라이언트 제외
                send(client_sockets[i], client_message, strlen(client_message), 0);
            }
        }
        memset(client_message, 0, 1024); // 메시지 버퍼 초기화
    }

    if(read_size == 0) {
        printf("Client disconnected\n");
        close(sock);
    } else if(read_size == -1) {
        perror("recv failed");
    }

    return 0;
}

int main() {
    int server_fd, new_socket, c;
    struct sockaddr_in server, client;
    pthread_t thread_id;

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        printf("Could not create socket\n");
    }
    printf("Socket created\n");

    // 소켓 옵션 설정
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // 소켓에 주소 할당
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed");
        return 1;
    }
    printf("bind done\n");

    // Listen
    listen(server_fd, 3);

    // 클라이언트 연결을 기다림
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(server_fd, (struct sockaddr *)&client, (socklen_t*)&c))) {
        printf("Connection accepted\n");

        client_sockets[client_count++] = new_socket;
        if (pthread_create(&thread_id, NULL, client_handler, (void*) &new_socket) < 0) {
            perror("could not create thread");
            return 1;
        }
    }

    if (new_socket < 0) {
        perror("accept failed");
        return 1;
    }

    return 0;
}
