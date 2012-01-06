// Include Guard

//animation manager is a collection of CAnims

//

//


#ifndef CANIM_H
#define CANIM_H

// Headers
#include <SFML/Graphics.hpp>
#include <vector>
#include "mytypes.h"
#include "mymath.h"
#include "vec2.h"

using std::vector;

struct TAnimData    //each instance of ship gets a copy of this struct and passes it to CAnimManager to Draw()
{
    uint  mPrevAnim;
    uint  mCurAnim;
    uint  mCurFrame;
    float mLastUpdatedTime;
    float mPeriod;

    bool  mOscillate;
    bool  mRewinding;
    bool  mHold;
    
    // sf::Vector2f mOffset;
};

class CAnim : public sf::Sprite
{
    public:
                                    CAnim();
                                    ~CAnim();
            void                    Init(vector<sf::IntRect> fr, float f);
            void                    Init(sf::IntRect rect, uint maxF, float f);

            void                    GetFrame(const Vec2f& position, float orientation, TAnimData& data);

    // sf::Sprite wrapper funcs
            void                    Scale(float x, float y);
            void                    SetImage(const sf::Image* img);

            uint                    mMaxFrames;
    private:
            vector<sf::IntRect>     frames_;        //!< Array of sub-rectangles of sprite sheet
            float                   period_;
};

#endif // CANIM_H
