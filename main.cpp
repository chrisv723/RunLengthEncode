/*
 * main.cpp
 *
 *  Created on: Mar 20, 2020
 *  Author: Christopher
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class runLength
{
public:
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int whichMethod;
	string nameEncodeFile;
	runLength()
	{
		numRows = -1;
		numCols = -1;
		minVal = -1;
		maxVal = -1;
		whichMethod = -1;
		nameEncodeFile = "";
	}

	void encodeMethod1(ifstream &in, ofstream &encode) // encodes zeros and does not implement wrap around
	{ 
		string numRows, numCols, minVal, maxVal, curVal, nextVal;

		if (in.is_open() && !in.eof())
		{
			in >> numRows;
			in >> numCols;
			in >> minVal;
			in >> maxVal;
			encode << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
			int row = 0;
			while (row < stoi(numRows))
			{

				int col = 0;
				int length = 1;
				in >> curVal;
				encode << row << " " << col << " " << curVal << " ";

				while (col < stoi(numCols) - 1)
				{
					col++;

					in >> nextVal;
					if (stoi(nextVal) == stoi(curVal) && (col - 1 < stoi(numCols)))
					{
						length++;
					}
					else
					{
						encode << length << endl;
						curVal = nextVal;
						length = 1;
						encode << row << " " << col << " " << curVal << " ";
					}
				}
				encode << length << endl;
				row++;
			}
		}
	}

	void encodeMethod4(ifstream &in, ofstream &encode) // dont encode zeroes and implement wrap around
	{

		if (!in.eof())
		{
			string numRows, numCols, minVal, maxVal, curVal, nextVal;
			in >> numRows;
			this->numRows = stoi(numRows);
			in >> numCols;
			this->numCols = stoi(numCols);
			in >> minVal;
			this->minVal = stoi(minVal);
			in >> maxVal;
			this->maxVal = stoi(maxVal);
			encode << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;

			int zeroCnt = 0;
			int row = 0;
			int col = 0;
			int length = 0;
			int lastVal = skipZeros(in, row, col, zeroCnt);
			length++;
			encode << endl;
			encode << row << " " << col << " " << lastVal << " ";
			while (!in.eof())
			{
				int nextVal = skipZeros(in, row, col, zeroCnt);

				if (nextVal == lastVal)
				{
					length++;
					lastVal = nextVal;
				}
				else
				{
					encode << length << endl;
					encode.flush();

					lastVal = nextVal;
					length = 1;
					encode << row << " " << col << " " << lastVal << " ";
					encode.flush();
				}
				//lastVal = nextVal;
			}
			encode << length << endl;
			encode.flush();
		}
		in.close();
		encode.close();
	}

	int skipZeros(ifstream &in, int &row, int &col, int &zeroCnt)
	{

		string curVal;
		string pixelVal;
		zeroCnt = 0;
		if (row == 0 && col == 0)
		{
		}
		else
			col++;
		in >> curVal;

		if (col >= this->numCols)
		{
			col = 0;
			row++;
		}
		while (stoi(curVal) == 0 && !in.eof())
		{

			col++;
			in >> curVal;

			if (stoi(curVal) == 0)
				zeroCnt = zeroCnt + 1;

			if (col >= this->numCols)
			{
				col = 0;
				row++;
			}
			if (stoi(curVal) != 0)
			{
				pixelVal = curVal;
			}
		}
		if (stoi(curVal) != 0)
		{
			pixelVal = curVal;
		}
		return stoi(pixelVal);
	}
};

int main(int argc, char *argv[])
{

	ifstream inFile;
	ofstream outFile;
	inFile.open(argv[1]);
	outFile.open(argv[3]);
	string numRows, numCols, minVal, maxVal;

	runLength *code = new runLength();
	string whichMethod = argv[2];
	string nameEncodeFile = argv[1];

	int whichM = stoi(whichMethod);
	string test;
	test.append(nameEncodeFile.substr(0, nameEncodeFile.find_first_of(".", 0)));
	test.append("_EncodeMethod" + whichMethod + ".txt");

	ofstream encodeFile;

	switch (whichM)
	{
	case 1:
		cout << "method 1";
		encodeFile.open(test.c_str());
		encodeFile << "File: " << nameEncodeFile << " Method: " << whichM << endl;
		code->encodeMethod1(inFile, encodeFile);
		break;

	case 4:
		cout << "method 4";
		encodeFile.open(test.c_str());
		encodeFile << "File: " << nameEncodeFile << " Method: " << whichM << endl;
		code->encodeMethod4(inFile, encodeFile);
		break;

	default:
		cout << "Wrong Input";
	}
	inFile.close();
	encodeFile.close();
	return 0;
}
