#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "../include/bird.hpp"
#include "../include/pipe.hpp"


#define SPEED 3.8
typedef enum {
    LOGO,
    GAMEPLAY,
    ENDING,
} gameScreen;


static int screenWidth = 500;
static int screenHeight = 800;
static bool gameOver = false;
static int score = 0;
static bool pass_pipe = false;
static gameScreen currentScreen = LOGO;
static bool is_flapping = false;

class Logo {
private:
    sf::Texture logoTexture, buttonTexture;
    sf::Sprite logoSprite, buttonSprite;
    sf::Vector2f logoPos, buttonPos;
    float velocity;
    const float upperBound = 80.0f;
    const float lowerBound = 120.0f;

public:
    Logo(const sf::Texture& logoTexture, const sf::Texture& buttonTexture, float initialVelocity) :
        logoTexture(logoTexture), buttonTexture(buttonTexture), velocity(initialVelocity) {
        logoSprite.setTexture(this->logoTexture);
        buttonSprite.setTexture(this->buttonTexture);
        logoPos   = sf::Vector2f(screenWidth / 2 - logoTexture.getSize().x /2, 100);
        buttonPos = sf::Vector2f(screenWidth / 2 - buttonTexture.getSize().x / 2, 500);
        logoSprite.setPosition(logoPos);
        buttonSprite.setPosition(buttonPos);
    }

    void update() {
        logoPos.y += velocity;
        if (logoPos.y <= upperBound || logoPos.y >= lowerBound) {
            velocity *= -1;
        }

        logoSprite.setPosition(logoPos);
    }

    void draw(sf::RenderWindow& win) {
        win.draw(logoSprite);
        win.draw(buttonSprite);
    }
     
    sf::Vector2f getButtonPos(void) const {
        return buttonPos;
    }

    sf::Vector2u getButtonSize(void) const {
        return buttonTexture.getSize();
    }
};

std::mt19937 gen(std::time(nullptr));

int randint(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

bool isIntersecting(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}

void managePipes(std::vector<Pipe>& pipes, sf::Clock& clock, sf::Texture& topPipeTexture, sf::Texture& bottomPipeTexture) {
    float elapsed = clock.getElapsedTime().asSeconds();

    pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
                               [](const Pipe& p) { return p.isOutOfBounds(); }),
                pipes.end());

    if (elapsed >= TIME_GAP) {
        pipes.emplace_back(screenWidth, randint(-200, 0), topPipeTexture, bottomPipeTexture);
        clock.restart();
    }
}


