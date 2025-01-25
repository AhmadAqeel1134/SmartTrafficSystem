#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>
#include "vehicle.h"
#include "trafficLight.h"
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
using namespace sf;

set<string> numPlateSet;
int count = 0;
class Lane
{
public:
    int laneCapacity;
    string laneDirection;
    deque<vehicle> vehicleDeque;
    Vector2f initialPosition;
    float spacing;
    float windowHeight;
    float windowWidth;
    float spawnInterval;
    float timeSinceLastSpawn;
    Clock spawnClock;
    Clock speedUpdateClock;
    deque<vehicle> challanedCarsDequeue;
    deque<vehicle> challanedHeavyDeqeue;

    Lane(int capacity, string direction, Vector2f initialPos, float winHeight, float windowWidth, float vehicleSpacing, float spawnInterval)
    {
        srand(time(0));
        this->laneCapacity = capacity;
        this->laneDirection = direction;
        this->initialPosition = initialPos;
        this->windowHeight = winHeight;
        this->windowWidth = windowWidth;
        this->spacing = vehicleSpacing;
        this->spawnInterval = spawnInterval;
        this->timeSinceLastSpawn = 0.0f;
    }

    string genNumPlate()
    {
        string letters = "MNOPQRSTUVWXYZABCDEFGHIJKL";
        string num = "0123458967";
        string numPlate = "";
        for (int i = 0; i < 3; i++)
        {
            int index = rand() % 26;
            numPlate += letters[index];
        }
        for (int i = 0; i < 4; i++)
        {
            int index = rand() % 10;
            numPlate += num[index];
        }
        return numPlate;
    }

    void initializeVehicles(Texture texture[], string type, float speed)
    {
        if (vehicleDeque.size() >= laneCapacity)
            return;

        Time elapsedTime = spawnClock.getElapsedTime();
        if (elapsedTime.asSeconds() - timeSinceLastSpawn >= spawnInterval)
        {
            string numPlate = genNumPlate();
            Vector2f position = initialPosition;
            if (laneDirection == "North" || laneDirection == "South")
            {
                if (!vehicleDeque.empty())
                {
                    const vehicle &lastVehicle = vehicleDeque.back();
                    if (laneDirection == "North")
                    {
                        position.y = lastVehicle.position.y - spacing;
                    }
                    else
                    {
                        position.y = lastVehicle.position.y + spacing;
                    }
                }
            }
            else if (laneDirection == "East" || laneDirection == "West")
            {
                if (!vehicleDeque.empty())
                {
                    const vehicle &lastVehicle = vehicleDeque.back();
                    if (laneDirection == "East")
                    {
                        position.x = lastVehicle.position.x + spacing;
                    }
                    else
                    {
                        position.x = lastVehicle.position.x - spacing;
                    }
                }
            }

            int index = rand() % 4;
            vehicleDeque.push_back(vehicle(texture[index], type, speed, position, numPlate, laneDirection, false));

            cout << "New car with num plate " << numPlate
                 << " is initialized at time " << elapsedTime.asSeconds()
                 << " with speed " << speed << " and is in lane " << laneDirection << "." << endl;

            timeSinceLastSpawn = elapsedTime.asSeconds();
            spawnClock.restart();
        }
    }

