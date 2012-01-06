///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Base class for ship client
 *	\file		CGenericShip.h
 *	\author		Fish
 *	\date		July 20, 2011
 *  \brief      Contains some common code shared by all ship clients
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CGENERICSHIP_H
#define CGENERICSHIP_H

// Headers
#include "GameObjects/CPilotHuman.h"
#include "GameObjects/CPilotInterceptor.h"
#include "my_utilities/CAnim.h"
#include "IShipFlyweight.h"
#include "CShipFactory.h"
#include "IPilot.h"
#include "CGenericObject.h"
#include "CGameEngine.h"


enum TShipType      { INTERCEPTOR_CHURCH=0, INTERCEPTOR_TRADE };
enum EEngineThrust  { FWD=0, REV, LEFT, RIGHT, NONE };

struct TEngineInput
{
    TEngineInput() : dir(NONE), steer(0.f) {}
    //TEngineInput(EEngineThrust d, float p) : dir(d), pwr(p) {}
    EEngineThrust   dir;
    float           pwr;
    float           steer;
};


struct TShipRegister
{
    TShipRegister() {}
    TShipRegister(CGenericShip* shp, uint flt, uint squad, uint i) :
                  ship(shp), fleet(flt), squadron(squad), index(i) {}

    CGenericShip* ship;
    uint fleet, squadron, index;
};


class CGenericShip : public CGenericObject
{
    public:
    //! Ctor, Dtor
                                CGenericShip(CPolygon poly, CGameEngine* game) : CGenericObject(poly), ship_base_(CShipFactory::GetShip(LIGHT, game)), pilot_(0), query_rect_(CRect(0.f, 0.f, 25.f, 25.f)), bullet_spawn_(Vec2f(poly.GetPoint(0)+Vec2f(5.f, 0.f)))
                                {  }
    virtual                     ~CGenericShip()         {   delete pilot_;  }
                                CGenericShip(const CGenericShip& ship) : CGenericObject(ship)
                                {
                                    ship_base_ = ship.ship_base_;
                                    if (pilot_) pilot_->SetShip(this);
                                }

    virtual void                Init(sf::Image*)        {}

    virtual void                Update()
                                { Step();                       }

    virtual void                CalcMoves(CGameEngine *game, CStarSystem& level)
                                { pilot_->CalcMoves(game, level);       }

    virtual void                Draw(CGameEngine *game)
    {
        sf::Shape tempPoly;

//        tempPoly = CRectToSFRect(*body_.GetBoundingRect());
//        game->GetApp()->Draw(tempPoly);

        tempPoly = CPolyToSFPoly(GetPolygon());
        game->GetApp()->Draw(tempPoly);


    }

    virtual CRect*              GetQueryRect()  { query_rect_.SetPos(GetPos()); return &query_rect_; }

    virtual void                FireEngine(const TEngineInput& c)        = 0;

    virtual TShipStats          GetShipStats() { return ship_base_->GetShipStats(); }

    virtual void                GetPilot(uint type) = 0;

    virtual Vec2f              GetBulletSpawn(){ return bullet_spawn_.Transformed(GetPos(), GetOrient()); }

    protected:
            IShipFlyweight*     ship_base_;
            IPilot*             pilot_;
            CRect               query_rect_;
            Vec2f               bullet_spawn_;

    // Temp


    //! Internal Methods
            //CGenericShip (const CGenericShip&); // Don't implement
};

#endif // CGENERICSHIP_H
