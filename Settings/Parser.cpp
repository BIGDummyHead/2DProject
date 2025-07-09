//
// Created by shawn on 7/1/2025.
//

#include "Parser.h"

#include <fstream>
#include <SDL_log.h>
#include <sstream>

void Parser::cleanString(std::string &ref) {

}

bool Parser::fileExist(const std::string &path) {
    FILE* f = fopen(path.c_str(), "r");
    if(!f) {
        return false;
    }

    fclose(f);
    return true;
}



bool Parser::parseBool(const std::string &p) {

    if(p.length() == 1) {
        return p == "1";
    }

    if(p.length() >= 4){
        return p == "true";
    }

    return false;
}


std::unordered_map<std::string, std::string> Parser::readFile(const std::string &path) {

    FILE* file = fopen(path.c_str(), "r");

    std::unordered_map<std::string, std::string> parsedMap{};
    if(!file) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "The file you are trying to write to does not exist");
        return parsedMap;
    }
    fclose(file);

    std::ifstream inFile(path);
    std::string parsingLine;

    while(std::getline(inFile, parsingLine)) {
        if(parsingLine.empty())
            continue; //we may want to continue reading

        std::stringstream data;
        int dataMarks = 0;
        std::string key;

        //Able to parse a line like so->
        //"key" : "value"
        //" is encountered, datamarks is index + 1 allowing for data to be added to the stream:
        //key is then added to the stream. Then '"' is encountered once more. Creating the "key" string as the upcoming key
        //Since 2 '"' marks were found, now when a ':' symbol is encountered the value is ready to be read
        //When '"' is encountered a 3rd time, data begins being read once more, allowing for value to be added.
        //When the final '"' is found (4th), the parsed map then added the value to the map using the key and value

        for(auto c : parsingLine) {
            if(c == '"') {
                dataMarks++;

                if(dataMarks == 4) {

                    parsedMap[key] = data.str();
                }

                continue;
            }


            if(c == ':' && dataMarks == 2) {

                key = data.str();

                //set data to empty and clear of any clerical errors
                data.str("");
                data.clear();
                if(parsedMap.contains(key)) {
                    throw std::runtime_error("This key already exist when parsing");
                }

                continue;
            }

            //ensures the value being added is inside
            if(dataMarks % 2 != 0) {
                data << c;
            }
        }
    }

    return parsedMap;
}

void Parser::writeFile(const std::string &path, const std::unordered_map<std::string, std::string>& items, const bool create) {

    FILE* file = fopen(path.c_str(), "w");

    if(!file) {

        if(!create) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "The file you are trying to write to does not exist");
            return;
        }

        //TODO implement a system to create the file when it does not exist
    }


    std::stringstream parsed;
    for (const auto& item: items) {


        //std::string firstPart = item.first;
        //std::string secPart = item.second;

        parsed << "\"" << item.first << "\"" << ":" << "\"" << item.second << "\"" << std::endl;;

    }
    std::string asStr = parsed.str();
    const char* writing = asStr.c_str();

    fprintf(file, writing);
    fclose(file);
    //fprintf()
}

