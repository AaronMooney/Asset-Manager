#include "TileMap.hpp"

//Add tile to map
void TileMap::add(Tile tile) {
	tileMap.push_back(tile);
}

//Remove tile from map
void TileMap::remove(sf::Vector2f &tilePos) {
	std::vector<Tile>::iterator it;
	for (it = tileMap.begin(); it != tileMap.end(); ) {
		if (it->getPosition().x == tilePos.x && it->getPosition().y == tilePos.y)
			it = tileMap.erase(it);
		else
			++it;
	}
}

//Remove all tiles from map
void TileMap::clear() {
	tileMap.erase(tileMap.begin(), tileMap.end());
}

//Draw method
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto &tile : tileMap)
		target.draw(tile);
}