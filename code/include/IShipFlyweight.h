///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Pure abstract class for CShipFactory
 *	\file		IShipFlyweight.h
 *	\author		Fish
 *	\date		July 20, 2011
 *  \brief      Base class for flyweight implementation. All flyweight ships inherit from this
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ISHIPFLYWEIGHT_H
#define ISHIPFLYWEIGHT_H

// Headers
#include "my_utilities/CRigidBody.h"
#include "SFML/Graphics.hpp"
#include "CGameEngine.h"

struct TShipHandling
{
    // Max bounds
    float max_vel;
    float max_steer;

    // Thrust steer values
    float thrust_fwd;
    float thrust_rev;
    float steer;
    float steer_clip;

    // Dampening effects
    float thrust_damp;
    float ang_decay_fwd;
    float ang_decay_rev;
};

struct TShipStats
{
    // Base stats
    float hull_max;                 //!< Max "health"
    float hull_regen;
    float hull_regen_timeout;       //!< Time before regen starts
    float shields_max;
    float shields_regen;
    float shields_regen_timeout;

    // Engine data
    TShipHandling handling;
    TShipHandling handling_burst;
};

class IShipFlyweight
{
    public:
                                    IShipFlyweight(const TShipStats& s, CGameEngine* game) : stats_(s) {}

        // Getters
        TShipStats&                 GetShipStats()      { return stats_;            }
    protected:
            TShipStats              stats_;


};

#endif // ISHIPFLYWEIGHT_H