void resetGame(Bird& bird, std::vector<Pipe>& pipes) {
    currentScreen = LOGO;
    bird.reset();
    pipes.clear();
    gameOver = false;
    score = 0;
    pass_pipe = false;
    is_flapping = false;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Flappy Bird", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    sf::Image icon;
    if (!icon.loadFromFile("./Assets/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load sounds
    // die.wav  hit.wav  point.wav  swoosh.wav  wing.wav
    sf::SoundBuffer die, hit, point, wing, swoosh;
    if (!die.loadFromFile("./Assets/Sounds/die.wav") || 
        !hit.loadFromFile("./Assets/Sounds/hit.wav") || 
        !point.loadFromFile("./Assets/Sounds/point.wav")||
        !wing.loadFromFile("./Assets/Sounds/wing.wav") ||
        !swoosh.loadFromFile("./Assets/Sounds/swoosh.wav")) {
        std::cerr << "Failed to load sounds!" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound die_sound = sf::Sound(die);
    sf::Sound hit_sound  = sf::Sound(hit);
    sf::Sound point_sound = sf::Sound(point);
    sf::Sound swoosh_sound = sf::Sound(swoosh);
    point_sound.setBuffer(point);

    sf::Texture backgroundTexture, groundTexture, topPipeTexture, bottomPipeTexture, logoTexture, buttonTexture, instructionTexture;
    if (!backgroundTexture.loadFromFile("./Assets/background.png") ||
        !groundTexture.loadFromFile("./Assets/ground.png") ||
        !topPipeTexture.loadFromFile("./Assets/top_pipe.png") ||
        !bottomPipeTexture.loadFromFile("./Assets/bottom_pipe.png") ||
        !logoTexture.loadFromFile("./Assets/flappy_bird_logo.png") ||
        !buttonTexture.loadFromFile("./Assets/button_play.png") ||
        !instructionTexture.loadFromFile("./Assets/instructions.png")) {
        std::cerr << "Failed to load textures" << std::endl;
        return EXIT_FAILURE;
    }


    sf::Sprite backgroundSprite(backgroundTexture), groundSprite(groundTexture), instructionSprite(instructionTexture);

    // Set ground position
    sf::Vector2f groundPos(0, screenHeight - groundSprite.getGlobalBounds().height);
    groundSprite.setPosition(groundPos);

    instructionSprite.setPosition(sf::Vector2f(screenWidth / 2 - instructionTexture.getSize().x / 2, 55));
    Logo logoScreen(logoTexture, buttonTexture, 1.1);
 
    // Create bird
    Bird bird(screenHeight, "./Assets/Grumpy/yellow_bird.png", wing);

    // Create pipes
    std::vector<Pipe> pipes;

    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile("./Assets/Font/flappy-font.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(44);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(screenWidth / 2, 20);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (currentScreen == LOGO) {
                        sf::Vector2i mp = sf::Mouse::getPosition(window);
                        if(mp.x > logoScreen.getButtonPos().x && 
                        mp.x <= logoScreen.getButtonPos().x + logoScreen.getButtonSize().x &&
                        mp.y > logoScreen.getButtonPos().y &&
                        mp.y <= logoScreen.getButtonPos().y + logoScreen.getButtonSize().y) {
                            currentScreen = GAMEPLAY;  
                            swoosh_sound.play();
                        }
                    }

                    else if (currentScreen == GAMEPLAY && !gameOver) {
                        is_flapping = true;
                    }
                    else if (gameOver) {
                        currentScreen = LOGO; 
                        resetGame(bird, pipes);
                    }


                }

            }
        }

         if (!gameOver || currentScreen == LOGO) {
             groundPos.x -= SPEED;
             if (groundPos.x <= -80) {
                groundPos.x = 0;
             }
         }

       groundSprite.setPosition(groundPos);
        if (is_flapping)
            bird.applyGravity();

        if (!gameOver && currentScreen == GAMEPLAY) {

            bird.animate();
            if(is_flapping) { 
            managePipes(pipes, clock, topPipeTexture, bottomPipeTexture);
            for (Pipe& pipe : pipes) {
                pipe.update();
                if (isIntersecting(bird.getSprite(), pipe.getTopSprite()) ||
                    isIntersecting(bird.getSprite(), pipe.getBottomSprite())) {
                    hit_sound.play();
                    die_sound.play();
                    gameOver = true;
                }
            }

            // Score
            Pipe pipe = pipes.front();
            if (bird.getPosition().x > pipe.getPosition().x &&
                bird.getPosition().x + bird.getSize().x / 3 < pipe.getPosition().x + pipe.getSize().x ) {
                pass_pipe = true;
            }   

            if (pass_pipe) {
                if(bird.getPosition().x + bird.getSize().x / 3 > pipe.getPosition().x + pipe.getSize().x) {
                    point_sound.play(); 
                    score += 1;
                    pass_pipe = false; 
                }
            }

           if (bird.getPosition().y + bird.getSize().y  >= groundPos.y) {
                hit_sound.play();
                gameOver = true;
            }
            bird.flap(event);
            }
        }

        scoreText.setString(std::to_string(score));
        window.clear(sf::Color::White);
        window.draw(backgroundSprite);

        switch(currentScreen)
        {
             case LOGO:
                logoScreen.update();
                logoScreen.draw(window);
                break;
             case GAMEPLAY:
                if(!is_flapping)
                    window.draw(instructionSprite);
                else {
                    for (Pipe &pipe: pipes) {
                        pipe.draw(window);
                    } 
                }

                bird.draw(window);
                window.draw(scoreText);
                break;
            default:
                 break;
        }

        window.draw(groundSprite);
        window.display();

    }

    return 0;
}
