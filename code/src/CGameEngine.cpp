///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CGameEngine.cpp
 *	\author		Fish
 *	\date		Jul 24, 2011
 *  \brief      Implementation of game engine
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "CGameEngine.h"
#include "IGameState.h"

using std::string;

CGameEngine::CGameEngine() : resources_(CResourceManager()), gameClock_(sf::Clock()), camera_(sf::View(sf::FloatRect(0, 0, screen_w, screen_h)))
{
    running_ = true;
}


bool CGameEngine::Init(int scrnw, int scrnh, int scrnbpp, float fps, string title)
{
    app_ = new sf::RenderWindow(sf::VideoMode(scrnw, scrnh, scrnbpp), title);

    screen_w    = scrnw;
    screen_h    = scrnh;
    screen_bpp  = scrnbpp;

    gameClock_.Reset();

    app_->UseVerticalSync(false);
    app_->SetFramerateLimit(fps);
    
    camera_ = sf::View(sf::FloatRect(0, 0, screen_w, screen_h));
    app_->SetView(camera_);

    return true;
}

void CGameEngine::Cleanup()
{
    while (!states.empty())
    {
		states.back()->Cleanup();
		states.pop_back();
	}
}


void CGameEngine::ChangeState(IGameState *state)
{
    if(!states.empty())
    {
        states.back()->Cleanup();
        states.pop_back();
    }
    states.push_back(state);
    states.back()->Init(this);
}

void CGameEngine::HandleInput()
{
    states.back()->HandleInput(this);
}
void CGameEngine::Update()
{
    states.back()->Update(this);
}

void CGameEngine::Draw()
{
    states.back()->Draw(this);
}

void CGameEngine::PushState(IGameState *state)
{
    states.push_back(state);
}

void CGameEngine::PopState()
{
    if (!states.empty())
    {
        states.back()->Cleanup();
        states.pop_back();
        states.back()->Resume();
    }
}


float CGameEngine::GetElapsedTime()
{
    return gameClock_.GetElapsedTime();
}

sf::RenderWindow* CGameEngine::GetApp()
{
    return app_;
}
