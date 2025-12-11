#pragma once

#include <memory>
#include <string>

#include "npc.h"

class NPCFactory {
public:
    enum class NPCType { ORC, KNIGHT, BEAR, UNKNOWN };
    
    static std::shared_ptr<NPC> createNPC(NPCType type, 
                                         const std::string& name,
                                         Point position);
    
    //нпс из файла
    static std::shared_ptr<NPC> createNPCFromString(const std::string& data);
    
    static NPCType stringToType(const std::string& typeStr);
    static std::string getAllTypes();
};
