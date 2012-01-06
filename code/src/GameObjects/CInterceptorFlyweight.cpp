#include "GameObjects/CInterceptorFlyweight.h"

CInterceptorFlyweight::CInterceptorFlyweight(const TShipStats& s, CGameEngine* game) : IShipFlyweight(s, game), animManager_()
{

}

CInterceptorFlyweight::~CInterceptorFlyweight()
{
    //dtor
}
