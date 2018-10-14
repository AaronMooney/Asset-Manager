#include "TextureManager.hpp"

TextureManager::TextureManager() {}

//Load texture from file
void TextureManager::load(std::string filename, int size) {

	//Create an Image from file
	sf::Image image;
	if (!image.loadFromFile(filename)) {
		std::cout << "Error Loading" << std::endl;
		return;
	}

	//Cycle through image tile by tile and compare pixels of each tile to skip duplicate tiles
	for (int x = 0, y = 0, z = 0; z < (image.getSize().x / size) * (image.getSize().y / size); ++z) {
		if (x >= image.getSize().x) {
			x = 0;
			y += size;
		}
		sf::Color currentPixel = image.getPixel(x, y);
		bool duplicate = true;
		for (int testX = x + 1, testY = y, i = 0; i < size; ++i) {
			if (testX >= x + size) {
				testX = x;
				testY++;
			}
			if (currentPixel != image.getPixel(testX, testY)) {
				duplicate = false;
				break;
			}
			testX++;
		}
		if (!duplicate) {
			sf::Texture texture;
			texture.loadFromImage(image, sf::IntRect(x, y, size, size));
			m_textures.emplace(std::to_string(z), texture);
		}
		x += size;
	}
}


std::map<std::string, sf::Texture> &TextureManager::getTextures() {
	return m_textures;
}

