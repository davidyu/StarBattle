///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CFleet.cpp
 *	\author		Fish
 *	\date		Jul 31, 2011
 *  \brief      A hierarchical class for managing fleets
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "my_utilities/misc_helper.h"
#include "CSquadron.h"

#include "CFleet.h"

using std::vector;



CFleet::CFleet()
{
    //ctor
}

CFleet::~CFleet()
{
}
//
//void CFleet::GetAll(vector<CGenericShip*>& all)
//{
//    for (uint i=0; i<squadrons_.size(); ++i)    squadrons_[i]->GetAll(all);
//}

//void CFleet::Update()
//{
//    for (uint i=0; i<squadrons_.size(); ++i)
//    {
//        squadrons_[i].Update();
//    }
//}
void CFleet::CalcMoves(CGameEngine *game, CStarSystem& level)
{
    for (uint i=0; i<squadrons_.size(); ++i)
    {
        squadrons_[i].CalcMoves(game, level);
    }
}
//void CFleet::Draw(CGameEngine *game)
//{
//    for (uint i=0; i<squadrons_.size(); ++i)
//    {
//        squadrons_[i].Draw(game);
//    }    
//}

void CFleet::AddSquadron()
{
    squadrons_.push_back(CSquadron());
}
void CFleet::AddShip(CGenericShip* ship, uint squad)
{
    if (squad>=squadrons_.size())   return;

    squadrons_[squad].AddShip(ship);
}

CSquadron& CFleet::operator[](uint index)
{
    if (index>=squadrons_.size())
        assert(false); // Big no-no
        
    return squadrons_[index];
}
