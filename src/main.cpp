/* Copyright(C) Kacper Tomaszewski */

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

struct WindowInformation
{
    int Width;
    int Height;
};

struct MovementSpeed
{
    float X;
	float Y;
};

// The Shape class will store the different shapes as pointers with their respective movement speeds and texts
class Shape
{
public:
    Shape(const std::shared_ptr<sf::Shape>& shape, const std::shared_ptr<MovementSpeed>& speed, const sf::Text& text)
    {
        mShape = shape;
        mSpeed = speed;
        mName = text;
    }

	std::shared_ptr<sf::Shape> GetShape()
    {
        return mShape;
    }

    sf::Text* GetName()
    {
        return &mName;
    }

    std::shared_ptr<MovementSpeed> GetSpeed()
    {
        return mSpeed;
    }

private:
    std::shared_ptr<sf::Shape> mShape;
    sf::Text mName;
    std::shared_ptr<MovementSpeed> mSpeed;
};

int main(int argc, char* argv[])
{
    // Variables to store the information taken from the config file
    std::string first, name, file;
    sf::Font myFont;
    int fSize, r, g, b;
    sf::Color fColor;
    float initX, initY, speedX, speedY, radius, width, height;

    // Load in the config file
    std::fstream fin("bin/config.txt");

    // The window will always be the first item in the list
    WindowInformation wInfo{};
    fin >> first >> wInfo.Width >> wInfo.Height;

    // Create the SFML window with the Width, Height and name taken from the config file
    sf::RenderWindow window(sf::VideoMode(wInfo.Width, wInfo.Height),"SFML Assignment 1");

    /* From now on the order can be random, so we loop throughand check which object we do have */
    // create vectors to hold the given shapes, their speeds and their names
    std::vector<Shape> shapes;

    while (fin >> first)
    {
        /* Depending on the first word in the config file, decide what to do and what information to take in */

        if (first == "Font")
        {
            fin >> file >> fSize >> r >> g >> b;
            if (!myFont.loadFromFile("bin/" + file))
            {
                std::cerr << "Error while loading the font file\n";
                exit(-1);
            }
            fColor = sf::Color(r, g, b);
        }
        else if (first == "Circle")
        {
            fin >> name >> initX >> initY >> speedX >> speedY >> r >> g >> b >> radius;
            // Create the shape and add color to it
            sf::Color color(r, g, b);
            sf::CircleShape circle(radius);
            circle.setFillColor(color);
            circle.setPosition(initX, initY);

            // Create a MovementSpeed object
            auto speed = std::make_shared<MovementSpeed>();
            speed->X = speedX;
            speed->Y = speedY;

            // Create the text and adjust the origin of the text to center it in the shape
            sf::Text text(name, myFont, fSize);
            text.setFillColor(fColor);
            const sf::FloatRect bounds(text.getGlobalBounds());
            sf::Vector2f numRectCenter(bounds.width / 2.f + bounds.left, bounds.height / 2.f + bounds.top);
            text.setOrigin(numRectCenter);
            text.setPosition(initX + circle.getLocalBounds().width / 2.f, initY + circle.getLocalBounds().height / 2.f);

            // Create a shape class pointer with the created objects and push it to the vector
            std::shared_ptr<sf::Shape> tmp = std::make_shared<sf::CircleShape>(circle);
            Shape tmp2(tmp, speed, text);
            shapes.push_back(tmp2);
        }
    	else if (first == "Rectangle")
        {
            fin >> name >> initX >> initY >> speedX >> speedY >> r >> g >> b >> width >> height;
            // Create the shape and add color to it
            sf::Color color(r, g, b);
            sf::RectangleShape rect(sf::Vector2f(width, height));
            rect.setFillColor(color);
            rect.setPosition(initX, initY);

            // Create a MovementSpeed object
            auto speed = std::make_shared<MovementSpeed>();
            speed->X = speedX;
            speed->Y = speedY;

            // Create the text and adjust the origin of the text to center it in the shape
            sf::Text text(name, myFont, fSize);
            text.setFillColor(fColor);
            const sf::FloatRect bounds(text.getGlobalBounds());
            sf::Vector2f numRectCenter(bounds.width / 2.f + bounds.left, bounds.height / 2.f + bounds.top);
            text.setOrigin(numRectCenter);
            text.setPosition(initX + rect.getLocalBounds().width / 2.f, initY + rect.getLocalBounds().height / 2.f);

            // Create a shape class pointer with the created objects and push it to the vector
            std::shared_ptr<sf::Shape> tmp = std::make_shared<sf::RectangleShape>(rect);
            Shape tmp2(tmp, speed, text);
            shapes.push_back(tmp2);
        }
    }

    // Game loop
    while (window.isOpen())
    {
        // Event polling
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the positions of the shapes and their texts
        for (auto& shape : shapes)
        {
	        if (shape.GetShape()->getGlobalBounds().top < 0 || shape.GetShape()->getGlobalBounds().top + shape.GetShape()->getGlobalBounds().height >= wInfo.Height)
	        {
                shape.GetSpeed()->Y *= -1.0f;
	        }
        	else if (shape.GetShape()->getGlobalBounds().left < 0 || shape.GetShape()->getGlobalBounds().left + shape.GetShape()->getGlobalBounds().width >= wInfo.Width)
            {
                shape.GetSpeed()->X *= -1.0f;
            }

            shape.GetShape()->setPosition(shape.GetShape()->getPosition().x + shape.GetSpeed()->X,
                shape.GetShape()->getPosition().y + shape.GetSpeed()->Y);
            shape.GetName()->setPosition(shape.GetShape()->getPosition().x + shape.GetShape()->getLocalBounds().width / 2.f,
                shape.GetShape()->getPosition().y + shape.GetShape()->getLocalBounds().height / 2.f);
        }

        // Update the window
        window.clear();
        for (auto& shape : shapes)
        {
            window.draw(*shape.GetShape());
            window.draw(*shape.GetName());
        }
        window.display();
    }

    return 0;
}