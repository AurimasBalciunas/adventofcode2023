#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;


bool isAllZeros(vector<int>& nums)
{
    for(auto& num : nums)
    {
        if(num != 0)
        {
            return false;
        }
    }
    return true;
}


int predictNextRecursive(vector<int> sequence)
{
    vector<int> diffVec;
    for(int i = 1; i < sequence.size(); i++)
    {
        diffVec.push_back(sequence[i] - sequence[i-1]);
    }

    if(!isAllZeros(diffVec))
    {

        return sequence[sequence.size()-1] + predictNextRecursive(diffVec);
    }
    return sequence[sequence.size()-1];
}


long long pt1(vector<vector<int>>& numbers)
{
    long long ans = 0;
    for(auto& numVec : numbers)
    {
        int next = predictNextRecursive(numVec);
        ans += static_cast<long long>(next);
    }
    return ans;
}


int predictNextRecursive2(vector<int> sequence)
{
    vector<int> diffVec;
    for(int i = 1; i < sequence.size(); i++)
    {
        diffVec.push_back(sequence[i] - sequence[i-1]);
    }

    if(!isAllZeros(diffVec))
    {

        return sequence[0] - predictNextRecursive2(diffVec);
    }
    return sequence[0];
}


long long pt2(vector<vector<int>>& numbers)
{
    long long ans = 0;
    for(auto& numVec : numbers)
    {
        int next = predictNextRecursive2(numVec);

        ans += static_cast<long long>(next);

    }
    return ans;
}


int main()
{
    string filename = "input.txt";
    fstream file(filename);
    if(!file.is_open())
    {
        cerr << "Error! Could not open file..." << endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<int>> numbers;
    string line;

    while(getline(file, line))
    {
        vector<int> localNumbers;
        istringstream iss(line);
        int num;
        while(iss >> num)
        {
            localNumbers.push_back(num);
        }
        numbers.push_back(localNumbers);
    }

    auto pt1Ans = pt1(numbers);
    cout << "Pt1 " << pt1Ans << endl;


    auto pt2Ans = pt2(numbers);
    cout << "Pt2: " << pt2Ans << endl;

}