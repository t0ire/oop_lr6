#include <sstream>
#include <iostream>
#include <algorithm>  

#include "factory.h"

std::shared_ptr<NPC> NPCFactory::createNPC(NPCType type, 
                                          const std::string& name,
                                          Point position) {
    switch (type) {
        case NPCType::ORC:
            return std::make_shared<Orc>(name, position);
        case NPCType::KNIGHT:
            return std::make_shared<Knight>(name, position);
        case NPCType::BEAR:
            return std::make_shared<Bear>(name, position);
        default:
            std::cerr << "неизвестный тип" << std::endl;
            return nullptr;
    }
}

std::shared_ptr<NPC> NPCFactory::createNPCFromString(const std::string& data) {
    std::stringstream ss(data);
    std::string typeStr, name;
    int x, y;
    
    if (ss >> typeStr >> name >> x >> y) {
        NPCType type = stringToType(typeStr);
        if (type != NPCType::UNKNOWN) {  
            Point pos(x, y);
            if (pos.isValid()) {
                return createNPC(type, name, pos);
            } else {
                std::cerr << "неверные координаты: " << x << "," << y << std::endl;
            }
        }
    } else {
        std::cerr << "ошибка парсинга строки: " << data << std::endl;
    }
    
    return nullptr;
}

NPCFactory::NPCType NPCFactory::stringToType(const std::string& typeStr) {
    std::string lowerType = typeStr;
    std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
    
    if (lowerType == "orc") 
        return NPCType::ORC;
    if (lowerType == "knight") 
        return NPCType::KNIGHT;
    if (lowerType == "bear") 
        return NPCType::BEAR;
    
    std::cerr << "неизвестный тип персонажа: " << typeStr << std::endl;
    return NPCType::UNKNOWN; 
}

std::string NPCFactory::getAllTypes() {
    return "Orc, Knight, Bear";
}