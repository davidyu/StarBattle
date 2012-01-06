///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		StarSystem.h
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Contains all game objects and related code; level manager
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CSTARSYSTEM_H
#define CSTARSYSTEM_H

// Headers
#include "CBullet.h"
#include "CGenericShip.h"
#include "CCollisionManager.h"
#include "my_utilities/mymath.h"
#include <SFML/Graphics.hpp>
#include <vector>

using std::vector;
using std::pair;

class IShip;
class CRect;
class CFleet;
class CGameEngine;

const int MAX_OBJECTS = 4000;

class CStarSystem
{
    public:
                                            CStarSystem();
    virtual                                 ~CStarSystem();

            void                            Init(CGameEngine* game);
    //! Provide a ship model and team to add a ship to the pool
            bool                            AddShip(TShipType type, uint flt, uint squad, CGameEngine* game);
            bool                            AddObject(const Vec2f& pos, EObjectType type);
            void                            RemoveShip(uint index);
            void                            RemoveObject(uint index);
            void                            CalcMoves(CGameEngine* game);
            void                            Update(CGameEngine* game);
            void                            Draw(CGameEngine* game);
            
            void                            SpawnBullet(const Vec2f& pos, const Vec2f& vel, float orient);

//            IFleetComponent*&               GetFleet(int i)             { return 0; }
            vector<TShipRegister>&          GetShipRegister()     { return ship_register_; }
            CGenericShip*                   GetHumanShip()        { return ship_register_[0].ship; }
            

    private:
            vector<CGenericShip*>           ship_pool_;     //!< All ships
            vector<TShipRegister>           ship_register_; //!< Pointer to ships & some data
            vector<CGenericObject*>         object_pool_;   //!< Pointers to non-ships in object pool
            vector<CFleet>                  fleets_;            //!< A pool of all the ships organized into 2 teams

            vector<CBullet*>                bullets_;       //!< Pointers to objects in pool
            CPoolManager<CBullet>           bullet_manager_; //!< Manages all bullet instances
            CCollisionManager               collision_manager_;
    // Internal Methods
            void                            DoCollision();


    // Internal Methods
                                            CStarSystem(const CStarSystem&);                // Don't implement




};

#endif // CSTARSYSTEM_H
