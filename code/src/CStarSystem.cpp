///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		StarSystem.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Implementation of the level manager
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "CBullet.h"
#include "CFleet.h"
#include "CStarSystem.h"

#include "my_utilities/CPoolManager.h"
#include "my_utilities/misc_helper.h"
#include "my_utilities/Collision.h"
#include <iostream>

#include "GameObjects/CInterceptor.h"

using std::vector;
using std::cout;
using std::endl;

CPolygon InterceptorPolygon();

CStarSystem::CStarSystem() : bullet_manager_(CPoolManager<CBullet>()), collision_manager_(CCollisionManager())
{
    // Data initialization
    //fleets_.reserve(1);

    // Resource initialization
    //resources_.RegisterImage("resources/sprites/Test_Ship_Sprite.png", "interceptor");


    object_pool_.reserve(MAX_OBJECTS);



}

CStarSystem::~CStarSystem()
{
    miscH::FreeClear(object_pool_);
    miscH::FreeClear(ship_pool_);
    miscH::FreeClear(bullets_);
}

void CStarSystem::Init(CGameEngine* game)
{
    fleets_.clear();
    fleets_.push_back(CFleet());
    fleets_.push_back(CFleet());
    fleets_[0].AddSquadron();
    fleets_[1].AddSquadron();

    for (uint i=0; i<50; ++i)  AddShip(INTERCEPTOR_CHURCH, 0, 0, game);
    AddShip(INTERCEPTOR_CHURCH, 1, 0, game);

    uint Seed = 10;
    sf::Randomizer::SetSeed(Seed);

    bullet_manager_.Init(1000);

}

bool CStarSystem::AddShip(TShipType type, uint flt, uint squad, CGameEngine* game)
{
    if (flt>=fleets_.size())    return false;

    switch (type)
    {
        case INTERCEPTOR_CHURCH:
            ship_pool_.push_back(new CInterceptor(InterceptorPolygon(), game));
            ship_pool_.back()->SetPos(Vec2f(sf::Randomizer::Random(0.0f, 9000.f), sf::Randomizer::Random(0.f, 9000.f)));
            break;
        default:
            break;
    }
    CGenericShip* added_ship = ship_pool_.back();

    fleets_[flt].AddShip(added_ship, squad);
    ship_register_.push_back(TShipRegister(added_ship, flt, squad, ship_pool_.size()-1));
    ship_register_.back().ship->GetPilot(0);
    ship_register_[0].ship->GetPilot(1);

    return true;
}

void CStarSystem::RemoveShip(uint index)
{
    ship_register_.erase(ship_register_.begin()+index);
}

void CStarSystem::RemoveObject(uint index)
{
    delete object_pool_[index];
    object_pool_.erase(object_pool_.begin()+index);
}

void CStarSystem::CalcMoves(CGameEngine* game)
{
    vector<vector<CGenericShip*> > results;
    collision_manager_.PruneQueryShip(ship_pool_, results);

    for (uint i=0; i<fleets_.size(); ++i)
    {
        fleets_[i].CalcMoves(game, *this);
    }
}

void CStarSystem::Update(CGameEngine* game)
{

    for (uint i=0; i<object_pool_.size(); ++i)
    {
        object_pool_[i]->Update();
    }
    for (uint i=0; i<ship_pool_.size(); ++i)
    {
        ship_pool_[i]->Update();
    }
    for (uint i=0; i<bullets_.size(); ++i)
    {
        bullets_[i]->Update();
        if (bullets_[i]->BulletExpired())
        {
            bullet_manager_.CheckinItem(bullets_[i]);
            bullets_.erase(bullets_.begin()+i);
            --i;
        }

    }
    DoCollision();
}

void CStarSystem::Draw(CGameEngine* game)
{

    vector<CGenericObject*> all_objects = object_pool_;
    all_objects.insert(all_objects.end(), ship_pool_.begin(), ship_pool_.end());

    vector<CGenericObject*> objects_in_view;

    // The reason why bullets are done separately of all objects is because of radix sorts have already sorted the two groups separately
    collision_manager_.OcclusionCull(&game->GetCameraRect(), all_objects, bullets_, objects_in_view);


    for (uint i=0; i<objects_in_view.size(); ++i)
    {
        objects_in_view[i]->Draw(game);
    }
}

void CStarSystem::SpawnBullet(const Vec2f& pos, const Vec2f& vel, float orient)
{
    CBullet* bullet = bullet_manager_.CheckoutItem();
    if (bullet)
    {
        bullets_.push_back(bullet);
        bullets_.back()->Init(pos, vel, orient);
    }

}

void CStarSystem::DoCollision()
{
    vector<CGenericObject*> all_objects = object_pool_;
    all_objects.insert(all_objects.end(), ship_pool_.begin(), ship_pool_.end());

    collision_manager_.PruneObjectObject(all_objects);
    collision_manager_.PruneBulletObject(bullets_, all_objects);


}
