#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "Menu.h"
#include "Player.h"
#include "Gameover.h"
#include "Gamewin.h"
#include "Score.h"
#include "ScoreList.h"

#define GAME_MENU 0
#define GAME_PLAY 1
#define GAME_SCOREBOARD 2
#define GAME_OVER 3
#define SAVE_SCORE 4
#define GAME_WIN 5

using namespace sf;
void setTiles();
void setWalls();
void setstars();

std::vector<std::vector<int>> tiles;

sf::RectangleShape wall;
std::vector<sf::RectangleShape> walls;

sf::CircleShape star;
std::vector<sf::CircleShape> stars;

std::string name = "";

using namespace sf;

Vector2f normalize(const Vector2f& source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return Vector2f(source.x / length, source.y / length);
    else
        return source;
}

int main()
{
    RenderWindow window(VideoMode(1600,720), "Game", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
    Menu menu(window.getSize().x, window.getSize().y);
    Gameover gameover(window.getSize().x, window.getSize().y);
    Gamewin gamewin(window.getSize().x, window.getSize().y);

    //leaderboard
    ScoreList score_list("highscore.txt");
    score_list.loadFile();

    //menu
    sf::Texture texturebg;
    if (!texturebg.loadFromFile("menu.PNG")) //ใส่background
    {
        //haddle error
    }
    sf::Sprite background;
    background.setTexture(texturebg);

    //song
    sf::Music music;
    if (!music.openFromFile("Mainmusic.OGG"))
    {
        std::cout << "ERROR" << std::endl;
    }

    music.play();
    music.setLoop(true);

    sf::SoundBuffer collectsound;
    if (!collectsound.loadFromFile("colletcoin.OGG"))
    {
        std::cout << "ERROR" << std::endl;
    }

    sf::Sound sound;
    sound.setBuffer(collectsound);


    sf::SoundBuffer enemysound;
    if (!enemysound.loadFromFile("enemysound.OGG"))
    {
        std::cout << "ERROR" << std::endl;
    }

    sf::Sound enemywin;
    enemywin.setBuffer(enemysound);


    sf::SoundBuffer itemsound;
    if (!itemsound.loadFromFile("itemsound.OGG"))
    {
        std::cout << "ERROR" << std::endl;
    }

    sf::Sound collectitem;
    collectitem.setBuffer(itemsound);

    //map
    sf::Texture mapTex;
    sf::Sprite map;
    mapTex.loadFromFile("background.PNG");
    map.setTexture(mapTex);

    //game over
    sf::Texture gameoverTex;
    sf::Sprite gameoverbg;
    gameoverTex.loadFromFile("game over.PNG");
    gameoverbg.setTexture(gameoverTex);

    //game win
    sf::Texture gamewinTex;
    sf::Sprite gamewinbg;
    gamewinTex.loadFromFile("gamewin.PNG");
    gamewinbg.setTexture(gamewinTex);

    //leaderboard
    sf::Texture leaderboardTex;
    sf::Sprite leaderboardbg;
    leaderboardTex.loadFromFile("leaderboard.PNG");
    leaderboardbg.setTexture(leaderboardTex);
    
    //save score
    sf::Texture scoreTex;
    sf::Sprite scorebg;
    scoreTex.loadFromFile("SAVESCORE.PNG");
    scorebg.setTexture(scoreTex);

    //RectangleShape wall;
    wall.setFillColor(sf::Color::Transparent);
    wall.setSize(sf::Vector2f(40.f, 40.f));
    setTiles();
    setWalls();

    //star
    star.setRadius(12.f);   
    sf::Texture money;
    if (!money.loadFromFile("money.PNG"))
    {
        std::cout << "ERROR ERROR ERROR" << std::endl;
    }
    star.setTexture(&money);
    setstars(); // <- รีเซ็ตเหรียญ

    //item
    sf::Texture pngtime2;
    pngtime2.loadFromFile("scoretime2.PNG");
    sf::Sprite time2;
    time2.setTexture(pngtime2);
    time2.setScale(.07f, .07f);
    time2.setPosition(rand() % 1600 + 0, rand() % 720);

    sf::Texture pngitem;
    pngitem.loadFromFile("sunny.PNG");
    sf::Sprite item;
    item.setTexture(pngitem);
    item.setScale(.07f, .07f);
    item.setPosition(rand() % 1600 + 0, rand() % 720);

    //animation
    float movementSpeed = 6.f;
    float preWidth = 115.f;
    float preHeight = 102.f;
    float width = 86.f;
    float height = 155.f;
    bool moving = false;
    int standNum = 1;
    int moveNum = 1;
    bool turnLeft = false;

    IntRect currentFrame = IntRect(preWidth, preHeight, width, height);
    Clock aniTime;

    sf::Texture texture;
    sf::Sprite sprite;
    texture.loadFromFile("player.PNG");
    sprite.setTexture(texture);
    sprite.setTextureRect(currentFrame);
    sprite.scale(.30f, .30f);
    sprite.setPosition(45.f, 45.f); // <- รีเซ็ตตำแหน่งของ Player

    int iPos = sprite.getPosition().x;
    int jPos = sprite.getPosition().y;

    sf::Vector2f playerPosition = sf::Vector2f(iPos, jPos);

    //collision wall
    FloatRect nextPos;

    //score
    int score = 0;

    sf::Font puimek;
    puimek.loadFromFile("puimek.ttf");

    sf::Text lblScore;
    lblScore.setCharacterSize(45);
    lblScore.setPosition({ 1335,270 });
    lblScore.setFont(puimek);
    lblScore.setFillColor(sf::Color::Black);
    lblScore.setString("Score : " + to_string(score));

    //exit while play game
    sf::Text exit;
    exit.setCharacterSize(30);
    exit.setPosition({ 1305,380 });
    exit.setFont(puimek);
    exit.setFillColor(sf::Color::Black);
    exit.setString("EXIT Press Backspace");

    //enemy
    sf::Texture preenemy;
    sf::Sprite enemy;
    preenemy.loadFromFile("enemy.PNG");
    enemy.setTexture(preenemy);
    enemy.scale(.2f, .2f);
    enemy.setPosition(1000.f, 600.f); // <- รีเซ็ตตำแหน่งของ Enemy

    Vector2f dir;
    Vector2f dir_norm;

    int xPos = enemy.getPosition().x;
    int yPos = enemy.getPosition().y;

    sf::Vector2f enemyPosition = sf::Vector2f(xPos, yPos);
    dir = playerPosition - enemyPosition;
    dir_norm = normalize(dir);

    float enemyspeed = 0.003f;

    ////bullet
    ////define object
    //std::vector<Bullet> bulletVec;
    //std::vector<sf::RectangleShape> bullet;
    //bool isFiring = false;

    int game_state = GAME_MENU;

    while (window.isOpen())
    {   
        if(game_state == GAME_MENU) 
        {
            Event ev;
            while (window.pollEvent(ev)) 
            {   
                if (ev.type == Event::Closed) 
                    window.close();
                if (ev.type == Event::TextEntered && ev.text.unicode == 13) 
                {
                    switch (menu.GetPressedItem()) 
                    {
                        case 0:
                            std::cout << "PLAY has been pressed" << std::endl;
                            game_state = GAME_PLAY;
                            break;
                        
                        case 1:
                            std::cout << "Leaderboard has been pressed" << std::endl;
                            game_state = GAME_SCOREBOARD;
                            break;
                        
                        case 2:
                            window.close();
                            break;
                    }
                }
                switch (ev.type) {
                    case sf::Event::KeyReleased:
                        switch (ev.key.code) {
                            case sf::Keyboard::Up:
                                menu.MoveUp();
                                break;

                            case sf::Keyboard::Down:
                                menu.MoveDown();
                                break;
                        }
                        break;
                }
            }
        }
        if (game_state == GAME_PLAY)
        {
            Event ev;
            while (window.pollEvent(ev)) 
            {
                if (ev.type == Event::Closed) 
                    window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::BackSpace))
            {
                lblScore.setCharacterSize(45);
                lblScore.setPosition({ 1335,270 });
                //lblScore.setPosition({ 1490,300 });
                score = 0;
                name = "";
                setstars();
                sprite.setPosition(45.f, 45.f);
                enemy.setPosition(1000.f, 600.f);
                item.setPosition(rand() % 1500 + 0, rand() % 720);
                time2.setPosition(rand() % 1500 + 0, rand() % 720);
                game_state = GAME_MENU;

            }

            /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                isFiring = true;
            }*/

            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                sprite.move(-movementSpeed, 0.f);
                moving = true;
                turnLeft = true;

                if (Keyboard::isKeyPressed(Keyboard::W))
                {
                    sprite.move(0.f, -movementSpeed);
                }
                else if (Keyboard::isKeyPressed(Keyboard::S))
                {
                    sprite.move(0.f, movementSpeed);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::D))
            {
                sprite.move(movementSpeed, 0.f);
                moving = true;
                turnLeft = false;
                if (Keyboard::isKeyPressed(Keyboard::W))
                {
                    sprite.move(0.f, -movementSpeed);
                }
                else if (Keyboard::isKeyPressed(Keyboard::S))
                {
                    sprite.move(0.f, movementSpeed);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::W))
            {
                sprite.move(0.f, -movementSpeed);
                moving = true;
            }
            else if (Keyboard::isKeyPressed(Keyboard::S))
            {
                sprite.move(0.f, movementSpeed);
                moving = true;
            }
            else
            {
                moving = false;
            }
            if (standNum >= 5)
            {
                standNum = 1;
            }
            if (moveNum >= 7)
            {
                moveNum = 1;
            }
            if (aniTime.getElapsedTime().asSeconds() >= 0.1f)
            {
                if (moving == false)
                {
                    moveNum = 1;
                    preHeight = 102.f;
                    width = 86.f;
                    switch (standNum)
                    {
                    case 1: preWidth = 271.f;
                        break;
                    case 2: preWidth = 421.f;
                        break;
                    case 3: preWidth = 573.f;
                        break;
                    case 4: preWidth = 115.f;
                        break;
                    default:
                        break;
                    }
                    standNum++;
                }
                if (moving == true)
                {
                    standNum = 1;
                    preHeight = 332.f;
                    width = 91.f;
                    switch (moveNum)
                    {
                    case 1: preWidth = 113.f;
                        break;
                    case 2: preWidth = 272.f;
                        break;
                    case 3: preWidth = 424.f;
                        break;
                    case 4: preWidth = 576.f;
                        break;
                    case 5: preWidth = 735.f;
                        break;
                    case 6: preWidth = 891.f;
                        break;
                    default:
                        break;
                    }
                    moveNum++;
                }
                if (turnLeft == false)
                {
                    currentFrame = IntRect(preWidth, preHeight, width, height);
                }
                else if (turnLeft == true)
                {
                    currentFrame = IntRect(preWidth + width, preHeight, -width, height);
                }
                sprite.setTextureRect(currentFrame);
                aniTime.restart();
            }
            //exit while play
            if (Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            //collision wall
            for (auto& wall : walls)
            {
                FloatRect playerBounds = sprite.getGlobalBounds();
                FloatRect wallBounds = wall.getGlobalBounds();

                nextPos = playerBounds;

                if (wallBounds.intersects(nextPos))
                {
                    //right
                    if (playerBounds.left < wallBounds.left
                        && playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
                        && playerBounds.top < wallBounds.top + wallBounds.height
                        && playerBounds.top + playerBounds.height > wallBounds.top)
                    {
                        //movementSpeed = 0.f;
                        sprite.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
                    }
                    //left
                    else if (playerBounds.left > wallBounds.left
                        && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
                        && playerBounds.top < wallBounds.top + wallBounds.height
                        && playerBounds.top + playerBounds.height > wallBounds.top)
                    {
                        //movementSpeed = 0.f;
                        sprite.setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
                    }
                    //bottom
                    if (playerBounds.top < wallBounds.top
                        && playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
                        && playerBounds.left < wallBounds.left + wallBounds.width
                        && playerBounds.left + playerBounds.width > wallBounds.left)
                    {
                        //movementSpeed = 0.f;
                        sprite.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
                    }
                    //top
                    else if (playerBounds.top > wallBounds.top
                        && playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
                        && playerBounds.left < wallBounds.left + wallBounds.width
                        && playerBounds.left + playerBounds.width > wallBounds.left)
                    {
                        //movementSpeed = 0.f;
                        sprite.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
                    }
                }
            }
            //enemy move
            dir = sprite.getPosition() - enemy.getPosition();
            enemy.move(enemyspeed * dir);

            //enemy collision wall
            //left
            if (enemy.getPosition().x < 0.f)
            {
                enemy.setPosition(0.f, enemy.getPosition().y);
                if (enemy.getPosition().x == 0)
                {
                    enemy.move(enemyspeed * dir);
                }
            }
            //top
            if (enemy.getPosition().y < 0.f)
            {
                enemy.setPosition(enemy.getPosition().x, 0.f);
                if (enemy.getPosition().y == 0)
                {
                    enemy.move(enemyspeed * dir);
                }
            }
            //right
            if (enemy.getPosition().x + enemy.getGlobalBounds().width > 1280)
            {
                enemy.setPosition(1280 - enemy.getGlobalBounds().width, enemy.getPosition().y);
                if (enemy.getPosition().x > 1280)
                {
                    enemy.move(enemyspeed * dir);
                }
            }
            //bottom
            if (enemy.getPosition().y + enemy.getGlobalBounds().height > 720)
            {
                enemy.setPosition(enemy.getPosition().x, 720 - enemy.getGlobalBounds().height);
                if (enemy.getPosition().y > 720)
                {
                    enemy.move(enemyspeed * dir);
                }
            }
            //collision star
            for (int i=0;i<stars.size();i++)
            {
                FloatRect playerBounds = sprite.getGlobalBounds();
                FloatRect starBounds = stars.at(i).getGlobalBounds();

                nextPos = playerBounds;
                //score
                if (starBounds.intersects(nextPos))
                {
                    sound.play();
                    score = score + 2;
                    lblScore.setString("Score : " + to_string(score));
                    stars.erase(stars.begin()+i);
                    break;
                }
            }

            //collision item2
            FloatRect time2Bounds = time2.getGlobalBounds();
            if (time2Bounds.intersects(nextPos))
            {
                collectitem.play();
                score = score * 2;
                lblScore.setString("Score : " + to_string(score));
                time2.setPosition(sf::Vector2f(4234432, 4234423));
            }

            //collision item
            FloatRect itemBounds = item.getGlobalBounds();
            if (itemBounds.intersects(nextPos))
            {
                //skill
                collectitem.play();
                score = score * 10;
                lblScore.setString("Score : " + to_string(score));
                item.setPosition(sf::Vector2f(4234432, 4234423));
            }

            //collision enemy
            FloatRect playerBounds = sprite.getGlobalBounds();
            FloatRect enemyBounds = enemy.getGlobalBounds();
     
            if (enemyBounds.intersects(playerBounds))
            {
                //sound
                enemywin.play();
                game_state = GAME_OVER;
            }

            //collision screen
            //left
            if (sprite.getPosition().x < 0.f)
                sprite.setPosition(0.f, sprite.getPosition().y);
            //top
            if (sprite.getPosition().y < 0.f)
                sprite.setPosition(sprite.getPosition().x, 0.f);
            //right
            if (sprite.getPosition().x + sprite.getGlobalBounds().width > 1280)
                sprite.setPosition(1280 - sprite.getGlobalBounds().width, sprite.getPosition().y);
            //bottom
            if (sprite.getPosition().y + sprite.getGlobalBounds().height > 720)
                sprite.setPosition(sprite.getPosition().x, 720 - sprite.getGlobalBounds().height);

            //bullet
            /*if (isFiring == true)
            {
                Bullet newBullet(sf::Vector2f(30, 10));
                newBullet.setPos(sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y));
                bulletVec.push_back(newBullet);
                isFiring = false;
            }*/

            //GAME WIN
            if (stars.empty())
            {
                score = score * 2;
                lblScore.setString("Score : " + to_string(score));
                game_state = GAME_WIN;
            }
            cout << stars.size() << endl;
        }  

        if (game_state == GAME_OVER)
        {
            Event ev;
            while (window.pollEvent(ev))
            {   
                if (ev.type == Event::Closed)
                    window.close();
                if(ev.type == sf::Event::TextEntered && ev.text.unicode == 13) 
                {
                    switch (gameover.GetPressedItem())
                    {
                        case 0:
                            std::cout << "SAVE SCORE has been pressed" << std::endl;
                            game_state = SAVE_SCORE;
                            break;

                        case 1:
                            std::cout << "QUIT has been pressed" << std::endl;
                            lblScore.setCharacterSize(45);
                            //lblScore.setPosition({ 1335,270 });
                            lblScore.setPosition({ 1490,300 });
                            score = 0;
                            name = "";
                            setstars();
                            sprite.setPosition(45.f, 45.f);
                            enemy.setPosition(1000.f, 600.f);
                            item.setPosition(rand() % 1500 + 0, rand() % 720);
                            time2.setPosition(rand() % 1500 + 0, rand() % 720);
                            /*bulletVec.clear();*/
                            game_state = GAME_MENU;
                            break;
                    }
                }
                switch (ev.type)
                {
                case sf::Event::KeyReleased:
                    switch (ev.key.code)
                    {
                    case sf::Keyboard::Up:
                        gameover.MoveUp();
                        break;

                    case sf::Keyboard::Down:
                        gameover.MoveDown();
                        break;
                    }
                    break;
                }
            }
        }
        if (game_state == GAME_WIN)
        {
        Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                    window.close();
                if(ev.type == sf::Event::TextEntered && ev.text.unicode == 13) {

                    switch (gamewin.GetPressedItem())
                    {
                        case 0:
                            std::cout << "SAVE SCORE has been pressed" << std::endl;
                            game_state = SAVE_SCORE;
                            break;

                        case 1:
                            std::cout << "QUIT has been pressed" << std::endl;
                            lblScore.setCharacterSize(45);
                            //lblScore.setPosition({ 1335,270 });
                            lblScore.setPosition({ 1490,300 });
                            score = 0;
                            name = "";
                            setstars();
                            sprite.setPosition(45.f, 45.f);
                            enemy.setPosition(1000.f, 600.f);
                            item.setPosition(rand() % 1500 + 0, rand() % 720);
                            time2.setPosition(rand() % 1500 + 0, rand() % 720);
                            /*bulletVec.clear();*/
                            game_state = GAME_MENU;
                            break;
                    }
                }
                switch (ev.type)
                {
                case sf::Event::KeyReleased:
                    switch (ev.key.code)
                    {
                    case sf::Keyboard::Up:
                        gamewin.MoveUp();
                        break;

                    case sf::Keyboard::Down:
                        gamewin.MoveDown();
                        break;
                    }
                    break;
                }
            }
        }

        if (game_state == SAVE_SCORE)
        {
            Event ev;
            while (window.pollEvent(ev))
            {
                switch (ev.type)
                {
                case Event::Closed:
                    window.close();
                    break;
                case Event::TextEntered:
                    std::cout << ev.text.unicode << std::endl;
                    //delete name
                    if (ev.text.unicode == 8)
                    {
                        if (name.size() > 0)
                            name.erase(name.end() - 1);
                    }
                    //save name
                    if (ev.text.unicode == 13)
                    {
                        score_list.addEntry(name, score);
                        game_state = GAME_MENU;
                        lblScore.setCharacterSize(45);
                        //lblScore.setPosition({ 1335,270 });
                        lblScore.setPosition({ 1490,300 });
                        score = 0;
                        name = "";
                        setstars();
                        sprite.setPosition(45.f, 45.f);
                        enemy.setPosition(1000.f, 600.f);
                        item.setPosition(rand() % 1500 + 0, rand() % 720);
                        time2.setPosition(rand() % 1500 + 0, rand() % 720);
                        /*bulletVec.clear();*/
                    }
                    //input name
                    if ((ev.text.unicode >= 'a' && ev.text.unicode <= 'z') ||
                        (ev.text.unicode >= 'A' && ev.text.unicode <= 'Z'))
                    {
                        if (name.size() < 10)
                        {
                            name.push_back(ev.text.unicode);
                        }
                    }
                    std::cout << name << std::endl;
                    break;

                default:
                    break;    
                }
            }
        }
        if (game_state == GAME_SCOREBOARD)
        {
            Event ev;
            while (window.pollEvent(ev))
            {
                switch (ev.type)
                {
                case Event::Closed:
                    window.close();
                    break;
                case Event::TextEntered:
                    if (ev.text.unicode == 13)
                    {
                        game_state = GAME_MENU;
                    }
                    break;
                }
            }
        }
        window.clear();

        if (game_state == GAME_MENU) 
        {
            /* วาดเมนู */
            window.draw(background);
            menu.draw(window);
        } 
        if (game_state == GAME_PLAY) 
        {
            /* วาดเกม (ผู้เล่น, ศัตรู, ไอเทม) */
            window.draw(map);
            for (auto& wall : walls)
            {
                window.draw(wall);
            }
            for (auto& star : stars)
            {
                window.draw(star);
            }
            window.draw(sprite);
            window.draw(enemy);
            window.draw(item);
            window.draw(time2);
            /*for (int i = 0;i < bulletVec.size();i++)
            {
                bulletVec[i].draw(window);
                bulletVec[i].fire(3);
            }*/
            /*for (int i = 0;i < bulletVec.size();i++)
            {
                if (bulletVec[i].getRight() >= enemy.getPosition().x &&
                    bulletVec[i].getTop() < enemy.getPosition().y + enemy.getScale().y &&
                    bulletVec[i].getBottom() > enemy.getPosition().y)
                {
                    enemy.setPosition(sf::Vector2f(4234432, 4234423));
                    bulletVec[i].setPos(sf::Vector2f(4234432, 4234423));
                }
                FloatRect wallBounds = wall.getGlobalBounds();
                if (bulletVec[i].getRight() == wallBounds.left ||
                    bulletVec[i].getTop() == wallBounds.top ||
                    bulletVec[i].getBottom() == wallBounds.top)
                {
                    bulletVec[i].setPos(sf::Vector2f(4234432, 4234423));
                }
            }*/
            window.draw(lblScore);
            window.draw(exit);
        }
        if (game_state == GAME_OVER)
        {
            window.draw(gameoverbg);
            gameover.draw(window);
        }
        if (game_state == GAME_WIN)
        {
            window.draw(gamewinbg);
            gamewin.draw(window);
        }
        if (game_state == SAVE_SCORE)
        {
            window.draw(scorebg);
            Text text;
            text.setFont(puimek);
            lblScore.setFillColor(Color::Black);
            lblScore.setPosition(810, 90);
            lblScore.setCharacterSize(75);
            lblScore.setOrigin(lblScore.getLocalBounds().width / 2, lblScore.getLocalBounds().height / 2);
            window.draw(lblScore);

            text.setPosition(800, 245);
            text.setFillColor(Color::Black);
            text.setCharacterSize(75);
            text.setString(" ENTER YOUR NAME ");
            text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
            window.draw(text);

            text.setPosition(800, 400);
            text.setFillColor(Color::Black);
            text.setCharacterSize(75);
            text.setString(name + "_");
            text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
            window.draw(text);
        }
        if (game_state == GAME_SCOREBOARD)
        {
            window.draw(leaderboardbg);
            Text text;
            text.setFont(puimek);
            text.setFillColor(Color::White);
            text.setCharacterSize(50);
            int show_max_entry = 5;
            for (int i = 0;i < score_list.get().size();i++)
            {
                if (i >= show_max_entry)
                    break;
                text.setPosition(600, 275 + (i * 78));
                text.setString(score_list.get().at(i).getName());
                window.draw(text);
            }
            for (int i = 0;i < score_list.get().size();i++)
            {
                if (i >= show_max_entry)
                    break;
                text.setPosition(950, 275 + (i * 78));
                text.setString(to_string(score_list.get().at(i).getScore()));
                window.draw(text);
            }
        }
        window.display();
    }
    
    score_list.saveFile();
    return 0;
}

