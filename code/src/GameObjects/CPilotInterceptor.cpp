///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CPilotInterceptor.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Implementation of AI interceptor pilot
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "CStarSystem.h"
#include "SFML/Graphics.hpp"

#include "GameObjects/CPilotInterceptor.h"

// Temporary debug include
#include <iostream>


CPilotInterceptor::CPilotInterceptor() : target_(0)
{
    //ctor
}

CPilotInterceptor::~CPilotInterceptor()
{

}



void CPilotInterceptor::CalcMoves(CGameEngine* game, CStarSystem& level)
{
    if (!target_)
    {
        AcquireTarget(level);
    }
    else
    {
        ChaseTarget(level);      // Temporary func, move to strat later?

    }

}

float DesiredVelocity(float theta)
{
    float k = .001f;
    return theta * k;
}


void CPilotInterceptor::ChaseTarget(CStarSystem& level)
{
    if (!target_)   return;

    TShipHandling ship_handling = ship_->GetShipStats().handling;
    TEngineInput c;
    Vec2f target_offset     = target_->GetPos() - ship_->GetPos();
    Vec2f temp              = Vec2f(1.0f, 0);
    Vec2f target_relative   = target_offset.Rotated(-ship_->GetOrient()).Normalized();

    const float SLOWING_THRESH = pi/4;//.f;
    float ang_offset        = temp.Angle(target_relative) * mysign(target_relative.y);
    float desired_vel       = ang_offset*(ship_handling.max_steer/SLOWING_THRESH);
    float ang_vel_diff      = (desired_vel - ship_->GetAngVel());
    float b                 = ang_vel_diff/ship_handling.steer;

    if (fabs(ang_offset)>SLOWING_THRESH)
        c.steer = sign(ang_offset)*1.f;
    else
        c.steer = Capped(b, 1.f);           // I didn't want to use a value higher than 1.f, but it's too hard T_T
                                            // More than 1.f means AI ship is "better" than human ship

//    ship_->IncAngVel(Capped(ang_vel_diff, 0.02)); //Same effect as lines 67 - 70
    c.dir = FWD;
    c.pwr = 1.f;
    ship_->FireEngine(c);

    static int cooldown = 0;
    if (fabs(ang_offset) <0.1f && cooldown <= 0)
    {
        level.SpawnBullet(ship_->GetBulletSpawn(), ship_->GetVel() + Vec2f(2.f, 0.f).Rotated(ship_->GetOrient()), ship_->GetOrient());
        cooldown = 120;
    }
    else if (cooldown > 0)
        --cooldown;
}
int CalcTargetValue(CGenericShip* src, CGenericShip* target)
{
    Vec2f v = src->GetPos() - target->GetPos();
    float distS = v.LengthSquared();
    if (distS < 625)    return 25;
    else                return (int)(9000.0f/distS);
}

void CPilotInterceptor::AcquireTarget(CStarSystem& level)
{
    vector<TShipRegister> ship_register = level.GetShipRegister();
    target_ = ship_register[0].ship; return;
//    for (uint i=0; i<ships.size(); ++i)
//    {
//        if (CalcTargetValue(ship_, ships[i])>=1 && ships[i]!=ship_)
//        {
//
//            target_ = ships[i];
//            std::cout << "Got target" << i << std::endl;
//            return;
//        }
//        else std::cout << "Got no target" << std::endl;
//
//
//    }
}
