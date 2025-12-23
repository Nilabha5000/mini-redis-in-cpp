#include "database.h"

long long Database::currentTimeMs(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}
bool Database::isExpired(std::string &key){
    if(this->db[key].expireAt == -1) return false;
    long long now = currentTimeMs();
    return (now >= this->db[key].expireAt);
}
void Database::set(std::vector<std::string> &tokens){
    //to check tokens array contains right number of arguments or not.
    if(tokens.size() == 3 || tokens.size() == 5){
        //extracting key and value from the tokens array.
        std::string key = tokens[1];
        std::string value = tokens[2];
        //check for if there is a duplicate key
        if(this->db.find(key) != db.end()){
            throw std::string("-ERR duplicate key\r\n");
        }
    
        long long expireAt = -1;
        //check for if token has more than 3 argumants.
        if(tokens.size() > 3){
           std::transform(tokens[3].begin(), tokens[3].end(), tokens[3].begin(),
           [](unsigned char c){ return std::toupper(c); });
           if(tokens[3] != "EX")
                throw std::string("-ERR wrong argument\r\n");
           if(!isNumber(tokens[4]))
                throw std::string("-ERR it is not number for expiration\r\n");
        
            long long ttl = std::stoll(tokens[4]);
            if(ttl <= 0)
               throw std::string("-ERR invalid expire time\r\n");
            
            expireAt = currentTimeMs() + ttl*1000LL;
        }
    
        //set the key value pair
        this->db[key] = {value,expireAt};
   }
   else{
        throw std::string("-ERR wrong number of arguments\r\n");
   }  
}

std::string Database::get(std::vector<std::string> &tokens){
   //to check tokens array contains right number of arguments or not.
    if(tokens.size() != 2){
        throw std::string("-ERR wrong number of arguments\r\n");
    }
   
    std::string key = tokens[1];
    auto it = this->db.find(key);
    if(it != this->db.end() && isExpired(key)){
        this->db.erase(key);
    }
    if(this->db.find(key) == this->db.end())
    {
        throw std::string("$-1\r\n");
    }
    
    return this->db[key].data;

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
    bool res = true;
    if(isExpired(key)){
        res = false;
    }
    this->db.erase(key);
    return res;

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
    bool res = true;
    if(isExpired(key))
       res = false;
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
        this->db[key].data = "1";
        this->db[key].expireAt = -1;
        return this->db[key].data;
    }
    if(isExpired(key)){
        this->db.erase(key);
        return "0";
    }
    //check if the value is not a number then throw value is not an integer or out of range.
    if(!isNumber(this->db[key].data)){
        throw std::string("-ERR value is not an integer or out of range\r\n");
    }
    //increment the value and store back to database i.e hash table
    int val = std::stoi(this->db[key].data);
    val++;
    this->db[key].data = std::to_string(val);

    return this->db[key].data;
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
        this->db[key].data = "0";
        this->db[key].expireAt = -1;
        return this->db[key].data;
    }
    if(isExpired(key)){
        this->db.erase(key);
        return "0";
    }
    //check if the value is not a number then throw value is not an integer or out of range.
    if(!isNumber(this->db[key].data)){
        throw std::string("-ERR value is not an integer or out of range\r\n");
    }
    //converting string to integer.
    int val = std::stoi(this->db[key].data);
    //decrement the value.
    val--;
    //assigning the updated value to that key. 
    this->db[key].data = std::to_string(val);

    return this->db[key].data;
}