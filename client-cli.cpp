#include "client-cli.h"
#include <iostream>
using namespace std;

Client ::Client(){
      this->clientFd = socket(AF_INET,SOCK_STREAM,0);
}
Client :: ~Client(){
      if(this->clientFd >= 0) close(this->clientFd);
}
vector<string> Client ::tokenize(string &cmd){
    if(cmd.empty()){
        return {};
    }
    vector<string>tokens;

      bool isquote = false;
    string word = "";
    int l = 0 , r = cmd.length()-1;
    //for left trim
    for(; cmd[l] == ' '; l++);
    //for right trim
    for(; cmd[r] == ' '; r--);

    for(int i = l; i < r+1; ++i){
        if(cmd[i] == '\"'){
           //check for if it a ending quote
           if(isquote) isquote = false;
           else isquote = true; //for opening qoute
              
        }
        //check if a charecter is a ' ' or not 
        else if(isquote || cmd[i] != ' '){
            word += cmd[i];
        }
        else{
            if(word.length() != 0)
               tokens.push_back(word);
            word = "";
        }
    }

    tokens.push_back(word);

    return tokens;
}
bool Client ::isConnectedSuccesfully(){
    if(this->clientFd < 0)
       return false;
    struct sockaddr_in address;
    address.sin_family = AF_INET; 
    address.sin_port = htons(PORT);
    inet_pton(AF_INET,"127.0.0.1", &address.sin_addr.s_addr);
    int result = connect(this->clientFd,(struct sockaddr*)&address, sizeof(address));
     
    if(result < 0){
        return false;
    }

    return true;
}


void Client::cliLoop(){

    string cmd;
    while(true){
        cout<<"> ";
        getline(cin,cmd);
          
        if(cmd == "exit")
            break;
        if(cmd != ""){
            vector<string>tokens = tokenize(cmd);
            string encodedStr = encodeResp(tokens);
            ssize_t n = send(this->clientFd,encodedStr.c_str(), encodedStr.length(),0);

            if(n == -1) cerr<<"send failed";
        }

        

        cmd.clear();
    }

}

string Client::encodeResp(vector<string>&tokens){
       string encodedStr = "*" + to_string(tokens.size()) + "\r\n";

       for(auto token : tokens){
           encodedStr += "$"+to_string(token.length())+"\r\n";
           encodedStr += token;
           encodedStr += "\r\n";
       }

       return encodedStr;
}
int main(){
     
    Client c1;

    if(c1.isConnectedSuccesfully()){
        c1.cliLoop();
    }

    return 0;
}