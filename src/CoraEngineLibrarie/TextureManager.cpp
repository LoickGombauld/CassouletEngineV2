#include <CoraEngineLibrarie/TextureManager.hpp>
#include <iostream>



TextureManager::TextureManager()
{
}

TextureManager::TextureManager(std::string filePath)
{
    for (const auto& entry : std::filesystem::directory_iterator("Resources/" + filePath)) {

        sf::Texture texture;
        texture.loadFromFile(entry.path().string());
        m_textures.push_back(texture);
        std::cout << entry.path().filename().string() <<" Has added " << std::endl;
    }
}



sf::Texture* TextureManager::GetTexture(int index)  {
    return &m_textures[index];
}

int TextureManager::TextureCount()
{
    return m_textures.size();
}

sf::Texture* TextureManager::GetRandomTexture()
{
    return 	&m_textures[ rand() % m_textures.size()];
}

