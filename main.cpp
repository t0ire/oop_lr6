#include <iostream>

#include "dungeon.h"
#include "factory.h"

int main() {
    Dungeon d;
    
    d.addNPC(NPCFactory::createNPC(NPCFactory::NPCType::ORC, "орк", Point(0, 0)));
    d.addNPC(NPCFactory::createNPC(NPCFactory::NPCType::BEAR, "медведь", Point(0, 0)));
    
    std::cout << "до боя: " << d.countAlive() << " персонажей" << std::endl;
    
    d.battle(10);  
    
    std::cout << "после боя: " << d.countAlive() << " персонажей" << std::endl;
    
    return 0;
}