#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

vector<pair<int, int>> parseInput(string filename)
{
    ifstream file(filename);
    string line;
    vector<int> times;
    vector<int> distances;

    if (getline(file, line)) {
        istringstream iss(line.substr(line.find(':') + 1));
        int time;
        while (iss >> time) {
            times.push_back(time);
        }
    }
    if (getline(file, line)) {
        istringstream iss(line.substr(line.find(':') + 1));
        int distance;
        while (iss >> distance) {
            distances.push_back(distance);
        }
    }
    file.close();

    vector<pair<int, int>> ans;
    for(int i = 0; i < times.size(); i++)
    {
        ans.push_back(make_pair(times[i], distances[i]));
    }

    return ans;
}

int countDigits(int num)
{
    int digits = 0;
    while(num > 0)
    {
        num/=10;
        ++digits;
    }
    return digits;
}

pair<long long, long long> parseInputPt2(string filename)
{
    ifstream file(filename);
    string line;
    vector<int> times;
    vector<int> distances;

    if (getline(file, line)) {
        istringstream iss(line.substr(line.find(':') + 1));
        int time;
        while (iss >> time) {
            times.push_back(time);
        }
    }
    if (getline(file, line)) {
        istringstream iss(line.substr(line.find(':') + 1));
        int distance;
        while (iss >> distance) {
            distances.push_back(distance);
        }
    }
    file.close();

    long long totalTime = 0;

    for(int time : times)
    {
        auto digits = countDigits(time);
        cout << " Digits was  " <<digits <<endl;
        totalTime *= pow(10, digits);
        totalTime += time;
        cout << "time now " << totalTime << endl;
    }

    long long totalDistance = 0;
    for(int distance : distances)
    {
        auto digits = countDigits(distance);
        totalDistance *= pow(10, digits);
        totalDistance += distance;
    }

    return make_pair(totalTime, totalDistance);
}

void pt1(vector<pair<int,int>> input)
{
    int total = 1;
    for(auto& elem : input)
    {
        int time = elem.first;
        int distance = elem.second;

        // Run binary search once to find minimum amt of time on button
        int minTime = -1;
        int low = 0;
        int high = time;
        while(low <= high)
        {
            int mid = low + (high - low)/2;
            
            int speedReached = mid;
            int timeLeft = time - mid;
            int distanceTraveled = speedReached * timeLeft;
            if(distanceTraveled >= distance)
            {
                minTime == -1 ? minTime = mid : minTime = min(minTime, mid);
                high = mid-1;
            }
            else
            {
                low = mid + 1;
            }
            cout << " mid " << mid << " speed reached " << speedReached << " time left " << timeLeft << " distance traveled" << distanceTraveled << endl;
        }
        cout << "Min time reached was " << minTime << endl;
        
        // Run binary search again to find maximum amt time on button
        int maxTime = -1;
        low = minTime;
        high = time;
        while(low <= high)
        {
            int mid = low + (high - low)/2;
            
            int speedReached = mid;
            int timeLeft = time - mid;
            int distanceTraveled = speedReached * timeLeft;
            if(distanceTraveled >= distance)
            {
                maxTime == -1 ? maxTime = mid : maxTime = max(maxTime, mid);
                low = mid+1;
            }
            else
            {
                
                high = mid - 1;
            }
            cout << " mid " << mid << " speed reached " << speedReached << " time left " << timeLeft << " distance traveled" << distanceTraveled << endl;

        }
        cout << "Max time reached was " << maxTime << endl << endl << endl;
        if(maxTime == -1 || minTime == -1)
        {
            cout << "Could not find time" << endl;
            exit(EXIT_FAILURE);
        }
        total *= (maxTime - minTime + 1);

    }
    cout << "PT 1 Total: " << total << endl;
}

void pt2(long long time, long long distance)
{
    long long total = 1;

    // Run binary search once to find minimum amt of time on button
    long long minTime = -1;
    long long low = 0;
    long long high = time;
    while(low <= high)
    {
        long long mid = low + (high - low)/2;
        
        long long speedReached = mid;
        long long timeLeft = time - mid;
        long long distanceTraveled = speedReached * timeLeft;
        if(distanceTraveled >= distance)
        {
            minTime == -1 ? minTime = mid : minTime = min(minTime, mid);
            high = mid-1;
        }
        else
        {
            low = mid + 1;
        }
        cout << " mid " << mid << " speed reached " << speedReached << " time left " << timeLeft << " distance traveled" << distanceTraveled << endl;
    }
    cout << "Min time reached was " << minTime << endl;
    
    // Run binary search again to find maximum amt time on button
    long long maxTime = -1;
    low = minTime;
    high = time;
    while(low <= high)
    {
        long long mid = low + (high - low)/2;
        
        long long speedReached = mid;
        long long timeLeft = time - mid;
        long long distanceTraveled = speedReached * timeLeft;
        if(distanceTraveled >= distance)
        {
            maxTime == -1 ? maxTime = mid : maxTime = max(maxTime, mid);
            low = mid+1;
        }
        else
        {
            
            high = mid - 1;
        }
        cout << " mid " << mid << " speed reached " << speedReached << " time left " << timeLeft << " distance traveled" << distanceTraveled << endl;

    }
    cout << "Max time reached was " << maxTime << endl << endl << endl;
    long long marginOfError = maxTime - minTime + 1;
    if(maxTime == -1 || minTime == -1)
    {
        cout << "Could not find time" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Total ways to beat " << marginOfError << endl;
}

int main()
{
    auto timeAndDist = parseInput("input.txt");
    pt1(timeAndDist);

    auto timeAndDist2 = parseInputPt2("input.txt");
    pt2(timeAndDist2.first, timeAndDist2.second);
    return -1;


    return 0;

}