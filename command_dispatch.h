#ifndef COMMAND_DISPATCH_H
#define COMMAND_DISPATCH_H
#include "database.h"
#include "server.h"
#include <functional>
class CommandDispatcher{
    std::unordered_map<std::string, std::function<std::string(std::vector<std::string>&, Database &)>>command;
    Server &s;
   public:
   CommandDispatcher(Server &s) : s(s){}
   void setAllCommands(){
      command["SET"] = [&](std::vector<std::string> &tokens, Database &db){
              db.set(tokens);
              return std::string("+OK\r\n");
      };
      command["GET"] = [&](std::vector<std::string> &tokens, Database &db){
              std::string res = db.get(tokens);
              return std::string("$"+std::to_string(res.length())+"\r\n"+res+"\r\n");
      };
      command["DEL"] = [&](std::vector<std::string> &tokens, Database &db){
             return db.isDeletedSuccesfully(tokens) ? std::string(":1\r\n") : std::string(":0\r\n");
      };
      command["EXISTS"] = [&](std::vector<std::string> &tokens, Database &db){
             return  db.isExists(tokens) ? std::string(":1\r\n") : std::string(":0\r\n");
      };
      command["INCR"] = [&](std::vector<std::string> &tokens, Database &db){
              if(tokens.size() > 2 || tokens.size() < 2){
                return std::string("-ERR wrong number of arguments\r\n");
              }
              std::string res = db.incr(tokens);
              return std::string(":"+res+"\r\n");
      };
      command["INCRBY"] = [&](std::vector<std::string> &tokens, Database &db){
              if(tokens.size() < 3){
                  return std::string("-ERR wrong number of arguments\r\n");
              }
              std::string res = db.incr(tokens);
              return std::string(":"+res+"\r\n");
      };
      command["DECR"] = [&](std::vector<std::string> &tokens, Database &db){
              if(tokens.size() > 2 || tokens.size() < 2){
                return std::string("-ERR wrong number of arguments\r\n");
              }
              std::string res = db.decr(tokens);
              return std::string(":"+res+"\r\n");
      };
      command["DECRBY"] = [&](std::vector<std::string> &tokens, Database &db){
                if(tokens.size() < 3){
                  return std::string("-ERR wrong number of arguments\r\n");
                }
              std::string res = db.decr(tokens);
              return std::string(":"+res+"\r\n");
      };
      command["PING"] = [&](std::vector<std::string> &tokens, Database &db){
              if(tokens.size() == 1)
                  return std::string("+PONG\r\n");
              else
              return std::string("$" + std::to_string(tokens[1].size())+"\r\n" +tokens[1]+ "\r\n");
      };
      
   }
   bool isExecuteSuccesfully(std::vector<std::string>&tokens , Database &db, int clientFd){
         if(tokens.empty()) return false;
         std::string cmd = tokens[0];
       std::transform(cmd.begin(), cmd.end(), cmd.begin(),
      [](unsigned char c){ return std::toupper(c); });

      auto it = command.find(cmd);
        if(it == command.end()){
              return false;
         }
        std::string res = it->second(tokens,db);
        s.servSend(res,clientFd);
         return true;
   }

};

#endif