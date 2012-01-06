///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CGameEngine.h
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Initializes game, manages states
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CGAMEENGINE_H
#define CGAMEENGINE_H

// Headers
#include "my_utilities/mymath.h"
#include "my_utilities/CResourceManager.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using std::vector;
using std::string;

class IGameState;

class CGameEngine
{
    public:
                                            CGameEngine();
    virtual                                 ~CGameEngine()                      { delete app_;          }

            bool                            Init(int scrnw, int scrnh, int scrnbpp, float FPS, string title="");
            void                            Cleanup();

            void                            ChangeState(IGameState* state);
            void                            PushState(IGameState* state);
            void                            PopState();

            void                            HandleInput();
            void                            Update();
            void                            Draw();

            bool                            Running()
                                            { return running_;      }

            void                            Quit()
                                            { running_ = false; app_->Close(); }

            void                            SetCameraPos(float x, float y)
                                            {   camera_.SetCenter(x, y); app_->SetView(camera_); }

            CRect                           GetCameraRect() { return SFRectToCRect(camera_.GetRect()); }
    // Getters
            CResourceManager&               GetResourceManager()                { return resources_; }
            int                             GetScreenWidth()                    { return screen_w;      }
            int                             GetScreenHeight()                   { return screen_h;      }
            float                           GetElapsedTime();
            sf::RenderWindow*               GetApp();
            bool                            GetEvent(sf::Event& e)              { return app_->GetEvent(e);             }


    private:

            vector<IGameState*>             states;
            sf::RenderWindow*               app_;
            CResourceManager                resources_;

            sf::Clock                       gameClock_;
            sf::View                        camera_;

            bool                            running_;
            int                             screen_w;
            int                             screen_h;
            int                             screen_bpp;
};

#endif // CGAMEENGINE_H
