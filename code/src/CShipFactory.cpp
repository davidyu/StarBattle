///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Implementation of CShipFactory
 *	\file		CShipFactory.cpp
 *	\author		Fish
 *	\date		July 20, 2011
 *  \brief      Manages and distributes instances singular instances of ships
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "GameTemplates.h"
#include "GameObjects/CInterceptorFlyweight.h"
#include "my_utilities/misc_helper.h"

#include "CShipFactory.h"


using std::map;
using std::pair;

// Initialize Static Members
uint CShipFactory::shipsCount_ = 0;
map<SHIP_TYPE, IShipFlyweight*>  CShipFactory::ships_;

CShipFactory::CShipFactory()
{
    //ctor
}

CShipFactory::~CShipFactory()
{
    for(std::map<SHIP_TYPE, IShipFlyweight*>::iterator it = ships_.begin(); it != ships_.end(); ++it)
    {
        IShipFlyweight* Value = (*it).second;
        delete Value;
    }
}

IShipFlyweight* CShipFactory::GetShip(SHIP_TYPE type, CGameEngine* game)
{
    IShipFlyweight* newShip;

    if (ships_.find(type)==ships_.end())
    {
        switch (type)
        {
            case LIGHT:
                newShip = new CInterceptorFlyweight(InterceptorStats(), game);
            default:
                newShip = new CInterceptorFlyweight(InterceptorStats(), game);
        }

        ships_.insert(pair<SHIP_TYPE, IShipFlyweight*>(type, newShip));
    }

    return ships_[type];
}
