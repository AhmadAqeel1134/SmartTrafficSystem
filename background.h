#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include<vector>
using namespace sf;
using namespace std;

class background
{
    public:
    Texture verRoadText;
    Texture horRoadText;
    Texture roadIntersecText;
    Sprite verRoadSprite;
    Sprite horRoadSprite;
    Sprite roadIntersecSprite;
    vector<Sprite> roads;

    background()
    {
     verRoadText.loadFromFile("./images/YaxisRoad.jpeg"); 
     horRoadText.loadFromFile("./images/XaxisRoad.jpeg"); 
     roadIntersecText.loadFromFile("./images/intersectionRoad.jpeg");   
     verRoadSprite.setTexture(verRoadText);
     horRoadSprite.setTexture(horRoadText);
     roadIntersecSprite.setTexture(roadIntersecText);



     verRoadSprite.setPosition(300,0);
     for(int i=0;i<10;i++)
     {
        Sprite temp=verRoadSprite;
        temp.setPosition(300,i*100);
        roads.push_back(temp);
     }
     horRoadSprite.setPosition(0,300);
     for(int i=0;i<10;i++)
     {
        Sprite temp=horRoadSprite;
        temp.setPosition(i*100,300);
        roads.push_back(temp);
     }
     roadIntersecSprite.setPosition(300,300);
     roads.push_back(roadIntersecSprite);
    
    }

    void drawRoads(RenderWindow & window)
    {
        for(const auto& sprite: roads)
        {
         window.draw(sprite);
        }
    }

};