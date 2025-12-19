#include "database.h"


void Database::set(std::vector<std::string> &tokens){
    //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 3){
        throw std::string("-ERR wrong number of arguments\r\n");
    }

    std::string key = tokens[1];
    std::string value = tokens[2];
    //check if there is a duplicate key
    if(this->db.find(key) != db.end()){
        throw std::string("-ERR duplicate key\r\n");
    }
    //set the key value pair
    this->db[key] = value;
}

std::string Database::get(std::vector<std::string> &tokens){
   //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 2){
        throw std::string("-ERR wrong number of arguments\r\n");
    }
   
    std::string key = tokens[1];
    if(this->db.find(key) == this->db.end())
    {
        throw std::string("$-1\r\n");
    }

    return this->db[key];

}

bool Database::isDeletedSuccesfully(std::vector<std::string> &tokens){
   //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 2){
        throw std::string("-ERR wrong number of arguments\r\n");
    }
   
    std::string key = tokens[1];
    if(this->db.find(key) == this->db.end())
    {
        return false;
    }
    this->db.erase(key);
    return true;

}

bool Database::isExists(std::vector<std::string> &tokens){
   //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 2){
        throw std::string("-ERR wrong number of arguments\r\n");
    }
   
    std::string key = tokens[1];
    if(this->db.find(key) == this->db.end())
    {
        return false;
    }
    return true;

}