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
#include <signal.h>
#define PORT 6379
class Server{
   int serverFd;
   friend void clientHandler(int clientFd , void*db, void *cd , Server *server);
   public:
   Server();
   ~Server();
   void servSend(std::string res,int clientFd);
   std::string servRecieve(int clientFd);
   void servLoop();
   void decodeResp(std::vector<std::string>&tokens , std::string &encodeStr);
   bool startSuccesfully();

};

#endif
