#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include <string>

#define CONTAINS_FIELD(x) ([](credential cred){ return containsKey(std::string { x }, cred); })
using credential = std::map<std::string, std::string>;
using fieldVerifier = std::function<bool(credential)>;

template<typename T, typename S>
bool containsKey(T& key, std::map<T, S>& map) {
    auto find { map.find(key) };
    auto end { map.end() };
    bool result { find != end };
    return result;
}

const std::vector<fieldVerifier> defaultKeyList = { 
    CONTAINS_FIELD("byr"),
    CONTAINS_FIELD("iyr"), 
    CONTAINS_FIELD("eyr"), 
    CONTAINS_FIELD("hgt"), 
    CONTAINS_FIELD("hcl"), 
    CONTAINS_FIELD("ecl"), 
    CONTAINS_FIELD("pid")
    /*it took an hour to realize this shouldn't be here: , "cid"*/
};

const std::vector<fieldVerifier> part2KeyList {
    [](credential cred){
        std::string key {"byr"};
        if(!containsKey(key, cred))
            return false;
        try {
            int value { std::stoi(cred[key]) };
            return value >= 1920 && value <= 2002;
        } catch (...) {
            return false;
        }
    },
    [](credential cred){
        std::string key {"iyr"};
        if(!containsKey(key, cred))
            return false;
        try {
            int value { std::stoi(cred[key]) };
            return value >= 2010 && value <= 2020;
        } catch(...) {
            return false;
        }
    },
    [](credential cred){
        std::string key {"eyr"};
        if(!containsKey(key, cred))
            return false;
        try {
            int value { std::stoi(cred[key]) };
            return value >= 2020 && value <= 2030;
        } catch(...) {
            return false;
        }
    },
    [](credential cred){
        std::string key {"hgt"};
        if(!containsKey(key, cred))
            return false;
        try {
            std::string suffix { cred[key].substr(cred[key].length() - 2) };
            if(suffix == "cm") {
                int value { std::stoi(cred[key].substr(0, cred[key].length() - 2)) };
                return value >= 150 && value <= 193;
            } else if(suffix == "in") {
                int value { std::stoi(cred[key].substr(0, cred[key].length() - 2)) };
                return value >= 59 && value <= 76;
            }
        } catch (...) { }
        return false;
    },
    [](credential cred){
        std::string key {"hcl"};
        if(!containsKey(key, cred))
            return false;
        try {
            if(cred[key].length() != 7 || cred[key].at(0) != '#')
                return false;

            // if it errors, will return false
            std::stoi(cred[key].substr(1), 0, 16);
            return true;
        } catch (...) {
            return false;
        }
    },
    [](credential cred){
        std::string key {"ecl"};
        if(!containsKey(key, cred))
            return false;
        return cred[key] == "amb"
            || cred[key] == "blu"
            || cred[key] == "brn"
            || cred[key] == "gry"
            || cred[key] == "grn"
            || cred[key] == "hzl"
            || cred[key] == "oth";
    },
    [](credential cred){
        std::string key {"pid"};
        if(!containsKey(key, cred))
            return false;
        if(cred[key].length() != 9)
            return false;
        for(char c : cred[key]) {
            if(c < '0' || c > '9')
                return false;
        }
        return true;
    }
};

bool credentialIsValid(credential& cred, std::vector<fieldVerifier> keyList = defaultKeyList) {
    for(auto predicate : keyList) { 
        if(!predicate(cred))
            return false;
    }

    return true;
}

enum class parse_state {
    parsing_key,
    parsing_value,
    end_of_pair_or_credential
};


void parseCredentials(std::ifstream& input, std::vector<credential>& list) {
    char currentChar { 0 };
    parse_state state { parse_state::parsing_key };
    std::string tempKey { };
    std::string tempValue { };
    credential tempCredential { };
    while(input.get(currentChar)) {
        switch(state) {
            case parse_state::end_of_pair_or_credential:
                if(currentChar == '\n') {
                    list.push_back(tempCredential);
                    tempKey = std::string { };
                    tempValue = std::string { };
                    tempCredential = credential { };
                    break;
                }
            case parse_state::parsing_key:
                if(currentChar == ':') {
                    state = parse_state::parsing_value;
                    break;
                }
                tempKey.push_back(currentChar);
                break;
            case parse_state::parsing_value:
                if(currentChar == ' ' || currentChar == '\n') {
                    tempCredential[tempKey] = tempValue;
                    tempKey = std::string { };
                    tempValue = std::string { };
                    state = currentChar == '\n' ? parse_state::end_of_pair_or_credential
                                                : parse_state::parsing_key;
                    break;
                }
                tempValue.push_back(currentChar);
        }
    }
    tempCredential[tempKey] = tempValue;
    list.push_back(tempCredential);
}

int main() {
    std::ifstream input { "4 - input.txt" };
    std::vector<credential> credentials { };

    parseCredentials(input, credentials);

    // part 1
    int validCount { 0 };
    for(credential thing : credentials) {
        if(credentialIsValid(thing))
            validCount++;
    }
    std::cout << validCount << '\n';

    // part 2
    int valid2Count { 0 };
    for(credential thing : credentials) {
        if(credentialIsValid(thing ,part2KeyList))
            valid2Count++;
    }
    std::cout << valid2Count;
}