    void initializeHeavyVehicles(Texture &tex, string type, float speed)
    {
        if (vehicleDeque.size() >= laneCapacity)
            return;

        Time elapsedTime = spawnClock.getElapsedTime();
        if (elapsedTime.asSeconds() - timeSinceLastSpawn >= spawnInterval)
        {
            string numPlate = genNumPlate();
            Vector2f position = initialPosition;
            if (laneDirection == "North" || laneDirection == "South")
            {
                if (!vehicleDeque.empty())
                {
                    const vehicle &lastVehicle = vehicleDeque.back();
                    if (laneDirection == "North")
                    {
                        position.y = lastVehicle.position.y - spacing;
                    }
                    else
                    {
                        position.y = lastVehicle.position.y + spacing;
                    }
                }
            }
            else if (laneDirection == "East" || laneDirection == "West")
            {
                if (!vehicleDeque.empty())
                {
                    const vehicle &lastVehicle = vehicleDeque.back();
                    if (laneDirection == "East")
                    {
                        position.x = lastVehicle.position.x + spacing;
                    }
                    else
                    {
                        position.x = lastVehicle.position.x - spacing;
                    }
                }
            }

            vehicleDeque.push_back(vehicle(tex, type, speed, position, numPlate, laneDirection, false));

            cout << "New truck with num plate " << numPlate
                 << " is initialized at time " << elapsedTime.asSeconds()
                 << " with speed " << speed << " and is in lane " << laneDirection << "." << endl;

            timeSinceLastSpawn = elapsedTime.asSeconds();
            spawnClock.restart();
        }
    }

    void initializeEmergencyVehicles(Texture &tex, string type, float speed, float chance)
    {
        // cout<<"High Emergency"<<endl;
        if (vehicleDeque.size() >= laneCapacity)
            return;

        Time elapsedTime = spawnClock.getElapsedTime();

        if (elapsedTime.asSeconds() >= spawnInterval)
        {
            float randomChance = static_cast<float>(rand()) / RAND_MAX;
            if (1)
            {
                string numPlate = genNumPlate();
                Vector2f position = initialPosition;

                if (laneDirection == "North" || laneDirection == "South")
                {
                    if (!vehicleDeque.empty())
                    {
                        const vehicle &lastVehicle = vehicleDeque.back();
                        if (laneDirection == "North")
                            position.y = lastVehicle.position.y - spacing;
                        else
                            position.y = lastVehicle.position.y + spacing;
                    }
                }
                else if (laneDirection == "East" || laneDirection == "West")
                {
                    if (!vehicleDeque.empty())
                    {
                        const vehicle &lastVehicle = vehicleDeque.back();
                        if (laneDirection == "East")
                            position.x = lastVehicle.position.x + spacing;
                        else
                            position.x = lastVehicle.position.x - spacing;
                    }
                }

                vehicleDeque.push_back(vehicle(tex, type, speed, position, numPlate, laneDirection, true));
                cout << "Emergency vehicle spawned: " << numPlate << " in " << laneDirection << endl;
            }

            spawnClock.restart();
        }
    }
    void updateSpeedAfterEveryFiveSeconds()
    {
        if (speedUpdateClock.getElapsedTime().asSeconds() <= 5.0f)
            return;

        for (auto &veh : vehicleDeque)
        {
            float speed = veh.getSpeed() + (5.0f * 0.01f);
            if (!veh.isOutOfOrder)
            {
               // cout << "Speed increased by 5 km/h" << endl;
                veh.setSpeed(speed);
            }
        }

        speedUpdateClock.restart(); // Restart the clock for the next update
    }

    // Below is the "Round Robbin Implementaion" for cars along north,south,east and west lane

    void moveVehiclesAlongNorth(trafficLight &tr)
    {
        for (auto &veh : vehicleDeque)
        {

            if (tr.isLightOn[0] || (veh.position.y > 270 && !tr.isLightOn[0]))
            {
                veh.moveAlongNorth();
                if (veh.isOutOfBoundsForNorth(windowHeight) && veh.type != "Emergency")
                {
                    string numPlate = veh.numPlate;
                    vehicleDeque.pop_front();
                    veh.position.y = 20;
                    veh.sprite.setPosition(veh.position);
                    vehicleDeque.push_back(veh);
                }
            }
        }
    }
    void moveVehiclesAlongSouth(trafficLight &tr)
    {
        for (auto &veh : vehicleDeque)
        {
            if (tr.isLightOn[2] || (veh.position.y < 470 && !tr.isLightOn[2]))
            {
                veh.moveAlongSouth();
                if (veh.isOutOfBoundsForSouth(windowHeight) && veh.type != "Emergency")
                {
                    string numPlate = veh.numPlate;
                    vehicleDeque.pop_front();
                    veh.position.y = 780;
                    veh.sprite.setPosition(veh.position);
                    vehicleDeque.push_back(veh);
                }
            }
        }
    }

