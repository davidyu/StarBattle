#include "../CAnim.h"

CAnim::CAnim() : period_(0)
{

}

CAnim::~CAnim()
{

}

void CAnim::Init(vector<sf::IntRect> fr, float f)
{
    frames_ = fr;
    mMaxFrames = frames_.size();
    period_ = 1.0f/f;
}
void CAnim::Init(sf::IntRect rect, uint maxF, float f)
{
    frames_.push_back(rect);//WRONG.!! needs to push back all frames! Assume sprites go horizontally, and of uniform size
    mMaxFrames = maxF;
    period_ = 1.0f/f;
}
void CAnim::GetFrame(const Vec2f& position, float orientation, TAnimData& data)
{

}

//void CAnim::OnAnimate()
//{
//    if (!hold_ && clock_.GetElapsedTime() >= period_)
//    {
//        clock_.Reset();
//
//        if (oscillate_)
//        {
//            //take care of edge cases (reset at either ends of animation)
//            if (rewinding_ && curFrame_ - 1 < 0)
//            {
//                curFrame_ = 0;
//                rewinding_ = false;
//            }
//
//            if (!rewinding_ && curFrame_ + 1 > mMaxFrames - 1)
//            {
//                curFrame_ = mMaxFrames - 1;
//                rewinding_ = true;
//            }
//
//            //proceed as usual...
//            if (rewinding_)
//                --curFrame_;
//            else
//                ++curFrame_;
//        }
//        else
//        {
//            if (curFrame_ + 1 < mMaxFrames)
//                ++curFrame_;
//            else
//                curFrame_ = 0;
//        }
//    }
//
//    //drawing starts here...
//    sf::Sprite::SetSubRect(frames_[curFrame_]); //and...that's it!
//}


void CAnim::Scale(float x, float y)
{
    sf::Sprite::Scale(x, y);
}

void CAnim::SetImage(const sf::Image* img)
{
    sf::Sprite::SetImage(*img);

}
