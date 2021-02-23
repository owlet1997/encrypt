//
// Created by User on 22.02.2021.
//
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>
#include <ctime>
#include "file.h"

// Разделить файл на путь и имя файла
std::vector<std::string> file::SNV_splitFilename(const std::string fileName) {
    size_t found;
    found = fileName.find_last_of("/\\");
    if (found == std::string::npos)
        return {"", fileName};
    return {fileName.substr(0, found) + "/", fileName.substr(found + 1)};
}

std::vector<uint32_t> file::SNV_readFileBinary(std::string fileName) {
    std::ifstream input(fileName, std::ios::binary | std::ios::in);
    std::vector<uint32_t> v;
    if (input.is_open()) {
        char *buffer = new char[4];
        while (input.read(buffer, 4)) {
            uint32_t num;
            memcpy(&num, buffer, 4);
            v.push_back(num);
        }
        input.close();
        return v;
    }
    std::cerr << "No file found" << std::endl;
    return {};
}


bool file::SNV_writeToFile(std::string path, std::vector<uint32_t> v) {
    std::ofstream output(path);
    if (output.is_open()) {
        for (int i = 0; i < v.size(); i++) {
            char *buf = new char[4];
            memcpy(buf, &v[i], 4);
            for (int j = 0; j < 4; j++) {
                output << (char) buf[j];
            }
        }
        output.close();
        return true;
    } else {
        return false;
    }
}
