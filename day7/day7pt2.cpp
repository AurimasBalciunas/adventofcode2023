#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

using namespace std;

enum class CardType
{
    kA = 14,
    kK = 13,
    kQ = 12,
    kJ = 1, // J = joker = worth 1 for part 2
    kT = 10,
    k9 = 9,
    k8 = 8,
    k7 = 7,
    k6 = 6,
    k5 = 5,
    k4 = 4,
    k3 = 3,
    k2 = 2
};

unordered_map<char, CardType> CARD_TO_VAL{
    {'A', CardType::kA},
    {'K', CardType::kK},
    {'Q', CardType::kQ},
    {'J', CardType::kJ},
    {'T', CardType::kT},
    {'9', CardType::k9},
    {'8', CardType::k8},
    {'7', CardType::k7},
    {'6', CardType::k6},
    {'5', CardType::k5},
    {'4', CardType::k4},
    {'3', CardType::k3},
    {'2', CardType::k2}
};

enum class HandType
{
    kFiveOfAKind = 6,
    kFourOfAKind = 5,
    kFullHouse = 4,
    kThreeOfAKind = 3,
    kTwoPair = 2,
    kOnePair = 1,
    kHighCard = 0
};

struct Hand
{
    int bid;
    HandType handType;
    string hand;
    long long valueRepresentation;

    bool operator<(const Hand& s) const
    {
        if(this->handType == s.handType)
        {
            return this->valueRepresentation < s.valueRepresentation;
        }
        return static_cast<int>(this->handType) < static_cast<int>(s.handType);
    }

    HandType DetermineHandType(string hand)
    {
        unordered_map<CardType, int> types;
        int jokers = 0;
        int elements = 0;

        int mostCommonCount = -1;
        CardType mostCommonCard = CardType::k2; // arbitrary
        
        for(const auto& ch: hand)
        {
            CardType ct = CARD_TO_VAL[ch];
            if(ct == CardType::kJ)
            {
                jokers++;
                continue;
            }

            if(types.find(ct) == types.end())
            {
                types[ct] = 0;
                elements++;
            }
            types[ct]++;
            if(types[ct] > mostCommonCount)
            {
                mostCommonCount = types[ct];
                mostCommonCard = ct;
            }
        }

        // 1 type or all jokers
        if(elements <= 1)
        {
            return HandType::kFiveOfAKind;
        }

        // Add jokers to the most common card
        types[mostCommonCard] += jokers;

        if(elements == 2)
        {
            int hasFour = false;
            for (const auto& pair : types) 
            {
                if(pair.second == 4)
                {
                    hasFour = true;
                    break;       
                }
            }
            return hasFour ? HandType::kFourOfAKind : HandType::kFullHouse;
        }

        if(elements == 3)
        {
            bool hasThree = false;
            for (const auto& pair : types) 
            {
                if(pair.second == 3)
                {
                    hasThree = true;
                    break;       
                }
            }
            return hasThree ? HandType::kThreeOfAKind : HandType::kTwoPair;
        }

        if(elements == 4)
        {
            return HandType::kOnePair;
        }

        return HandType::kHighCard;
    }

    long long DetermineValueRepresentation(string hand)
    {
        long long val = 0;
        for(const auto& ch: hand)
        {
            CardType ct = CARD_TO_VAL[ch];
            val *= 100;
            val += static_cast<long long>(ct);
        }

        return val;
    }

    Hand(string hand, int bidAmount)
    {
        this->bid = bidAmount;
        this->hand = hand;
        this->handType = DetermineHandType(hand);
        this->valueRepresentation = DetermineValueRepresentation(hand);
    }
};

void pt2(vector<Hand>& handVec)
{
    sort(handVec.begin(), handVec.end());
    long long total = 0;
    int multiplier = 1;
    for(const auto& hand : handVec)
    {
        total+=(hand.bid * multiplier);
        multiplier++;
    }
    cout << "Total was " << total;
}

int main()
{
    string filename = "input.txt";

    vector<Hand> handVec;

    string hand;
    int bidAmount;
    ifstream file(filename);
    while(file >> hand >> bidAmount)
    {
        Hand temp{hand, bidAmount};
        handVec.push_back(temp);
    }

    for(const auto& hand : handVec)
    {
        cout << "Hand: " << hand.hand << ",  handType: " << static_cast<int>(hand.handType) << ", valueRepresentation: " << hand.valueRepresentation << endl;
    }

    pt2(handVec);


}

// 248923854 too high (messed up ordering in enum class HandType)
// 248453531 just right 