#ifndef COMMAND_DISPATCH_H
#define COMMAND_DISPATCH_H
#include "database.h"
#include "server.h"
#include <functional>
class CommandDispatcher{
    std::unordered_map<std::string, std::function<void(std::vector<std::string>&, Database &)>>command;
    Server &s;
   public:
   CommandDispatcher(Server &s) : s(s){}
   void setAllCommands(){
      command["SET"] = [&](std::vector<std::string> &tokens, Database &db){
              db.set(tokens);
              s.servSend("+OK\r\n");
      };
      command["GET"] = [&](std::vector<std::string> &tokens, Database &db){
              std::string res = db.get(tokens);
              s.servSend("$"+std::to_string(res.length())+"\r\n"+res+"\r\n");
      };
      command["DEL"] = [&](std::vector<std::string> &tokens, Database &db){
              db.isDeletedSuccesfully(tokens) ? s.servSend(":1\r\n") : s.servSend(":0\r\n");
      };
      command["EXISTS"] = [&](std::vector<std::string> &tokens, Database &db){
              db.isExists(tokens) ? s.servSend(":1\r\n") : s.servSend(":0\r\n");
      };
      command["INCR"] = [&](std::vector<std::string> &tokens, Database &db){
              std::string res = db.incr(tokens);
              s.servSend(":"+res+"\r\n");
      };
      command["DECR"] = [&](std::vector<std::string> &tokens, Database &db){
              std::string res = db.decr(tokens);
              s.servSend(":"+res+"\r\n");
      };
      command["PING"] = [&](std::vector<std::string> &tokens, Database &db){
              if(tokens.size() == 1)
                  s.servSend("+PONG\r\n");
              else
              s.servSend("$" + std::to_string(tokens[1].size())+"\r\n" +tokens[1]+ "\r\n");
      };
      
   }
   bool isExecuteSuccesfully(std::vector<std::string>&tokens , Database &db){
         if(tokens.empty()) return false;
         std::string cmd = tokens[0];
       std::transform(cmd.begin(), cmd.end(), cmd.begin(),
      [](unsigned char c){ return std::toupper(c); });

      auto it = command.find(cmd);
        if(it == command.end()){
              return false;
         }
        it->second(tokens,db);
         return true;
   }

};

#endif