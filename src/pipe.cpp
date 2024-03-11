#include "../include/pipe.hpp" 


Pipe::Pipe(float x, float y, sf::Texture &pipe_top, sf::Texture &pipe_bottom)
{
    size = pipe_top.getSize();
    top_pos = sf::Vector2f(x, y);
    top_sprite.setTexture(pipe_top);
    top_sprite.setPosition(top_pos);

    bottom_pos = sf::Vector2f(top_pos.x, top_pos.y + size.y + GAP);
    bottom_sprite.setTexture(pipe_bottom);
    bottom_sprite.setPosition(bottom_pos);
}

void Pipe::update()
{
    top_pos.x -= SCROLL_SPEED;
    bottom_pos.x -= SCROLL_SPEED;
    top_sprite.setPosition(top_pos);
    bottom_sprite.setPosition(bottom_pos);
}

void Pipe::draw(sf::RenderWindow &win)
{
    win.draw(bottom_sprite);
    win.draw(top_sprite);
}

bool Pipe::isOutOfBounds() const {
    return (top_pos.x + size.x < 0);

}

sf::Sprite Pipe::getTopSprite(void) const
{
    return top_sprite;
}

sf::Sprite Pipe::getBottomSprite(void) const
{
    return bottom_sprite;
}

sf::Vector2f Pipe::getPosition(void) const
{
    return top_pos;
}
sf::Vector2u Pipe::getSize(void) const
{
    return size;
}
         
