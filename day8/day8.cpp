#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <string>

using namespace std;

void pt1(const string& origin, const string& destination, const unordered_map<string, pair<string, string>>& map, const vector<char>& moves)
{
    long long currMove = 0;
    string current = origin;
    while(current != destination)
    {
        char move = moves[currMove%moves.size()];
        current = (move == 'L') ? map.at(current).first : map.at(current).second;
        currMove++;
    }
    cout << "It took " << currMove << " moves to reach the destination." << endl; 
}


int main()
{
    string filename = "input.txt";

    unordered_map<string, pair<string, string>> map;
    vector<char> moves;

    ifstream file(filename);
    if(!file.is_open())
    {
        cout << "Failed to open file" << endl;
        exit(EXIT_FAILURE);
    }
    
    string line;

    getline(file, line);
    for(const auto& ch : line)
    {
        moves.push_back(ch);
    }

    getline(file, line); //skip the blank line

    string origin = "AAA";
    string destination = "ZZZ";
    while(getline(file, line))
    {
        string node = line.substr(0, 3);
        string leftChild = line.substr(7,3);
        string rightChild = line.substr(12, 3);
        map[node] = make_pair(leftChild, rightChild);
    }

    cout << "Origin " << origin << " Destination " << destination << endl;
    pt1(origin, destination, map, moves);
    
    
}