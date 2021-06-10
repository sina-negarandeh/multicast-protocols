#include "utils.hpp"
#include <string>

using namespace std;

vector<string> splitCommand(string command, char delim) {
    vector<string> duppedline;
    istringstream stream(command);
    while (stream) {
        string word;
        getline(stream, word, delim);
        duppedline.push_back(word);
    }
    if (duppedline[duppedline.size() - 1] == "")
        duppedline.pop_back();
    return duppedline;
}

string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

IP::IP(std::string ip_string){
    vector<string> duppedline =  splitCommand(ip_string, '.');
    if (duppedline.size() != 4){throw "Invalid IP format!";}
    ip_string_ = ip_string;
    for (string part:duppedline){
        ip_ints_.push_back(stoi(part));
    }
}

IP::IP(std::vector<int> ip_ints){
    if (ip_ints.size() != 4){throw "Invalid IP format!";}
    ip_ints_ = ip_ints;
    string ip_string = to_string(ip_ints[0]);
    for (int i = 1 ; i < ip_ints.size() ; i++){
        ip_string += "." + to_string(ip_ints[i]);
    }
    ip_string_ = ip_string;
}

IP::IP(int part1, int part2, int part3, int part4){
    vector<int> ip_ints;
    ip_ints.push_back(part1);
    ip_ints.push_back(part2);
    ip_ints.push_back(part3);
    ip_ints.push_back(part4);
    if (ip_ints.size() != 4){throw "Invalid IP format!";}
    ip_ints_ = ip_ints;
    string ip_string = to_string(ip_ints[0]);
    for (int i = 1 ; i < ip_ints.size() ; i++){
        ip_string += "." + to_string(ip_ints[i]);
    }
    ip_string_ = ip_string;
}