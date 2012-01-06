///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CBullet.h
 *	\author		Fish
 *	\date		August 2, 2011
 *  \brief      Base class for bullet
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CBULLET_H
#define CBULLET_H

// Headers
#include "CGenericObject.h"
#include "my_utilities/CPoolManager.h"
#include "my_utilities/CPoolable.h"
#include "CGameEngine.h"
#include <SFML/Graphics.hpp>

class CWeapon;
class CBulletFlyweight;

class CBullet : public CGenericObject, public CPoolable<CBullet>
{
    public:
    friend class CPoolManager<CBullet>; // Only pool manager can create class instance
    friend class CWeapon;

    virtual                         ~CBullet() {};
    virtual void                    Init(const Vec2f& pos, const Vec2f& vel, float orient)
            {
                life_elapsed_ = 0;
                SetPos(pos);
                SetVel(vel);
                SetOrient(orient);
            }
    virtual void                    Update() { Step(); ++life_elapsed_;}
    virtual void                    Draw(CGameEngine* game)
            {
                sf::Shape poly = CPolyToSFPoly(GetPolygon());
                game->GetApp()->Draw(poly);
            }
    //void                            OnCollide(CGenericShip)
            bool                    BulletExpired() {return life_elapsed_ > lifetime_; }

    protected:
            CBulletFlyweight*   bullet_base_;
            uint                lifetime_;
            uint                life_elapsed_;


    // Internal Methods
        // Hidden ctor
                                CBullet() : CGenericObject(CPolygon(3, 3), 40), lifetime_(240), life_elapsed_(0) {}
        // Hidden copy ctor
                                CBullet(const CBullet& b) : CGenericObject(b)
            {
               *this = b;
            }
        // Hidden assignment op
            CBullet&            operator=(const CBullet& b)
            {
                if (this != &b)
                {
                    bullet_base_    = b.bullet_base_;
                    lifetime_       = b.lifetime_;
                    life_elapsed_   = b.life_elapsed_;
                    next_           = b.next_;
                }
                return *this;
            }



};

#endif // CBULLET_H
