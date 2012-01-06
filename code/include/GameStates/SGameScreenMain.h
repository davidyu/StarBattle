///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		SGameScreenMain.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Game screen where player controls his ship
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef SGAMESCREENMAIN_H
#define SGAMESCREENMAIN_H

// Headers
#include "CStarSystem.h"
#include "IGameState.h"
#include <vector>

using std::vector;

class SGameScreenMain: public IGameState
{
    public:
                                            ~SGameScreenMain();
    static                                  SGameScreenMain* GetInst() { return &inst_; }

            bool                            Init(CGameEngine* game);
            void                            Cleanup();

            void                            Pause();
            void                            Resume();

            void                            HandleInput(CGameEngine* game);
            void                            Update(CGameEngine* game);
            void                            Draw(CGameEngine* game);

            void                            Reset();

    protected:
        SGameScreenMain(){}

    private:
        static SGameScreenMain inst_;

        CStarSystem                         starSystem_;

    // Internal Methods
                                            SGameScreenMain(SGameScreenMain const&);           // Don't Implement
        void                                operator=(SGameScreenMain const&);                 // Don't implement

};


#endif // SGAMESCREENMAIN_H
