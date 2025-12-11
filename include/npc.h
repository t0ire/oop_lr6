#pragma once

#include <string>
#include <memory>

#include "point.h"

class Visitor;
class Orc;
class Knight;
class Bear;

class NPC {
public:
    NPC(const std::string& name, Point position);
    virtual ~NPC() = default;
    
    virtual std::string getType() const = 0;
    virtual void accept(Visitor& visitor) = 0;
    virtual bool canAttack(const NPC* other) const = 0;
    
    std::string getName() const;
    Point getPosition() const;
    bool isAlive() const;
    void die();

    virtual void print() const = 0;
    
protected:
    std::string name;
    Point position;
    bool alive = true;
};

class Orc : public NPC {
public:
    Orc(const std::string& name, Point position);
    std::string getType() const override { return "Orc"; }
    void accept(Visitor& visitor) override;
    bool canAttack(const NPC* other) const override;
    void print() const override;
};

class Knight : public NPC {
public:
    Knight(const std::string& name, Point position);
    std::string getType() const override { return "Knight"; }
    void accept(Visitor& visitor) override;
    bool canAttack(const NPC* other) const override;
    void print() const override;
};

class Bear : public NPC {
public:
    Bear(const std::string& name, Point position);
    std::string getType() const override { return "Bear"; }
    void accept(Visitor& visitor) override;
    bool canAttack(const NPC* other) const override;
    void print() const override;
};
