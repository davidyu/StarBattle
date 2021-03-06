///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CInterceptorFlyweight.h
 *	\author		Fish
 *	\date		August 2, 2011
 *  \brief      Flyweight for interceptors
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CINTERCEPTORFLYWEIGHT_H
#define CINTERCEPTORFLYWEIGHT_H

#include "CWeapon.h"
#include "IShipFlyweight.h"
#include "CGameEngine.h"
#include "my_utilities/CAnimManager.h"

class CInterceptorFlyweight : public IShipFlyweight
{
    public:
                                        CInterceptorFlyweight(const TShipStats& s, CGameEngine* game);
                                        ~CInterceptorFlyweight();


    private:
        CAnimManager                    animManager_;
        CWeapon                         weapon_main_;

};

#endif // CINTERCEPTORFLYWEIGHT_H
