/*
filename server_ipaddress portno

argv[0] filename
argv[1] server_ipaddress
argv[2] portno
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>




// Provides POSFIX Operating System API for  UNIX like  systems
#include<unistd.h>



#include<sys/types.h>
#include<sys/socket.h>
// The <netinet/in. h> header also defines the IN6ADDR_LOOPBACK_INIT macro. This macro must be constant at 
// compile time and can be used to initialize a variable of type struct in6_addr to the IPv6 loopback address.
#include<netinet/in.h>




// The <netdb. h> header defines the hostent structure that includes at least the following 
// members: char *h_name Official name of the host. char **h_aliases A pointer to an array of pointers 
// to alternative host names, terminated by a null pointer.
#include<netdb.h>

void error(const char* msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;

    //hostent= info such as hostname ,ipv4 add etc.
    struct hostent *server;

    char buffer[255];
    if(argc<3){
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening Socket");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "Error, no such host");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("connection failed");
    }


    while (1)
    {
        bzero(buffer, 255);

        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n < 0){
            error("Error on writing");
        }


        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if(n < 0){
            error("error on Reading");
        }
        printf("Server: %s", buffer);

        int i = strncmp("exit", buffer, 4);
        if(i == 0){
            break;
        }
        
    }
    
        close(sockfd);
        return 0;
}

