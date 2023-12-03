#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// get each line and store into vector of vector of strings
// create another matrix of bools that stores whether a position is valid
// create a matrix of ints that stores the value of the int in a vector of ints
// for every int position, if it is non-zero and valid, add it to sum and continue till we are at a 0

// for every symbol, mark the up/down/left/right diags positions as valid
// go through, for each number you encounter, if any of its positions are valid, add it to sum and go to next 

using namespace std;

bool isValidPos(int i, int j, int width, int height)
{
    return (i >=0 && j >= 0 && i < height && j < width);
}

int main()
{
    // file open and error check
    string filename = "input.txt";
    ifstream file(filename);
    if(!file)
    {
        throw(runtime_error("File not found: " + filename));
    }
    // read in all lines
    vector<string> lines;
    string line;
    while(getline(file, line))
    {
        lines.push_back(line);
    }

    int height = lines.size();
    int width = lines[0].size();

    //process neighbors
    vector<vector<int>> positions{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1,1}};
    vector<vector<bool>> neighbors(height, vector<bool>(width, false));
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(!isdigit(lines[i][j]) && lines[i][j] != '.')
            {
                for(auto &pos : positions)
                {
                    if(isValidPos(i+pos[0], j+pos[1], width, height))
                    {
                        neighbors[i+pos[0]][j+pos[1]] = true;
                    }
                }
            }
        }
    }

    long long sum{0};
    // find nums, add them to sum
    vector<vector<int>> nums(height, vector<int>(width, 0));
    for(int i = 0; i < nums.size(); i++)
    {
        for(int j = 0; j < nums[0].size(); j++)
        {
            if(isdigit(lines[i][j]))
            {
                //process the num
                int left = j;
                int right = j;
                while(right < width && isdigit(lines[i][right]))
                {
                    ++right;
                }
                --right;
                int num = stoi(lines[i].substr(left, right - left + 1));
                cout << "found num " << num << endl;

                // add it to sum if appropriate
                for(int pos = left; pos <= right; pos++)
                {
                    if(neighbors[i][pos])
                    {
                        sum += num;
                        break;
                    }
                }

                // skip over this num
                j = right+1;
            }
        }
    }
    
    cout << "Overall sum was " << sum << endl;

    return 0;
}