    void moveVehiclesAlongWest(trafficLight &tr)
    {
        for (auto &veh : vehicleDeque)
        {
            if (tr.isLightOn[1] || (veh.position.x > 345 && !tr.isLightOn[1]))
            {
                veh.moveAlongWest();
                if (veh.isOutOfBoundsForWest(windowWidth) && veh.type != "Emergency")
                {
                    string numPlate = veh.numPlate;
                    vehicleDeque.pop_front();
                    veh.position.x = 20;
                    veh.sprite.setPosition(veh.position);
                    vehicleDeque.push_back(veh);
                }
            }
        }
    }
    void moveVehiclesAlongEast(trafficLight &tr)
    {
        for (auto &veh : vehicleDeque)
        {
            if (tr.isLightOn[3] || (veh.position.x < 440 && !tr.isLightOn[3]))
            {
                veh.moveAlongEast();
                if (veh.isOutOfBoundsForEast(windowWidth) && veh.type != "Emergency")
                {
                    string numPlate = veh.numPlate;
                    vehicleDeque.pop_front();
                    veh.position.x = 780;
                    veh.sprite.setPosition(veh.position);
                    vehicleDeque.push_back(veh);
                }
            }
        }
    }

    void drawVehicles(RenderWindow &window, Font &font)
    {
        for (auto &veh : vehicleDeque)
        {
            veh.draw(window);
            veh.drawSpeed(window, font);
        }
    }

    void checkChallanStatus()
    {
        deque<vehicle> remainingVehicles;

        for (auto &veh : vehicleDeque)
        {
            float s = veh.speed / 0.01f;
            if (veh.type == "Regular" && s > 60 && !veh.challanStatus)
            {
                veh.challanStatus = true;
                writeChallanOnFile(veh, s);
                veh.hasChallanWritten = true;
                challanedCarsDequeue.push_back(veh);
                cout << "Regular car with number plate " << veh.numPlate
                     << " has been challaned for overspeeding." << endl;
            }
            else if (veh.type == "Heavy" && s > 40 && !veh.challanStatus)
            {
                veh.challanStatus = true;
                writeChallanOnFile(veh, s);
                veh.hasChallanWritten = true;
                challanedHeavyDeqeue.push_back(veh);
                cout << "Heavy vehicle with number plate " << veh.numPlate
                     << " has been challaned for overspeeding." << endl;
            }
            else
            {
                remainingVehicles.push_back(veh);
            }
        }

        vehicleDeque = remainingVehicles;
    }

    void writeChallanOnFile(vehicle &veh, float speed)
    {
        int fd = open("ModuleOneChallanLog.txt", O_WRONLY | O_APPEND);
        if (fd == -1)
        {
            cout << "Error 404: Can't log challan details on file" << endl;
        }
        else
        {
            string speedToWrite = to_string(static_cast<int>(speed)) + " km/h";
            string toWrite = "Challan Issued to Car-> Type : " + veh.type + " , Num Plate: " + veh.numPlate + " at speed " + speedToWrite + " in lane: " + veh.laneDir + "\n";
            write(fd, toWrite.c_str(), toWrite.length());
            close(fd);
        }
    }

    void resetSpeed()
    {
        for (auto &veh : vehicleDeque)
        {
            float s = veh.getSpeed() / 0.01f;
            if (veh.type == "Regular" && s > 60)
                veh.setSpeed(10.0f * 0.01f);

            else if (veh.type == "Heavy" && s > 40)
                veh.setSpeed(10.0f * 0.01f);
        }
    }
    // // we will use Inter Process Communication to write challan
    // void writeChallanOnFile(vehicle &veh, float speed)
    // {

