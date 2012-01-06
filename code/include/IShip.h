///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Pure abstract class for CShipFactory
 *	\file		IShip.h
 *	\author		Fish
 *	\date		July 20, 2011
 *  \brief      Base class for flyweight implementation. All flyweight ships inherit from this
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef ISHIP_H
#define ISHIP_H

// Headers
#include "my_utilities/CRigidBody.h"
#include "SFML/Graphics.hpp"

class IShip
{
    public:

    virtual void                        Update(Vec2f& p, Vec2f& v, float& o, float& av)     = 0;


//    virtual void                        HandleCommands(sf::RenderWindow App, int gamepad, int gameInfo)         = 0;

//    // Getters
//    virtual const Vec2f&                GetPos()            const                                               = 0;
//    virtual const Vec2f&                GetVel()            const                                               = 0;
//    virtual float                       GetOrient()         const                                               = 0;
//    virtual float                       GetAngVel()         const                                               = 0;
//
//    // Setters
//    virtual void                        SetPos(const Vec2f& p)                                                  = 0;
//    virtual void                        SetVel(const Vec2f& v)                                                  = 0;
//    virtual void                        SetOrient(float o)                                                      = 0;
//    virtual void                        SetAngVel(float av)                                                     = 0;

    protected:
    // Base stats of ship
            float                       hullMax_;           //!< Highest hull integrity (HP) value allowed
            float                       hullRegen_;       //!< Hp Regen rate
            float                       shieldsMax_;        //!< Highest shields value allowed
            float                       shieldsRegen_;    //!< Shields regen  rate

    // Physics stuff
            CRigidBody                  body_;

//    // Physics-related members
//            Vec2f                       pos_;             //!< Position of the ship
//            Vec2f                       vel_;             //!< Speed & direction ship isheading in
//            float                       orient_;          //!< Facing directin in degrees
//            float                       angVel_;          //!< Angular velocity in degrees

};

#endif // ISHIP_H
