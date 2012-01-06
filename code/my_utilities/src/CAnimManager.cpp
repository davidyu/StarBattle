#include "../CAnimManager.h"

using std::pair;

CAnimManager::CAnimManager()
{
    
}

CAnimManager::~CAnimManager()
{
    
}

void CAnimManager::Init()
{
    //inits anims_ vector with blank CAnims
    CAnim empty;
    for (uint i = 0; i < MAX_ANIMS; ++i)
        anims_.push_back(empty);
}

void CAnimManager::Draw(const Vec2f& position, float orientation, TAnimData animData, CGameEngine* game)
{
    float lastUpdatedTime = game->GetElapsedTime();
    
    
    //game->GetApp()->Draw(/* some CAnim here... */);
    // if (!animData.mHold && (lastUpdatedTime - animData.mLastUpdatedTime) >= animData.mPeriod)
    // {
    //     animData.mLastUpdatedTime = lastUpdatedTime;
    // 
    //     if (animData.mOscillate)
    //     {
    //         //take care of edge cases (reset at either ends of animation)
    //         if (animData.mRewinding && animData.mCurFrame - 1 < 0)
    //         {
    //             animData.mCurFrame = 0;
    //             animData.mRewinding = false;
    //         }
    // 
    //         if (!animData.mRewinding && animData.mCurFrame + 1 > mMaxFrames - 1)
    //         {
    //             curFrame_ = mMaxFrames - 1;
    //             rewinding_ = true;
    //         }
    // 
    //         //proceed as usual...
    //         if (rewinding_)
    //             --curFrame_;
    //         else
    //             ++curFrame_;
    //     }
    //     else
    //     {
    //         if (curFrame_ + 1 < mMaxFrames)
    //             ++curFrame_;
    //         else
    //             curFrame_ = 0;
    //     }
    // }
    // 
    // //drawing starts here...
    // sf::Sprite::SetSubRect(frames_[curFrame_]); //and...that's it!
}

void CAnimManager::AddCAnim(int key, CAnim newCAnim)
{
    anims_.at(key) = newCAnim;
}