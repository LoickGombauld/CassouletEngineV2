#include <CoraEngineLibrarie/MapManager.hpp>
#include <iostream>


MapManager::MapManager()
{
}

MapManager::MapManager(std::string filePath)
{
    for (const auto& entry : std::filesystem::directory_iterator("Resources/" + filePath)) {
        sf::Image map;
        map.loadFromFile(entry.path().string());
        m_maps.push_back(map);
        std::cout << entry.path().filename().string() << " Has added " << std::endl;
    }
}

sf::Image* MapManager::GetLevel(int index) {
    if ((m_maps.size() - 1) < index)
    {
        return nullptr;
    }

    return &m_maps[index];
}


sf::Image* MapManager::GetRandomLevel()
{
    return 	&m_maps[rand() % m_maps.size()];
}