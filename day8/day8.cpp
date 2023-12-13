#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

void pt1(const unordered_map<string, pair<string, string>>& map, const vector<char>& moves)
{
    string origin = "AAA";
    string destination = "ZZZ"; 
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

void pt2(const unordered_map<string, pair<string, string>>& map, const vector<char>& moves)
{
    vector<string> travelers;
    for(auto& elem : map)
    {
        if(elem.first[2] == 'A')
        {
            travelers.push_back(elem.first);
        }
    }

    for(auto& traveler : travelers)
    {
        cout << "Traveler : " << traveler << endl;
    }

    // Why not just calculate how long it would take each one to get to dest then find least common multiple
    vector<int> moveCounts(travelers.size(), 0);
    for(int i = 0; i < travelers.size(); i++)
    {
        string traveler = travelers[i];
        long long currMove = 0;
        while(traveler[2] != 'Z')
        {
            char move = moves[currMove%moves.size()];
            traveler = (move == 'L') ? map.at(traveler).first : map.at(traveler).second;
            currMove++;
        }
        moveCounts[i] = currMove;
        cout << "It took traveler " << i << " " << currMove << " moves to reach the destination" << endl;
    }


    long long total = 1;
    for(int i = 0; i < moveCounts.size(); i++)
    {
        total = lcm(total, moveCounts[i]);
    }
    cout << "Pt2 Total was " << total << endl;


}
//14523852448103 too high
//14265111103729 is the ans


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

    // skip the blank line
    getline(file, line);

    while(getline(file, line))
    {
        string node = line.substr(0, 3);
        string leftChild = line.substr(7,3);
        string rightChild = line.substr(12, 3);
        map[node] = make_pair(leftChild, rightChild);
    }

    pt1(map, moves);
    pt2(map, moves);
    
    
}