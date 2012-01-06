///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains source code for my 2D vector class
 *	\file		CWeapon.h
 *	\author		Fish
 *	\date		August 14, 2011
 *  \brief      Base class responsible for spawning bullets and their data
 *              should be instantiated in flyweight
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CWEAPON_H
#define CWEAPON_H

// Headers
#include "CBullet.h"

struct TWeaponData
{
    float cooldown;
};

class CStarSystem;

class CWeapon
{
    public:
                                CWeapon() {}
                                CWeapon(const CBullet& b);
    virtual                     ~CWeapon();

            //! Calls CStarSystem's spawn bullet and alters the weapon data passed to it
            void                SpawnBullet(CStarSystem& level, TWeaponData& data);

    // Getters
            float               GetCooldown() const         { return reload_time_; }
            const Vec2f&        GetShootVel() const         { return shoot_vel_; }
    // Setters
            void                SetCooldown(float c)        { reload_time_ = c;    }
            void                SetShootVel(const Vec2f& v) { shoot_vel_ = v;   }

    protected:
            CBullet             bullet_;        //!< Template for spawning bullets
            float               reload_time_;   //!< Min time inbetween shots
            Vec2f               shoot_vel_;     //!< Vel at which the bullet exits weapon
            Vec2f               spawn_pos_;     //!< Where the shot originates from
};

#endif // CWEAPON_H
