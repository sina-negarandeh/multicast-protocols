#include "utils.hpp"

using namespace std;

vector<string> splitCommand(string command) {
    vector<string> duppedline;
    istringstream stream(command);
    while (stream) {
        string word;
        stream >> word;
        duppedline.push_back(word);
    }
    if (duppedline[duppedline.size() - 1] == "")
        duppedline.pop_back();
    return duppedline;
}
