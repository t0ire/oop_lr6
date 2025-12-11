#pragma once

#include <memory>

class Orc;
class Knight;
class Bear;
class NPC;
class Dungeon;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(Orc* orc) = 0;
    virtual void visit(Knight* knight) = 0;
    virtual void visit(Bear* bear) = 0;
};

class BattleVisitor : public Visitor {
public:
    BattleVisitor(std::shared_ptr<NPC> attacker, Dungeon* dungeon);
    void visit(Orc* orc) override;
    void visit(Knight* knight) override;
    void visit(Bear* bear) override;
    
private:
    std::shared_ptr<NPC> attacker;
    Dungeon* dungeon;
    
    void processBattle(NPC* defender);
};
