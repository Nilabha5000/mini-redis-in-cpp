#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 6379
class Server{
   int serverFd;
   int clientFd;
   public:
   Server();
   ~Server();
   void servSend(std::string res);
   std::string servRecieve();
   void servLoop();
   void decodeResp(std::vector<std::string>&tokens , std::string &encodeStr);
   bool startSuccesfully();

};

#endif
