#ifndef TileMap_hpp
#define TileMap_hpp

#include <vector>
#include "Tile.hpp"

class TileMap : public sf::Drawable
{
public:
	void add(Tile tile);
	void remove( sf::Vector2f &tilePos);
	void clear();
private:
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
	std::vector<Tile> tileMap;
};
#endif
