#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include "file.h"

void genKey(std::string path) {
    std::mt19937 generator;
    generator.seed(time(0));

    // Запись в файл
    std::ofstream output_file(path + "key.txt");

    std::uniform_int_distribution<> distrib(33, 126);

    for (int i = 0; i < 16; i++) {
        char buffer = (char)distrib(generator);
            output_file << buffer;
    }
    output_file.close();
}

// Шифрование с помощью алгоритма TEA
void SNV_encrypt(std::string fileName) {
    std::cout << "\nStart encryption of file " << fileName << std::endl;

    std::vector<uint32_t> v = file::SNV_readFileBinary(fileName);
    std::vector<uint32_t> k = file::SNV_readFileBinary(file::SNV_splitFilename(fileName)[0] + "key.txt");

    if (v.empty()){
        std::cerr << "\nError file reading" << std::endl;
        return;
    }

    uint32_t sum = 0;
    uint32_t delta = 0x9e3779b9;

    for (int i = 0; i < 32; ++i) {
        sum += delta;
        v[0] += ((v[1] << 4) + k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + k[1]);
        v[1] += ((v[0] << 4) + k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + k[3]);
    }

    bool flag = file::SNV_writeToFile(fileName + ".enc", v);
    if (flag){
        std::cout << "\nResult of encryption had been written to file " << fileName + ".enc" << std::endl;
    } else {
        std::cerr << "\nFile opening error" << std::endl;
    }
}

// Дешифрование с помощью алгоритма TEA
void SNV_decrypt(const std::string& fileName, std::string keyFile) {

    std::cout << "\nStart decryption of file " << fileName << std::endl;
    // Чтение данных для дешифровки
    std::vector<uint32_t> v = file::SNV_readFileBinary(fileName);
    if (v.empty()){
        std::cerr << "\nError file reading" << std::endl;
        return;
    }

    // Чтение ключа
    std::vector<uint32_t> k = file::SNV_readFileBinary(std::move(keyFile));

    // Дешифрование
    uint32_t sum = 0xC6EF3720;
    uint32_t delta = 0x9e3779b9;

    for (int i = 0; i < 32; ++i) {
        v[1] -= ((v[0] << 4) + k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + k[3]);
        v[0] -= ((v[1] << 4) + k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + k[1]);
        sum -= delta;
    }

    // Определение имени выходного файла
    std::string initFileName = fileName.substr(0, fileName.find_last_of("."));
    std::string outputFile = initFileName;
    int num = 1;
    while (std::ifstream(outputFile).good()) {
        outputFile = initFileName;
        outputFile.insert(outputFile.find_last_of('.'), "(" + std::to_string(num++) + ")");
    }

    // Запись результатов дешифрования в выходной файл
    bool flag = file::SNV_writeToFile(outputFile, v);
    if (flag){
        std::cout << "\nResult of decryption had been written to file " << outputFile << std::endl;
    } else {
        std::cerr << "\nFile opening error" << std::endl;
    }
}

int main(int argc, char *argv[]) {

//    if (argc > 2) {
//        if (strcmp(argv[1], "-e") == 0) {
//            genKey(file::SNV_splitFilename(argv[2])[0]);
//            SNV_encrypt(argv[2]);
//        } else if (strcmp(argv[1], "-d") == 0 && argv[3]) {
//            SNV_decrypt(argv[2], argv[3]);
//        } else {
//            std::cerr << "Wrong parameters" << std::endl;
//            return 1;
//        }
//    } else {
//        genKey("");
//        SNV_encrypt("test.txt");
//    }

    std::vector<uint32_t> v = file::readWholeFileBinary("test.txt");
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    file::SNV_writeToWholeFile("new.txt", v);

    return 0;
}
