///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CPilotHuman.h
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Player controlled pilot
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CPILOTHUMAN_H
#define CPILOTHUMAN_H


// Headers
#include "IPilot.h"
#include "SFML/Graphics.hpp"
#include <vector>

using std::vector;

class CGameEngine;
class CGenericShip;
class TPair;

class CPilotHuman : public IPilot
{
    public:
                                        CPilotHuman();
    virtual                             ~CPilotHuman();
                                        //CPilotHuman(const CPilotHuman& c);

            void                        SetShip(CGenericShip* s)                              { ship_ = s;    }
    virtual void                        CalcMoves(CGameEngine* game, CStarSystem& level);
            void                        AcquireTarget();


    private:
            CGenericShip*               ship_;
            CGenericShip*               target_;
};

#endif // CPILOTHUMAN_H
