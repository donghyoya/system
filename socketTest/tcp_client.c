// tcp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_IP "172.20.0.2"
#define PORT 8081

int main(){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client";
    char buffer[1024] = {0};

    int try = 0;

    while(1){
        if(try > 15){
            break;
        }
        //소켓 생성
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            printf("\n Socket creation error \n");
            return -1;
        }

        //서버 주소 설정
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // 서버 IP 주소 설정
        if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0){
            printf("\n Invalid address / address not supported \n");
            return -1;
        }

        // 서버에 연결
        if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
            printf("\nConnection Failed. Error: %s\n", strerror(errno));
            close(sock);
            sleep(2);
            continue;
        }

        // 데이터 전송 및 응답 수신
        for (int i = 0; i < 10; ++i) {
            send(sock, message, strlen(message), 0);
            printf("Hello message sent\n");
            
            memset(buffer, 0, 1024);
            int valread = read(sock, buffer, 1024);
            if (valread == 0) {
                // 서버가 연결을 닫았음
                printf("Server disconnected\n");
                break;
            }
            printf("Message from server: %s\n", buffer);
            sleep(1);
        }

        close(sock);

        try++;
    }

    return 0;
}

