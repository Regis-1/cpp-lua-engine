#include "Resources.hpp"

Resources::Resources() {}

Resources::~Resources() {
    m_textures.clear();
}

bool Resources::load_texture(std::string name, std::string path,
        const sf::IntRect& int_rect) {
    
    sf::Texture tex;
    if (!tex.loadFromFile(RESOURCES_PATH + path, int_rect)) {
        std::cout << "ERROR: Couldn't load " << path << " texture!" << std::endl;
        return false;
    }

    m_textures[name] = tex;

    return true;
}

void Resources::unload_texture(std::string name) {
    m_textures.erase(name);
}

void Resources::create_sprite(int entity, std::string& name, sf::IntRect& int_rect) {
    sf::Sprite sprite(m_textures[name]);
    std::pair<int, sf::Sprite> sprite_pair(entity, sprite);
    m_sprites.emplace(sprite_pair);
}

sf::Texture& Resources::get_texture(std::string name) {
    return m_textures[name];
}

sf::Sprite& Resources::get_sprite(int entity) {
    return m_sprites.at(entity);
}
