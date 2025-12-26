#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <mutex>
struct Value
{
     std::string data;
     long long expireAt;
};

class Database{
    std::mutex m;
    std::unordered_map<std::string, Value>db;
    long long currentTimeMs();
    bool isExpired(std::string &key);
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