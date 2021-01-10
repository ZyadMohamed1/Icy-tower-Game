#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Startmenu.h"
int score = 0, Highscore;

void Playerscore()
{
    std::fstream file;
    std::string HighS;

    file.open("Score.txt", std::ios::in);
    std::getline(file, HighS);
    file.close();

    Highscore = std::stoi(HighS);
    std::cout << Highscore << std::endl;
    while (true)
    {
        if (score > Highscore)
        {
            Highscore = score;
            file.open("score.txt", std::ios::out);
            file << Highscore;
            file.close();
        }
    }
}


struct Pos
{
    int x, y;
    float size;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "Icy Tower", sf::Style::Close | sf::Style::Titlebar);
    Startmenu startmenu(window.getSize().x, window.getSize().y);
    sf::Event event;
    window.setFramerateLimit(60);//Setting frame limit to 60

    //player1 score
    sf::Font f1;
    f1.loadFromFile("text/ALISO.otf");
    sf::Text t1;
    std::string text = "SCORE: ";
    text += std::to_string(score);
    sf::String textscreen = text;
    t1.setFont(f1);
    t1.setCharacterSize(20);
    t1.setString(textscreen);
    t1.setFillColor(sf::Color::White);
    t1.setPosition(sf::Vector2f(60.0f, 420));

    //player2 score
    sf::Text t2;
    std::string text2 = "SCORE: ";
    text2 += std::to_string(score);
    sf::String textscreen2 = text2;
    t2.setFont(f1);
    t2.setCharacterSize(20);
    t2.setString(textscreen2);
    t2.setFillColor(sf::Color::White);
    t2.setPosition(sf::Vector2f(500.0f, 420));

    //corona virus
    sf::CircleShape corona(40);
    sf::Texture CoronaTexture;
    CoronaTexture.loadFromFile("textures/coco.png");
    corona.setTexture(&CoronaTexture);

    //start menu
    sf::Texture Startmenu;
    Startmenu.loadFromFile("textures/ice.png");
    sf::Sprite Startmenuspirit;
    Startmenuspirit.setTexture(Startmenu);
    Startmenuspirit.setTextureRect(sf::IntRect(0, 0, 640, 480));
    bool startgame = true;
    sf::Texture instruction;
    instruction.loadFromFile("textures/instructions.png");
    sf::Sprite instruc;
    instruc.setTexture(instruction);
    instruc.setTextureRect(sf::IntRect(0, 0, 640, 480));
    bool inst = false;

    //startmenu music
    sf::SoundBuffer buffer, bufferjump, bufferfall;
    if (!buffer.loadFromFile("Music/media.io_startmenu.wav"))
    {
        return -1;
    }
    if (!bufferjump.loadFromFile("Music/jump_lo.wav"))
    {
        return -1;
    }
    if (!bufferfall.loadFromFile("Music/falling.wav"))
    {
        return -1;
    }
    sf::Sound menumusic, jumpingsound, fallingsound;
    menumusic.setBuffer(buffer);
    jumpingsound.setBuffer(bufferjump);
    fallingsound.setBuffer(bufferfall);
    menumusic.play();
    menumusic.setVolume(200.0f);

    //Creating textures and loading them from their file
    sf::Texture texture, texture2, texture3, texture4, texture5, texture1;
    texture.loadFromFile("textures/BiggyT.png");
    texture1.loadFromFile("textures/BiggyT.png");
    texture2.loadFromFile("textures/real BG.png");
    texture3.loadFromFile("textures/log.png");
    texture4.loadFromFile("textures/game.png");
    texture5.loadFromFile("textures/blackwall.png");

    //First level background textures
    //Creating the background
    sf::RectangleShape background(sf::Vector2f(640.0f, 480.0f));
    background.setTexture(&texture2);

    //next level background
    sf::RectangleShape BlackBackground(sf::Vector2f(640.0f, 480.0f));
    sf::RectangleShape Rightblackwall(sf::Vector2f(70.0f, 480.0f));
    sf::RectangleShape Leftblackwall(sf::Vector2f(70.0f, 480.0f));
    BlackBackground.setTexture(&texture4);
    Rightblackwall.setTexture(&texture5);
    Leftblackwall.setTexture(&texture5);
    Rightblackwall.setPosition(sf::Vector2f(590.0f, 0.0f));
    Leftblackwall.setPosition(sf::Vector2f(-20.0f, 0.0f));

    //walls
    std::vector<sf::RectangleShape> walls;
    sf::RectangleShape wall1, wall2;
    wall1.setFillColor(sf::Color::Red);
    wall2.setFillColor(sf::Color::Red);
    wall1.setSize(sf::Vector2f(40.0f, 480.0f));
    wall1.setPosition(sf::Vector2f(590.0f, 0.0f));
    wall2.setSize(sf::Vector2f(50.0f, 480.0f));
    wall2.setPosition(sf::Vector2f(0.0f, 0.0f));
    walls.push_back(wall1);
    walls.push_back(wall2);


    //Creating a mainlog
    sf::RectangleShape mainlog(sf::Vector2f(550.0f, 10.0f));
    mainlog.setPosition(45.0f, 450.0f);
    mainlog.setTexture(&texture3);

    //creating more than one log
    int y = 420;
    sf::RectangleShape test(sf::Vector2f(200.0f, 10.0f));
    test.setTexture(&texture3);
    Pos platform[10];
    for (int i = 0;i < 6;i++)
    {
        // start positioning form 60 and increased by 280 by a random value
        platform[i].x = rand() % 280 + 60;
        platform[i].y = 460 - y;
        y -= 70;
    }

    //Creating IntRect and setting it's default height and width to 0,0 so that it can be changed in the loop
    sf::IntRect rectSourceSprite(0, 0, 30, 50);
    sf::IntRect rectSourceSprite2(0, 0, 30, 50);

    //creating a ground
    sf::RectangleShape ground(sf::Vector2f(5000.0f, 60.0f));
    ground.setPosition(0.0f, 475.0f);
    ground.setFillColor(sf::Color::Transparent);


    //Collision
    //Player 1
    sf::FloatRect nextPos;

    float moveSpeed = 4.2;//Default moving speed
    float jumpSpeed = -9;//Default jumping speed

    sf::Vector2f velocity(0.0f, 0.0f);//Velocity vector, is changed in the loop

    float gravity = 1;//Default gravity

    float acc = 1;//Default acceleration
    float brake = 1;
    bool accON = false, decON = false;


    bool jump = false, fall = false;
    int jumpCounter = 1;

    bool groundCheck = false, logCheck = false;

    //Creating sprite
    sf::Sprite sprite(texture, rectSourceSprite);
    sprite.setPosition(150, 400);

    sf::RectangleShape rectCollision(sf::Vector2f(30.0f, 1.0f));
    rectCollision.setFillColor(sf::Color::Transparent);
    rectCollision.setPosition(150, 452);
    sf::Clock clock;

    //Player 2
    sf::FloatRect nextPos2;

    float moveSpeed2 = 4.2;//Default moving speed
    float jumpSpeed2 = -9;//Default jumping speed

    sf::Vector2f velocity2(0.0f, 0.0f);//Velocity vector, is changed in the loop

    float gravity2 = 1;//Default gravity

    float acc2 = 1;//Default acceleration
    float brake2 = 1;
    bool accON2 = false, decON2 = false;


    bool jump2 = false, fall2 = false;
    int jumpCounter2 = 1;

    bool groundCheck2 = false, logCheck2=false;

    //Creating sprite
    sf::Sprite sprite2(texture1, rectSourceSprite2);
    sprite2.setPosition(400, 400);

    sf::RectangleShape rectCollision2(sf::Vector2f(30.0f, 1.0f));
    rectCollision2.setFillColor(sf::Color::Transparent);
    rectCollision2.setPosition(400, 452);
    sf::Clock clock2;


    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Event::KeyReleased)
            {
                if (sf::Keyboard::Key::Right | sf::Keyboard::Key::Left) {
                    rectSourceSprite.top = 0;
                    accON = false;//Disables acceleration
                    decON = true;//Enables deceleration till movement stops
                }//END OF KEY RELEASED
            }
            if (sf::Event::KeyReleased)
            {
                if (sf::Keyboard::Key::D | sf::Keyboard::Key::A) {
                    rectSourceSprite2.top = 0;
                    accON2 = false;//Disables acceleration
                    decON2 = true;//Enables deceleration till movement stops
                }//END OF KEY RELEASED
            }
            switch (event.type)
            {

            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::Up:
                    startmenu.MoveUp();
                    break;

                case sf::Keyboard::Down:
                    startmenu.MoveDown();
                    break;

                case sf::Keyboard::Return:
                    switch (startmenu.getpressed())
                    {
                    case 0:
                        std::cout << "play button is pressed" << std::endl;
                        startgame = false;
                        break;
                    case 1:
                        std::cout << "instruction button is pressed" << std::endl;
                        startgame = false;
                        inst = true;
                        break;
                    case 2:
                        std::cout << "option button is pressed" << std::endl;
                        break;
                    case 6:
                        window.close();
                        break;
                    }

                    break;
                }

            }//END OF SWITCH


        }//END OF EVENT LOOP

        //PLAYER 1

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            rectSourceSprite.top = 50;
            velocity.x = moveSpeed * acc;
            accON = true;
            brake = 1;
        }//END OF RIGHT

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            rectSourceSprite.top = 100;
            velocity.x = -moveSpeed * acc;
            accON = true;
            brake = 1;
        }//END OF LEFT

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            jumpingsound.play();
            rectSourceSprite.top = 150;
            if (!jump && !fall)
            {
                jump = true;//Allows the jumping loop to be entered
            }

        }//END OF SPACE




        if (accON) {//Accelerates the player gradually until key is released


            if (acc >= 2.2)//Doesn't let acceleration increase over 2.2
                acc = 2.2;
            else if (acc > 0)
                acc += 0.0400;//Gradually increases acceleration if it less than 2.2

        }//END OF ACCELERATION


        else if (decON && !accON) {//Decelerates the player gradually until he moves again or stops moving
            acc = 1;

            if (brake <= 0) {
                velocity.x = 0;
                decON = false;
            }

            else if (velocity.x != 0) {
                velocity.x *= brake;
                brake -= 0.02;
            }

        }//END OF DECELERATION



        if (rectCollision.getGlobalBounds().intersects(mainlog.getGlobalBounds()))
            groundCheck = true;
        else
            groundCheck = false;


        if (jump) {//If jump=true , The player will jump upwards until the counter limit is reached and then fall will be set =true

            if (jumpCounter == 30) {//When maximum jumping limit is reached,the player stops jumping and will begin falling
                velocity.y = 0;
                jump = false;
                fall = true;
                jumpCounter = 1;
                gravity = 1;
            }

            else {

                if (jumpCounter >= 1)
                    velocity.y = jumpSpeed * gravity;
                else
                    velocity.y = jumpSpeed;

                jumpCounter += 1;
                gravity -= 0.05;
            }
        }//END OF JUMP




        else if (fall) {//If fall=true , The player will keep falling downwards until he collides with the ground or a log

            if (groundCheck || logCheck) {
                velocity.y = 0;
                fall = false;
                gravity = 1;
            }

            else {
                velocity.y = -jumpSpeed * gravity;
                gravity += 0.001;
            }

        }//END OF FALL



        else if (!groundCheck && !logCheck) {//Keeps the player falling when in mid-air(until he collides with an object)
            velocity.y = -jumpSpeed * gravity;
            gravity += 0.01;
        }

        else if (groundCheck || logCheck) {//Stops the player once he has collided with either the ground or a log
            velocity.y = 0;
            gravity = 1;
        }


        if (clock.getElapsedTime().asSeconds() > 0.2f)//Moving Frames
        {

            if (rectSourceSprite.left == 90)//Resets the animation if the last frame is reached
                rectSourceSprite.left = 0;

            else
                rectSourceSprite.left += 30;//Moves one frame until the end of the animation is reached


            clock.restart();
            sprite.setTextureRect(rectSourceSprite);
        }//END OF ANIMATION


           //PLAYER 2

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            rectSourceSprite2.top = 50;
            velocity2.x = moveSpeed2 * acc2;
            accON2 = true;
            brake2 = 1;
        }//END OF RIGHT

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            rectSourceSprite2.top = 100;
            velocity2.x = -moveSpeed2 * acc2;
            accON2 = true;
            brake2 = 1;
        }//END OF LEFT

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            jumpingsound.play();
            rectSourceSprite2.top = 150;
            if (!jump2 && !fall2)
            {
                jump2 = true;//Allows the jumping loop to be entered
            }

        }//END OF SPACE




        if (accON2) {//Accelerates the player gradually until key is released


            if (acc2 >= 2.2)//Doesn't let acceleration increase over 2.2
                acc2 = 2.2;
            else if (acc2 > 0)
                acc2 += 0.0400;//Gradually increases acceleration if it less than 2.2

        }//END OF ACCELERATION


        else if (decON2 && !accON2) {//Decelerates the player gradually until he moves again or stops moving
            acc2 = 1;

            if (brake2 <= 0) {
                velocity2.x = 0;
                decON2 = false;
            }

            else if (velocity2.x != 0) {
                velocity2.x *= brake;
                brake2 -= 0.02;
            }

        }//END OF DECELERATION



        if (rectCollision2.getGlobalBounds().intersects(mainlog.getGlobalBounds()))
            groundCheck2 = true;
        else
            groundCheck2 = false;


        if (jump2) {//If jump=true , The player will jump upwards until the counter limit is reached and then fall will be set =true

            if (jumpCounter2 == 30) {//When maximum jumping limit is reached,the player stops jumping and will begin falling
                velocity2.y = 0;
                jump2 = false;
                fall2 = true;
                jumpCounter2 = 1;
                gravity2 = 1;
            }

            else {

                if (jumpCounter2 >= 1)
                    velocity2.y = jumpSpeed2 * gravity2;
                else
                    velocity2.y = jumpSpeed2;

                jumpCounter2 += 1;
                gravity2 -= 0.05;
            }
        }//END OF JUMP




        else if (fall2) {//If fall=true , The player will keep falling downwards until he collides with the ground or a log

            if (groundCheck2 || logCheck2) {
                velocity2.y = 0;
                fall2 = false;
                gravity2 = 1;
            }

            else {
                velocity2.y = -jumpSpeed2 * gravity2;
                gravity2 += 0.001;
            }

        }//END OF FALL



        else if (!groundCheck2 && !logCheck2) {//Keeps the player falling when in mid-air(until he collides with an object)
            velocity2.y = -jumpSpeed2 * gravity2;
            gravity2 += 0.01;
        }

        else if (groundCheck2 || logCheck2) {//Stops the player once he has collided with either the ground or a log
            velocity2.y = 0;
            gravity2 = 1;
        }


        if (clock2.getElapsedTime().asSeconds() > 0.2f)//Moving Frames
        {

            if (rectSourceSprite2.left == 90)//Resets the animation if the last frame is reached
                rectSourceSprite2.left = 0;

            else
                rectSourceSprite2.left += 30;//Moves one frame until the end of the animation is reached


            clock2.restart();
            sprite2.setTextureRect(rectSourceSprite2);
        }//END OF ANIMATION



        //collision of walls Player 1
        for (auto& wall : walls)
        {
            sf::FloatRect playerBound = sprite.getGlobalBounds();
            sf::FloatRect rectangleBound = rectCollision.getGlobalBounds();
            sf::FloatRect wallBounds = wall.getGlobalBounds();
            nextPos = playerBound;
            nextPos.left += velocity.x;
            nextPos.top += velocity.y;

            if (wallBounds.intersects(nextPos))
            {
                
                //Right collision with wall
                if (playerBound.left < wallBounds.left
                    && playerBound.left + playerBound.width < wallBounds.left + wallBounds.width
                    && playerBound.top < wallBounds.top + wallBounds.height
                    && playerBound.top + playerBound.height > wallBounds.top
                    )
                {
                    velocity.x = -velocity.x * 1;
                    sprite.setPosition(wallBounds.left - playerBound.width, playerBound.top);
                }
                //left collision with wall
                else if (playerBound.left > wallBounds.left
                    && playerBound.left + playerBound.width > wallBounds.left + wallBounds.width
                    && playerBound.top < wallBounds.top + wallBounds.height
                    && playerBound.top + playerBound.height > wallBounds.top
                    )
                {
                    velocity.x = -velocity.x * 1;
                    sprite.setPosition(wallBounds.left + wallBounds.width, playerBound.top);
                }
                //Right collision with wall (rectangle under player)
                if (rectangleBound.left < wallBounds.left
                    && rectangleBound.left + rectangleBound.width < wallBounds.left + wallBounds.width
                    && rectangleBound.top < wallBounds.top + wallBounds.height
                    && rectangleBound.top + rectangleBound.height > wallBounds.top
                    )
                {
                    velocity.x = -velocity.x * 1;
                    rectCollision.setPosition(wallBounds.left - rectangleBound.width, rectangleBound.top);
                }
                //left collision with wall (rectangle under player)
                else if (rectangleBound.left > wallBounds.left
                    && rectangleBound.left + rectangleBound.width > wallBounds.left + wallBounds.width
                    && rectangleBound.top < wallBounds.top + wallBounds.height
                    && rectangleBound.top + rectangleBound.height > wallBounds.top
                    )
                {
                    velocity.x = 0.f;
                    rectCollision.setPosition(wallBounds.left + wallBounds.width, rectangleBound.top);
                }
            }
        }

        //collision of walls Player 2
        for (auto& wall : walls)
        {
            sf::FloatRect playerBound2 = sprite2.getGlobalBounds();
            sf::FloatRect rectangleBound2 = rectCollision2.getGlobalBounds();
            sf::FloatRect wallBounds2 = wall.getGlobalBounds();
            nextPos2 = playerBound2;
            nextPos2.left += velocity2.x;
            nextPos2.top += velocity2.y;

            if (wallBounds2.intersects(nextPos2))
            {

                //Right collision with wall
                if (playerBound2.left < wallBounds2.left
                    && playerBound2.left + playerBound2.width < wallBounds2.left + wallBounds2.width
                    && playerBound2.top < wallBounds2.top + wallBounds2.height
                    && playerBound2.top + playerBound2.height > wallBounds2.top
                    )
                {
                    velocity2.x = -velocity2.x * 1;
                    sprite2.setPosition(wallBounds2.left - playerBound2.width, playerBound2.top);
                }
                //left collision with wall
                else if (playerBound2.left > wallBounds2.left
                    && playerBound2.left + playerBound2.width > wallBounds2.left + wallBounds2.width
                    && playerBound2.top < wallBounds2.top + wallBounds2.height
                    && playerBound2.top + playerBound2.height > wallBounds2.top
                    )
                {
                    velocity2.x = -velocity2.x * 1;
                    sprite2.setPosition(wallBounds2.left + wallBounds2.width, playerBound2.top);
                }
                //Right collision with wall (rectangle under player)
                if (rectangleBound2.left < wallBounds2.left
                    && rectangleBound2.left + rectangleBound2.width < wallBounds2.left + wallBounds2.width
                    && rectangleBound2.top < wallBounds2.top + wallBounds2.height
                    && rectangleBound2.top + rectangleBound2.height > wallBounds2.top
                    )
                {
                    velocity2.x = -velocity2.x * 1;
                    rectCollision2.setPosition(wallBounds2.left - rectangleBound2.width, rectangleBound2.top);
                }
                //left collision with wall (rectangle under player)
                else if (rectangleBound2.left > wallBounds2.left
                    && rectangleBound2.left + rectangleBound2.width > wallBounds2.left + wallBounds2.width
                    && rectangleBound2.top < wallBounds2.top + wallBounds2.height
                    && rectangleBound2.top + rectangleBound2.height > wallBounds2.top
                    )
                {
                    velocity2.x = 0.f;
                    rectCollision2.setPosition(wallBounds2.left + wallBounds2.width, rectangleBound2.top);
                }
            }
        }


        if (startgame == true)
        {
            window.clear();
            window.draw(Startmenuspirit);
            startmenu.draw(window);
            window.display();
        }
        else if (inst == true && startgame == false)
        {
            window.clear();
            window.draw(instruc);
            window.display();
        }
        else if (startgame == false && inst == false)
        {
            menumusic.stop();
            rectCollision.move(velocity);
            sprite.move(velocity);//Moves the player according to the changes in velocity inside the loop
            rectCollision2.move(velocity2);
            sprite2.move(velocity2);//Moves the player according to the changes in velocity inside the loop
            window.clear();
            window.draw(BlackBackground);
            window.draw(Rightblackwall);
            window.draw(Leftblackwall);
            window.draw(mainlog);
            logCheck = false;//lazem tob2a false hena bas 
            logCheck2 = false;
            for (int i = 0;i < 6;i++)
            {

                test.setPosition(platform[i].x, platform[i].y);
                window.draw(test);

                if (rectCollision.getGlobalBounds().intersects(test.getGlobalBounds()))
                    logCheck = true;
                if (rectCollision2.getGlobalBounds().intersects(test.getGlobalBounds()))
                    logCheck2 = true;
            }
            window.draw(t1);
            window.draw(t2);
            window.draw(rectCollision);
            window.draw(sprite);
            window.draw(rectCollision2);
            window.draw(sprite2);
            window.draw(corona);
            window.display();
        }
    }//END OF WINDOW LOOP
    return 0;

}//END OF MAIN
