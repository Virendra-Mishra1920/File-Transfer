#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        fprintf(stderr,"please enter the port number\n");
        exit(1);
    }

    int sockfd,newsockfd,portno;
    char buffer[1024];
    int n;

    struct sockaddr_in serv_addr,clint_addr;
    socklen_t clilen;

    //creating socket

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        error("Error to opening socket\n");
    }

    bzero((char*)&serv_addr,sizeof(serv_addr));
    portno=atoi(argv[1]);

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(portno);

    if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        error("Binding failed\n");
    }

    //listen connection

    listen(sockfd,5);
    clilen=sizeof(clilen);

    //accept request

    newsockfd=accept(sockfd,(struct sockaddr*)&clint_addr,&clilen);

    if(newsockfd<0)
    {
        error("Not accepting\n");
    }

    FILE *f;
    int ch=0;
    f=fopen("kp_received.txt","a");
    int words;
    read(newsockfd,&words,sizeof(int));

    while (ch!=words)
    {
        read(newsockfd,buffer,255);
        fprintf(f,"%s ",buffer);
        ch++;
    }

    printf("File recived successfully.");
    


    close(newsockfd);
    close(sockfd);
    return 0;
}
