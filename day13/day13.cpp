#include <fstream>
#include <string>
#include <iostream>

using namespace std;

//rows above
int findRowsAbove(vector<string>& vec)
{
    int height = vec.size();
    int width = vec[0].size();

    string prevRow = vec[0];
    bool foundMirror = false;
    for(int i = 1; i < height; i++)
    {
        if (vec[i] == prevRow)
        {
            int start = i;
            int low = i-1;
            int high = i;
            while(low-1 >= 0 && high+1 < vec.size() && vec[low-1] == vec[high+1])
            {
               --low;
               ++high;
            }
            // Only the answer if we got through the whole vector
            if(low-1 < 0 || high + 1 == vec.size())
            {
                return start;
            }
        }
        else
        {
            prevRow = vec[i];
        }
    }
    return 0; // didn't find matching in rows
}

int findColsLeft(vector<string>& vec)
{
    int ogHeight = vec.size();
    int ogWidth = vec[0].size();

    // rotate
    vector<string> rotated(ogWidth, string(ogHeight, ' '));
    for(int i = 0; i < ogHeight; i++)
    {
        for(int j = 0; j < ogWidth; j++)
        {
            rotated[j][i] = vec[i][j];
        }
    }

    // pass into findRowsAbove
    return findRowsAbove(rotated);
}


//columns to the right
int pt1(vector<vector<string>> vecs)
{
    int total = 0;
    for(auto& vec : vecs)
    {
        int rowTotal = findRowsAbove(vec);
        cout << "Row total: " << rowTotal << endl;
        if(rowTotal > 0)
        {
            total += rowTotal * 100;
        }
        else
        {
            int colTotal = findColsLeft(vec);
            cout << "Col total: " << colTotal << endl;
            total += colTotal;
        }
    }
    return total;
}


int main()
{
    string filename{"input.txt"};
    ifstream file(filename);
    if(!file.is_open())
    {
        cerr << "Can't open file";
        exit(EXIT_FAILURE);
    }

    vector<vector<string>> vecs{};
    vector<string> matrix{};
    string line;
    while(getline(file, line))
    {
        if(line == "")
        {
            vecs.push_back(matrix);
            matrix = {};
        }
        else
        {
            matrix.push_back(line);
        }
    }
    vecs.push_back(matrix);

    cout << "Ans to pt 1 is " << pt1(vecs) << endl;
    
}