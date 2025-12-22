#ifndef CLIENT_CLI_H
#define CLIENT_CLI_H
#include <cstring>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 6379
#define BULK_STR(str) str[0] == '$'
#define SIMPLE_STR(str) str[0] == '+'
#define INTEGER(str) str[0] == ':'
#define ERROR(str) str[0] == '-'

class Client{
    int clientFd;
    std::vector<std::string>tokenize(std::string &cmd);
    public:
    Client();
    bool isConnectedSuccesfully();
    std::string clientRecieve(void );
    std::string encodeResp(std::vector<std::string>&tokens);
    bool isSendSuccesfully(std::string &encodedStr);
    void decodeResp(std::string &encodeStr);
    void cliLoop(void);
    ~Client();
};
#endif