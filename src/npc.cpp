#include <iostream>

#include "npc.h"
#include "visitor.h"

NPC::NPC(const std::string& name, Point position) 
    : name(name), position(position) {}

std::string NPC::getName() const {
    return name;
}

Point NPC::getPosition() const {
    return position;
}

bool NPC::isAlive() const {
    return alive;
}

void NPC::die() {
    alive = false;
}

Orc::Orc(const std::string& name, Point position) 
    : NPC(name, position) {}

void Orc::accept(Visitor& visitor) {
    visitor.visit(this);
}

bool Orc::canAttack(const NPC* other) const {
    //орк атакует медведя
    return other->getType() == "Bear";
}

void Orc::print() const {
    std::cout << "орк '" << name << "' [" << position.x << "," << position.y << "]";
    if (!alive) std::cout << " кик";
    std::cout << std::endl;
}

Knight::Knight(const std::string& name, Point position) 
    : NPC(name, position) {}

void Knight::accept(Visitor& visitor) {
    visitor.visit(this);
}

bool Knight::canAttack(const NPC* other) const {
    //рыцарь атакует орка
    return other->getType() == "Orc";
}

void Knight::print() const {
    std::cout << "рыцарь '" << name << "' [" << position.x << "," << position.y << "]";
    if (!alive) std::cout << " кик";
    std::cout << std::endl;
}

Bear::Bear(const std::string& name, Point position) 
    : NPC(name, position) {}

void Bear::accept(Visitor& visitor) {
    visitor.visit(this);
}

bool Bear::canAttack(const NPC* other) const {
    //медведь атакует рыцаря
    return other->getType() == "Knight";
}

void Bear::print() const {
    std::cout << "медведь '" << name << "' [" << position.x << "," << position.y << "]";
    if (!alive) std::cout << " кик";
    std::cout << std::endl;
}