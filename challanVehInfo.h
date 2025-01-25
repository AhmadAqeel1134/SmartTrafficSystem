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

class challanedVehInfo
{
public:
    const static int size = 64;
    string types[size];
    string numPlates[size];
    double speeds[size];
    string laneDirs[size];
    string issueDates[size];
    string issueTimes[size];
    string dueDates[size];
    bool hasChallanGen[size];
    bool challanPaidStatus[size];
    int count;

    challanedVehInfo()
    {
        count = 0;
        for (int i = 0; i < size; i++)
        {
            hasChallanGen[i] = false;
            challanPaidStatus[i]=false;
        }
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
            size_t typeStart = line.find("Type : ") + 7;
            size_t typeEnd = line.find(", Num Plate:");
            types[count] = line.substr(typeStart, typeEnd - typeStart);

            size_t numPlateStart = typeEnd + 13;
            size_t numPlateEnd = line.find(" at speed ");
            numPlates[count] = line.substr(numPlateStart, numPlateEnd - numPlateStart);

            size_t speedStart = numPlateEnd + 10;
            size_t speedEnd = line.find(" km/h in lane: ");
            speeds[count] = stod(line.substr(speedStart, speedEnd - speedStart));

            size_t laneStart = speedEnd + 14;
            laneDirs[count] = line.substr(laneStart);

            count++;
        }

        file.close();
    }

    int isValidNumPlate(string numPlate)
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
    string getCurrentDate()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        string day = to_string(ltm->tm_mday);
        string month = to_string(ltm->tm_mon + 1);
        string year = to_string(1900 + ltm->tm_year);

        if (day.length() == 1)
            day = "0" + day;
        if (month.length() == 1)
            month = "0" + month;

        return day + "/" + month + "/" + year;
    }

    string getCurrentTime()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        string hour = to_string(ltm->tm_hour);
        string min = to_string(ltm->tm_min);
        string sec = to_string(ltm->tm_sec);

        if (hour.length() == 1)
            hour = "0" + hour;
        if (min.length() == 1)
            min = "0" + min;
        if (sec.length() == 1)
            sec = "0" + sec;

        return hour + ":" + min + ":" + sec;
    }

    string getDueDate(string issueDate)
    {
        int day, month, year;
        sscanf(issueDate.c_str(), "%d/%d/%d", &day, &month, &year);

        day += 3;

        if (day > 30) // Simplified for 30-day months
        {
            day -= 30;
            month += 1;
        }
        if (month > 12)
        {
            month = 1;
            year += 1;
        }

        string dueDay = to_string(day);
        string dueMonth = to_string(month);
        string dueYear = to_string(year);

        if (dueDay.length() == 1)
            dueDay = "0" + dueDay;
        if (dueMonth.length() == 1)
            dueMonth = "0" + dueMonth;

        return dueDay + "/" + dueMonth + "/" + dueYear;
    }

    void generateChallan(string numPlate)
    {
        int index = isValidNumPlate(numPlate);
        if (index == -1)
            return;

        int fd = open("ChallanGenerated.txt", O_WRONLY | O_APPEND);
        if (fd == -1)
        {
            cout << "Error 404: Can't log challan details on file" << endl;
        }
        else if (hasChallanGen[index] == 0)
        {
            string speedToWrite = to_string(static_cast<int>(speeds[index])) + " km/h";
            int amt;
            if (types[index] == "Regular ")
            {
                amt = 5850; // after service charges
            }
            else if (types[index] == "Heavy ")
            {
                amt = 8190; // after service charges
            }
            string amtToWrite = to_string(amt) + " PKR";
            string issueDate = getCurrentDate();
            string issueTime = getCurrentTime();
            string dueDate = getDueDate(issueDate);

            issueDates[index] = issueDate;
            issueTimes[index] = issueTime;
            dueDates[index] = dueDate;

            string toWrite = "Challan Generated For Car-> Type :" + types[index] + ", Num Plate:" + numPlates[index] + ", Speed:" + speedToWrite + ", Lane:" + laneDirs[index] + ", Amount:" + amtToWrite + ", Issue Date:" + issueDate +
                             ", Issue Time:" + issueTime + ", Due Date:" + dueDate + "\n";

            write(fd, toWrite.c_str(), toWrite.length());
            hasChallanGen[index] = true;
            close(fd);
        }
    }

    void displayData()
    {
        for (int i = 0; i < count; ++i)
        {
            cout << "Type: " << types[i] << endl;
            cout << ", Num Plate: " << numPlates[i] << endl;
            cout << ", Speed: " << speeds[i] << endl;
            cout << ", Lane: " << laneDirs[i] << endl;
            cout << ", Issue Date: " << issueDates[i] << endl;
            cout << ", Issue Time: " << issueTimes[i] << endl;
            cout << ", Due Date: " << dueDates[i] << endl;
        }
    }
};
