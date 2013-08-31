#pragma once

#include <string>

namespace Nultima
{

class Game
{
public:
    Game(std::string worldFile, std::string stateFile);
    ~Game();

    void mainloop();
    void display();

private:
    void handleKeyboard();
};

}; // namespace
