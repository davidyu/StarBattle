///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		Cinterceptor.h
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Class for the interceptor ship variants
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CINTERCEPTOR_H
#define CINTERCEPTOR_H

// Headers
#include "CGenericShip.h"
#include "my_utilities/mymath.h"
#include "my_utilities/CAnim.h"
#include <SFML/Graphics.hpp>

// TO DO:
/*
 - Implement state &  power curve for roll?
 */



class CInterceptor : public CGenericShip
{
    public:
                                CInterceptor(CPolygon poly, CGameEngine* game);
                                ~CInterceptor();

    virtual void                Init(sf::Image* img);

    virtual void                GetPilot(uint type);
    virtual void                Update();
    virtual void                FireEngine(const TEngineInput& c);

    private:
            enum                EEngineState   { READY, BURST, ROLLING };
            EEngineState        engineState_;
            TAnimData           animData_;

    // Internal Methods
            void                EngineForward(float pwr);
            void                EngineReverse(float pwr);
            void                EngineSteer(const TEngineInput& c);
            void                EngineStrafe(const TEngineInput& c);

            void                DecayAngVel(float rate);
            void                CapVel();


};

#endif // CINTERCEPTOR_H
