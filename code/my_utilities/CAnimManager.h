///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CAnimManager.h
 *	\author
 *	\date		Jul 27, 2011
 *  \brief      Animation manager base class that other managers will inherit from
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CANIMMANAGER_H_
#define CANIMMANAGER_H_

// Headers
#include <vector>
#include <map>
#include "CAnim.h"
#include "CGameEngine.h"

using std::vector;
using std::map;

// Enums of all CAnim keys for vector<int, CAnim>

namespace Church
{
    enum InterceptorAnimKey { INTERCEPTOR_BODY, INTERCEPTOR_THRUSTERS };
}

const int MAX_ANIMS = 2;

class CAnimManager
{
   public:
                                           CAnimManager();
                                           ~CAnimManager();
           void                            Init(/* set as you see fit */);
           void                            AddCAnim(int key, CAnim newCAnim);
           void                            Draw(const Vec2f& position, float orientation, TAnimData animData, CGameEngine* game);

   //! Given the position/angle and frame data, this function passes the data to the appropriate CAnim GetFrame function
           // sf::Sprite&                     GetSprite(const Vec2f& position, float orientation, TAnimData& data);

   protected:
           vector<CAnim>                   anims_;

};



#endif //CANIMMANAGER_H_
