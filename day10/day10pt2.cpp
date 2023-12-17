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


vector<vector<char>> parseRawInput(string filename)
{
    vector<vector<char>> inputMap;
    ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Error: Couldn't open file";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(file, line))
    {
        vector<char> localMap;
        for(auto& ch : line)
        {
            localMap.push_back(ch);
        }
        inputMap.push_back(localMap);
    }
    return inputMap;
}

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
bool loopFinder(vector<vector<string>>& map, position prev, position curr, int& distance, vector<position>& positions, string& ended)
{    
    positions.push_back(curr);
    string mapMoves = map[curr.y][curr.x];
    if(mapMoves == "X")
    {
        if(prev.x == curr.x + 1)
        {
            ended = "right";
        }
        else if(prev.x == curr.x - 1)
        {
            ended = "left";
        }
        else if(prev.y == curr.y + 1)
        {
            ended = "down";
        }
        else
        {
            ended = "up";
        }
        return true;
    }
    distance +=1;
    for(auto& ch : mapMoves)
    {
        position newLoc{curr.y + dirMeaning[ch][0], curr.x + dirMeaning[ch][1]};
        if(newLoc != prev && acceptsCurrent(map, curr, newLoc))
        {
            return loopFinder(map, curr, newLoc, distance, positions, ended);
        }

    }

    return false;
}


unordered_map<char, vector<vector<int>>> upscaledKernels
{
    {'-', {{0,0,0},{1,1,1},{0,0,0}}},
    {'F', {{0,0,0},{0,1,1},{0,1,0}}},
    {'|', {{0,1,0},{0,1,0},{0,1,0}}},
    {'J', {{0,1,0},{1,1,0},{0,0,0}}},
    {'7', {{0,0,0},{1,1,0},{0,1,0}}},
    {'L', {{0,1,0},{0,1,1},{0,0,0}}},
    {'.', {{0,0,0},{0,0,0},{0,0,0}}},
    {'S', {{0,0,0},{0,0,0},{0,0,0}}},
};

vector<vector<int>> upscaleMap(vector<vector<char>>& originalMap) {
    int originalHeight = originalMap.size();
    int originalWidth = originalMap[0].size();
    vector<vector<int>> upscaledMap(originalHeight * 3, vector<int>(originalWidth * 3));

    for (int y = 0; y < originalHeight; ++y) {
        for (int x = 0; x < originalWidth; ++x) {
            char mapChar = originalMap[y][x];
            if (upscaledKernels.count(mapChar) > 0) {
                auto& kernel = upscaledKernels.at(mapChar);
                for (int dy = 0; dy < 3; ++dy) {
                    for (int dx = 0; dx < 3; ++dx) {
                        upscaledMap[y * 3 + dy][x * 3 + dx] = kernel[dy][dx];
                    }
                }
            }
            else
            {
                cerr << "Error: Character missing from kernel set";
                exit(EXIT_FAILURE);
            }
        }
    }
    return upscaledMap;
}

// too naive unless upscaled
void floodFill(vector<vector<int>>& drawing, int x, int y) {
    int height = drawing.size();
    int width = drawing[0].size();

    if(x < 0 || x >= width || y < 0 || y >= height || drawing[y][x] != 0) {
        return;
    }

    drawing[y][x] = -1; // Mark as exterior

    floodFill(drawing, x + 1, y);
    floodFill(drawing, x - 1, y);
    floodFill(drawing, x, y + 1);
    floodFill(drawing, x, y - 1);
}
void floodFillMap(vector<vector<int>>& drawing) {
    int height = drawing.size();
    int width = drawing[0].size();
    int total = 0;

    // Flood fill left, right columns
    for (int i = 0; i < height; i++) {
        floodFill(drawing, 0, i);
        floodFill(drawing, width - 1, i);
    }

    // Flood fill top, left rows
    for (int j = 0; j < width; j++) {
        floodFill(drawing, j, 0);
        floodFill(drawing, j, height - 1);
    }

    return;
    // for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //         //1 is edge, -1 is exterior point
    //         if (drawing[i][j] == 0) {
    //             total++;
    //         }
    //     }
    // }
    //return total;
}

int countDownScaled(vector<vector<int>> upscaled)
{
    vector<vector<int>> downscaled(upscaled.size()/3, vector<int>(upscaled[0].size()/3, 0));
    int total = 0;
    for(int y = 1; y < upscaled.size(); y+=3)
    {
        for(int x = 1; x < upscaled[0].size(); x += 3)
        {
            downscaled[y/3][x/3] = upscaled[y][x];
            if(downscaled[y/3][x/3] == 0)
            {
                total++;
            }
        }
    }
    
    for(int y = 0; y < downscaled.size(); y++)
    {
        for(int x = 0; x < downscaled[0].size(); x ++)
        {
            if(downscaled[y][x] == -1)
            {
                cout << '.';
            }
            else if (downscaled[y][x] == 1)
            {
                cout << '.';
            }
            else{
                cout << 'X';
            }
            // cout << downscaled[y][x];
        }
        cout << endl;
    }



    return total;
}


