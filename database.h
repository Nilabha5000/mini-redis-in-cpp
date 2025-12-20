#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <string>
#include <vector>
class Database{
    std::unordered_map<std::string, std::string>db;
    bool isNumber(std::string &str);
    public:    
    void set(std::vector<std::string>&tokens);
    std::string get(std::vector<std::string> &tokens);
    bool isDeletedSuccesfully(std::vector<std::string>&tokens);
    bool isExists(std::vector<std::string>&tokens);
    std::string incr(std::vector<std::string>&tokens);
    std::string decr(std::vector<std::string>&tokens);
};
#endif