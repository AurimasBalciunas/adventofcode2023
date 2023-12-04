#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>

using namespace std;


// custom hash function for pair
struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};


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
    vector<vector<int>> positions{{-1, -1}, {-1, 0}, {-1, 1}, {1, -1}, {1, 0}, {1,1}};
    vector<vector<int>> neighbors(height, vector<int>(width, 0));
    vector<vector<int>> ratios(height, vector<int>(width, 1));



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
                
                unordered_set<pair<int, int>, pair_hash> toexplore;
                
                for(int x = left; x <= right; x++)
                {
                    if(x == left)
                    {
                        if(isValidPos(i, x-1, width, height))
                        {
                            toexplore.insert(make_pair(i, x-1));
                        }
                    }
                    if(x == right)
                    {
                        if(isValidPos(i, x+1, width, height))
                        {
                            toexplore.insert(make_pair(i, x+1));
                        }
                    }
                    
                    for(auto pos : positions)
                    {
                        if(isValidPos(i+pos[0], x+pos[1], width, height))
                        {
                            toexplore.insert(make_pair(i+pos[0], x+pos[1]));
                        }
                    }
                }

                for(auto & elem : toexplore)
                {
                    neighbors[elem.first][elem.second] += 1;
                    ratios[elem.first][elem.second] *=num;
                }

                // skip over this num
                j = right+1;
            }
        }
    }

    long long sum = 0;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(lines[i][j] == '*' && neighbors[i][j] == 2)
            {
                sum += ratios[i][j];
            }
        }
    }
    
    cout << "Overall sum was " << sum << endl;

    return 0;
}
