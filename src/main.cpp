#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

//constexpr unsigned int SCREEN_WIDTH = 1280;
//constexpr unsigned int SCREEN_HEIGHT = 720;

struct WindowInformation
{
    std::string name;
    int width;
    int height;
};

struct MovementSpeed
{
    float x;
	float y;
};

int main(int argc, char* argv[])
{
    // Load in the config file
    std::fstream fin("bin/config.txt");

    // The window will always be the first item in the list
    WindowInformation wInfo;
    fin >> wInfo.name >> wInfo.width >> wInfo.height;

    // Create the SFML window with the width, height and name taken from the config file
    sf::RenderWindow window(sf::VideoMode(wInfo.width, wInfo.height), wInfo.name);

    /* From now on the order can be random, so we loop throughand check which object we do have */

    sf::Font myFont;

    // create vectors to hold the given shapes, their speeds and their names
    std::vector<std::shared_ptr<sf::Shape>> shapes;
    std::vector<sf::Text> names;
    std::vector<MovementSpeed> speeds;

    std::string first, name, file;
    int fSize, R, G, B;
    sf::Color fColor;
    float initX, initY, speedX, speedY, radius, width, height;


    while (fin >> first)
    {
        if (first == "Font")
        {
            fin >> file >> fSize >> R >> G >> B;
            if (!myFont.loadFromFile("bin/" + file))
            {
                std::cerr << "Error while loading the font file\n";
                exit(-1);
            }
            fColor = sf::Color(R, G, B);
        }
        else if (first == "Circle")
        {
            fin >> name >> initX >> initY >> speedX >> speedY >> R >> G >> B >> radius;
            sf::Color color(R, G, B);
            sf::CircleShape circle(radius);
            circle.setFillColor(color);
            circle.setPosition(initX, initY);

            MovementSpeed speed{ speedX, speedY };
            speeds.push_back(speed);

            sf::Text text(name, myFont, fSize);
            text.setFillColor(fColor);
            text.setOrigin(circle.getLocalBounds().left, circle.getLocalBounds().top);
            text.setPosition(circle.getGlobalBounds().top + circle.getGlobalBounds().height / 2.f, circle.getGlobalBounds().left + circle.getGlobalBounds().width / 2.f);

            names.push_back(text);
            std::shared_ptr<sf::Shape> tmp = std::make_shared<sf::CircleShape>(circle);
            shapes.push_back(tmp);

            window.draw(circle);
            window.draw(text);
        }
    	else if (first == "Rectangle")
        {
            fin >> name >> initX >> initY >> speedX >> speedY >> R >> G >> B >> width >> height;
            sf::Color color(R, G, B);
            sf::RectangleShape rect(sf::Vector2f(width, height));
            rect.setFillColor(color);
            rect.setPosition(initX, initY);

            MovementSpeed speed{ speedX, speedY };
            speeds.push_back(speed);

            sf::Text text(name, myFont, fSize);
            text.setFillColor(fColor);
            text.setOrigin(rect.getLocalBounds().left, rect.getLocalBounds().top);
            text.setPosition(rect.getGlobalBounds().top + rect.getGlobalBounds().height / 2.f, rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2.f);

            names.push_back(text);
            std::shared_ptr<sf::Shape> tmp = std::make_shared<sf::RectangleShape>(rect);
            shapes.push_back(tmp);

            window.draw(rect);
            window.draw(text);
        }
    }

    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (size_t i = 0; i < shapes.size(); i++)
        {
	        if (shapes[i]->getGlobalBounds().top < 0 || shapes[i]->getGlobalBounds().top + shapes[i]->getGlobalBounds().height >= wInfo.height)
	        {
                speeds[i].y *= -1.0f;
	        }
            if (shapes[i]->getGlobalBounds().left < 0 || shapes[i]->getGlobalBounds().left + shapes[i]->getGlobalBounds().width >= wInfo.width)
            {
                speeds[i].x *= -1.0f;
            }

            shapes[i]->setPosition(shapes[i]->getPosition().x + speeds[i].x, shapes[i]->getPosition().y + speeds[i].y);
            names[i].setPosition(round((shapes[i]->getGlobalBounds().left + shapes[i]->getGlobalBounds().width / 2.5f)), round((shapes[i]->getGlobalBounds().top + shapes[i]->getGlobalBounds().height / 2.5f)));

        }

        window.clear();
        for (size_t i = 0; i < shapes.size(); i++)
        {
            window.draw(*shapes[i]);
            window.draw(names[i]);
        }
        window.display();
    }

    /* SFML test code -- ignore all of it
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML works!");
    window.setFramerateLimit(60);


    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(300, 300);
    float circleMovementSpeed = 0.5f;

    std::vector<sf::RectangleShape> rectangles;

    sf::Font myFont;

    if (!myFont.loadFromFile("bin/fonts/tech.ttf"))
    {
        std::cerr << "Error while loading the font file\n";
        exit(-1);
    }

    sf::Text text("Sample Text", myFont, 24);
    text.setPosition(0, SCREEN_HEIGHT - (float)text.getCharacterSize() - 5);


    for (int x = 0; x < 25; x++)
    {
        for (int y = 0; y < 15; y++)
        {
            sf::RectangleShape rect(sf::Vector2f(15, 15));
            rect.setPosition(x * 20, y * 20);
            rect.setFillColor(sf::Color(x * 10, y * 10, 0));
            rectangles.push_back(rect);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code: " << event.key.code << '\n';

                if (event.key.code == sf::Keyboard::X)
                {
                    circleMovementSpeed *= -1.0f;
                }
            }
        }

        circle.setPosition(circle.getPosition().x + circleMovementSpeed, circle.getPosition().y + circleMovementSpeed);

        window.clear();
        for (const auto& rect : rectangles)
        {
            window.draw(rect);
        }
        window.draw(circle);
        window.draw(text);
        window.display();
    }*/

    return 0;
}