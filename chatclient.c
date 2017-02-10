#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    char handle[10];
    char message[500];
    // char quit = "quit";
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[500];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    /*should recieve 'thank you for connecting from server'*/
    bzero(buffer,501);
    n = read(sockfd,buffer,501);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("server> %s\n",buffer);
    bzero(buffer,501);

    /* Get user handle*/
    printf("Client> Please enter your user handle: ");
    bzero(buffer,5000);
    fgets(handle,10,stdin);
    handle[strcspn(handle, "\n")] = 0;


    /*send username to server */
    n = write(sockfd,handle,strlen(handle));
    if (n < 0) 
        error("ERROR writing to socket");
    bzero(buffer,501);

    while(1){
        /*read in from server */
        n = read(sockfd,buffer,501);
        if (n < 0) 
             error("ERROR reading from socket");
        /*if the server has ended the session*/
        if(strcmp(buffer, "quit") == 0){
            printf("The server has ended the chat session, goodbye");
            return 0;
        }
        printf("server> %s\n",buffer);
        bzero(buffer,501);

        /*read in from client*/
        printf("%s> ", handle);
        bzero(buffer,501);
        fgets(message,501,stdin);
        printf("\n");
        //printf("%s",buffer);

        /* check if client wants to end session */
        if(strcmp(message, "\\quit\n") == 0 ){
            write(sockfd,"quit",strlen("quit"));
            break;
        }

        /*send message to server*/
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,501);
        // n = read(sockfd,buffer,501);
        // if (n < 0) 
        //      error("ERROR reading from socket");
        // printf("%s\n",buffer);
        // close(sockfd);
    }
    close(sockfd);
    printf("Thank you for chatting today, goodbye\n");
    return 0;
}