#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

class challanGenInfo
{
public:
    const static int size = 500;
    string types[size];
    string numPlates[size];
    double speeds[size];
    string laneDirs[size];
    double amounts[size];
    string issueDates[size];
    string issueTimes[size];
    string dueDates[size];
    bool hasPay[size];
    int count;

    challanGenInfo()
    {
        count = 0;
        for (int i = 0; i < size; i++)
            hasPay[i] = false;
    }

    string getType(int index)
    {
        return types[index];
    }
    string getNumPlate(int index)
    {
        return numPlates[index];
    }
    double getAmt(int index)
    {
        return amounts[index];
    }
    string getIssueDate(int index)
    {
        return issueDates[index];
    }
    string getIssueTime(int index)
    {
        return issueTimes[index];
    }

    void setHasPay(int index)
    {
        this->hasPay[index]=true;
    }
    bool getHasPay(int index)
    {
        return this->hasPay[index];
    }
    void getDataFromFile(const string &filePath)
    {
       
        ifstream file(filePath);
        if (!file.is_open())
        {
            cerr << "Error opening file!" << endl;
            return;
        }

        string line;
        while (getline(file, line) && count < size)
        {
             cout<<"*****************"<<endl;
            size_t typeStart = line.find("Type : ");
            if (typeStart == string::npos)
                continue;
            typeStart += 7;
            size_t typeEnd = line.find(", Num Plate:");
            if (typeEnd == string::npos)
                continue;

            types[count] = line.substr(typeStart, typeEnd - typeStart);

            size_t numPlateStart = typeEnd + 13;
            size_t numPlateEnd = line.find(", Speed:");
            if (numPlateEnd == string::npos)
                continue;

            numPlates[count] = line.substr(numPlateStart, numPlateEnd - numPlateStart);

            size_t speedStart = numPlateEnd + 8;
            size_t speedEnd = line.find(" km/h, Lane:");
            if (speedEnd == string::npos)
                continue;

            speeds[count] = stod(line.substr(speedStart, speedEnd - speedStart));

            size_t laneStart = speedEnd + 12;
            size_t laneEnd = line.find(", Amount:");
            if (laneEnd == string::npos)
                continue;

            laneDirs[count] = line.substr(laneStart, laneEnd - laneStart);

            size_t amountStart = laneEnd + 9;
            size_t amountEnd = line.find(", Issue Date:");
            if (amountEnd == string::npos)
                continue;

            amounts[count] = stod(line.substr(amountStart, amountEnd - amountStart));

            size_t dateStart = amountEnd + 12;
            size_t dateEnd = line.find(", Issue Time:");
            if (dateEnd == string::npos)
                continue;

            issueDates[count] = line.substr(dateStart, dateEnd - dateStart);

            size_t timeStart = dateEnd + 13;
            size_t timeEnd = line.find(", Due Date:");
            if (timeEnd == string::npos)
                continue;

            issueTimes[count] = line.substr(timeStart, timeEnd - timeStart);

            size_t dueStart = timeEnd + 11;
            if (dueStart < line.size())
                dueDates[count] = line.substr(dueStart);

            count++;
            
        }

        file.close();
    }
    int findChallan(string numPlate)
    {
        for (int i = 0; i < count; i++)
        {
            if (numPlates[i] == numPlate)
            {
                return i;
            }
        }
        return -1;
    }
    void displayData()
    {
        cout<<"^^^^^"<<endl;
        cout<<"count = "<<count<<endl;
        for (int i = 0; i < count; i++)
        {
            cout << "Record " << i + 1 << ":" << endl;
            cout << "Type: " << types[i] << endl;
            cout << "Num Plate: " << numPlates[i] << endl;
            cout << "Speed: " << speeds[i] << " km/h" << endl;
            cout << "Lane: " << laneDirs[i] << endl;
            cout << "Amount: " << amounts[i] << " PKR" << endl;
            cout << "Issue Date: " << issueDates[i] << endl;
            cout << "Issue Time: " << issueTimes[i] << endl;
            cout << "Due Date: " << dueDates[i] << endl;
        }
    }
};