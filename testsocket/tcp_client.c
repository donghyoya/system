// tcp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8081

int main(){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client";
    char buffer[1024] = {0};

    while(1){

        //소켓 생성
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            printf("\n Socket creation error \n");
            return -1;
        }

        //서버 주소 설정
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // 서버 IP 주소 설정
        if(inet_pton(AF_INET, "tcp_server", &serv_addr.sin_addr) <= 0){
            printf("\n Invalid address / address not supported \n");
            return -1;
        }

        // 서버에 연결
        if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
            printf("\nConnection Failed. Error: %s\n", strerror(errno));
            close(sock);
            sleep(1);
            continue;
        }

        // 데이터 전송 및 응답 수신
        send(sock, message, strlen(message), 0);
        printf("Hello message sent\n");
        read(sock, buffer, 1024);
        printf("Message from server: %s\n", buffer);

        close(sock);
        break;
    }

    return 0;
}

