#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

#include <vector>
#include <string>
#include <sstream>

#define READ 0
#define WRITE 1

std::vector<std::string> splitCommand(std::string command);

#endif  // UTILS_H_
