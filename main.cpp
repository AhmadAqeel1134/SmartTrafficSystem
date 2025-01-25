#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "lane.h"
#include "background.h"
#include "trafficLight.h"
#include "vehicle.h"
#include "challanVehInfo.h"
#include "genChallanInfo.h"

using namespace sf;
string numPlateInput = "";
void renderChallanWindow();
void renderUserPortalWindow();
void showAnalytics();
bool running = true;
float pixel = 0.01f;
string numPlate;
Texture NorthregularCarTextures[4];
Texture SouthregularCarTextures[4];
Texture WestregularCarTextures[4];
Texture EastregularCarTextures[4];

Texture texN, texS, texW, texE;
Texture ambN, ambS, ambW, ambE;

Texture truckN, truckS, truckW, truckE;

Lane regularVehLanes[4] =
    {
        Lane(10, "North", Vector2f(340, 0), 800, 800, 200.0f, 1.0f),
        Lane(10, "South", Vector2f(460, 800), 800, 800, 200.0f, 2.0f),
        Lane(10, "West", Vector2f(0, 350), 800, 800, 200.0f, 2.0f),
        Lane(10, "East", Vector2f(800, 470), 800, 800, 200.0f, 1.5f),
};

Lane heavyVehLanes[4] =
    {
        Lane(4, "North", Vector2f(390, 0), 800, 800, 200.0f, 15.0f),
        Lane(4, "South", Vector2f(420, 800), 800, 800, 200.0f, 15.0f),
        Lane(4, "West", Vector2f(0, 390), 800, 800, 200.0f, 15.0f),
        Lane(4, "East", Vector2f(800, 430), 800, 800, 200.0f, 15.0f),
};
Lane toeTruckVehLanes[8] =
    {
        Lane(1, "North", Vector2f(390, 0), 800, 800, 200.0f, 20.0f),
        Lane(1, "South", Vector2f(420, 800), 800, 800, 200.0f, 20.0f),
        Lane(1, "West", Vector2f(0, 390), 800, 800, 200.0f, 20.0f),
        Lane(1, "East", Vector2f(800, 430), 800, 800, 200.0f, 20.0f),
        Lane(1, "North", Vector2f(340, 0), 800, 800, 200.0f, 20.0f),
        Lane(1, "South", Vector2f(460, 800), 800, 800, 200.0f, 20.0f),
        Lane(1, "West", Vector2f(0, 350), 800, 800, 200.0f, 20.0f),
        Lane(1, "East", Vector2f(800, 470), 800, 800, 200.0f, 20.0f),

};

Lane emergencyVehLanes[4] = {
    // 15% chance in north and south
    Lane(1, "North", Vector2f(390, 40), 800, 800, 200.0f, 15.0f),
    Lane(1, "South", Vector2f(460, 760), 800, 800, 200.0f, 15.0f),
    // 20% chance in west and east lane
    Lane(1, "West", Vector2f(20, 430), 800, 800, 200.0f, 20.0f),
    Lane(1, "East", Vector2f(800, 390), 800, 800, 200.0f, 20.0f),
};

pthread_mutex_t northLaneMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t southLaneMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t eastLaneMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t westLaneMutex = PTHREAD_MUTEX_INITIALIZER;
trafficLight traffLigObj;

