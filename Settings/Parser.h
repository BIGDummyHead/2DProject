//
// Created by shawn on 7/1/2025.
//

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>


class Parser {
public:
    void cleanString(std::string& ref);
    //Reads a parser type file and returns the contents in a map for later use
    static std::unordered_map<std::string, std::string> readFile(const std::string& path);
    //Writes a parser type map to a file
    static void writeFile(const std::string& path, const std::unordered_map<std::string, std::string>& items, bool create = false);

    static bool parseBool(const std::string& p);

    static bool fileExist(const std::string& path);
};



#endif //PARSER_H
