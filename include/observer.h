#pragma once

#include <string>
#include <memory>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class FileObserver : public Observer {
public:
    FileObserver(const std::string& filename);
    void update(const std::string& message) override;
    
private:
    std::string filename;
};

class ConsoleObserver : public Observer {
public:
    void update(const std::string& message) override;
};
