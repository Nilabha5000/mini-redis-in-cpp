#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <string>
#include <vector>
class Database{
    std::unordered_map<std::string, std::string>db;
    public:
    
    void set(std::vector<std::string>&tokens);
    std::string get(std::vector<std::string> &tokens);
};
#endif