void setTiles()
{
    tiles = {
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
         {1,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1},
         {1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
         {1,0,0,0,1,1,0,1,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,0,0,1},
         {1,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,1,1,0,0,1},
         {1,0,1,1,0,0,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,1},
         {1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1},
         {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
         {1,0,1,1,1,1,1,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1},
         {1,0,1,0,0,0,0,0,1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,0,0,0,0,0,0,1,0,1},
         {1,0,1,0,1,1,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0,1,1,0,1,0,1},
         {1,0,1,0,1,1,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,0,1,0,1},
         {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
         {1,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,1},
         {1,0,1,1,0,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1},
         {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
}

void setWalls()
{
    for (int x = 0;x < 32;x++)
    {
        for (int y = 0;y < 18;y++)
        {
            if (tiles[y][x] == 1)
            {
                wall.setPosition(40.f * x, 40.f * y);
                walls.push_back(wall);
            }
        }
    }
}

void setstars()
{
    stars.clear();
    for (int x = 0;x < 32;x++)
    {
        for (int y = 0;y < 18;y++)
        {
            if (tiles[y][x] == 0)
            {
                //ใส่ดาว
                star.setPosition((40.5f * x) + 2.5, (40.5f * y) + 7);
                stars.push_back(star);
            }
        }
    }
}