    //     int fdPipe[2];
    //     int pipeId=pipe(fdPipe);
    //     int id = fork();
    //     if (id > 0)
    //     {
    //         // child process
    //         close(fdPipe[0]);
    //         string speedToWrite = to_string(static_cast<int>(speed)) + " km/h";
    //         string toWrite = "Challan Issued to Car-> Type : " + veh.type + " , Num Plate: " + veh.numPlate + " at speed " + speedToWrite + " in lane: " + veh.laneDir + "\n";
    //         write(fdPipe[1], toWrite.c_str(), toWrite.length());
    //         close(fdPipe[1]);
    //         exit(0);
    //     }
    //     else if (id == 0)
    //     {
    //         // parent process
    //         close(fdPipe[1]);
    //         char buffer[800];
    //         int bytesRead = read(fdPipe[0], buffer, sizeof(buffer) - 1);
    //         if (bytesRead > 0)
    //         {
    //             buffer[bytesRead] = '\0';
    //             int fd = open("ModuleOneChallanLog.txt", O_WRONLY | O_APPEND);
    //             if (fd == -1)
    //                 cout << "Error 404: Can't log challan details on file" << endl;
    //             else
    //             {
    //                 write(fd,buffer,bytesRead);
    //             }
    //         }
    //         close(fdPipe[0]);
    //     }
    // }

    void outOfOrder()
    {
        int count = 0;
        for (auto &veh : vehicleDeque)
        {
            if (rand() % vehicleDeque.size() == 0)
            {
                if (veh.isOutOfOrder == true)
                    continue;
                else
                {
                    veh.isOutOfOrder = true;
                    veh.sprite.setColor(Color::Red);
                    count++;
                    cout << "Vehicle with num plate " << veh.numPlate << " is out of order" << endl;
                }
            }
        }
        if (count == 0 && !vehicleDeque.empty())
        {

            vehicleDeque[1].isOutOfOrder = true;
            vehicleDeque[1].sprite.setColor(Color::Red);
        }
    }

    void takeOutOfOrder(Lane *toeTruckLanes)
    {
        for (auto &veh : vehicleDeque)
        {
            if (veh.isOutOfOrder)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (auto &v : toeTruckLanes[i].vehicleDeque)
                    {
                        if (veh.sprite.getGlobalBounds().intersects(v.sprite.getGlobalBounds()) && veh.laneDir == v.laneDir)
                        {
                           // cout << "collision hogaye" << endl;
                            v.sprite.setPosition(200, 200);
                            veh.sprite.setPosition(200, 200);
                            break;
                        }
                    }
                }
            }
        }
    }


            // if (veh.laneDir == "West" && veh.position.x > 0 && veh.position.x < 280 && veh.type == "Emergency")
            // {
            //     cout << "Lane Dir = " << veh.laneDir << " , Vehicle Position = " << veh.position.x << " , Vehicle Type = " << veh.type << endl;
            //     return 2;
            // }
            // else if (veh.laneDir == "East" && veh.position.x > 360 && veh.position.x < 800 && veh.type == "Emergency")
            // {
            //     cout << "Lane Dir = " << veh.laneDir << " , Vehicle Position = " << veh.position.x << " , Vehicle Type = " << veh.type << endl;
            //     return 3;
            // }
            // else if (veh.laneDir == "South" && veh.position.y > 520 && veh.position.y < 800 && veh.type == "Emergency")
            // {
            //     cout << "Lane Dir = " << veh.laneDir << " , Vehicle Position = " << veh.position.y << " , Vehicle Type = " << veh.type << endl;
            //     return 1;
            // }
    int onTheGreenLight()
    {
        for (auto &veh : vehicleDeque)
        {   
            if (veh.position.y > 0 && veh.position.y < 280 && veh.type == "Emergency")
            {
               // cout << "Lane Dir = " << veh.laneDir << " , Vehicle Position = " << veh.position.y << " , Vehicle Type = " << veh.type << endl;
                return 0;
            }
            
        }
        return 10;
    }
};
