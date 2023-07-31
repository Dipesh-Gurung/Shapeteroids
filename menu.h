#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include <cstring>
#define WIDTH 1200
#define HEIGHT 800
#define SPAWN_DISTANCE 1300

struct Enemy
{
    sf::Shape *shape;       // Pointer to the enemy's shape
    sf::Vector2f position;  // Position of the enemy
    sf::Vector2f direction; // Movement direction of the enemy
};

enum Scene
{
    UNDEFINED_SCENE = -1,
    START_MENU_SCENE = 0,
    GAMEPLAY_SCENE = 1,
    GAMEOVER_SCENE = 2,
    SETTINGS_SCENE = 3,
    ABOUT_SCENCE = 4
};

class Mainmenu
{
    sf::RenderWindow app;
    Scene currentScene;
    sf::Font font;
    sf::Text Play, Options, Exit, Menu, Scoretxt, gameover;
    sf::Clock clock;
  
    int bgchoice = 0;

public:
    Mainmenu();
    Scene StartMenu();
    Scene Gameplay();
    Scene GameOver();
    Scene Settings();
    void run();
    void FontType();
};
Mainmenu::Mainmenu()
    : app(sf::VideoMode(WIDTH, HEIGHT), "shapeteroids", sf::Style::Default), currentScene(Scene::START_MENU_SCENE)
{
    app.setFramerateLimit(60);
 
}
void Mainmenu::run()
{
    while (true)
    {
    label:
        switch (currentScene)
        {

        case Scene::START_MENU_SCENE:
            currentScene = StartMenu();
            goto label;

        case Scene::GAMEPLAY_SCENE:
            currentScene = Gameplay();
            break;

        case Scene::GAMEOVER_SCENE:
            currentScene = GameOver();
            break;

        case Scene::SETTINGS_SCENE:
            currentScene = Settings();
            break;

        // if switching to scene that does not exist, close application
        default:
            app.close();
            return;
        }
    }
}
Scene Mainmenu::StartMenu()
{
    sf::Texture background;
    sf::Sprite bgsprite;
    background.loadFromFile("D:\\Game Project 2023\\Utility\\images\\background_07.png");
    bgsprite.setTexture(background);

    FontType(); // calling the function that is setting the font type
    Play.setFont(font);
    Play.setString("PLAY");
    Play.setLetterSpacing(2.0f);
    Play.setCharacterSize(55);
    Play.setPosition(sf::Vector2f(500, 250));

    Options.setFont(font);
    Options.setString("OPTIONS");
    Options.setLetterSpacing(1.5f);
    Options.setCharacterSize(48);
    Options.setPosition(sf::Vector2f(500, 350));

    Exit.setFont(font);
    Exit.setString("EXIT");
    Exit.setLetterSpacing(1.5f);
    Exit.setCharacterSize(48);
    Exit.setPosition(sf::Vector2f(500, 450));

    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                app.close();
                return Scene::UNDEFINED_SCENE;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                auto mpos = sf::Mouse::getPosition(app);
                if (Play.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    // change scene to gameplay
                    std::cout << "play";
                    return Scene::GAMEPLAY_SCENE;
                }

                if (Options.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    // change to settings scene
                    std::cout << "Options";
                    return Scene::SETTINGS_SCENE;
                }

                if (Exit.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    // quit application
                    // switch to out of bounds scene
                    return Scene::UNDEFINED_SCENE;
                }
            }
        }

        app.clear();
        app.draw(bgsprite);
        app.draw(Play);
        app.draw(Options);
        app.draw(Exit);
        //  Play.draw(app);
        //  Options.draw(app);
        // Exit.draw(app)
        app.display();
    }
}
void Mainmenu::FontType()
{
    if (!font.loadFromFile("D:\\Game Project 2023\\Utility\\fonts\\Baisley.otf"))
    {
        std::cout << "The font is there" << std::endl;
    }
}
Scene Mainmenu::Gameplay()
{

    
    FontType();
    Scoretxt.setFont(font);
    Scoretxt.setString("SCORE: ");
    Scoretxt.setOrigin(0.0f, 0.0f);
    Scoretxt.setFillColor(sf::Color::Green);
    Scoretxt.setCharacterSize(65);
    Scoretxt.setLetterSpacing(1.5f);
    Scoretxt.setOutlineThickness(1.0f);

    sf::Texture background;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 13;
    if (bgchoice == 0)
    {
        background.loadFromFile("D:\\Game Project 2023\\Utility\\imagesbackground_01.jpg");
    }

    if (bgchoice == 1)
    {
        background.loadFromFile("D:\\Game Project 2023\\Utility\\images\\background_02.png");
    }
    else if (bgchoice == 2)
    {
        background.loadFromFile("D:\\Game Project 2023\\Utility\\images\\background_03.png");
    }
    else if (bgchoice == 3)
    {
        background.loadFromFile("D:\\Game Project 2023\\Utility\\images\\background_06.jpg");
    }
    sf::Sprite bgsprite;
    bgsprite.setTexture(background);

    // Get app dimensions
    sf::Vector2u appSize = app.getSize();
    float appWidth = static_cast<float>(appSize.x);
    float appHeight = static_cast<float>(appSize.y);

    // Player Parameters
    bool x;
    bool y;
    int playerLives = 3; // Player's lives

    // Player circle
    sf::CircleShape player(36.f);                        // Create a circle shape with a radius of 36
    player.setFillColor(sf::Color::White);               // Set the player's fill color
    player.setOrigin(36.f, 36.f);                        // Set origin to center of the player circle
    player.setPosition(appWidth / 2.f, appHeight / 2.f); // Center player circle in the app

    std::vector<sf::CircleShape> bullets; // Vector to store bullets

    float playerspeed = 2.5f, enemyspeed = 2.5f; // Increase the speed of the circle

    sf::Music music;
    if (!music.openFromFile("D:\\Game Project 2023\\Utility\\audio\\music.ogg"))
    {
        std::cout << "error";
    }

    // Create enemies
    sf::ConvexShape triangle(3);
    triangle.setPoint(0, sf::Vector2f(0.f, 0.f));
    triangle.setPoint(1, sf::Vector2f(30.f, 60.f));
    triangle.setPoint(2, sf::Vector2f(60.f, 0.f));
    triangle.setFillColor(sf::Color::Green);

    sf::RectangleShape rectangle(sf::Vector2f(60.f, 60.f));
    rectangle.setFillColor(sf::Color::Blue);

    sf::ConvexShape star(10);
    star.setPoint(0, sf::Vector2f(30.f, 0.f));
    star.setPoint(1, sf::Vector2f(36.f, 20.f));
    star.setPoint(2, sf::Vector2f(56.f, 20.f));
    star.setPoint(3, sf::Vector2f(39.f, 32.f));
    star.setPoint(4, sf::Vector2f(45.f, 52.f));
    star.setPoint(5, sf::Vector2f(30.f, 40.f));
    star.setPoint(6, sf::Vector2f(15.f, 52.f));
    star.setPoint(7, sf::Vector2f(21.f, 32.f));
    star.setPoint(8, sf::Vector2f(4.f, 20.f));
    star.setPoint(9, sf::Vector2f(24.f, 20.f));
    star.setFillColor(sf::Color::Yellow);

    std::vector<Enemy> enemies;

    sf::Clock enemySpawnTimer;                       // Timer for enemy spawning
    sf::Time enemySpawnInterval = sf::seconds(0.5f); // Interval between enemy spawns

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    music.play();
    while (app.isOpen())
    {

        sf::Event Event;
        while (app.pollEvent(Event))
        {
            // Exit when Escape is pressed
            if (Event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                app.close();
            }
            // Shoot bullet when left mouse button is pressed
            if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Left)
            {
                sf::CircleShape newBullet(5.f);         // Create a new bullet circle with a radius of 5
                newBullet.setFillColor(sf::Color::Red); // Set the bullet's fill color to red

                // Get the position of the mouse relative to the app
                sf::Vector2i mousePosition = sf::Mouse::getPosition(app);
                newBullet.setPosition(player.getPosition());                                                                                                    // Set position of the bullet
                newBullet.setRotation(std::atan2(mousePosition.y - newBullet.getPosition().y, mousePosition.x - newBullet.getPosition().x) * 180.f / 3.14159f); // Set rotation of the bullet towards the mouse position

                bullets.push_back(newBullet); // Add the bullet to the vector
            }
        }

        // MOVING PLAYER
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player.move(0, -playerspeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.move(0, playerspeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(-playerspeed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.move(playerspeed, 0);
        }

        // Wrap player when it goes off the screen
        if (player.getPosition().y < 0)
        {
            player.setPosition(player.getPosition().x, appHeight);
        }
        else if (player.getPosition().y > appHeight)
        {
            player.setPosition(player.getPosition().x, 0);
        }

        if (player.getPosition().x < 0)
        {
            player.setPosition(appWidth, player.getPosition().y);
        }
        else if (player.getPosition().x > appWidth)
        {
            player.setPosition(0, player.getPosition().y);
        }

        app.clear();
        app.draw(bgsprite);

        // UPDATE BULLETS
        for (size_t i = 0; i < bullets.size(); i++)
        {
            float angle = bullets[i].getRotation() * 3.14159 / 180.f; // Convert rotation to radians
            float moveX = 8.f * std::cos(angle);                      // Calculate movement along X-axis based on angle
            float moveY = 8.f * std::sin(angle);                      // Calculate movement along Y-axis based on angle
            bullets[i].move(moveX, moveY);                            // Move the bullet

            // Remove the bullet if it goes off the screen
            if (bullets[i].getPosition().y < 0 || bullets[i].getPosition().y > appHeight || bullets[i].getPosition().x < 0 || bullets[i].getPosition().x > appWidth)
            {
                bullets.erase(bullets.begin() + i);
                i--; // Decrement i to account for the erased bullet
            }
        }

        // Update enemy spawn
        if (enemySpawnTimer.getElapsedTime() > enemySpawnInterval)
        {
            // Spawn new enemy
            Enemy newEnemy;
            int shapeIndex = std::rand() % 2; // Generate a random index to select a shape
            if (shapeIndex == 0)              // Triangle shape
            {
                newEnemy.shape = &triangle;
            }
            else if (shapeIndex == 1) // Rectangle shape
            {
                newEnemy.shape = &rectangle;
            }
            else if (shapeIndex == 2) // Star shape
            {
                newEnemy.shape = &star;
            }

            // Generate a random position for the enemy
            // Generate a random position for the enemy
            float angle = rand() % 360;
            newEnemy.position = sf::Vector2f(SPAWN_DISTANCE * cos(angle * 3.14159 / 180), SPAWN_DISTANCE * sin(angle * 3.14159 / 180));

            // Calculate a random direction for the enemy
            float directionX = static_cast<float>(std::rand() % 3 - 1); // Generate a random value between -1 and 1
            float directionY = static_cast<float>(std::rand() % 3 - 1); // Generate a random value between -1 and 1
            newEnemy.direction = sf::Vector2f(directionX, directionY);

            enemies.push_back(newEnemy);

            // Restart the timer
            enemySpawnTimer.restart();
        }

        // Update enemies and check collision with bullets
        for (size_t enemyIndex = 0; enemyIndex < enemies.size(); enemyIndex++)
        {
            Enemy &enemy = enemies[enemyIndex];

            // Move the enemy
            enemy.position += enemy.direction * enemyspeed;

            // Check collision with bullets
            for (size_t i = 0; i < bullets.size(); i++)
            {
                // Check if bullet intersects with enemy shape
                if (bullets[i].getGlobalBounds().intersects(enemy.shape->getGlobalBounds()))
                {
                    // Remove the enemy and bullet
                    enemies.erase(enemies.begin() + enemyIndex);
                    bullets.erase(bullets.begin() + i);
                    i--;
                   // Decrement i to account for the erased bullet
                    // Break the inner loop as the bullet can only hit one enemy
                    break;
                }
            }

            // Wrap enemy when it goes off the screen
            if (enemy.position.y < 0.f && enemy.direction.y < 0.f)
            {
                enemy.position.y = appHeight;
            }
            else if (enemy.position.y > appHeight && enemy.direction.y > 0.f)
            {
                enemy.position.y = 0.f;
            }

            if (enemy.position.x < 0.f && enemy.direction.x < 0.f)
            {
                enemy.position.x = appWidth;
            }
            else if (enemy.position.x > appWidth && enemy.direction.x > 0.f)
            {
                enemy.position.x = 0.f;
            }

            // Set the position of the enemy shape
            enemy.shape->setPosition(enemy.position);

            // Draw the enemy
            app.draw(*enemy.shape);

            // Check if enemy touches player
            if (enemy.shape->getGlobalBounds().intersects(player.getGlobalBounds()))
            {
                return Scene::GAMEOVER_SCENE; // Game over
            }
        }

        // Draw the bullets
        for (const auto &bullet : bullets)
        {
            app.draw(bullet);
        }
        
        
        app.draw(player);
        app.display();
    }
}

Scene Mainmenu::GameOver()
{
    
    sf::Music music;
    if (!music.openFromFile("D:\\Game Project 2023\\Utility\\audio\\bgm_26.wav"))
    {
        std::cout << "error";
    }
    music.play();
    sf::Texture background;
    sf::Sprite bgsprite;
    background.loadFromFile("D:\\Game Project 2023\\Utility\\images\\background_05.png");
    bgsprite.setTexture(background);
    FontType(); // calling the function that is setting the font type

    gameover.setFont(font);
    gameover.setString("GAME OVER");
    gameover.setCharacterSize(80);
    gameover.setLetterSpacing(2.0f);
    gameover.setPosition(sf::Vector2f(300, 250));

    Play.setFont(font);
    Play.setString("PLAY");
    Play.setCharacterSize(48);
    Play.setLetterSpacing(2.0f);
    Play.setPosition(sf::Vector2f(500, 350));

    Menu.setFont(font);
    Menu.setString("MENU");
    Menu.setCharacterSize(48);
    Menu.setLetterSpacing(1.5f);
    Menu.setPosition(sf::Vector2f(500, 450));

    Exit.setFont(font);
    Exit.setString("EXIT");
    Exit.setCharacterSize(48);
    Exit.setLetterSpacing(1.5f);
    Exit.setPosition(sf::Vector2f(500, 550));

    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                app.close();
                return Scene::UNDEFINED_SCENE;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                auto mpos = sf::Mouse::getPosition(app);
                if (Play.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    // change scene to gameplay
                   
                    return Scene::GAMEPLAY_SCENE;
                }

                if (Menu.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    // change to settings scene
                    
                    return Scene::START_MENU_SCENE;
                }

                if (Exit.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    // quit application
                    // switch to out of bounds scene
                    return Scene::UNDEFINED_SCENE;
                }
            }
        }

        app.clear();
        app.draw(bgsprite);
        app.draw(gameover);
        app.draw(Play);
        app.draw(Menu);
        app.draw(Exit);
        //  Play.draw(app);
        //  Options.draw(app);
        // Exit.draw(app)
        app.display();
    }
}
Scene Mainmenu::Settings()
{
    sf::Text bg1, bg2, bg3;
    bg1.setFont(font);
    bg1.setString("SPACE1");
    bg1.setCharacterSize(48);
    bg1.setLetterSpacing(2.0f);
    bg1.setPosition(sf::Vector2f(500, 250));

    bg2.setFont(font);
    bg2.setString("SPACE2");
    bg2.setCharacterSize(48);
    bg2.setLetterSpacing(1.5f);
    bg2.setPosition(sf::Vector2f(500, 350));

    bg3.setFont(font);
    bg3.setString("EXCLUSIVE");
    bg3.setCharacterSize(48);
    bg3.setLetterSpacing(1.5f);
    bg3.setPosition(sf::Vector2f(500, 450));
    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                app.close();
                return Scene::UNDEFINED_SCENE;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                auto mpos = sf::Mouse::getPosition(app);
                if (bg1.getGlobalBounds().contains((sf::Vector2f)mpos))
                {

                    bgchoice = 1;
                    return Scene::START_MENU_SCENE;
                }

                if (bg2.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    bgchoice = 2;
                    return Scene::START_MENU_SCENE;
                }

                if (bg3.getGlobalBounds().contains((sf::Vector2f)mpos))
                {
                    bgchoice = 3;
                    return Scene::START_MENU_SCENE;
                }
            }
        }
        app.clear();
        app.draw(bg1);
        app.draw(bg2);
        app.draw(bg3);
        app.display();
    }
}