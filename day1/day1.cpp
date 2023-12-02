#include <iostream>
#include <fstream>
#include <string>

using namespace std;


// using std::isdigit from cctypes felt like cheating
bool isDigit(char ch)
{
	return ((ch - 48 >= 0) && (ch - 48 <= 9));
}


int main()
{	
	// file open and error check
	string filename = "input.txt";
	ifstream file(filename);	
	if(!file)
	{
		throw(std::runtime_error("File not found: " + filename));
	}
	
	long long sum{0};	
	string line;

	// iterate through every line
	while(getline(file, line))
	{
		// iterate through every digit in the line
		int firstDigit  = -1;
		int lastDigit = -1;
		for(int i = 0; i < line.size(); i++)
		{
			if(isDigit(line[i]))
			{
				if(firstDigit == -1)
				{
					firstDigit = line[i] - 48;
				}
				lastDigit = line[i] - 48;
			}
		}

		// error check and add values to overall sum
		if(firstDigit == -1 || lastDigit == -1)
		{
			throw(std::runtime_error("No digits found in line: " + line));
		}
		int lineDigit = firstDigit*10 + lastDigit;
		sum += lineDigit;
		cout << "Added " << lineDigit << " to sum" <<  endl;
	}
	
	cout << "Sum is: " << sum;
	file.close();
	return 0;
}
