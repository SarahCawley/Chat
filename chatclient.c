/*Sarah Cawley 2/12/2017 Intro to computer networks
# project 1 "create a client/ server chat program using sockets"
# C client code inspired by http://www.linuxhowtos.org/C_C++/socket.htm
*/

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

/* read message from the server
    takes the buffer and the sockfd
    returns 1 if the server has ended the chat session
    returns 0 if server has not ended the chat session
*/
int readFromServer(char buffer[] , int sockfd){
    int n;
    bzero(buffer,501);
    n = read(sockfd,buffer,501);
    if (n < 0) 
         error("ERROR reading from socket");
    /*if the server has ended the session*/
    if(strcmp(buffer, "quit") == 0){
        printf("The server has ended the chat session, goodbye\n");
        return 1;
    }
    printf("server> %s\n",buffer);
    bzero(buffer,501);
    return 0;
}

/* Sends message to server
    Takes in user handle, empty message, buffer, and sockfd
    asks the user for a message to send
    checks if the user would like to quit the chat
    Returns 1 if client wants to end chat
    Returns 0 if client does not want to end chat
*/
int sendToServer(char handle[], char message[], char buffer[], int sockfd){
    int n;
    printf("%s> ", handle);
    bzero(message,501);
    fgets(message,501,stdin);
    //check to see if client want to end chat
    if(strcmp(message, "\\quit\n") == 0 ){
        write(sockfd,"quit",strlen("quit"));
        bzero(message,501);    
        /*send message to server telling it to listen*/
        close(sockfd);
        printf("To open a new chat press enter");
        bzero(buffer,501);
        fgets(buffer,501,stdin);
        bzero(buffer,501);
        return 1;
    }
    else{
        n = write(sockfd,message,strlen(message));
        if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,501);
        return 0;
    } 
}


/* Gets the client user's handle
    takes in the sockfd and empty handle[]
    sends user handle to server
*/
int getHandle(int sockfd, char handle[]){
    int n;
    bzero(handle,10);
    printf("Client> Please enter your user handle: ");

    fgets(handle,10,stdin);
    handle[strcspn(handle, "\n")] = 0;

    //send handle to socket
    n = write(sockfd, handle,strlen(handle));
    //handle[strcspn(handle, "\n")] = 0;
    if (n < 0) 
        error("ERROR writing to socket");
    return 0;
}


/*Created connection with server
    Takes the portnumber, the server pointer and the server address
    I could not get this working as a function becuase I got a segment fault at the bcopy
    When I ran the code inline it worked fine, I ran out of time to trouble shoot and get this working
*/
int openSocket(int portno, struct hostent *server, struct sockaddr_in serv_addr){
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    // server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr=malloc(sizeof(unsigned long));
    //HERE IS WHERE I AM GETTING THE SEGMENT FAULT
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    return sockfd;
}

int main(int argc, char *argv[])
{
    char handle[10];
    char message[500];
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int serverMessage; //message from server
    int quit; //does client want to quit

    //Check if all arguments were sent in on command line
    char buffer[500];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    //create connection
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    printf("the server is: %s\n", server->h_name);
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
    readFromServer(buffer, sockfd);

    /* Get user handle, send to server*/
    getHandle(sockfd, handle);
    printf("%s, please start the chat\n", handle);   

    while(1){
        /*read in from client*/
        quit = sendToServer(handle, message, buffer, sockfd);

        //reconnect back to server 
        if(quit == 1){          
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) 
                error("ERROR opening socket");
            // server = gethostbyname(argv[1]);
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
            readFromServer(buffer, sockfd);
            /* Get user handle, send to server*/
            getHandle(sockfd, handle);
            sendToServer(handle, message, buffer, sockfd);
        } 

        /*read in from server */
        serverMessage = readFromServer(buffer, sockfd);
        //if server has ended the session
        if (serverMessage == 1){
            return 0;
        }   
    }
    close(sockfd);
    return 0;
}
