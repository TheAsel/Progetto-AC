#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

#define BUF_SIZE 10
#define IP "127.0.0.1"



int main(int n_args, char** args){
    int porta;
    int ret;
    int sd;
    char buffer[BUF_SIZE];
    socklen_t len;
    struct sockaddr_in server;
    fd_set MASTER, READY;
    int max_fd;

    if(n_args != 2){
        printf("Errore: numero di porta non inserito\n");
        exit(0);
    }
    ret = sscanf(args[1],"%d",&porta); // OVERFLOW?
    if(!ret){
        printf("Numero di porta non valido\n");
        exit(0);
    }

    sd = socket(AF_INET,SOCK_STREAM,0);
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(porta);
    inet_pton(AF_INET,IP,&server.sin_addr);
    ret = connect(sd,(struct sockaddr*)&server,sizeof(server));
    if(ret == -1){
        perror("Errore:");
        exit(0);
    }
    FD_ZERO(&MASTER);
    FD_ZERO(&READY);
    FD_SET(STDIN_FILENO,&MASTER);
    FD_SET(sd,&MASTER);

    max_fd = sd;

    while(1){
        READY = MASTER;
        select(max_fd+1, &READY, NULL,NULL,NULL);
        for(int i=0; i <=max_fd; i++){
            if(!FD_ISSET(i,&READY))
                continue;
            else if(i == STDIN_FILENO){
                /* 
                fgets(buffer,BUF_SIZE,stdin);
                printf("Letto e invio: %s",buffer);
                send(sd,buffer,BUF_SIZE,0); // gestione errore se non manda tutti i byte? 
                */
                
            }
            else if(i == sd){
                /* 
                ret = recv(i,buffer,BUF_SIZE,0);
                if(!ret){
                    close(i);
                    FD_CLR(i,&MASTER);
                    exit(0); 
                }
                printf("RICEVUTO: %s",buffer);
                */
            }
        }
    }
    close(sd);
}