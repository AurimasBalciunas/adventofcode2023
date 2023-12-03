#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

// define trienode and tree helpers

class TrieNode 
{
public:
    TrieNode* trie;
    TrieNode* children[26];
    bool isWord;
    int value;
    TrieNode() 
    {
        isWord = false;
        value = 0;
        for(auto &child : children)
        {
            child = nullptr;
        }
    }
};


class Trie 
{
private:
    TrieNode* root;
public:
    Trie() 
    {
        root = new TrieNode();
    }

    void insert(string word, int value) 
    {
        TrieNode* curr = root;
        for(auto &letter : word){
            if(curr->children[letter - 'a'] == nullptr)
            {
                curr->children[letter - 'a'] = new TrieNode();
            }
            curr = curr->children[letter - 'a'];
        }
        curr->isWord = true;
        curr->value = value;
    }

    pair<bool, int> isWord(string word)
    {
        TrieNode* curr = root;
        for(auto &letter : word)
        {
            if(curr->children[letter - 'a'] == nullptr)
            {
                return make_pair(false, 0);
            }
            curr = curr->children[letter - 'a'];
        }
        if(!curr->isWord)
        {
            return make_pair(false, 0);
        }
        return make_pair(true, curr->value);

    }

    bool search(string word)
    {
        TrieNode* curr = root;
        for(auto &letter : word)
        {
            if(curr->children[letter - 'a'] == nullptr)
            {
                return false;
            }
            curr = curr->children[letter - 'a'];
        }
        return true;
    }
};


// using std::isdigit from cctypes still felt like cheating
bool isDigit(char ch)
{
    return ((ch - 48 >= 0) && (ch - 48 <= 9));
}


int main() {
    // constructing trie with possible desired elements
    Trie* trie = new Trie();
    vector<pair<string, int>> vecs{{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};
    for(auto &num : vecs)
    {
        trie->insert(num.first, num.second);
    }

    // file open and error check
    string filename = "input.txt";
    ifstream file(filename);
    if(!file)
    {
        throw(runtime_error("File not found: " + filename));
    }


    long long sum{0};
    string line;
    
    // iterate through every line
    while(getline(file, line))
    {
        int firstDigit = -1;
        int lastDigit = -1;

        for(int i = 0; i < line.size(); i++)
        {
            // first 2ptr while it matches trie entries
            string s(1, line[i]);
            if(trie->search(s))
            {
                int left = i;
                int right = i;
                while(right < line.size() && trie->search(line.substr(left, right - left + 1)))
                {
                    pair<bool, int> wordiness = trie->isWord(line.substr(left, right - left + 1));
                    // every time we find 2ptr selection that is a word, update words based off value
                    // wordiness.first == is it a word, wordiness.second == what is its value, 
                    if(wordiness.first == true)
                    {
                        if(firstDigit == -1)
                        {
                            firstDigit = wordiness.second;
                        }
                        lastDigit = wordiness.second;
                    }
                    right++;
                }
            }
            // otherwise see if digit
            else if(isDigit(line[i]))
            {
                if(firstDigit == -1)
                {
                    firstDigit = line[i] - '0';
                }
                lastDigit = line[i] - '0';
            }
        }
        
        cout << "First digit was " << firstDigit << " and last digit was " << lastDigit << endl;
        sum += firstDigit*10 + lastDigit;
    }
    
    cout << "Sum is " << sum << endl;
    return 0;
}

