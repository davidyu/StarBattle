///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CFleet.h
 *	\author		Fish
 *	\date		Jul 31, 2011
 *  \brief      A hierarchical class for managing fleets
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CFLEET_H
#define CFLEET_H

// Headers
#include "CGenericShip.h"
#include "CSquadron.h"
#include <vector>
#include "my_utilities/mytypes.h"

using std::vector;
using std::pair;

class CGenericShip;

class CFleet
{
    public:
                                    CFleet();
    virtual                         ~CFleet();

//        void                    Update();
        void                    CalcMoves(CGameEngine *game, CStarSystem& level);
//        void                    Draw(CGameEngine *game);

        void                    GetAll(vector<CGenericShip*>& all);
        void                    AddSquadron();
        void                    AddShip(CGenericShip* ship, uint squad);
        
        CSquadron&              operator[](uint index);


    private:
            vector<CSquadron>      squadrons_;

};

#endif // CFLEET_H
