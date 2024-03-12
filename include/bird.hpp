#ifndef BIRD_HPP
#define BIRD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class Bird {
public:
    explicit Bird(int screenHeight, const std::string& texturePath, sf::SoundBuffer &buffer);
    void reset();
    void flap(const sf::Event &event);
    void applyGravity();
    void draw(sf::RenderWindow& window);
    void animate();
    sf::Sprite getSprite(void) const;
    sf::Vector2f getPosition(void) const;
    sf::Vector2u getSize(void) const;

private:
    static constexpr float gravity = 0.6f;
    sf::Texture birdTexture;
    sf::Sprite birdSprite;
    sf::Vector2f position;
    sf::Vector2u textureSize;
    int screenHeight;
    int currentFrame = 0;
    int framesCounter = 0;
    float velocity = 0.0f;
    float strength = -10.0f;
    bool gotPressed = false;
    float rotation = 0.0f;
    sf::Sound wing;
    // Helper functions
    bool loadTexture(const std::string& path);
    void updatePosition();
    void updateRotation();
};

#endif
