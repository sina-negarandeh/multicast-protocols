#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#define READ 0
#define WRITE 1

std::vector<std::string> splitCommand(std::string command, char delim=' ');
std::string readFileIntoString(const std::string& path);

class IP{
    public:
    IP(std::string ip_string);
    IP(std::vector<int> ip_ints);
    IP(int part1, int part2, int part3, int part4);
    std::string get_string(){return ip_string_;}    
    std::vector<int> get_parts(){return ip_ints_;}

    private:
    std::string ip_string_;
    std::vector<int> ip_ints_;
};

#endif  // UTILS_H_
