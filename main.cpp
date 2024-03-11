#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "./include/bird.hpp"
#include "./include/pipe.hpp"

static int screenWidth = 500;
static int screenHeight = 800;
static bool gameOver = false;
static int score = 0;
static bool pass_pipe = false;


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
    bird.reset();
    pipes.clear();
    gameOver = false;
    score = 0;
    pass_pipe = false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Flappy Bird", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    sf::Image icon;
    if (!icon.loadFromFile("./Assets/icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load sounds
    // die.wav  hit.wav  point.wav  swoosh.wav  wing.wav
    sf::SoundBuffer die, hit, point, wing;
    if (!die.loadFromFile("./Assets/Sounds/die.wav") || 
        !hit.loadFromFile("./Assets/Sounds/hit.wav") || 
        !point.loadFromFile("./Assets/Sounds/point.wav")||
        !wing.loadFromFile("./Assets/Sounds/wing.wav")) {
        std::cerr << "Failed to load sounds!" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound die_sound = sf::Sound(die);
    sf::Sound hit_sound  = sf::Sound(hit);
    sf::Sound point_sound = sf::Sound(point);
    point_sound.setBuffer(point);
    // Load textures
    sf::Texture backgroundTexture, groundTexture, topPipeTexture, bottomPipeTexture;
    if (!backgroundTexture.loadFromFile("./Assets/background.png") ||
        !groundTexture.loadFromFile("./Assets/ground.png") ||
        !topPipeTexture.loadFromFile("./Assets/top_pipe.png") ||
        !bottomPipeTexture.loadFromFile("./Assets/bottom_pipe.png")) {
        std::cerr << "Failed to load textures" << std::endl;
        return EXIT_FAILURE;
    }

    // Create sprites
    sf::Sprite backgroundSprite(backgroundTexture), groundSprite(groundTexture);

    // Set ground position
    sf::Vector2f groundPos(0, screenHeight - groundSprite.getGlobalBounds().height);
    groundSprite.setPosition(groundPos);

    // Create bird
    Bird bird(screenHeight, "./Assets/yellow_bird.png", wing);

    // Create pipes
    std::vector<Pipe> pipes;
    pipes.emplace_back(screenWidth, randint(-200, 0), topPipeTexture, bottomPipeTexture);

    sf::Clock clock;
    sf::Font font;
    if (!font.loadFromFile("./Assets/Score/flappy-font.ttf")) {
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
            if (event.type == sf::Event::MouseButtonPressed && gameOver) {
                resetGame(bird, pipes);
            }
        }

        if (!gameOver) {
            managePipes(pipes, clock, topPipeTexture, bottomPipeTexture);
            groundPos.x -= SCROLL_SPEED;
            if (groundPos.x <= -80) {
                groundPos.x = 0;
            }
            groundSprite.setPosition(groundPos);

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

            bird.animate();
            bird.flap(event);
        }
        scoreText.setString(std::to_string(score));
        window.clear(sf::Color(135, 206, 235));
        window.draw(backgroundSprite);
        for (Pipe& pipe : pipes) {
            pipe.draw(window);
        }
        window.draw(groundSprite);
        bird.applyGravity();
        bird.draw(window);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
