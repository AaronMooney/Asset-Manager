#ifndef Ui_hpp
#define Ui_hpp

#include "TileMap.hpp"
#include "TextureManager.hpp"
#include <SFML/Graphics.hpp>

class Gui {
public:
	Gui();
	~Gui();

	void tileSelectionWindow();
	void placeOrRemove(TileMap &map);
	void selection();
	void placeTile(TileMap & map, sf::Vector2i& tilePos);
	void removeTile(TileMap & map, sf::Vector2i& tilePos);
	void handleOutOfBounds(sf::Vector2i& tilePos, int& startXPos);
	void grid();
	void menu();
	void handleInput();
	void update();
	void render();
	Window *getWindow();
	sf::Time getElapsed();
	void restartClock();
private:
	float editorWidth = 32 * 10.0f;
	TextureManager m_textureManager;
	sf::Texture *m_selectedTexture;
	sf::Vector2i m_mousePosition;
	int numTilesx;
	int numTilesy;
	int mapWidth;
	int mapHeight;
	int tilesToDrawRow;
	int tilesToDrawCol;
	Window m_window;
	TileMap m_map;
	sf::Clock m_clock;
	float m_elapsed;
};
#endif

