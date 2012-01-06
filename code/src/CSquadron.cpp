///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CSquadron.h
 *	\author		Fish
 *	\date		Jul 31, 2011
 *  \brief      A hierarchical class for managing squadrons
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "GameObjects/CInterceptor.h"
#include "my_utilities/misc_helper.h"

#include <SFML/Graphics.hpp>
#include "CSquadron.h"

using std::vector;
using std::pair;


CSquadron::CSquadron()
{
}

CSquadron::~CSquadron()
{
}


void CSquadron::AddShip(CGenericShip* ship)
{
    ships_.push_back(ship);
}

void CSquadron::CalcMoves(CGameEngine *game, CStarSystem& level)
{
    for (uint i=0; i<ships_.size(); ++i)
    {
        ships_[i]->CalcMoves(game, level);
    } 
}
CGenericShip* CSquadron::operator[](uint i)
{
    if (i>=ships_.size())
        assert(false);
        
    return ships_[i];
}
//void CSquadron::Update()
//{
//    for (uint i=0; i<ships_.size(); ++i)
//    {
//        ships_[i].Update();
//    } 
//}

//void CSquadron::Draw(CGameEngine *game)
//{
//    for (uint i=0; i<ships_.size(); ++i)
//    {
//        ships_[i].Draw(game);
//    }
//}
