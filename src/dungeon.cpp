#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include "dungeon.h"
#include "factory.h"
#include "visitor.h"

Dungeon::Dungeon() {}

void Dungeon::addNPC(std::shared_ptr<NPC> npc) {
    if (npc && npc->getPosition().isValid()) {
        npcs.push_back(npc);
    } else {
        std::cerr << "не удалось добавить NPC" << std::endl;
    }
}

void Dungeon::printAll() const {
    if (npcs.empty()) {
        std::cout << "подземелье пусто" << std::endl;
        return;
    }
    
    int index = 1;
    for (const auto& npc : npcs) {
        std::cout << std::setw(3) << index++ << ". ";
        npc->print();
    }
    
    std::cout << std::endl;
}

void Dungeon::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ошибка открытия файла" << filename << std::endl;
        return;
    }
    
    file << "# Файл сохранения подземелья" << std::endl;
    file << "# Формат: Тип Имя X Y" << std::endl;
    file << "# Типы: Orc, Knight, Bear" << std::endl;
    file << "# Координаты: 0-500" << std::endl;
    file << std::endl;
    
    int saved = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            file << npc->getType() << " " 
                 << npc->getName() << " "
                 << npc->getPosition().x << " " 
                 << npc->getPosition().y << std::endl;
            saved++;
        }
    }
    
    file.close();
    std::cout << "сохранено " << saved << " персонажей в файл '" << filename << "'" << std::endl;
}

void Dungeon::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ошибка открытия файла" << filename << std::endl;
        return;
    }
    
    std::string line;
    int loaded = 0;
    int lineNum = 0;
    
    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue;
        
        auto npc = NPCFactory::createNPCFromString(line);
        if (npc) {
            npcs.push_back(npc);
            loaded++;
        } else {
            std::cerr << "ошибка в строке " << lineNum << ": " << line << std::endl;
        }
    }
    
    file.close();
    std::cout << "загружено " << loaded << " персонажей из файла '" << filename << "'" << std::endl;
}

void Dungeon::battle(int range) {
    if (npcs.empty()) {
        std::cout << "нет персонажей" << std::endl;
        return;
    }
    
    notifyObservers("начало битвы, удар: " + std::to_string(range) + "м");
    
    auto npcsCopy = npcs;
    bool battleOccurred = false;
    
    for (size_t i = 0; i < npcsCopy.size(); ++i) {
        if (!npcsCopy[i]->isAlive()) continue;
        
        for (size_t j = 0; j < npcsCopy.size(); ++j) {
            if (i == j) continue; //не сами с собой
            if (!npcsCopy[j]->isAlive()) continue;
            
            double dist = npcsCopy[i]->getPosition().distanceTo(npcsCopy[j]->getPosition());
            if (dist <= range) {
                battleOccurred = true;
                BattleVisitor visitor(npcsCopy[i], this);
                npcsCopy[j]->accept(visitor);
            }
        }
    }
    
    if (!battleOccurred) {
        std::cout << "никто не находится в радиусе боя" << std::endl;
        notifyObservers("персонажи слишком далеко");
    }
    
    //кик мёртвых 
    size_t before = npcs.size();
    npcs.erase(
        std::remove_if(npcs.begin(), npcs.end(),
            [](const std::shared_ptr<NPC>& npc) { return !npc->isAlive(); }),
        npcs.end()
    );
    size_t killed = before - npcs.size();
    
    notifyObservers("конец битвы. убито: " + std::to_string(killed) + 
                   ", осталось: " + std::to_string(countAlive()));
}

void Dungeon::attachObserver(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void Dungeon::notifyObservers(const std::string& message) {
    for (const auto& observer : observers) {
        observer->update(message);
    }
}

size_t Dungeon::countAlive() const {
    return std::count_if(npcs.begin(), npcs.end(),
        [](const std::shared_ptr<NPC>& npc) { return npc->isAlive(); });
}