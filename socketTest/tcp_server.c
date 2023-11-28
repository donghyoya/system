#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8081

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *message = "hello from server";

    printf("Start Running Server..");

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0){
        perror("Scoket failed");
        exit(EXIT_FAILURE);
    }

    // 주소 구조체 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 소켓에 주소 할당
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd,3) < 0){
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    while(1){
        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
            perror("Accept");
            continue;
        }
        
        while(1){
            memset(buffer, 0, 1024);
            int valread = read(new_socket, buffer, 1024);
            if(valread == 0){
                //클라이언트가 연결 닫음
                printf("Client disconnected\n");
                break;
            }
            printf("Message from client: %s\n", buffer);
            send(new_socket, message, strlen(message), 0);
        }

        close(new_socket);
    }


    /*
    for(int i = 0; i< 10; ++i){
        memset(buffer, 0 , 1024);
        read(new_socket, buffer, 1024);
        printf("Message from client: %s\n",buffer);
        send(new_socket, message, strlen(message), 0);

        sleep(1);
    }
    */

    close(server_fd);

    return 0;
    
}