// too naive, assumes non self-intersecting
int calculateEnclosedArea(vector<vector<char>>& drawing)
{
    /*
    For each row in the drawing, go from left to right
    As soon as you hit an edge, mark bool Inside as true, continue to next point
    While you hit non-edge points, add them to the total
    As soon as you hit an edge, mark bool Inside as false
    
    More generally
    For each row in the drawing, go from left to right
    bool inside starts false
    If you hit an edge (1 in drawing), toggle bool inside, continue to next point
    If you hit a non-edge, if inside is true, add it to the count, if inside is false, do not add to count
    
    |, FJ and L7 are the ONLY REAL WALLS
    */

    int height = drawing.size();
    int width = drawing[0].size();
    int total = 0;
    for(int i = 0; i < height; i++)
    {
        char wallstart = ' ';
        int wallcount = 0;
        for(int j = 0; j < width; j++)
        {
            switch(drawing[i][j])
            {
                case 'F': //wall starter
                    wallstart = 'F';
                    break;
                case 'L': //wall starter
                    wallstart = 'L';
                    break;
                case '|': //wall starter
                    wallcount += 1;
                    break;
                case 'J': // wall ender
                    if(wallstart == 'F')
                    {
                        wallcount += 1;
                    }
                    wallstart = ' ';
                    break;
                case '7': // wall ender
                    if(wallstart == 'L')
                    {
                        wallcount+=1;
                    }
                    wallstart = ' ';
                    break;
                default:
                    if(wallcount % 2 == 1)
                    {
                        total+=1;
                    }
                    break;

            }
        }
    }
    return total;

}


// // too naive, assumes non self-intersecting
// int calculateEnclosedArea(vector<vector<int>>& drawing)
// {
//     /*
//     For each row in the drawing, go from left to right
//     As soon as you hit an edge, mark bool Inside as true, continue to next point
//     While you hit non-edge points, add them to the total
//     As soon as you hit an edge, mark bool Inside as false
    
//     More generally
//     For each row in the drawing, go from left to right
//     bool inside starts false
//     If you hit an edge (1 in drawing), toggle bool inside, continue to next point
//     If you hit a non-edge, if inside is true, add it to the count, if inside is false, do not add to count
//     */

//     int height = drawing.size();
//     int width = drawing[0].size();
//     int total = 0;
//     for(int i = 0; i < height; i++)
//     {
//         bool inside = false;
//         for(int j = 0; j < width; j++)
//         {
//             //is an edge
//             if(drawing[i][j] == 1)
//             {
//                 inside = !inside;
//             }
//             //is not an edge
//             else
//             {
//                 if(inside)
//                 {
//                     cout << "Counting enclosed at " << i << " " << j << endl;
//                     total++;
//                 }
//             }
//         }
//     }
//     return total;

// }


vector<vector<int>> drawPositions(vector<position> positions, int height, int width)
{
    vector<vector<int>> drawing(height, vector<int>(width, 0));
    for(auto& pos : positions)
    {
        drawing[pos.y][pos.x]++;
    }

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(drawing[i][j] == 1)
            {
                cout << "1";
            }
            else{
                cout << "0";
            }
            //cout << drawing[i][j];
            if(drawing[i][j] > 1)
            {
                cerr << "Houston, we have a problem";
                exit(EXIT_FAILURE);
            }
        }
        cout << endl;
    }

    return drawing;
}


