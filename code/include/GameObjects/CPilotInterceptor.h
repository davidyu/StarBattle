///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CPilotInterceptor.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Implementation of AI interceptor pilot
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CPILOTINTERCEPTOR_H
#define CPILOTINTERCEPTOR_H

// Headers
#include "IPilot.h"

class CGameEngine;
class CStarSystem;

class CPilotInterceptor : public IPilot
{
    public:
                                        CPilotInterceptor();
                                        //CPilotInterceptor(const CInterceptor& c);
                                        virtual ~CPilotInterceptor();
            void                        SetShip(CGenericShip* s)                              { ship_ = s;    }
            void                        CalcMoves(CGameEngine* game, CStarSystem& level);
            enum                        PILOT_STATE { SEEK = 0 };

            

    protected:
            CGenericShip*               target_;
    
    // Internal Methods
            void                        AcquireTarget(CStarSystem& level);
            void                        ChaseTarget(CStarSystem& level);



};

#endif // CPILOTINTERCEPTOR_H
