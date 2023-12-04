#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

using namespace std;

int main()
{
    // file open and error check
    string filename = "input.txt";
    ifstream file(filename);
    if(!file)
    {
        throw(runtime_error("File not found: " + filename));
    }
    int totalGames = 0;
    string line;

    while(getline(file, line))
    {
        totalGames++;
    }
    file.clear();
    file.seekg(0, std::ios::beg); // Move the pointer back to the beginning of the file

    vector<int> multiples(totalGames, 1);
    long long totalCards = 0;
    int game = 0;
    while(getline(file, line))
    {
        //cout << line << endl;
        size_t pos = line.find(':');
        line = line.substr(pos + 2);
        cout << line << endl;
        
        pos = line.find('|');
        string winning = line.substr(0, pos);
        string numbers = line.substr(pos+2);
        //cout << winning << endl << numbers <<endl;
        //cout << numbers[0] << endl << numbers[1] << endl;
        unordered_set<int> winningSet;
        cout << numbers[0] <<endl<< endl;
        for(int i = 0; i < winning.size(); i+=3)
        {
            winningSet.insert(stoi(winning.substr(i, 2)));
        }

        int copiesWon = 0;
        for(int i = 0; i < numbers.size(); i+= 3)
        {
            int num = stoi(numbers.substr(i, 2));
            if(winningSet.find(num)!=winningSet.end())
            {
                copiesWon++;
            }
        }

        cout << copiesWon <<" matching numbers in game  " << game+1 << endl; //+1 to match input

        for(int i = game+1; i < game + 1 + copiesWon && i < totalGames; ++i)
        {
            multiples[i] = multiples[i] + multiples[game];
            cout << "Game " << game+1 << " won " << multiples[game] << " copies of " << i << ", multiples[" << i << "] now " << multiples[i] << endl;
            
        }
        cout << endl;

        totalCards += multiples[game];
        ++game;
    }
    
    cout << "Total cards was  " << totalCards << endl;
    return 0;
}
//940 too low
//1151 too low