vector<position> pt2(vector<vector<string>>& map, vector<vector<char>>& rawInput)
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

    vector<position> finalPositions;
    if(acceptsCurrent(map, start, up))
    {
        int loopdist = 1;
        vector<position> positions{};
        string ended = "";
        bool fullLoop = loopFinder(map, start, up, loopdist, positions, ended);
        cout << "ENDED: " << ended << endl;
        if(fullLoop)
        {
            ans = max(ans, loopdist);
            finalPositions = positions;

            if(ended == "left")
            {
                rawInput[start.y][start.x] = 'J';
            }
            else if(ended == "right")
            {
                rawInput[start.y][start.x] = 'L';
            }
            else if(ended == "up")
            {
                rawInput[start.y][start.x] = 'S';
            }
            else if(ended == "down")
            {
                rawInput[start.y][start.x] = '|';
            }
        }
    }

    else if(acceptsCurrent(map, start, down))
    {
        int loopdist = 1;
        vector<position> positions{};
        string ended = "";
        bool fullLoop = loopFinder(map, start, down, loopdist, positions, ended);
        cout << "ENDED: " << ended << endl;
        if (fullLoop)
        {
            ans = max(ans, loopdist);
            finalPositions = positions;
            if(ended == "left")
            {
                rawInput[start.y][start.x] = '7';
            }
            else if(ended == "right")
            {
                rawInput[start.y][start.x] = 'F';
            }
            else if(ended == "up")
            {
                rawInput[start.y][start.x] = '|';
            }
            else if(ended == "down")
            {
                rawInput[start.y][start.x] = 'S';
            }
        }
    }

    else if(acceptsCurrent(map, start, left))
    {
        int loopdist = 1;
        vector<position> positions{};
        string ended = "";
        bool fullLoop = loopFinder(map, start, left, loopdist, positions, ended);
        cout << "ENDED: " << ended << endl;

        if(fullLoop)
        {
            ans = max(ans, loopdist);
            finalPositions = positions;
            if(ended == "left")
            {
                rawInput[start.y][start.x] = 'S';
            }
            else if(ended == "right")
            {
                rawInput[start.y][start.x] = '-';
            }
            else if(ended == "up")
            {
                rawInput[start.y][start.x] = 'J';
            }
            else if(ended == "down")
            {
                rawInput[start.y][start.x] = '7';
            }
        }
    }

    else if(acceptsCurrent(map, start, right))
    {
        int loopdist = 1;
        vector<position> positions{};
        string ended = "";
        bool fullLoop = loopFinder(map, start, right, loopdist, positions, ended);
        cout << "ENDED: " << ended << endl;
        if(fullLoop)
        {
            ans = max(ans, loopdist);
            finalPositions = positions;
            if(ended == "left")
            {
                rawInput[start.y][start.x] = '-';
            }
            else if(ended == "right")
            {
                rawInput[start.y][start.x] = 'S';
            }
            else if(ended == "up")
            {
                rawInput[start.y][start.x] = 'L';
            }
            else if(ended == "down")
            {
                rawInput[start.y][start.x] = 'F';
            }
        }
            
    }
    return finalPositions;
}

vector<vector<char>> reconstructFromPositions(vector<position> positions, vector<vector<char>>& rawInput)
{
    int height = rawInput.size();
    int width = rawInput[0].size();
    vector<vector<char>> reconstructed(height, vector<char>(width, '.'));
    for(auto& pos : positions)
    {
        reconstructed[pos.y][pos.x] = rawInput[pos.y][pos.x];
    }
    return reconstructed;

}

int main()
{
    string filename = "input.txt";
    auto input = parseInput(filename);
    auto rawInput = parseRawInput(filename);

    cout << "Raw input:" << endl;
    for(auto& in : rawInput)
    {
        for(auto& i : in)
        {
            cout << i;
        }
        cout << endl;
    }

    auto positions = pt2(input, rawInput);
    cout << "Raw input modified:" << endl;
    for(auto& in : rawInput)
    {
        for(auto& i : in)
        {
            cout << i;
        }
        cout << endl;
    }

    auto rawInputReconstructed = reconstructFromPositions(positions, rawInput);
    cout << "Raw input reconstructed:" << endl;
    for(auto& in : rawInputReconstructed)
    {
        for(auto& i : in)
        {
            cout << i;
        }
        cout << endl;
    }


    //auto drawing = drawPositions(positions, input.size(), input[0].size());

    auto upscaled = upscaleMap(rawInputReconstructed);
    for(auto& in : upscaled)
    {
        for(auto& i : in)
        {
            if(i == 1)
            {
                cout << "X";
            }
            else
            {
                cout << ".";
            }
            //cout << i;
        }
        cout << endl;
    }
    floodFillMap(upscaled); // flood fill upscaled map
    auto total = countDownScaled(upscaled); // downscale back to OG dimensions and count -1
    cout << "Pt2 Answer is " << total << endl; 
    //countInSet(upscale); // count the number of filled points (-1)


    

    // auto area = calculateEnclosedAreaFlood(upscaled);
    // cout << "Area was " << area/9 << endl;
    // Too naive approach
    //auto enclosedArea = calculateEnclosedArea(drawing);

    // auto enclosedArea = calculateEnclosedAreaFlood(drawing);
    // cout << "Pt2: enclosed area is " << enclosedArea << endl;

    // auto total = calculateEnclosedArea(rawInput);
    // cout << "Enclosed area was" << total << endl;

    return 0;;

}
