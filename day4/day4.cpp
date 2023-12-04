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

    long long sum = 0;
    int game = 0;
    string line;
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

        int cardValue = 0;
        cout << numbers <<endl;
        for(int i = 0; i < numbers.size(); i+= 3)
        {
            int num = stoi(numbers.substr(i, 2));
            if(winningSet.find(num)!=winningSet.end())
            {
                cout << num << " matched" << endl;
                if(cardValue == 0)
                {
                    cardValue = 1;
                    cout << "Cardvalue is 1" << endl;
                }
                else
                {
                    cardValue *= 2;
                    cout << "Doubling cardvalue" << endl;
                }
            }
        }

        cout << "Game " << game << " worth " << cardValue << endl;
        ++game;
        sum += cardValue;
    }
    
    cout << "Sum was  " << sum << endl;
    return 0;
}
