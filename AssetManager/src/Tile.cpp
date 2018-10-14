#include "Tile.hpp"

//Tile Constructor
Tile::Tile(sf::Texture *tex, sf::Vector2f pos) {
	texture = tex;
	vertices = sf::VertexArray(sf::Quads, 4);
	vertices[0].texCoords = { 0 , 0 };
	vertices[1].texCoords = { 32 , 0 };
	vertices[2].texCoords = { 32 , 32 };
	vertices[3].texCoords = { 0 , 32 };
	
	vertices[0].position = { getPosition().x , getPosition().y };
	vertices[1].position = { getPosition().x + 32 , getPosition().y };
	vertices[2].position = { getPosition().x + 32 , getPosition().y + 32 };
	vertices[3].position = { getPosition().x , getPosition().y + 32 };
	setPosition(pos);
}

//Draw method
void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform = getTransform();
	states.texture = texture;
	target.draw(vertices, states);
}