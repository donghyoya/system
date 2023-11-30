#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_IP "172.21.0.2"
#define PORT 8081

int sock;
pthread_t receive_thread;

void *receive_messages(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char server_reply[1024];

    while(1) {
        if(recv(sock, server_reply, 1024, 0) < 0) {
            puts("recv failed");
            break;
        }

        puts("Server reply: ");
        puts(server_reply);
        memset(server_reply, 0, sizeof(server_reply)); // 버퍼 초기화
    }

    return 0;
}

int main() {
    struct sockaddr_in server;

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket\n");
    }
    printf("Socket created\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // 서버에 연결 시도
    while (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("connect failed. Error\n");
        sleep(1); // 재시도 전에 잠시 대기
    }

    printf("Connected\n");

    if (pthread_create(&receive_thread, NULL, receive_messages, (void*) &sock) < 0) {
        perror("could not create thread");
        return 1;
    }

    // 메시지 전송
    char message[1024];
    while(1) {
        scanf("%s", message);
        if(send(sock, message, strlen(message), 0) < 0) {
            puts("Send failed");
            return 1;
        }
    }

    pthread_join(receive_thread, NULL);
    close(sock);

    return 0;
}
