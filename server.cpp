#include "server.h"
#include "database.h"
#include "command_dispatch.h"
#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool>running = true;
void handleSigInt(int signo){
        running = false;
}
Server ::Server(){
   this->serverFd = socket(AF_INET,SOCK_STREAM,0);
}

bool Server:: startSuccesfully(){
     // check if the server fd is less than 0
    if(this->serverFd < 0)
    {
        cerr<<"Error to start the server";
        return false;
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
        return false;
    }
    if(listen(this->serverFd,10) < 0){
        cerr<<"Error to listen";
        close(this->serverFd);
         this->serverFd = -1;
        return false;
    }
    
      return true;
}
void clientHandler(int clientFd , void *db , void *cd, Server *server){
    Database *dBase = (Database*)db;
    CommandDispatcher *cmdd = (CommandDispatcher*)cd;
    while(true){
       string str = server->servRecieve(clientFd);
        if(str.length() == 0) {
            cout<<"Client disconnected\n";
            break;
        }
        vector<string>tokens;
        server->decodeResp(tokens,str);
        if(tokens.empty()){
            server->servSend("-ERR cannot decode\r\n",clientFd);
            continue;
        }
        try{
           if(!cmdd->isExecuteSuccesfully(tokens,*dBase,clientFd)){
              server->servSend("-ERR unknown command\r\n",clientFd);
           }
        }
        catch(string e){
            server->servSend(e,clientFd);
        }
    }
    close(clientFd);
}
void Server::servLoop(){
    Database db;
    CommandDispatcher cd(*this);
    cd.setAllCommands();
    vector<thread>threads;
    while(running){
        
        int clientFd = accept(this->serverFd,nullptr, nullptr);
        if(clientFd < 0){
           cerr<<"connection failed\n";
           continue;
        }
        
       threads.emplace_back(thread(clientHandler,clientFd,&db, &cd, this));
    }
    
    for (auto &t : threads) t.join();
}
void Server::decodeResp(vector<string>&tokens , string &encodedStr){
    int i = 0;
    //to check if the encoded string starts with * or not.
    if(encodedStr[i] != '*')
    {
        return;
    }

    string strNumber="";

    //extract the total number of arguments of encoded str.
    while(isdigit(encodedStr[++i])){
        strNumber += encodedStr[i];
    }

    int numberOfTokens = atoi(strNumber.c_str()); //number of arguments
    int k = 0;
    while(k < numberOfTokens && i < encodedStr.length()){
       strNumber.clear();
       // incrementing i by 2 for ignoring /r/n
       i += 2;

       if(encodedStr[i] != '$')
       {
           return;
       }
       //extract the total number of charecter present in a token
       while(isdigit(encodedStr[++i])){
        strNumber += encodedStr[i];
       }
       // incrementing i by 2 for ignoring /r/n
       i += 2;

       string word = "";
       //it tells how many charecters in a word
       int n = atoi(strNumber.c_str());
       int count = 0;
       //extract word form the encoded string
       while(count < n){
           word += encodedStr[i++];
           count++;
       }

       tokens.push_back(word);
       k++;
    }
}
string Server ::servRecieve(int clientFd){
    char buffer[500];
    memset(buffer,0,500);
    ssize_t n = recv(clientFd,buffer,500,0);
    if(n <= 0) return "";
    buffer[n] = '\0';
    string recievedStr = buffer;

    return recievedStr;
}
void Server::servSend(string res, int clientFd){

    if(clientFd < 0){

        cerr<<"response can not be send";
        return;
    }

    ssize_t n = send(clientFd,res.c_str(), res.length(), 0);

    if(n == -1){

        cerr<<"sending failed";
    }
}
Server :: ~Server(){
    if(this->serverFd >= 0) close(this->serverFd);
}
int main(){
    signal(SIGINT,handleSigInt);
    cout<<"This is server running on PORT:" <<PORT<<"\n";
    Server serv;
    if(serv.startSuccesfully()){

        serv.servLoop();
    }
    
    return 0;
}