float generateRandomSpeed(float min, float max)
{
    min = min * 0.01f;
    max = max * 0.01f;
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void initializeTextures()
{
    // cout << "Initializing Textures" << endl;
    NorthregularCarTextures[0].loadFromFile("./images/regCar1.jpeg");
    NorthregularCarTextures[1].loadFromFile("./images/regCar2.jpeg");
    NorthregularCarTextures[2].loadFromFile("./images/regCar3.jpeg");
    NorthregularCarTextures[3].loadFromFile("./images/regCar4.jpeg");

    SouthregularCarTextures[0].loadFromFile("./images/carSouth1.jpeg");
    SouthregularCarTextures[1].loadFromFile("./images/carSouth2.jpeg");
    SouthregularCarTextures[2].loadFromFile("./images/carSouth3.jpeg");
    SouthregularCarTextures[3].loadFromFile("./images/carSouth4.jpeg");

    WestregularCarTextures[0].loadFromFile("./images/carWest1.jpeg");
    WestregularCarTextures[1].loadFromFile("./images/carWest2.jpeg");
    WestregularCarTextures[2].loadFromFile("./images/carWest3.jpeg");
    WestregularCarTextures[3].loadFromFile("./images/carWest4.jpeg");

    EastregularCarTextures[0].loadFromFile("./images/carEast1.jpeg");
    EastregularCarTextures[1].loadFromFile("./images/carEast2.jpeg");
    EastregularCarTextures[2].loadFromFile("./images/carEast3.jpeg");
    EastregularCarTextures[3].loadFromFile("./images/carEast4.jpeg");

    texN.loadFromFile("./images/heavyCarNorth.jpeg");
    texS.loadFromFile("./images/heavyCarSouth.jpeg");
    texW.loadFromFile("./images/heavyCarWest.jpeg");
    texE.loadFromFile("./images/heavyCarEast.jpeg");

    ambN.loadFromFile("./images/ambNorth.jpeg");
    ambS.loadFromFile("./images/ambSouth.jpeg");
    ambW.loadFromFile("./images/ambWest.jpeg");
    ambE.loadFromFile("./images/ambEast.jpeg");

    truckN.loadFromFile("./images/northToeTruck.png");
    truckS.loadFromFile("./images/southToeTruck.png");
    truckW.loadFromFile("./images/westToeTruck.png");
    truckE.loadFromFile("./images/eastToeTruck.png");
}

// Initialize vehicles for each lane
void initializeVehicles(string type, float speed)
{
    if (type == "Regular")
    {
        pthread_mutex_lock(&northLaneMutex);
        regularVehLanes[0].initializeVehicles(NorthregularCarTextures, type, speed);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        regularVehLanes[1].initializeVehicles(SouthregularCarTextures, type, speed);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        regularVehLanes[2].initializeVehicles(EastregularCarTextures, type, speed);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        regularVehLanes[3].initializeVehicles(WestregularCarTextures, type, speed);
        pthread_mutex_unlock(&westLaneMutex);
    }
    else if (type == "Heavy")
    {
        pthread_mutex_lock(&northLaneMutex);
        heavyVehLanes[0].initializeHeavyVehicles(texN, type, speed);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        heavyVehLanes[1].initializeHeavyVehicles(texS, type, speed);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        heavyVehLanes[2].initializeHeavyVehicles(texE, type, speed);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        heavyVehLanes[3].initializeHeavyVehicles(texW, type, speed);
        pthread_mutex_unlock(&westLaneMutex);
    }
    else if (type == "Emergency")
    {
        pthread_mutex_lock(&northLaneMutex);
        emergencyVehLanes[0].initializeEmergencyVehicles(ambN, type, speed, 0.2f); // 0.2f=20% chance);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        emergencyVehLanes[1].initializeEmergencyVehicles(ambS, type, speed, 0.05f); // 0.05f=5% chance
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        emergencyVehLanes[2].initializeEmergencyVehicles(ambW, type, speed, 0.3f); // 0.3f=30% chance);
        pthread_mutex_unlock(&westLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        emergencyVehLanes[3].initializeEmergencyVehicles(ambE, type, speed, 0.1f); // 0.1f=10% chance);
        pthread_mutex_unlock(&eastLaneMutex);
    }
    else if (type == "ToeTruck")
    {
        pthread_mutex_lock(&northLaneMutex);
        toeTruckVehLanes[0].initializeHeavyVehicles(truckN, type, speed);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        toeTruckVehLanes[1].initializeHeavyVehicles(truckS, type, speed);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        toeTruckVehLanes[2].initializeHeavyVehicles(truckE, type, speed);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        toeTruckVehLanes[3].initializeHeavyVehicles(truckW, type, speed);
        pthread_mutex_unlock(&westLaneMutex);

        pthread_mutex_lock(&northLaneMutex);
        toeTruckVehLanes[4].initializeHeavyVehicles(truckN, type, speed);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        toeTruckVehLanes[5].initializeHeavyVehicles(truckS, type, speed);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        toeTruckVehLanes[6].initializeHeavyVehicles(truckE, type, speed);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        toeTruckVehLanes[7].initializeHeavyVehicles(truckW, type, speed);
        pthread_mutex_unlock(&westLaneMutex);
    }
    else

        return;
}
void *updateNorthLaneRegular(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&northLaneMutex);
        regularVehLanes[0].moveVehiclesAlongNorth(traffLigObj);
        regularVehLanes[0].checkChallanStatus();
        regularVehLanes[0].updateSpeedAfterEveryFiveSeconds();

        pthread_mutex_unlock(&northLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateSouthLaneRegular(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&southLaneMutex);
        regularVehLanes[1].moveVehiclesAlongSouth(traffLigObj);
        regularVehLanes[1].checkChallanStatus();
        regularVehLanes[1].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&southLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateEastLaneRegular(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&eastLaneMutex);
        regularVehLanes[2].moveVehiclesAlongEast(traffLigObj);
        regularVehLanes[2].checkChallanStatus();
        regularVehLanes[2].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&eastLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateWestLaneRegular(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&westLaneMutex);
        regularVehLanes[3].moveVehiclesAlongWest(traffLigObj);
        regularVehLanes[3].checkChallanStatus();
        regularVehLanes[3].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&westLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateNorthLaneHev(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&northLaneMutex);
        heavyVehLanes[0].moveVehiclesAlongNorth(traffLigObj);
        heavyVehLanes[0].checkChallanStatus();
        heavyVehLanes[0].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&northLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateSouthLaneHev(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&southLaneMutex);
        heavyVehLanes[1].moveVehiclesAlongSouth(traffLigObj);
        heavyVehLanes[1].checkChallanStatus();
        heavyVehLanes[1].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&southLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateEastLaneHev(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&eastLaneMutex);
        heavyVehLanes[2].moveVehiclesAlongEast(traffLigObj);
        heavyVehLanes[2].checkChallanStatus();
        heavyVehLanes[2].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&eastLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateWestLaneHev(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&westLaneMutex);
        heavyVehLanes[3].moveVehiclesAlongWest(traffLigObj);
        heavyVehLanes[3].checkChallanStatus();
        heavyVehLanes[3].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&westLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateNorthLaneAmb(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&northLaneMutex);
        emergencyVehLanes[0].moveVehiclesAlongNorth(traffLigObj);
        emergencyVehLanes[0].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&northLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateSouthLaneAmb(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&southLaneMutex);
        emergencyVehLanes[1].moveVehiclesAlongSouth(traffLigObj);
        emergencyVehLanes[1].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&southLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateEastLaneAmb(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&eastLaneMutex);
        emergencyVehLanes[3].moveVehiclesAlongEast(traffLigObj);
        emergencyVehLanes[3].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&eastLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateWestLaneAmb(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&westLaneMutex);
        emergencyVehLanes[2].moveVehiclesAlongWest(traffLigObj);
        emergencyVehLanes[2].updateSpeedAfterEveryFiveSeconds();
        pthread_mutex_unlock(&westLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateNorthLaneTruck(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&northLaneMutex);
        toeTruckVehLanes[0].moveVehiclesAlongNorth(traffLigObj);
        pthread_mutex_unlock(&northLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateSouthLaneTruck(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&southLaneMutex);
        toeTruckVehLanes[1].moveVehiclesAlongSouth(traffLigObj);
        pthread_mutex_unlock(&southLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateEastLaneTruck(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&eastLaneMutex);
        toeTruckVehLanes[2].moveVehiclesAlongEast(traffLigObj);
        pthread_mutex_unlock(&eastLaneMutex);
        usleep(10000);
    }
    return NULL;
}

void *updateWestLaneTruck(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&westLaneMutex);
        toeTruckVehLanes[3].moveVehiclesAlongWest(traffLigObj);
        pthread_mutex_unlock(&westLaneMutex);
        usleep(10000);
    }
    return NULL;
}

int main()
{
    RenderWindow window(VideoMode(800, 800), "Smart Traffic Management System");
    background roadBackground;
    trafficLight trafficlight;
    Clock timePassed;

    srand(time(0));

    // Initialize textures
    initializeTextures();
    // Load font
    Font font;
    if (!font.loadFromFile("./font/Arial.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // Initialize threads
    pthread_t northThreadReg, southThreadReg, eastThreadReg, westThreadReg;
    pthread_create(&northThreadReg, NULL, updateNorthLaneRegular, NULL);
    pthread_create(&southThreadReg, NULL, updateSouthLaneRegular, NULL);
    pthread_create(&eastThreadReg, NULL, updateEastLaneRegular, NULL);
    pthread_create(&westThreadReg, NULL, updateWestLaneRegular, NULL);

    pthread_t northThreadHev, southThreadHev, eastThreadHev, westThreadHev;
    pthread_create(&northThreadHev, NULL, updateNorthLaneHev, NULL);
    pthread_create(&southThreadHev, NULL, updateSouthLaneHev, NULL);
    pthread_create(&eastThreadHev, NULL, updateEastLaneHev, NULL);
    pthread_create(&westThreadHev, NULL, updateWestLaneHev, NULL);

    pthread_t northThreadAmb, southThreadAmb, eastThreadAmb, westThreadAmb;
    pthread_create(&northThreadAmb, NULL, updateNorthLaneAmb, NULL);
    pthread_create(&southThreadAmb, NULL, updateSouthLaneAmb, NULL);
    pthread_create(&westThreadAmb, NULL, updateWestLaneAmb, NULL);
    pthread_create(&eastThreadAmb, NULL, updateEastLaneAmb, NULL);

    pthread_t northThreadTruck, southThreadTruck, eastThreadTruck, westThreadTruck;
    pthread_create(&northThreadTruck, NULL, updateNorthLaneTruck, NULL);
    pthread_create(&southThreadTruck, NULL, updateSouthLaneTruck, NULL);
    pthread_create(&eastThreadTruck, NULL, updateEastLaneTruck, NULL);
    pthread_create(&westThreadTruck, NULL, updateWestLaneTruck, NULL);

    // min=1.0f,max=60.0f;
    float min = 1.0f, max = 20.0f;
    float regularSpeed = generateRandomSpeed(min, max);
    // min=1.0f,max=40.0f;
    min = 1.0f, max = 20.0f;
    float heavySpeed = generateRandomSpeed(min, max);
    // min=1.0f,max=80.0f;
    min = 1.0f, max = 80.0f;
    float ambSpeed = generateRandomSpeed(min, max);
    min = 20.0f, max = 30.0f;
    float truckSpeed = generateRandomSpeed(min, max);

    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::C)
            {
                renderChallanWindow();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::U)
            {
                renderUserPortalWindow();
            }
        }

        initializeVehicles("Regular", regularSpeed);
        initializeVehicles("Heavy", heavySpeed);
        initializeVehicles("Emergency", ambSpeed);
        initializeVehicles("ToeTruck", truckSpeed);

        window.clear();
        roadBackground.drawRoads(window);

        if (timePassed.getElapsedTime().asSeconds() >= 30.0f)
        {
            int ind = rand() % 4;
            int num = rand() % 100;
            if (num % 2 == 0)
                regularVehLanes[ind].outOfOrder();
            else
                heavyVehLanes[ind].outOfOrder();
            timePassed.restart();
        }

        for (int i = 0; i < 4; i++)
        {
            regularVehLanes[i].checkChallanStatus();
            regularVehLanes[i].resetSpeed();
            heavyVehLanes[i].checkChallanStatus();
            heavyVehLanes[i].resetSpeed();
            regularVehLanes[i].takeOutOfOrder(toeTruckVehLanes);
            heavyVehLanes[i].takeOutOfOrder(toeTruckVehLanes);
        }
        int index = -1;
        index = emergencyVehLanes[0].onTheGreenLight();

        if (index != 10)
        {
            for (int i = 0; i < 4; i++)
            {

                traffLigObj.isLightOn[i] = false;
            }

            traffLigObj.isLightOn[index] = true;
            index = -1;
        }

        traffLigObj.RoundRobbinTrafficLight();
        traffLigObj.displayTrafficLight(window);
        pthread_mutex_lock(&northLaneMutex);
        regularVehLanes[0].drawVehicles(window, font);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        regularVehLanes[1].drawVehicles(window, font);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        regularVehLanes[2].drawVehicles(window, font);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        regularVehLanes[3].drawVehicles(window, font);
        pthread_mutex_unlock(&westLaneMutex);

        pthread_mutex_lock(&northLaneMutex);
        heavyVehLanes[0].drawVehicles(window, font);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        heavyVehLanes[1].drawVehicles(window, font);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        heavyVehLanes[2].drawVehicles(window, font);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        heavyVehLanes[3].drawVehicles(window, font);
        pthread_mutex_unlock(&westLaneMutex);

        pthread_mutex_lock(&northLaneMutex);
        emergencyVehLanes[0].drawVehicles(window, font);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        emergencyVehLanes[1].drawVehicles(window, font);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        emergencyVehLanes[2].drawVehicles(window, font);
        pthread_mutex_unlock(&westLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        emergencyVehLanes[3].drawVehicles(window, font);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&northLaneMutex);
        toeTruckVehLanes[0].drawVehicles(window, font);
        pthread_mutex_unlock(&northLaneMutex);

        pthread_mutex_lock(&southLaneMutex);
        toeTruckVehLanes[1].drawVehicles(window, font);
        pthread_mutex_unlock(&southLaneMutex);

        pthread_mutex_lock(&eastLaneMutex);
        toeTruckVehLanes[2].drawVehicles(window, font);
        pthread_mutex_unlock(&eastLaneMutex);

        pthread_mutex_lock(&westLaneMutex);
        toeTruckVehLanes[3].drawVehicles(window, font);
        pthread_mutex_unlock(&westLaneMutex);

        window.display();
    }

    pthread_join(northThreadReg, NULL);
    pthread_join(southThreadReg, NULL);
    pthread_join(eastThreadReg, NULL);
    pthread_join(westThreadReg, NULL);

    pthread_join(northThreadHev, NULL);
    pthread_join(southThreadHev, NULL);
    pthread_join(eastThreadHev, NULL);
    pthread_join(westThreadHev, NULL);

    pthread_join(northThreadAmb, NULL);
    pthread_join(southThreadAmb, NULL);
    pthread_join(eastThreadAmb, NULL);
    pthread_join(westThreadAmb, NULL);

    pthread_join(northThreadTruck, NULL);
    pthread_join(southThreadTruck, NULL);
    pthread_join(eastThreadTruck, NULL);
    pthread_join(westThreadTruck, NULL);
    return 0;
}

void renderChallanWindow()
{
    RenderWindow challanWindow(sf::VideoMode(750, 500), "Challan Generator");
    challanedVehInfo challanedVehRoad;
    challanedVehRoad.getDataFromFile("./ModuleOneChallanLog.txt");

    Font font;
    if (!font.loadFromFile("./font/Arial.ttf"))
        return;

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./images/pic.jpg"))
        return;

    Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    RectangleShape inputBox(Vector2f(400.f, 50.f));
    inputBox.setFillColor(Color(240, 240, 240));
    inputBox.setOutlineColor(Color(0, 122, 204));
    inputBox.setOutlineThickness(2.f);
    inputBox.setPosition(200.f, 300.f);

    sf::Text inputText(numPlateInput, font, 20);
    inputText.setPosition(215.f, 310.f);
    inputText.setFillColor(Color::Black);

    RectangleShape submitButton(Vector2f(200, 50));
    submitButton.setPosition(300.f, 400.f);
    submitButton.setFillColor(Color(0, 122, 204));

    sf::Text buttonText("Submit", font, 20);
    buttonText.setPosition(360.f, 410.f);
    buttonText.setFillColor(Color::White);

    while (challanWindow.isOpen())
    {
        sf::Event cEvent;
        while (challanWindow.pollEvent(cEvent))
        {
            if (cEvent.type == sf::Event::Closed)
                challanWindow.close();

            if (cEvent.type == sf::Event::KeyPressed)
            {
                if (cEvent.key.code == sf::Keyboard::Backspace && !numPlateInput.empty())
                {
                    numPlateInput.pop_back();
                    inputText.setString(numPlateInput);
                }
            }

            if (cEvent.type == sf::Event::TextEntered)
            {
                if (cEvent.text.unicode >= 32 && cEvent.text.unicode <= 126)
                {
                    numPlateInput += static_cast<char>(cEvent.text.unicode);
                    inputText.setString(numPlateInput);
                }
            }

            if (cEvent.type == sf::Event::MouseButtonPressed && cEvent.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(challanWindow);
                if (submitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    int num = challanedVehRoad.isValidNumPlate(numPlateInput);
                    if (num != -1)
                    {
                        cout << "num" << num << endl;
                        challanedVehRoad.generateChallan(numPlateInput);
                    }
                    challanWindow.close();
                }
            }
        }

        challanWindow.clear();
        challanWindow.draw(backgroundSprite);
        challanWindow.draw(inputBox);
        challanWindow.draw(inputText);
        challanWindow.draw(submitButton);
        challanWindow.draw(buttonText);
        challanWindow.display();
    }
}

void renderUserPortalWindow()
{
    RenderWindow userPortalWindow(sf::VideoMode(1024, 1024), "User Portal");
    challanGenInfo obj;
   obj.getDataFromFile("./ChallanGenerated.txt");
    obj.displayData();
    int index = -1;

    Font font;
    if (!font.loadFromFile("./font/Arial.ttf"))
        return;

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./images/pic2.png"))
        return;

    Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Existing UI Elements
    RectangleShape inputBox(Vector2f(400.f, 50.f));
    inputBox.setFillColor(Color(240, 240, 240));
    inputBox.setOutlineColor(Color(0, 122, 204));
    inputBox.setOutlineThickness(2.f);
    inputBox.setPosition(312.f, 250.f);

    Text inputText("", font, 20);
    inputText.setPosition(190.f, 260.f);
    inputText.setFillColor(Color::Black);

    RectangleShape submitButton(Vector2f(200, 50));
    submitButton.setPosition(412.f, 350.f);
    submitButton.setFillColor(Color(0, 122, 204));

    Text buttonText("Submit", font, 20);
    buttonText.setPosition(450.f, 360.f);
    buttonText.setFillColor(Color::White);

    Text resultText("", font, 16);
    resultText.setPosition(200.f, 420.f);
    resultText.setFillColor(Color::Black);

    std::string numPlateInput = "";
    bool showAmountInput = false;
    std::string enteredAmount = "";

    // Pay UI Elements
    RectangleShape amountInputBox(Vector2f(300.f, 50.f));
    amountInputBox.setFillColor(Color(240, 240, 240));
    amountInputBox.setOutlineColor(Color(0, 122, 204));
    amountInputBox.setOutlineThickness(2.f);
    amountInputBox.setPosition(362.f, 600.f);

    Text amountText("", font, 20);
    amountText.setPosition(amountInputBox.getPosition().x + 10, amountInputBox.getPosition().y + 10);
    amountText.setFillColor(Color::Black);

    RectangleShape payButton(Vector2f(200.f, 50.f));
    payButton.setPosition(412.f, 680.f);
    payButton.setFillColor(Color(0, 122, 204));

    Text payButtonText("Pay", font, 20);
    payButtonText.setPosition(475.f, 690.f);
    payButtonText.setFillColor(Color::White);

    while (userPortalWindow.isOpen())
    {
        sf::Event event;
        while (userPortalWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                userPortalWindow.close();

            // Handle text input for number plate
            if (event.type == sf::Event::TextEntered)
            {
                if (!showAmountInput && event.text.unicode >= 32 && event.text.unicode <= 126)
                {
                    numPlateInput += static_cast<char>(event.text.unicode);
                    inputText.setString(numPlateInput);
                }

                if (showAmountInput && event.text.unicode >= '0' && event.text.unicode <= '9')
                {
                    enteredAmount += static_cast<char>(event.text.unicode);
                    amountText.setString(enteredAmount);
                }
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (!showAmountInput && event.key.code == sf::Keyboard::Backspace && !numPlateInput.empty())
                {
                    numPlateInput.pop_back();
                    inputText.setString(numPlateInput);
                    index = obj.findChallan(numPlateInput);
                }

                if (showAmountInput && event.key.code == sf::Keyboard::Backspace && !enteredAmount.empty())
                {
                    enteredAmount.pop_back();
                    amountText.setString(enteredAmount);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(userPortalWindow);

                if (!showAmountInput && submitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    if (index != -1)
                    {
                        cout<<"index "<<index<<endl;
                        std::stringstream ss;
                        ss << "Car Type: " <<obj.getType(index) << "\n"
                           << "Number Plate: " << obj.getNumPlate(index) << "\n"
                           << "Amount: " << obj.getAmt(index) << "\n"
                           << "Issue Date: " << obj.getIssueDate(index) << "\n"
                           << "Issue Time: " << obj.getIssueTime(index);

                        resultText.setString(ss.str());
                        showAmountInput = true;
                    }
                    else
                    {
                        resultText.setString("Challan not found.");
                    }
                }

                if (showAmountInput && payButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    // Logic for payment could go here
                    std::cout << "Payment of amount: " << enteredAmount << " processed." << std::endl;
                    resultText.setString("Payment Successful.");
                }
            }
        }

        // Clear window and redraw everything
        userPortalWindow.clear();
        userPortalWindow.draw(backgroundSprite);
        userPortalWindow.draw(inputBox);
        userPortalWindow.draw(inputText);
        userPortalWindow.draw(submitButton);
        userPortalWindow.draw(buttonText);
        userPortalWindow.draw(resultText);

        if (showAmountInput)
        {
            userPortalWindow.draw(amountInputBox);
            userPortalWindow.draw(amountText);
            userPortalWindow.draw(payButton);
            userPortalWindow.draw(payButtonText);
        }

        userPortalWindow.display();
    }
}
