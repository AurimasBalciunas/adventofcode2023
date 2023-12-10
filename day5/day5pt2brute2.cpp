#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <unordered_map>
#include <thread>
#include <mutex>

using namespace std;
using MapEntry = tuple<signed long long, signed long long, signed long long>;

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

void bruteForce(tuple<signed long long, signed long long> &seedtuple, vector<MapProcessor *> &processedMaps, signed long long& lowestFinal, mutex & m)
{
    cout << endl << endl << "Investigating seed tuple " << get<0>(seedtuple) << " " << get<1>(seedtuple) << endl;
    signed long long from = get<0>(seedtuple);
    signed long long to = from + get<1>(seedtuple) - 1;
    cout << "Going from " << from  << " to " << to << endl;
    signed long long lowest= INT_MAX;
    for(auto seed = from; seed <= to; seed++)
    {
        if (seed%1000000 == 0)
        {
            cout << "On seed " << seed << endl;
        }
        auto current = seed;
        for(auto & map : processedMaps)
        {
            current = map->getMappedValue(current);
        }
        lowest = min(lowest, current);
        //cout << "Final score for seed " << seed << " was " << current << endl;
    }

    m.lock();
    lowestFinal = min(lowestFinal, lowest);
    m.unlock();
    cout << "lowest Final now  " << lowestFinal <<endl;

    
    cout << endl;
}


int main()
{
    string filename = "input.txt";
    ifstream file(filename);
    if(!file)
    {
        throw(runtime_error("File not found: " + filename));
    }
    string line;

    vector<tuple<signed long long, signed long long>> seeds;
    vector<MapProcessor *> processedMaps;
    while(getline(file, line))
    {
        if(line.substr(0,5) == "seeds")
        {
            string seedString = line.substr(6);
            //cout << seedString;
            istringstream iss(seedString);
            signed long long num;
            signed long long num2;
            while (iss >> num >> num2) {
                seeds.push_back({num, num2});
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
    mutex m;
    vector<thread> threads;
    for(auto seedtuple : seeds)
    {
        bruteForce(seedtuple, processedMaps, lowestFinal, m);
    }
    // for(auto seedtuple : seeds)
    // {
    //     threads.emplace_back(bruteForce, ref(seedtuple), ref(processedMaps),ref(lowestFinal), ref(m));
    // }

    // for (auto& thread : threads) {
    //     thread.join();
    // }

    cout << "Lowest score seen was " << lowestFinal << endl;

    for(auto map : processedMaps)
    {
        delete(map);
    }

    return 0;
}

//pt1: 
//55841637 too low

//pt2:
//315920268 too high
//answer:108956227
//so not 541202251
//