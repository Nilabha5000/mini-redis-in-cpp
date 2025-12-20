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
        cout<<"mini-redis> ";
        getline(cin,cmd);
          
        if(cmd == "exit")
            break;
        if(cmd != ""){
            vector<string>tokens = tokenize(cmd);
            string encodedStr = encodeResp(tokens);
            if(!isSendSuccesfully(encodedStr)){
                cerr<<"send failed!\n";
            }

            string res = clientRecieve();
            decodeResp(res);

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

bool Client::isSendSuccesfully(string &encodedStr){
    ssize_t n = send(this->clientFd,encodedStr.c_str(), encodedStr.length(),0);

    if(n == -1) return false;
    return true;
}

string Client::clientRecieve(void){
    char resBuffer[4096] = {0};
    memset(resBuffer,0,4096);

    ssize_t n = recv(this->clientFd,resBuffer,4096,0);
    if(n == -1){
        return "";
    }

    string res = resBuffer;

    return res;
}
void Client::decodeResp(string &encodedStr){
    if(encodedStr.empty()){
        cout << "(empty response)\n";
        return;
    }

    string decodedStr = "";

    if(encodedStr[0] == '+' || encodedStr[0] == '-'){

        decodedStr = encodedStr.substr(1,encodedStr.length()-1);
        cout<<decodedStr;
    }
    else if(encodedStr[0] == '$'){
        string sNum = "";
        int i = 1;
        if(encodedStr[i] == '-')
        {
            sNum = "-";
            i++;
        }
        for(; isdigit(encodedStr[i]); ++i){
               sNum += encodedStr[i];
        }
         
        if(sNum == "-1")
        {
            decodedStr = "(nil)\r\n";
            cout<<decodedStr;
            return;
        }

        i +=2;
        int n = stoi(sNum);
        decodedStr = encodedStr.substr(i,n);
        cout<<decodedStr<<"\n";
    }

    else if(encodedStr[0] == ':'){
        string decodedStr = "(integer) ";
        
        for(int i = 1; encodedStr[i] != '\r'; ++i)
            decodedStr += encodedStr[i];
        cout<<decodedStr<<"\r\n";
    
    }
    
}
int main(){
     
    Client c1;

    if(c1.isConnectedSuccesfully()){
        c1.cliLoop();
    }

    return 0;
}