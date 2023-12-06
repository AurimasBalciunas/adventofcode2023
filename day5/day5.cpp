#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

using namespace std;
using MapEntry = tuple<signed long long, signed long long, signed long long>;

// Class for fun. All of the questions important logic lives in here
class MapProcessor {
private:
    vector<MapEntry> ranges; //start, end (inclusive), offset
public:
    MapProcessor(vector<MapEntry> initialMapping)
    {
        setRanges(initialMapping);
    }

    void setRanges(vector<MapEntry> initialMapping)
    {
        for(auto& vec : initialMapping)
        {
            signed long long destStart = get<0>(vec);
            signed long long sourceStart = get<1>(vec);
            signed long long rangeLength = get<2>(vec);
            //cout << "Took in " << destStart << " " << sourceStart << " " << rangeLength << endl;
            signed long long start = sourceStart;
            signed long long end = sourceStart + rangeLength - 1;
            signed long long offset = destStart - sourceStart;
            //cout << "Pushing back " << start << " " << end << " " << offset << endl;
            this->ranges.push_back({start, end, offset});
        }
    }

    signed long long getMappedValue(signed long long value)
    {
        for(auto & range: this->ranges)
        {
            auto start = get<0>(range);
            auto end = get<1>(range);
            auto offset = get<2>(range);

            if(start <= value && value <= end)
            {
                return value + offset;
            }
        }
        return value; // if no mapping, return og value
    }
};



int main()
{
    string filename = "input.txt";
    ifstream file(filename);
    if(!file)
    {
        throw(runtime_error("File not found: " + filename));
    }
    string line;

    vector<signed long long> seeds;
    vector<MapProcessor *> processedMaps;
    while(getline(file, line))
    {
        if(line.substr(0,5) == "seeds")
        {
            string seedString = line.substr(6);
            //cout << seedString;
            istringstream iss(seedString);
            signed long long num;
            while (iss >> num) {
                seeds.push_back(num);
            }
        }
        int mapNum = -1;
        //find a new map
        size_t pos = line.find(" map:");
        if (pos != string::npos) {
            //collect its internal mappings
            vector<MapEntry> localMap;
            while (getline(file, line) && !line.empty()) {
                istringstream iss(line);
                signed long long a, b, c;
                if (iss >> a >> b >> c) {
                    localMap.push_back({a, b, c});
                }
            }
            MapProcessor * map = new MapProcessor(localMap);
            processedMaps.push_back(map);
        }
    }

    signed long long lowestFinal = INT_MAX;
    for(auto seed : seeds)
    {
        auto current = seed;
        for(auto map : processedMaps)
        {
            current = map->getMappedValue(current);
        }
        lowestFinal = min(lowestFinal, current);
        cout << "Final score for seed " << seed << " was " << current << endl;
    }

    cout << "Lowest score seen was " << lowestFinal << endl;

    for(auto map : processedMaps)
    {
        delete(map);
    }

    return 0;
}

//55841637 too low