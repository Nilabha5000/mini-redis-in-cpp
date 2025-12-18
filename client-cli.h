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
class Client{
    int clientFd;
    std::vector<std::string>tokenize(std::string &cmd);
    public:
    Client();
    bool isConnectedSuccesfully();
    std::string clientRecieve(void );
    std::string encodeResp(std::vector<std::string>&tokens);
    bool isSendSuccesfully(std::string &encodedStr);

    void cliLoop(void);
    ~Client();
};
#endif