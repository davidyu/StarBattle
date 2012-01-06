///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CInterceptor.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Implementation of CInterceptor class
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "GameObjects/CPilotHuman.h"
#include "GameObjects/CPilotInterceptor.h"
#include "CShipFactory.h"
#include <vector>


#include "GameObjects/CInterceptor.h"

using std::vector;

CInterceptor::CInterceptor(CPolygon poly, CGameEngine* game) : CGenericShip(poly, game), engineState_(READY)
{
    //also init TAnimData

}

CInterceptor::~CInterceptor()
{
    //dtor
}

void CInterceptor::Init(sf::Image* img)
{

}

void CInterceptor::GetPilot(uint type)
{
    delete pilot_;

    if (type==1)
        pilot_ = new CPilotHuman();
    if (type==0)
        pilot_ = new CPilotInterceptor();

    pilot_->SetShip(this);
}

void CInterceptor::Update()
{
    Step();
}

void CInterceptor::FireEngine(const TEngineInput& c)
{
    switch(c.dir)
    {
        case FWD:
            EngineForward(c.pwr);
            break;
        case REV:
            EngineReverse(c.pwr);
            break;
        case LEFT:
        case RIGHT:
            EngineStrafe(c);
            break;
        case NONE:
        default:
            break;
    }
    EngineSteer(c);
    CapVel();
}


void CInterceptor::EngineForward(float pwr)
{
    TShipHandling&  stats        = ship_base_->GetShipStats().handling;
    float           thrust_mag   = stats.thrust_fwd * pwr;
    Vec2f           cv           = GetVel(); // Current vel
    Vec2f           dv           = Vec2f(thrust_mag, 0.f).Rotated(GetOrient()); // desired vel
    Vec2f           tv           = dv;

    if (cv.LengthSquared() >= 0.01) tv -= stats.thrust_damp * cv.PerpProjected(dv);




//float r1, r2;
// dv.Normalize();
//    // Calculate thrust vector
//    if (cv.LengthSquared() >= 0.0001)
//    {
//        if(QuadraticSolver(dv.y*dv.y + dv.x*dv.x, -2.f*(cv.x*dv.x+cv.y*dv.y), cv.x*cv.x + cv.y*cv.y - THRUST_MAG*THRUST_MAG, r1, r2)) // My amazing math skills. Ya!
//        {
//            r1 = r1;
//            r2 = r2;
//            tv = (dv*r1-cv);
//        }
//        else
//            tv = dv/100.f;
//
//        // dampening factor dep on speed?
//        // add minimum thrust direction in ship orientation ? related to dampening
//        tv = (tv*(1.f-dampeningFactor)+dv*(dampeningFactor))/2.f;
//        tv.Normalize();
//        tv *= THRUST_MAG;
//    }

    // Decrease angular velocity
    DecayAngVel(stats.ang_decay_fwd);
    IncVel(tv);
}
void CInterceptor::EngineReverse(float pwr)
{
    TShipHandling& stats        = ship_base_->GetShipStats().handling;
    float          thrust_mag   = stats.thrust_rev * pwr;
    Vec2f          tv           = -Vec2f(thrust_mag, 0.f).Rotated(GetOrient());

    IncVel(tv);
    DecayAngVel(stats.ang_decay_rev);
}

void CInterceptor::EngineSteer(const TEngineInput& c)
{
    TShipHandling& stats        = ship_base_->GetShipStats().handling;

    IncAngVel((c.steer) * stats.steer);
}

void CInterceptor::EngineStrafe(const TEngineInput& c)
{
    static uint rollExtent = 0;

    engineState_      = ROLLING;
}

void CInterceptor::DecayAngVel(float rate)
{
    TShipHandling&  stats       = ship_base_->GetShipStats().handling;
    float           angVel      = GetAngVel();

    if (fabs(angVel)<stats.steer_clip)
        SetAngVel(0.f);
    else
    {
        IncAngVel(-angVel*rate);
    }
}

void CInterceptor::CapVel()
{
    float max_vel       = ship_base_->GetShipStats().handling.max_vel;
    float max_ang_vel   = ship_base_->GetShipStats().handling.max_steer;
    Vec2f cv            = GetVel();
    float av            = GetAngVel();

    if (cv.LengthSquared() >= max_vel*max_vel)   SetVel(cv.Normalized() * max_vel);
    if (fabs(av) >= max_ang_vel) SetAngVel(max_ang_vel*mysign(av));
}
