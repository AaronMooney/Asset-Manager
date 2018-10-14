#include <iostream>
#include "Gui.hpp"

Gui::Gui() : m_window("Asset Manager", sf::Vector2u(1600, 900)), m_mousePosition({ 0,0 })
{
	m_clock.restart();
	srand(static_cast<unsigned int>(time(nullptr)));
	m_elapsed = 0.0f;
	m_textureManager.load("resources/terrain.png", 32);
	numTilesx = 15;
	numTilesy = 15;
	mapWidth = 32 * numTilesx;
	mapHeight = 224 + 32 * numTilesy;
	grid();
}

Gui::~Gui(){}

sf::Time Gui::getElapsed() { return m_clock.getElapsedTime(); }
void Gui::restartClock() { m_elapsed += m_clock.restart().asSeconds(); }
Window *Gui::getWindow() { return &m_window; }

//Window containing all the tiles to select from
void Gui::tileSelectionWindow() {
	//init imgui window
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 224));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!ImGui::Begin("Select Tile", nullptr, flags)) {
		ImGui::End();
		return;
	}
	//bind mouse position to grid
	auto mousePos = ImGui::GetMousePos();
	m_mousePosition = sf::Vector2i((int)(mousePos.x / 32) * 32, (int)(mousePos.y / 32) * 32);

	//add textures to window
	int i = 0;
	for (auto &texture : m_textureManager.getTextures()) {
		if (i % 31 != 0) {
			ImGui::SameLine();
		}
		ImGui::PushID(i++);
		if (ImGui::ImageButton(texture.second, 5)) {
			m_selectedTexture = &texture.second;
		}
		ImGui::PopID();
	}
	ImGui::End();
}

//Additional options window
void Gui::menu() {
	//init imgui window
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200, 224));
	ImGui::SetNextWindowSize(ImVec2(200, 250));
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	ImGui::StyleColorsClassic();
	if (!ImGui::Begin("Menu", nullptr, flags)) {
		ImGui::End();
		return;
	}
	//increase map size
	ImGui::Text("Map size");
	ImGui::InputInt("Columns", &numTilesx);
	ImGui::InputInt("Rows", &numTilesy);
	if (numTilesx > 45) {
		numTilesx = 45;
	}
	if (numTilesy > 20) {
		numTilesy = 20;
	}
	mapWidth = 32 * numTilesx;
	mapHeight = 224 + 32 * numTilesy;

	ImGui::Separator();

	//Increase columns/rows of texture to draw
	ImGui::Text("Increase selection Height/width");
	ImGui::InputInt("Height", &tilesToDrawRow);
	ImGui::InputInt("Width", &tilesToDrawCol);
	if (tilesToDrawCol <= 0) {
		tilesToDrawCol = 1;
	}
	if (tilesToDrawRow <= 0) {
		tilesToDrawRow = 1;
	}
	if (tilesToDrawCol > numTilesx) {
		tilesToDrawCol = numTilesx;
	}
	if (tilesToDrawRow > numTilesy) {
		tilesToDrawRow = numTilesy;
	}

	ImGui::Separator();

	//reset the map
	if (ImGui::Button("Clear map", ImVec2(100, 20))) {
		m_map.clear();
	}

	ImGui::Separator();

	//show current selected texture
	ImGui::Text("Current Selection");
	if (m_selectedTexture) {
		ImGui::Image(*m_selectedTexture, { 32,32 });
	}
	ImGui::End();
}

//Draw map grid
void Gui::grid() {
	for (int x = 0; x < mapWidth + 32; x += 32) {
		sf::VertexArray lines(sf::LinesStrip, 2);
		lines[0].position = sf::Vector2f(x, 224);
		lines[0].color = sf::Color(255, 255, 255, 100);
		lines[1].position = sf::Vector2f(x, mapHeight);
		lines[1].color = sf::Color(255, 255, 255, 100);
		m_window.GetRenderWindow()->draw(lines);
	}
	for (int y = 224; y < mapHeight + 32; y += 32) {
		sf::VertexArray lines(sf::LinesStrip, 2);
		lines[0].position = sf::Vector2f(0, y);
		lines[0].color = sf::Color(255, 255, 255, 100);
		lines[1].position = sf::Vector2f(mapWidth, y);
		lines[1].color = sf::Color(255, 255, 255, 100);
		m_window.GetRenderWindow()->draw(lines);
	}
}

