///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		SGameScreenMain.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Game screen where player controls his ship
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "GameObjects/CInterceptor.h"
#include "my_utilities/BoxPruner.h"

#include "GameStates/SGameScreenMain.h"

SGameScreenMain SGameScreenMain::inst_;

SGameScreenMain::~SGameScreenMain()
{
    //dtor
}

bool SGameScreenMain::Init(CGameEngine* game)
{
    starSystem_.Init(game);

    return true;
}

void SGameScreenMain::HandleInput(CGameEngine* game)
{
    starSystem_.CalcMoves(game);
}


void SGameScreenMain::Update(CGameEngine* game)
{
    starSystem_.Update(game);


}

void SGameScreenMain::Draw(CGameEngine* game)
{
    sf::RenderWindow* App = game->GetApp();

    App->Clear(sf::Color(50,50,128));

    starSystem_.Draw(game);

    Vec2f pos = starSystem_.GetHumanShip()->GetPos();

    game->SetCameraPos(pos.x, pos.y);

    Vec2f p0(0.f, 0.f);
    Vec2f p1(300.f, 0.f);
    sf::Color col(0, 0, 0);
    TLine l;

    if (LineIntersectPolygon(TLine(p0, p1), starSystem_.GetHumanShip()->GetPolygon(), l))
        col = sf::Color(255, 0, 0);

    sf::Shape myLine = sf::Shape::Line(p0.x, p0.y, p1.x, p1.y, 5, col);

    App->Draw(myLine);

    App->Display();

    /////////





//    /* Eye Candy*/
//    const int VERTEX = 5;
//    const int RADIUS = 50;
//
//    sf::Shape line[VERTEX];
//    sf::Shape stats, c;
//
//    float vals[VERTEX]  = { 0.5f, 0.2f, 0.8f, 0.4f, 0.1f };
//    //float vals[VERTEX]  = { 0.88f, 0.65f, 0.9f, 0.8f, 0.7 };
////    float speed     = 0.5f;
////    float hull      = 0.2f;
////    float weapons   = 0.8f;
////    float energy    = 0.4f;
////    float shields   = 0.1f;
//
//
//    for (uint i=0; i<VERTEX; ++i)
//    {
//        float a = 2.0f * pi * (i / (float) VERTEX) - pi/2.0f;
//        float x = cos(a)*RADIUS;
//        float y = sin(a)*RADIUS;
//        line[i] = sf::Shape::Line(100.0f, 100.0f, 100+x, 100+y, 1, sf::Color(200, 255, 0));
//        stats.AddPoint(100+x*vals[i], 100+y*vals[i], sf::Color(255, 200, 0, 200.0f));
//
//    }
//
//    c = sf::Shape::Circle(100, 100, RADIUS, sf::Color(255, 200, 255, 200.0f));
//
//    //App->Draw(c);
//    for (uint i=0; i<VERTEX; ++i)   App->Draw(line[i]);
//    App->Draw(stats);
//
//    /*End Eye Candy*/

}


void SGameScreenMain::Pause()
{

}
void SGameScreenMain::Resume()
{

}

void SGameScreenMain::Reset()
{

}

void SGameScreenMain::Cleanup()
{
    // Free images, if any?
}
