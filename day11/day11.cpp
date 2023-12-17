#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cmath>
#include <utility>

using namespace std;

struct position{
    int y;
    int x;
};

vector<string> parseInput(string filename)
{
    ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Could not open file " << filename << endl;
        exit(EXIT_FAILURE);
    }
    vector<string> input;
    string line;
    while(getline(file, line))
    {
        input.push_back(line);
    }
    file.close();
    return input;
}

vector<string> expandInput(const vector<string>& input)
{
    // Determine locs of blank rows
    vector<int> extraRows{};
    for(int i = 0; i < input.size(); i++)
    {
        bool empty = true;
        for(auto& elem : input[i])
        {
            if(elem == '#')
            {
                empty = false;
                break;
            }
        }
        if(empty)
        {
            extraRows.push_back(i);
        }
    }

    // Determine locs of blank cols
    vector<int> extraCols{};
    for(int j = 0; j < input[0].size(); j++)
    {
        bool empty = true;
        for(int i = 0; i < input.size(); i++)
        {
            if(input[i][j] == '#')
            {
                empty = false;
                break;
            }
        }
        if(empty)
        {
            extraCols.push_back(j);
        }
    }

    // Remap for rows
    vector<int> remappedRows(input.size(), 0);
    for(int i = 0; i < remappedRows.size(); i++)
    {
        remappedRows[i] = i;
    }
    for(auto& extraRow : extraRows)
    {
        for(int i = extraRow + 1; i <input.size(); i++)
        {
            remappedRows[i]++;
        }
    }

    // Remap for cols
    vector<int> remappedCols(input[0].size(), 0);
    for(int i = 0; i < remappedCols.size(); i++)
    {
        remappedCols[i] = i;
    }
    for(auto& extraCol : extraCols)
    {
        for(int i = extraCol + 1; i < input.size(); i++)
        {
            remappedCols[i]++;
        }
    }

    // Fill out new vector
    string emptyRow{};
    for(int i = 0; i < input[0].size() + extraCols.size(); i++)
    {
        emptyRow = emptyRow + ".";
    }
    vector<string> output(input.size() + extraRows.size(), emptyRow);
    for(int i = 0; i < input.size(); i++)
    {
        for(int j = 0; j < input[0].size(); j++)
        {
            output[remappedRows[i]][remappedCols[j]] = input[i][j];
        }
    }

    return output;
}

vector<position> findPositions(const vector<string>& map)
{
    int height = map.size();
    int width = map[0].size();

    vector<position> positions;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == '#')
            {
                positions.push_back(position{i, j});
            }
        }
    }
    return positions;
}

long long pt1(vector<string> input)
{
    // 1) Upscale: any rows and columns that are all empty space (all . no #) 
    // should be doubled in length.
    auto expanded = expandInput(input);

    // This was solving the wrong problem
    // // 2) BFS: Find the shortest path to node from each node. But only count 
    // // each pair once. Maybe a good way to do this would be to erase the element
    // // once I find all of its distances
    // long long totalTime = 0;
    // auto positions = findPositions(expanded);
    // int expected = positions.size() - 1;
    // for(int i = 0; i < positions.size(); i++)
    // {
    //     cout << "Searching for" << expected << endl;
    //     totalTime += bfs(expanded, expected, positions[i]);
    //     expanded[positions[i].y][positions[i].x] = '.';
    //     expected--;
    // }

    long long totalDistance = 0;
    auto positions = findPositions(expanded);
    for(int i = 0; i < positions.size(); i++)
    {
        for(int j = i+1; j < positions.size(); j++)
        {
            totalDistance += ((abs(positions[i].y - positions[j].y)) +
                             (abs(positions[i].x - positions[j].x)));
        }
    }

    return totalDistance;

}

pair<vector<int>, vector<int>> getRemaps(const vector<string>& input, int remapSize)
{
    // Determine locs of blank rows
    vector<int> extraRows{};
    for(int i = 0; i < input.size(); i++)
    {
        bool empty = true;
        for(auto& elem : input[i])
        {
            if(elem == '#')
            {
                empty = false;
                break;
            }
        }
        if(empty)
        {
            extraRows.push_back(i);
        }
    }

    // Determine locs of blank cols
    vector<int> extraCols{};
    for(int j = 0; j < input[0].size(); j++)
    {
        bool empty = true;
        for(int i = 0; i < input.size(); i++)
        {
            if(input[i][j] == '#')
            {
                empty = false;
                break;
            }
        }
        if(empty)
        {
            extraCols.push_back(j);
        }
    }

    // Remap for rows
    vector<int> remappedRows(input.size(), 0);
    for(int i = 0; i < remappedRows.size(); i++)
    {
        remappedRows[i] = i;
    }
    for(auto& extraRow : extraRows)
    {
        for(int i = extraRow + 1; i <input.size(); i++)
        {
            remappedRows[i]+=remapSize;

        }
    }

    // Remap for cols
    vector<int> remappedCols(input.size(), 0);
    for(int i = 0; i < remappedCols.size(); i++)
    {
        remappedCols[i] = i;
    }
    for(auto& extraCol : extraCols)
    {
        for(int i = extraCol + 1; i < input.size(); i++)
        {
            remappedCols[i]+=remapSize;   
        }
    }

    return make_pair(remappedRows, remappedCols);
}

long long pt2(vector<string>& input)
{
    //auto remaps = getRemaps(input, 100);
    auto remaps = getRemaps(input, 1000000-1);
    vector<int> rowRemaps = remaps.first;
    vector<int> colRemaps = remaps.second;
    auto positions = findPositions(input);

    cout << "row remaps : " << endl;
    int index = 0;
    for(auto& rm : rowRemaps)
    {
        cout << index << " -> " << rm << " ";
        index++;
        cout << endl;
    }
    cout << endl;

    cout << "col remaps : " << endl;
    index = 0;
    for(auto& rm : colRemaps)
    {
        cout << index << " -> " << rm << " ";
        index ++;
        cout << endl;
    }
    cout << endl << "POSITIONS" << endl;;
    for(auto& pos : positions)
    {
        cout << "POS Y " << pos.y << " POS X " << pos.x << endl;
        cout << "REMAPS TO " << rowRemaps[pos.y] << " " << colRemaps[pos.x] << endl;
    }

    long long totalDistance = 0;
    for(int i = 0; i < positions.size(); i++)
    {
        for(int j = i+1; j < positions.size(); j++)
        {
            totalDistance += ((abs(rowRemaps[positions[i].y] - rowRemaps[positions[j].y])) +
                             (abs(colRemaps[positions[i].x] - colRemaps[positions[j].x])));
        }
    }

    return totalDistance;
}
//543018860016 too high
//543018859934 too high
//543018317006 just right
//82000210 too low



int main()
{
    vector<string> inputFiles{"input_test.txt", "input.txt"};
    for(const auto& filename : inputFiles)
    {
        auto input = parseInput(filename);
        auto pt1Ans = pt1(input);
        cout << "Pt1 ans for " << filename <<  " was " << pt1Ans << endl;
        auto pt2Ans = pt2(input);
        cout << "Pt2 ans for " << filename <<  " was " << pt2Ans << endl;

    }
    return 0;
}