#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>

using namespace std;

unordered_map<char, string> dirs{
    {'|', "NS"},
    {'-', "EW"},
    {'L', "NE"},
    {'J', "NW"},
    {'7', "SW"},
    {'F', "SE"},
    {'.', "_"},
    {'S', "X"}
};

unordered_map<char, vector<int>> dirMeaning{
    {'N', {-1, 0}},
    {'E', {0, 1}},
    {'W', {0, -1}},
    {'S', {1, 0}},
};


vector<vector<string>> parseInput(string filename)
{
    vector<vector<string>> inputMap;
    ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Error: Couldn't open file";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(file, line))
    {
        vector<string> localMap;
        for(auto& ch : line)
        {
            if(dirs.find(ch) == dirs.end())
            {
                cerr << "Error: Unexpected value in input";
                exit(EXIT_FAILURE);
            }
            localMap.push_back(dirs[ch]);
        }
        inputMap.push_back(localMap);
    }
    return inputMap;
}

struct position{
    int y = 0;
    int x = 0;

    bool operator==(const position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const position& other) const {
        return !(*this == other);
    }
};

bool isValidPosition(position p, int height, int width)
{
    if(p.x < 0 || p.x >= width || p.y < 0 || p.y >= height)
    {
        return false;
    }
    return true;
}


bool acceptsCurrent(vector<vector<string>>& map, position from, position to)
{
    if(!isValidPosition(to, map.size(), map[0].size()))
        return false;
    for(auto& ch : map[to.y][to.x])
    {
        if(ch == '_')
        {
            return false;
        }
        if(ch == 'X')
        {
            return true;
        }

        position newLoc{to.y + dirMeaning[ch][0], to.x + dirMeaning[ch][1]};
        if(newLoc == from)
        {
            return true;
        }
    }
    return false;
}



// loopFinder returns -1 if the input is not a loop, 
// returns the length of the loop otherwise
// p is previous, c is current
bool loopFinder(vector<vector<string>>& map, position prev, position curr, int& distance)
{    
    string mapMoves = map[curr.y][curr.x];
    if(mapMoves == "X")
    {
        return true;
    }
    distance +=1;
    for(auto& ch : mapMoves)
    {
        position newLoc{curr.y + dirMeaning[ch][0], curr.x + dirMeaning[ch][1]};
        if(newLoc != prev && acceptsCurrent(map, curr, newLoc))
        {
            return loopFinder(map, curr, newLoc, distance);
        }

    }

    return false;
}


int pt1(vector<vector<string>>& map)
{  
    // Find the start pos
    position start;
    int height = map.size();
    int width = map[0].size();
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == "X")
            {
                start.y = i;
                start.x = j;
            }
        }
    }


    int furthestSpot = -1;
    // Launch loopFinder on all positions surrounding it
    position up{start.y-1, start.x};
    position down{start.y+1, start.x};
    position right{start.y, start.x+1};
    position left{start.y, start.x-1};

    int ans = 0;

    if(acceptsCurrent(map, start, up))
    {
        int loopdist = 1;
        bool fullLoop = loopFinder(map, start, up, loopdist);
        if(fullLoop)
            ans = max(ans, loopdist);
    }

    if(acceptsCurrent(map, start, down))
    {
        int loopdist = 1;
        bool fullLoop = loopFinder(map, start, down, loopdist);
        if (fullLoop)
            ans = max(ans, loopdist);
    }

    if(acceptsCurrent(map, start, left))
    {
        int loopdist = 1;
        bool fullLoop = loopFinder(map, start, left, loopdist);
        if(fullLoop)
            ans = max(ans, loopdist);
    }

    if(acceptsCurrent(map, start, right))
    {
        int loopdist = 1;
        bool fullLoop = loopFinder(map, start, right, loopdist);
        if(fullLoop)
            ans = max(ans, loopdist);
    }

    return ans/2;
}

int main()
{
    string filename = "input.txt";
    auto input = parseInput(filename);
    auto pt1ans = pt1(input);
    cout << "Ans to pt 1 was  " <<  pt1ans << endl;
    return -1;
}
