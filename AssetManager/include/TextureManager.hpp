#ifndef TextureManager_hpp
#define TextureManager_hpp

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class TextureManager {
public:
	TextureManager();
	void load(std::string filename, int size);
	std::map<std::string, sf::Texture> &getTextures();
private:
	std::map<std::string, sf::Texture> m_textures;
};
#endif

