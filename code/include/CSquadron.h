///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CSquadron.h
 *	\author		Fish
 *	\date		Jul 31, 2011
 *  \brief      A squadron
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Include Guard
#ifndef CSQUADRON_H
#define CSQUADRON_H

// Headers
#include "CGenericShip.h"
#include "my_utilities/mytypes.h"
#include <vector>

using std::vector;

class CGameEngine;
class CStarSystem;

class CSquadron
{
    public:
                                    CSquadron();
    virtual                         ~CSquadron();

//    virtual void                    Update();
    virtual void                    CalcMoves(CGameEngine *game, CStarSystem& level);
//    virtual void                    Draw(CGameEngine *game);

    vector<CGenericShip*>&          GetAll() { return ships_; }
    virtual void                    AddShip(CGenericShip* ship);
    
            CGenericShip*           operator[](uint i);

    private:
            vector<CGenericShip*>                ships_;

};

#endif // CSQUADRON_H
