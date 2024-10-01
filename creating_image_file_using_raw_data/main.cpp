#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

std::unordered_map<char, std::string> hexToDecimalMapping;

void testWrite() {
    std::ofstream file;
    char s[] = {'a','b','c'};

    file.open("test.bin", std::ios::binary | std::ios::out);
    file.write(s, sizeof(s));
    file.close();
}

void testRead() {
    std::ifstream file;

    file.open("checkerboard.jpeg", std::ios::in);

    char* buff = (char*) malloc(1);

    while(file.read(buff, 1)) {

        if (buff == NULL) {
            break;
        }

        std::cout<<*buff<<std::endl;

    }

    file.close();
}

uint8_t getInt(std::string bin) {
    uint8_t retVal=0;

    for (int i=0;i<bin.size();i++) {
        uint8_t val = (bin[i] == '0' ? 0 : 1);

        retVal += val * pow(2, 7-i);
    }

    return retVal;

}

void read(const char* filePath, char* buffer, size_t size) {
    std::ifstream inputFile;

    inputFile.open(filePath, std::ios::in);
    
    while (inputFile.read(buffer, size)) {
        if (buffer == NULL) break;
        
    }

    inputFile.close();
}

void convert(const char* inputHexFile) {
    size_t inputBufferSize = 1024*1024;
    char inputBuffer[inputBufferSize];
    size_t outPutBufferSize = 1024*1024*2;
    char outBuffer[outPutBufferSize];
    size_t i = 0;
    size_t actualOutPutBufferSize = 0;
    std::ofstream outFile;

    read(inputHexFile, inputBuffer, inputBufferSize);

    while ((i+1) < inputBufferSize)  {
        if (inputBuffer[i] == ' ') {
            ++i;
            continue;
        }

        if ((hexToDecimalMapping.find(inputBuffer[i]) == hexToDecimalMapping.end()) || 
            (hexToDecimalMapping.find(inputBuffer[i+1]) == hexToDecimalMapping.end())) {

            break;
        }

        outBuffer[actualOutPutBufferSize++] = getInt(hexToDecimalMapping.at(inputBuffer[i]) + hexToDecimalMapping.at(inputBuffer[i+1]));
        i += 2;
    }

    outBuffer[actualOutPutBufferSize] = '\0';
    outFile.open("real_file.jpeg", std::ios::binary | std::ios::out);
    outFile.write(outBuffer, actualOutPutBufferSize);
    outFile.close();
}

int main(int argc, char** argv) {

    hexToDecimalMapping['0'] =  "0000";
    hexToDecimalMapping['1'] =  "0001";
    hexToDecimalMapping['2'] =  "0010";
    hexToDecimalMapping['3'] =  "0011";
    hexToDecimalMapping['4'] =  "0100";
    hexToDecimalMapping['5'] =  "0101";
    hexToDecimalMapping['6'] =  "0110";
    hexToDecimalMapping['7'] =  "0111";
    hexToDecimalMapping['8'] =  "1000";
    hexToDecimalMapping['9'] =  "1001";
    hexToDecimalMapping['a'] =  "1010";
    hexToDecimalMapping['b'] =  "1011";
    hexToDecimalMapping['c'] =  "1100";
    hexToDecimalMapping['d'] =  "1101";
    hexToDecimalMapping['e'] =  "1110";
    hexToDecimalMapping['f'] =  "1111";
    convert(argv[1]);
}