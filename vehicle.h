#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace sf;

class vehicle
{
public:
    Sprite sprite;
    float speed;
    Vector2f position;
    string type;
    bool challanStatus;
    string numPlate;
    string laneDir; // North South East West
    Clock speedClock;
    bool isEmergency;
    bool hasChallanWritten;
    bool isOutOfOrder;

    vehicle(Texture &texture, string type, float speed, Vector2f initialPosition, string numPlate, string laneDir,bool isEmergency)
    {
        this->speed = speed;
        this->position = initialPosition;
        this->type = type;
        this->sprite.setTexture(texture);
        this->sprite.setPosition(position);
        sprite.setScale(0.8, 0.8);
        if(this->type=="ToeTruck")
        {
           this->sprite.setScale(0.2, 0.2); 
        }
        this->challanStatus = false;
        this->numPlate = numPlate;
        this->laneDir = laneDir;
        this->isEmergency=isEmergency;
        this->hasChallanWritten=false;
        this->isOutOfOrder=false;
    }

    void moveAlongNorth()
    {
        position.y += speed;
        sprite.setPosition(position);
    }

    void moveAlongSouth()
    {
        position.y -= speed;
        sprite.setPosition(position);
    }

    void moveAlongWest()
    {
        position.x += speed;
        sprite.setPosition(position);
    }

    void moveAlongEast()
    {
        position.x -= speed;
        sprite.setPosition(position);
    }

    bool isOutOfBoundsForNorth(float windowHeight)
    {
        if (position.y > windowHeight)
            return true;
        return false;
    }
    bool isOutOfBoundsForSouth(float windowHeight)
    {
        if (position.y < 0)
            return true;
        return false;
    }
    bool isOutOfBoundsForWest(float windowWidth)
    {
        if (position.x > windowWidth)
            return true;
        return false;
    }

    bool isOutOfBoundsForEast(float windowWidth)
    {
        if (position.x < 0)
            return true;
        return false;
    }

    float getSpeed()
    {
        return this->speed;
    }

    void setSpeed(float s)
    {
        this->speed = s;
    }
    void draw(RenderWindow &window)
    {
        window.draw(sprite);
    }

    void drawSpeed(RenderWindow &window, Font &font)
    {
        Text speedText;
        speedText.setFont(font);
        speedText.setFillColor(Color::Red);
        speedText.setCharacterSize(14);
        float speedInKmH = speed / 0.01f;
        speedText.setString(to_string(static_cast<int>(speedInKmH)) + " km/h");
        speedText.setPosition(
            position.x + sprite.getGlobalBounds().width / 2 - speedText.getGlobalBounds().width / 2,
            position.y - 20);
        window.draw(speedText);
    }
}; 



