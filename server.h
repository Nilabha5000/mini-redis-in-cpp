#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <cstring>
#include <unistd.h>
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
   void servSend(std::string &s);
   std::string cliRecieve();
   void start();

};

#endif
