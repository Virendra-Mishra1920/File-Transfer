/*
    argv[0]--->fileName
    argv[1]--->ipadress
    argv[2]--->portno
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd,portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int n;

    char buffer[255];
    if(argc<3)
    {
        fprintf(stderr,"usage %s hostname portno",argv[0]);
        exit(0);

    }

    portno=atoi(argv[2]);
     sockfd=socket(AF_INET,SOCK_STREAM,0);
     if(sockfd<0)
     {
         error("Error opening socket:\n");
        
     }

     server=gethostbyname(argv[1]);
     if(server==NULL)
     {
         fprintf(stderr,"Error,no such host");
     }
     bzero((char*)&serv_addr,sizeof(serv_addr));
     serv_addr.sin_family=AF_INET;

     bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port=htons(portno);

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        error("connection failed:\n");
    }

    bzero(buffer,255);
    FILE *f;

    int words=0;
    char c;
    f=fopen("kp.txt","r");
    while ((c=getc(f))!=EOF)
    {
        fscanf(f,"%s",buffer);
        if(isspace(c)|| c=='\t')
        words++;
    }

    write(sockfd,&words,sizeof(int));
    rewind(f);

    //write characters in the server

    char ch;
    while (ch!=EOF)
    {
        fscanf(f,"%s",buffer);
        write(sockfd,buffer,255);
        ch=fgetc(f);
    }
    
    printf("File sent successfuly.");
    


    
    close(sockfd);

    return 0;
}