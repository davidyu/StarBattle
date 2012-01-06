///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CPilotHuman.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Implementation human pilot controller
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "GameObjects/CPilotHuman.h"
#include "CGenericShip.h"
#include "CStarSystem.h"
#include "CGameEngine.h"
#include "my_utilities/mymath.h"

#include <iostream>

CPilotHuman::CPilotHuman()
{
    target_ = 0;
}

CPilotHuman::~CPilotHuman()
{
    //dtor
}
//
//CPilotHuman::CPilotHuman(const CPilotHuman& c)
//{
//    target_ = c.target_;
//    ship_   = c.
//}

void CPilotHuman::AcquireTarget()
{

}

void CPilotHuman::CalcMoves(CGameEngine* game, CStarSystem& level)
{
    const sf::Input& Input = game->GetApp()->GetInput();
    
    TEngineInput c;

    if (Input.IsKeyDown(sf::Key::Left))
    {
        c.steer = -1.f;
    }
    else if (Input.IsKeyDown(sf::Key::Right))
    {
        c.steer = 1.f;
    }

    if (Input.IsKeyDown(sf::Key::Up))
    {
        c.dir = FWD;
        c.pwr = 1.f;
    }
    else if (Input.IsKeyDown(sf::Key::Down))
    {
        c.dir = REV;
        c.pwr = 1.f;
    }

    ship_->FireEngine(c);
    
    static int cooldown = 0;
    if (Input.IsKeyDown(sf::Key::Space) && cooldown <= 0)
    {
        level.SpawnBullet(ship_->GetBulletSpawn(), ship_->GetVel() + Vec2f(5.f, 0.f).Rotated(ship_->GetOrient()), ship_->GetOrient());
        cooldown = 20;
    }
    else if (cooldown > 0)
        --cooldown;

    //std::cout << ship_->GetVel().Length() << std::endl;
}

