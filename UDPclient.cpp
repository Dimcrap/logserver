#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


void makeSendMsg(char * inputedbuffer,char * argv[],int &i);


int main(int argc,char * argv[]){

if(argc<3){
    std::cout<<"Usage: "<<argv[0]<<"\"PRIORITY\"[ERROR,WARN,INFO,DEBUG]  \"message\" " <<std::endl;
    return 1;
}

if(argv[1]!="ERROR" || argv[1]!="INFO" || argv[1]!="WARN" || argv[1]!="DEBUG"){
    std::cout<<"onprofit priority "<<argv[1]<<std::endl;
}

int i{0};
char msgbuffer[1024];
makeSendMsg(msgbuffer,argv,i);


int sock=socket(AF_INET ,SOCK_DGRAM , 0);
if(sock < 0){
    std::cerr<<"Socket creation failed \n" ;
    return 1;
}

sockaddr_in server_addr;
server_addr.sin_family=AF_INET;
server_addr.sin_port= htons(8888);
server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

char buffer[1024];

sendto(sock,msgbuffer,i,0,
(struct sockaddr *)&server_addr,sizeof(server_addr));

/*
while(true){
    

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock,&fds);
    struct timeval tv= {1,0};

    if(select(sock+1,&fds,NULL,NULL,&tv) > 0){
        socklen_t server_len= sizeof(server_addr);
        int bytes = recvfrom(sock,buffer,sizeof(buffer)-1,0,
                        (struct sockaddr *)&server_addr,&server_len);

        if(bytes >0){
            buffer[bytes] ='\0';
            std::cout<<"[Server] "<<buffer;
        }

    }else{
        std::cout<<"[NO response from server]\n";
    }
  }*/

  close(sock);
  return 0;

}



void makeSendMsg(char *inputedbuffer,char * argv[],int &i){
    /*int messageLength=strlen(argv[2])
    if(strlen(argv[2])>1024){
        std::cout<<"message truncted"
    }*/

    char buffer[1024];

    for (int j=0;argv[1][j]!='\0';j++){
        buffer[i++]=argv[1][j];
    }

    buffer[i++]='|';

    for(int j=0;argv[2][j]!='\0';j++){
        buffer[i++]=argv[2][j];
    }

    buffer[i]='\0';
    //std::cout<<"original message :"<<buffer<<std::endl;
    strcpy(inputedbuffer,buffer);
    int msglength=strlen(buffer);
    

    if(msglength>1024){
        std::cout<<"warning message is truncated\nfull message length : "<<msglength<<std::endl;
    }

};
