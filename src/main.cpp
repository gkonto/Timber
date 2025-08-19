#include <SFML/Graphics.hpp>
#include <sstream>

const int NUM_BRANCHES = 6;

enum class side
{
    LEFT,
    RIGHT,
    NONE
};

side branchPositions[NUM_BRANCHES];

static void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j >= 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }

    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Timber");

    sf::Texture textureBackground;
    textureBackground.loadFromFile("assets/graphics/background.png");
    sf::Sprite spriteBackground(textureBackground);
    spriteBackground.setPosition({0, 0});

    sf::Texture textureTree;
    textureTree.loadFromFile("assets/graphics/tree.png");
    sf::Sprite spriteTree(textureTree);
    spriteTree.setPosition({810, 0});

    sf::Texture textureBee;
    textureBee.loadFromFile("assets/graphics/bee.png");
    sf::Sprite spriteBee(textureBee);
    spriteBee.setPosition({0, 800});
    bool beeActive = false;
    float beeSpeed = 0.0f;

    sf::Texture textureCloud;
    textureCloud.loadFromFile("assets/graphics/cloud.png");
    sf::Sprite spriteCloud1(textureCloud);
    sf::Sprite spriteCloud2(textureCloud);
    sf::Sprite spriteCloud3(textureCloud);
    spriteCloud1.setPosition({0, 0});
    spriteCloud2.setPosition({0, 250});
    spriteCloud3.setPosition({0, 500});
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    sf::Clock clock;

    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize({timeBarStartWidth, timeBarHeight});
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition({(1920 / 2) - timeBarStartWidth / 2, 980});

    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;
    int score = 0;

    sf::Font font;
    font.openFromFile("assets/fonts/KOMIKAP_.ttf");
    sf::Text messageText(font);
    sf::Text scoreText(font);

    messageText.setFont(font);
    scoreText.setFont(font);

    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.position + textRect.size / 2.f);
    messageText.setPosition({1920 / 2.0f, 1080 / 2.0f});
    scoreText.setPosition({20, 20});

    sf::Texture textureBranch;
    textureBranch.loadFromFile("assets/graphics/branch.png");
    sf::Sprite branches[NUM_BRANCHES] = {
        sf::Sprite(textureBranch),
        sf::Sprite(textureBranch),
        sf::Sprite(textureBranch),
        sf::Sprite(textureBranch),
        sf::Sprite(textureBranch),
        sf::Sprite(textureBranch)
    };

    for (int i = 0; i < NUM_BRANCHES; ++i) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition({-2000, -2000});
        branches[i].setOrigin({220, 20});
    }

    updateBranches(0);
    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);

    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("assets/graphics/player.png");
    sf::Sprite spritePlayer(texturePlayer);
    spritePlayer.setPosition({580, 720});
    side playerSide = side::LEFT;

    sf::Texture textureRIP;
    textureRIP.loadFromFile("assets/graphics/rip.png");
    sf::Sprite spriteRIP(textureRIP);
    spriteRIP.setPosition({600, 860});

    sf::Texture textureAxe;
    textureAxe.loadFromFile("assets/graphics/axe.png");
    sf::Sprite spriteAxe(textureAxe);
    spriteAxe.setPosition({700, 830});
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    sf::Texture textureLog;
    textureLog.loadFromFile("assets/graphics/log.png");
    sf::Sprite spriteLog(textureLog);
    spriteLog.setPosition({810, 720});

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    bool acceptInput  = false;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event.value().is<sf::Event::KeyReleased>() && !paused) {
                acceptInput = true;
                spriteAxe.setPosition({2000, spriteAxe.getPosition().y});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                paused = false;
                score = 0;
                timeRemaining = 6;
                clock.restart();

                for (int i = 1; i < NUM_BRANCHES; ++i) {
                    branchPositions[i] = side::NONE;
                }
                spriteRIP.setPosition({675, 2000});
                spritePlayer.setPosition({580, 720});
                acceptInput = true;
            }
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (acceptInput) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                    playerSide = side::RIGHT;
                    score++;
                    timeRemaining += (2 / score) + .15;
                    spriteAxe.setPosition({AXE_POSITION_RIGHT, spriteAxe.getPosition().y});
                    spritePlayer.setPosition({1200, 720});
                    updateBranches(score);
                    spriteLog.setPosition({810, 720});
                    logSpeedX = -5000;
                    logActive = true;
                    acceptInput = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                    playerSide = side::LEFT;
                    score++;
                    timeRemaining += (2 / score) + .15;
                    spriteAxe.setPosition({AXE_POSITION_LEFT, spriteAxe.getPosition().y});
                    spritePlayer.setPosition({580, 720});
                    updateBranches(score);
                    spriteLog.setPosition({810, 720});
                    logSpeedX = 5000;
                    logActive = true;
                    acceptInput = false;
                }
            }
        }



        if (!paused) {
            // Update the scene
            sf::Time dt = clock.restart();
            timeRemaining -= dt.asSeconds();
            timeBar.setSize({timeBarWidthPerSecond * timeRemaining, timeBarHeight});

            if (!beeActive) {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition({2000, height});
                beeActive = true;
            } else {
                spriteBee.setPosition({spriteBee.getPosition().x - beeSpeed * dt.asSeconds(), spriteBee.getPosition().y});
                if (spriteBee.getPosition().x < -100) {
                    beeActive = false;
                }
            }

            if (!cloud1Active) {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition({-200, height});
                cloud1Active = true;
            } else {
                spriteCloud1.setPosition({spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y});
                if (spriteCloud1.getPosition().x > 1920) {
                    cloud1Active = false;
                }
            }

            if (!cloud2Active) {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition({-200, height});
                cloud2Active = true;
            } else {
                spriteCloud2.setPosition({spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y});
                if (spriteCloud2.getPosition().x > 1920) {
                    cloud2Active = false;
                }
            }

            if (!cloud3Active) {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition({-200, height});
                cloud3Active = true;
            } else {
                spriteCloud3.setPosition({spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y});
                if (spriteCloud3.getPosition().x > 1920) {
                    cloud3Active = false;
                }
            }

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; ++i) {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT) {
                    branches[i].setPosition({610, height});
                    branches[i].setRotation(sf::degrees(180.f));
                } else if (branchPositions[i] == side::RIGHT) {
                    branches[i].setPosition({1330, height});
                    branches[i].setRotation(sf::degrees(0.f));
                } else {
                    branches[i].setPosition({3000, height});
                }

                if (logActive) {
                    spriteLog.setPosition({spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())});
                    if (spriteLog.getPosition().x < -100 ||
                        spriteLog.getPosition().x > 2000) {
                        logActive = false;
                        spriteLog.setPosition({810, 720});
                    }
                }

                if (branchPositions[5] == playerSide) {
                    paused = true;
                    acceptInput = false;
                    spriteRIP.setPosition({525, 760});
                    spritePlayer.setPosition({2000, 660});
                    messageText.setString("SQUISHED!!");
                    sf::FloatRect textRect = messageText.getLocalBounds();
                    messageText.setOrigin(textRect.position + textRect.size / 2.f);
                    messageText.setPosition({1920 / 2.0f, 1080 / 2.0f});
                }

            }

            if (timeRemaining <= 0.0f) {
                paused = true;
                messageText.setString("Out of time!!");
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.position + textRect.size / 2.f);
                messageText.setPosition({1920 / 2.0f, 1080 / 2.0f});
            }
        }

        // Draw the scene
        window.clear();


        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(timeBar);
        if (paused) {
            window.draw(messageText);
        }
        for (int i = 0; i < NUM_BRANCHES; ++i) {
            window.draw(branches[i]);
        }
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        window.draw(spriteBee);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