//Draw texture to be drawn at mouse position
void Gui::selection() {
	sf::VertexArray box(sf::Quads, 4);
	box[0].position = m_window.GetRenderWindow()->mapPixelToCoords({ m_mousePosition.x , m_mousePosition.y });
	box[1].position = m_window.GetRenderWindow()->mapPixelToCoords({ m_mousePosition.x + (32 * tilesToDrawCol) , m_mousePosition.y });
	box[2].position = m_window.GetRenderWindow()->mapPixelToCoords({ m_mousePosition.x + (32 * tilesToDrawCol), m_mousePosition.y + (32 * tilesToDrawRow) });
	box[3].position = m_window.GetRenderWindow()->mapPixelToCoords({ m_mousePosition.x , m_mousePosition.y + (32 * tilesToDrawRow) });
	
	//If only drawing one tile then show tile texture preview
	bool multipleSelect = false;
	if (tilesToDrawRow == 1 && tilesToDrawCol == 1) {
		box[0].texCoords = sf::Vector2f(0, 0);
		box[1].texCoords = sf::Vector2f(32, 0);
		box[2].texCoords = sf::Vector2f(32, 32);
		box[3].texCoords = sf::Vector2f(0, 32);
	}
	//if multiple selected then show color preview
	else {
		multipleSelect = true;
		for (int i = 0; i < 4; ++i) {
			box[i].color = sf::Color(255, 255, 255, 125);
		}
	}
	
	if (m_mousePosition.x < mapWidth && m_mousePosition.y > 224 -32 && m_mousePosition.y < mapHeight) {
		if (multipleSelect) {
			m_window.GetRenderWindow()->draw(box);
		}
		else {
			m_window.GetRenderWindow()->draw(box, m_selectedTexture);
		}
	}
}

//Left click to place a new tile or right click to remove a tile
void Gui::placeOrRemove(TileMap &map) {
	if (ImGui::IsAnyMouseDown() && ImGui::GetMousePos().y > 224 && m_mousePosition.x < mapWidth && m_mousePosition.y < mapHeight) {
		sf::Vector2i tilePos({ m_mousePosition.x , m_mousePosition.y  });
		if (ImGui::IsMouseClicked(sf::Mouse::Left)) {
			placeTile(map, tilePos);
		}
		else if (ImGui::IsMouseClicked(sf::Mouse::Right)) {
			removeTile(map, tilePos);
		}
	}
}

//Add new tile or multiple tiles to map
void Gui::placeTile(TileMap &map, sf::Vector2i &tilePos) {
	int startXPos = tilePos.x;
	int startYPos = tilePos.y;

	for (int i = 0; i < tilesToDrawCol; ++i) {
		Tile* tile = new Tile(m_selectedTexture, m_window.GetRenderWindow()->mapPixelToCoords(tilePos));
		map.add(*tile);
		tilePos.x += 32;
		handleOutOfBounds(tilePos, startXPos);
	}
	for (int i = 1; i < tilesToDrawRow; ++i) {
		if (tilePos.x >= startXPos + (32 * tilesToDrawCol)) {
			tilePos.x = startXPos;
			tilePos.y += 32;
			handleOutOfBounds(tilePos, startXPos);
		}
		for (int j = 0; j < tilesToDrawCol; ++j) {
			Tile* tile = new Tile(m_selectedTexture, m_window.GetRenderWindow()->mapPixelToCoords(tilePos));
			map.add(*tile);
			tilePos.x += 32;
			handleOutOfBounds(tilePos, startXPos);
		}
	}
}

//Remove tile or multiple tiles from map
void Gui::removeTile(TileMap &map, sf::Vector2i &tilePos) {
	int startXPos = tilePos.x;
	int startYPos = tilePos.y;
	for (int i = 0; i < tilesToDrawCol; ++i) {
		map.remove((m_selectedTexture, m_window.GetRenderWindow()->mapPixelToCoords(tilePos)));
		tilePos.x += 32;
		handleOutOfBounds(tilePos, startXPos);
	}
	for (int i = 1; i < tilesToDrawRow; ++i) {
		if (tilePos.x >= startXPos + (32 * tilesToDrawCol)) {
			tilePos.x = startXPos;
			tilePos.y += 32;
			handleOutOfBounds(tilePos, startXPos);
		}
		for (int j = 0; j < tilesToDrawCol; ++j) {
			map.remove((m_selectedTexture, m_window.GetRenderWindow()->mapPixelToCoords(tilePos)));
			tilePos.x += 32;
			handleOutOfBounds(tilePos, startXPos);
		}
	}
}

//Ensure tiles are not drawn outside of the map
void Gui::handleOutOfBounds(sf::Vector2i &tilePos, int &startXPos) {
	if (tilePos.x >= mapWidth) {
		tilePos.x = startXPos;
	}
	if (tilePos.y >= mapHeight) {
		tilePos.y = mapHeight - 32;
	}
}

//handle user imput
void Gui::handleInput() {
	placeOrRemove(m_map);
}

//update function
void Gui::update() {
	m_window.Update();
	ImGui::SFML::Update(*getWindow()->GetRenderWindow(), getElapsed());
}

//render method
void Gui::render() {
	m_window.BeginDraw();

	m_window.GetRenderWindow()->draw(m_map);

	tileSelectionWindow();
	menu();
	grid();
	selection();

	ImGui::SFML::Render(*getWindow()->GetRenderWindow());
	getWindow()->EndDraw();
}