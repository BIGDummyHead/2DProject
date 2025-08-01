//
// Created by shawn on 7/31/2025.
//

#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

class FileHelper {
public:
    template<typename T>
    static std::vector<std::vector<T> > generateCsvMatrix(const std::string &file) requires std::is_integral_v<T> {
        std::vector<std::vector<T> > mapVector;

        std::ifstream csvFile(file);
        std::string line;

        while (std::getline(csvFile, line)) {
            std::stringstream ss(line);
            std::string value;
            std::vector<T> row;

            while (std::getline(ss, value, ',')) {
                int parseInt = std::stoi(value);

                if constexpr (std::is_same_v<T, bool>) {
                    parseInt = std::clamp(parseInt, 0, 1);
                }

                row.push_back(parseInt);
            }

            mapVector.push_back(row);
        }

        return mapVector;
    }
};


#endif //FILEHELPER_H
