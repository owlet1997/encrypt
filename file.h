//
// Created by User on 22.02.2021.
//

#ifndef ENCRYPT_FILE_H
#define ENCRYPT_FILE_H


class file {
public:
    static std::vector<std::string> SNV_splitFilename(const std::string fileName);
    static std::vector<uint32_t> SNV_readFileBinary(std::string fileName);
    static bool SNV_writeToFile(std::string path, std::vector<uint32_t> v);
};


#endif //ENCRYPT_FILE_H
