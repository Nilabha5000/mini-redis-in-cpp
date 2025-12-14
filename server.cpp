#include "server.h"
#include <iostream>
using namespace std;
Server ::Server(){
   this->clientFd = -1;
   this->serverFd = socket(AF_INET,SOCK_STREAM,0);
    
}

void Server::start(){
     // check if the server fd is less than 0
      if(this->serverFd < 0)
      {
          cerr<<"Error to start the server";
          return;
      }
     struct sockaddr_in address;
     struct sockaddr_in cliAddress;
     memset(&address, 0, sizeof(address));
     memset(&cliAddress , 0 , sizeof(cliAddress));
     address.sin_family = AF_INET;
     address.sin_port = htons(PORT);
     address.sin_addr.s_addr = INADDR_ANY;
     socklen_t addrlen = sizeof(address);
    if(bind(this->serverFd, (struct sockaddr*) &address, sizeof(address)) < 0){
        cerr<<"Error to bind";
        close(this->serverFd);
        this->serverFd = -1;
        return;
    }
    if(listen(this->serverFd,10) < 0){
        cerr<<"Error to listen";
        close(this->serverFd);
         this->serverFd = -1;
        return;
    }
    socklen_t cliAddrLen = sizeof(cliAddress);
    this->clientFd = accept(this->serverFd,(struct sockaddr*)&address, &addrlen);
        if(this->clientFd < 0){
           cerr<<"connection failed\n";
           return;
        } 
    while(true){
        

        string str = cliRecieve();
        if(str.length() == 0) {
            cerr<<"Client diconnected\n";
            break;
        }
        cout<<str<<"\n";
    }

}
string Server ::cliRecieve(){
    char buffer[500];
    memset(buffer,0,500);
    ssize_t n = recv(this->clientFd,buffer,500,0);
    if(n <= 0) return "";
    buffer[n] = '\0';
    string recievedStr = buffer;

    return recievedStr;
}
Server :: ~Server(){
    if(this->clientFd >= 0) close(this->clientFd);
    if(this->serverFd >= 0) close(this->serverFd);
}

int main(){
    cout<<"this is server is running on PORT:" <<PORT<<"\n";
    Server serv;
    serv.start();
    
    return 0;
}