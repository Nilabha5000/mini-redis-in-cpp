#include "database.h"


void Database::set(std::vector<std::string> &tokens){
    //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 3){
        throw std::string("-ERR wrong number of arguments\r\n");
    }
    //extracting key and value from the tokens array.
    std::string key = tokens[1];
    std::string value = tokens[2];
    //check for if there is a duplicate key
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
//it checks a given string is a number then return true otherwise false.
bool Database::isNumber(std::string &str){
    if(str.empty())
       return false;
    int i = 0;
    //check for negetive if it is negetive then increment i counter to 1.
    if(str[i] == '-')i++;

    for(; i < str.length(); ++i){
        //check if the char is not a digit then return false.
        if(!std::isdigit(str[i]))
           return false;
    }

    return true;
}
std::string Database::incr(std::vector<std::string>&tokens){
     //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 2){
        throw std::string("-ERR wrong number of arguments\r\n");
    }
    //extracting the key from the tokens array.
    std::string key = tokens[1];
    if(this->db.find(key) == this->db.end())
    {
        this->db[key] = "1";
        return this->db[key];
    }
    //check if the value is not a number then throw value is not an integer or out of range.
    if(!isNumber(this->db[key])){
        throw std::string("-ERR value is not an integer or out of range\r\n");
    }
    //increment the value and store back to database i.e hash table
    int val = std::stoi(this->db[key]);
    val++;
    this->db[key] = std::to_string(val);

    return this->db[key];
}

std::string Database::decr(std::vector<std::string>&tokens){
     //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 2){
        throw std::string("-ERR wrong number of arguments\r\n");
    }
    //extracting the key from the tokens array.
    std::string key = tokens[1];
    //check if the key is not present in the hashtable then 
    //store "0" on that key value.
    if(this->db.find(key) == this->db.end())
    {
        this->db[key] = "0";
        return this->db[key];
    }
    
    //check if the value is not a number then throw value is not an integer or out of range.
    if(!isNumber(this->db[key])){
        throw std::string("-ERR value is not an integer or out of range\r\n");
    }
    //converting string to integer.
    int val = std::stoi(this->db[key]);
    //decrement the value.
    val--;
    //assigning the updated value to that key. 
    this->db[key] = std::to_string(val);

    return this->db[key];
}