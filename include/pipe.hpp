#ifndef PIPE_H
#define PIPE_H
#include <SFML/Graphics.hpp>

#define GAP 200
#define SCROLL_SPEED 4.0f
#define TIME_GAP 1.2f

class Pipe {
    public:
        Pipe(float x, float y, sf::Texture &pipe_top, sf::Texture &pipe_bottom);
        void update(void);
        void draw(sf::RenderWindow &window);
        bool isOutOfBounds(void)const;
        sf::Sprite getTopSprite(void) const;
        sf::Sprite getBottomSprite(void) const;
        sf::Vector2f getPosition(void) const;
        sf::Vector2u getSize(void) const;
    private:
        sf::Vector2f top_pos, bottom_pos;
        sf::Sprite top_sprite, bottom_sprite;
        sf::Vector2u size;
 
};
#endif //PIPE_H
