#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm> 

using namespace sf;

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define GRAVITY 0.6
#define GAP 200
#define SCROLL_SPEED 4.0f
#define TIME_GAP 1.2f


static int screenWidth  = 500;
static int screenHeight = 800;
std::mt19937 gen(std::time(nullptr));

class Bird {
public:
    Texture birdTexture;
    Sprite birdSprite;
    Vector2u textureSize;
    Vector2f position;
    int screenHeight, currentFrame;
    float vel;
    float strenght;
    bool gotPressed;
    float rot;
    
    Bird(int screenHeight, const char* texturePath)
        : screenHeight(screenHeight), currentFrame(0), rot(0.0f) {
        birdTexture.loadFromFile(texturePath);
        textureSize = birdTexture.getSize();
        birdSprite.setTexture(birdTexture);
        birdSprite.setTextureRect(IntRect(0, 0, textureSize.x / 3, textureSize.y));
        position = (Vector2f) {100, float(screenHeight / 2 - textureSize.y / 2)};
        birdSprite.setPosition(position);
        birdSprite.setOrigin(textureSize.x / 6, textureSize.y / 2);
        vel = 0.0f;
        strenght = -12.0f;
        gotPressed = false;
    }

    void Update(Event &event) {
       if (event.type == Event::MouseButtonPressed && !gotPressed)
           if (event.mouseButton.button == Mouse::Left) {
                vel = strenght;
                rot = -20;
                gotPressed = true;
           }
       if (event.type == Event::MouseButtonReleased && gotPressed)
           if (event.mouseButton.button == Mouse::Left) {
               gotPressed = false;
           }

       vel += GRAVITY; 
       vel = MAX(strenght, MIN(vel, -(strenght)));
       if (vel > 5)
          rot += vel;
       rot = MAX(-80, MIN(rot, 80));
       position.y += vel;
       position.y = MAX(-100, MIN(position.y, 630));
       birdSprite.setPosition(position);
       birdSprite.setRotation(rot);

    }

   void Draw(RenderWindow& window) {
        window.draw(birdSprite);
    }
};

class Pipe {
    public:
        Vector2f top_pos, bottom_pos;
        Sprite top_sprite, bottom_sprite;
        Vector2u size;
        Pipe(float x, float y, Texture &pipe_top, Texture &pipe_bottom) {
            size = pipe_top.getSize();
            top_pos = Vector2f(x, y);
            top_sprite.setTexture(pipe_top);
            top_sprite.setPosition(top_pos);

            bottom_pos = Vector2f(top_pos.x, top_pos.y + size.y + GAP);
            bottom_sprite.setTexture(pipe_bottom);
            bottom_sprite.setPosition(bottom_pos);
        }

        void Update() 
        {
            top_pos.x -= SCROLL_SPEED;
            bottom_pos.x -= SCROLL_SPEED;
            top_sprite.setPosition(top_pos);
            bottom_sprite.setPosition(bottom_pos);
        }

        void Draw(RenderWindow &win)
        {
           win.draw(bottom_sprite);
           win.draw(top_sprite);
        }

        bool isOutOfBounds() const {
            return (top_pos.x + size.x < 0);
        }
};

int randint(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}


int main() 
{
    // Window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Flappy Bird", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    // Background 
    sf::Texture backgroundTexture, groundTexture;
    if (!backgroundTexture.loadFromFile("./Assets/background.png") || !groundTexture.loadFromFile("./Assets/ground.png")) {
        std::cerr << "Failed to load textures" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite backgroundSprite(backgroundTexture), groundSprite(groundTexture);
    // Ground
    sf::Vector2f groundPos(0, screenHeight - groundSprite.getGlobalBounds().height);
    groundSprite.setPosition(groundPos);
    // Bird
    Bird bird(screenHeight, "./Assets/yellow_bird.png");
    float framesCounter = 0;
    // pipes
    Texture top_pipe, bottom_pipe;
    if (!top_pipe.loadFromFile("./Assets/top_pipe.png") || !bottom_pipe.loadFromFile("./Assets/bottom_pipe.png")) {
        std::cout << "Failed to load pipe textures\n";
        return EXIT_FAILURE;
    }
    std::vector<Pipe> pipes;
    pipes.emplace_back(screenWidth, randint(-200, 0), top_pipe, bottom_pipe);
    Clock clock;
    float elapsed;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (++framesCounter >= (60/15))
        {
           if (++bird.currentFrame > 2) bird.currentFrame = 0;
           bird.birdSprite.setTextureRect(IntRect(bird.currentFrame * bird.textureSize.x / 3, 0, bird.textureSize.x / 3, bird.textureSize.y));
           framesCounter = 0;
        }
 
        elapsed  = clock.getElapsedTime().asSeconds();

        pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
                [](const Pipe& p) { return p.isOutOfBounds(); }),
                pipes.end());

        if (elapsed >= TIME_GAP) {
           pipes.emplace_back(screenWidth, randint(-200, 0), top_pipe, bottom_pipe);
           clock.restart();
        }

        groundPos.x -= SCROLL_SPEED;
        if (groundPos.x <= -80) {
            groundPos.x = 0;
        }
        groundSprite.setPosition(groundPos);

        window.clear(sf::Color(135, 206, 235));
        window.draw(backgroundSprite);
        for (Pipe &pipe: pipes){
            pipe.Update();
            pipe.Draw(window);
        }
        window.draw(groundSprite);
        bird.Update(event);
        bird.Draw(window);
        window.display();
    }

    return 0;
}
