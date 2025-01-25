#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "vehicle.h"
using namespace std;
using namespace sf;

class trafficLight
{
public:
    Texture redLightText;
    Texture greenLightText;
    Sprite redLightSprite;
    Sprite greenLightSprite;
    bool isLightOn[4];
    Clock clock;
    int crGreenLight;

    trafficLight()
    {
        redLightText.loadFromFile("./images/redLight.png");
        greenLightText.loadFromFile("./images/greenLight.png");

        redLightSprite.setTexture(redLightText);
        greenLightSprite.setTexture(greenLightText);

        redLightSprite.setScale(0.08, 0.08);
        greenLightSprite.setScale(0.08, 0.08);

        for (int i = 0; i < 4; ++i)
        {
            isLightOn[i] = false;
        }
        crGreenLight = 0;
        isLightOn[crGreenLight] = true;
    }

    void RoundRobbinTrafficLight()
    {
        if (clock.getElapsedTime().asSeconds() > 10.0)
        {
            int index = -1;
            for (int i = 0; i < 4; i++)
            {
                if (isLightOn[i] == 1)
                {
                    isLightOn[i] = false;
                    index=i;
                    break;
                }
            }
            index =index+1;
            if(index==4)
            {
                index=0;
            }

            isLightOn[index] = true;
            clock.restart();
        }
        return;
    }

    void drawRedLight(RenderWindow &window, int xAxis, int yAxis)
    {
        redLightSprite.setPosition(xAxis, yAxis);
        window.draw(redLightSprite);
    }

    void drawGreenLight(RenderWindow &window, int xAxis, int yAxis)
    {
        greenLightSprite.setPosition(xAxis, yAxis);
        window.draw(greenLightSprite);
    }

    void displayTrafficLight(RenderWindow &window)
    {
        if (!isLightOn[0]) // north
            drawRedLight(window, 410, 300);
        else
            drawGreenLight(window, 410, 300);
        if (!isLightOn[1]) // west
            drawRedLight(window, 330, 390);
        else
            drawGreenLight(window, 330, 390);
        if (!isLightOn[2])  //south
            drawRedLight(window, 410, 500);
        else
            drawGreenLight(window, 410, 500);
        if (!isLightOn[3]) //east
            drawRedLight(window, 470, 390);
        else
            drawGreenLight(window, 470, 390);
    }
};
