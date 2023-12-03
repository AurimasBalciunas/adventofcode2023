#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <algorithm>

using namespace std;

string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if(first == string::npos)
    {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (first - last + 1));
}

vector<string> split(string& tosplit, char delimiter)
{
    vector<string> tokens;
    stringstream ss(tosplit);
    string token;
    while(getline(ss, token, delimiter))
    {
        tokens.push_back(trim(token));
    }
    return tokens;
}

int main()
{
    unordered_map<string, int> allowed{{"red", 12}, {"green", 13}, {"blue", 14}};

    // file open and error check
    string filename = "input.txt";
    ifstream file(filename);
    if(!file)
    {
        throw(runtime_error("File not found: " + filename));
    }


    regex pattern(R"((\d+)\s*(blue|red|green))");
    smatch match;

    long long sum{0};
    string line;
    // iterate through every line
    int game = 1;
    while(getline(file, line))
    {
        
        unordered_map<string, int> mins{{"red", 0}, {"green", 0}, {"blue", 0}};
        
        string sub = line.substr(line.find(':') + 1);
        vector<string> cubeset = split(sub, ',');
        for(auto cube : cubeset)
        {
            while(regex_search(cube, match, pattern))
            {
                int number = stoi(match[1].str());
                string color = match[2].str();
                cube = match.suffix().str();
                mins[color] = max(mins[color], number);
            }
        }

        int local = 1;
        for(const auto & pair : mins) {
            local *= pair.second;
        }
        sum += local;

        game++;
    }

    
    cout << "The sum was " << sum << endl;
    return 0;


}
