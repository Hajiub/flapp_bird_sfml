#include "../include/bird.hpp" 
#include "../utils/utils.hpp"

Bird::Bird(int screenHeight, const std::string& texturePath, sf::SoundBuffer &buffer) : screenHeight(screenHeight) {
    if (!loadTexture(texturePath)) {
        throw std::runtime_error("Failed to load texture from path: " + texturePath);
    }
    birdSprite.setTexture(birdTexture);
    textureSize = birdTexture.getSize();
    birdSprite.setOrigin(sf::Vector2f(textureSize.x / 6, textureSize.y / 2));
    birdSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x /3, textureSize.y));
    wing.setBuffer(buffer);
    reset();
}

void Bird::reset() {
    position = sf::Vector2f(100.0f, screenHeight / 2.0f); // Example starting position
    birdSprite.setPosition(position);
    velocity = 0.0f;
    currentFrame = 0;
    framesCounter = 0;
    gotPressed = false;
    rotation = 0.0f;
    birdSprite.setRotation(rotation);
}

void Bird::flap(const sf::Event &event)
{
        if (event.type == sf::Event::MouseButtonPressed && !gotPressed)
            if (event.mouseButton.button == sf::Mouse::Left) {
                velocity = strength;
                rotation = -30;
                gotPressed = true;
                wing.play();
            }
        if (event.type == sf::Event::MouseButtonReleased && gotPressed)
            if (event.mouseButton.button == sf::Mouse::Left) {
                gotPressed = false;
            }

}

void Bird::animate(void)
{
    if(++framesCounter >= (60/10)) {
        if (++currentFrame > 2)currentFrame = 0;
        birdSprite.setTextureRect(sf::IntRect(currentFrame * textureSize.x / 3, 0, textureSize.x /3, textureSize.y));
        framesCounter = 0;
    }

}

void Bird::applyGravity(void)
{
    velocity += gravity;
    velocity = MAX(strength, MIN(velocity, 20));
    if (velocity > 5)
        rotation += velocity;
    rotation = MAX(-80, MIN(rotation, 80));
    position.y += velocity;
    position.y = MAX(-50, MIN(position.y, 630));
    birdSprite.setPosition(position);
    birdSprite.setRotation(rotation);

}

void Bird::draw(sf::RenderWindow &window)
{
    window.draw(birdSprite);
}

bool Bird::loadTexture(const std::string& path) {
    return birdTexture.loadFromFile(path);
}

sf::Sprite Bird::getSprite(void) const{
    return birdSprite;
}

sf::Vector2f Bird::getPosition(void) const {
    return position;
}

sf::Vector2u Bird::getSize(void) const {
    return textureSize;
}
