#include <iostream>

#include "visitor.h"
#include "npc.h"
#include "dungeon.h"

BattleVisitor::BattleVisitor(std::shared_ptr<NPC> attacker, Dungeon* dungeon)
    : attacker(attacker), dungeon(dungeon) {}

void BattleVisitor::processBattle(NPC* defender) {
    //живы ли оба
    if (!attacker->isAlive() || !defender->isAlive()) {
        return;
    }
    
    //может ли атакующий убить защитника
    bool attackerWins = attacker->canAttack(defender);
    bool defenderWins = defender->canAttack(attacker.get());
    
    std::string attackerName = attacker->getName() + " (" + attacker->getType() + ")";
    std::string defenderName = defender->getName() + " (" + defender->getType() + ")";
    
    if (attackerWins && !defenderWins) {
        defender->die();
        dungeon->notifyObservers(attackerName + " убивает " + defenderName);
    } 
    else if (!attackerWins && defenderWins) {
        attacker->die();
        dungeon->notifyObservers(defenderName + " убивает " + attackerName);
    }
    else if (attackerWins && defenderWins) {
        attacker->die();
        defender->die();
        dungeon->notifyObservers(attackerName + " и " + defenderName + " убивают друг друга");
    }
}

void BattleVisitor::visit(Orc* orc) {
    processBattle(orc);
}

void BattleVisitor::visit(Knight* knight) {
    processBattle(knight);
}

void BattleVisitor::visit(Bear* bear) {
    processBattle(bear);
}