#ifndef Tile_hpp
#define Tile_hpp

#include "Window.hpp"

class Tile : public sf::Drawable, public sf::Transformable {
public:
	Tile(sf::Texture *texture, sf::Vector2f pos);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Texture *texture;
	sf::VertexArray vertices;
};

#endif

