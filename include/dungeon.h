#pragma once

#include <vector>
#include <memory>

#include "npc.h"
#include "observer.h"

class Dungeon {
public:
    Dungeon();
    
    void addNPC(std::shared_ptr<NPC> npc);
    void printAll() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void battle(int range);  // range — дальность боя
    
    //наблюдатели
    void attachObserver(std::shared_ptr<Observer> observer);
    void notifyObservers(const std::string& message);
    
    size_t countAlive() const;
    size_t totalCount() const { return npcs.size(); }
    
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<Observer>> observers;
};
