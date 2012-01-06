#include "CGameEngine.h"
#include "GameStates/SGameScreenMain.h"

#include <stdlib.h>//needs these three lines
#include <vector>
#include <iostream>


const int SCREEN_W = 800;
const int SCREEN_H = 700;
const float FPS    = 60.0f;

int main()
{
    sf::Event event;
    CGameEngine game;
    game.Init(SCREEN_W, SCREEN_H, 32, FPS, "StarBattle");
    game.ChangeState(SGameScreenMain::GetInst());

    while (game.Running())
    {
        while (game.GetEvent(event))
        {
            if (event.Type == sf::Event::Closed)    game.Quit();
        }

        game.HandleInput();
        game.Update();
        game.Draw();

        std::cout << 1.f/game.GetApp()->GetFrameTime() << std::endl;

    }

    game.Cleanup();

    return EXIT_SUCCESS;
}
