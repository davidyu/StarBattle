#ifndef CRECT_H
#define CRECT_H

#include "Vec2.h"
#include "mytypes.h"
    class CRect
    {
        public:
            CRect() {}
            inline CRect(Vec2f topLeft, Vec2f bottomRight)
            {
                top_left_     = topLeft;
                bottom_right_ = bottomRight;
            }
            inline CRect(float top, float left, float bottom, float right)
            {
                top_left_     = Vec2f(left, top);
                bottom_right_ = Vec2f(right, bottom);
            }
            inline CRect(const CRect& r)
            {
                top_left_ = r.top_left_;
                bottom_right_ = r.bottom_right_;
            }

            inline void SetPos(Vec2f pos)
            {
                Vec2f displace = pos - (top_left_ + bottom_right_) / 2.f;
                bottom_right_ += displace;
                top_left_     += displace;
            }

            inline float GetMax(uint axis) const
            {
                if (axis==0)
                    return bottom_right_.x;

                return bottom_right_.y;
            }

            inline float GetMin(uint axis) const
            {
                if (axis==0)
                    return top_left_.x;

                return top_left_.y;
            }

            inline bool Contains(Vec2f point)
            {
                return (top_left_.x <= point.x     &&
                        bottom_right_.x >= point.x &&
                        top_left_.y <= point.y     &&
                        bottom_right_.y >= point.y );
            }

            inline bool Intersects(CRect* r)
            {
                return Intersects(*r);
            }
            inline bool Intersects(const CRect& r)
            {
                return (!(GetBottom() < r.GetTop() ||
                          GetTop() > r.GetBottom() ||
                          GetRight() < r.GetLeft() ||
                          GetLeft() > r.GetRight() ));
            }

            inline bool Intersects(CRect* r, uint axis)
            {
                return Intersects(*r, axis);
            }
            inline bool Intersects(const CRect& r, uint axis) const
            {
                // X axis
                if (axis==0)
                {
                    if (bottom_right_.x < r.top_left_.x || r.bottom_right_.x < top_left_.x)     return false;

                    return true;
                }

                // Y axis
                if (bottom_right_.y < r.top_left_.y || r.bottom_right_.y < top_left_.y)     return false;

                return true;

            }

            inline bool operator==(const CRect& r) const
            {
                return (top_left_ == r.top_left_ && bottom_right_ == r.bottom_right_);
            }

            inline void operator*=(float f)
            {
                top_left_ *= f;
                bottom_right_ *= f;
            }

            inline const CRect operator*(float f)
            {
                CRect temp(*this);
                temp *= f;
                return temp;
            }
            //setter

            //getter
            const inline Vec2f& GetTopLeft() const
            {
                return top_left_;
            }

            const inline Vec2f GetTopRight() const
            {
                return Vec2f(bottom_right_.x, top_left_.y);
            }

            const inline Vec2f GetBottomLeft() const
            {
                return Vec2f(top_left_.x, bottom_right_.y);
            }

            const inline Vec2f& GetBottomRight() const
            {
                return bottom_right_;
            }

            float inline GetTop() const
            {
                return top_left_.y;
            }

            float inline GetBottom() const
            {
                return bottom_right_.y;
            }

            float inline GetLeft() const
            {
                return top_left_.x;
            }

            float inline GetRight() const
            {
                return bottom_right_.x;
            }

            Vec2f inline GetCenter() const
            {
                return (top_left_ + bottom_right_)  / 2.0f;
            }

            void inline SetTopLeft(const Vec2f& p)
            {
                top_left_ = p;
            }

            void inline SetBottomRight(const Vec2f& p)
            {
                bottom_right_ = p;
            }

            Vec2f top_left_;
            Vec2f bottom_right_;
        private:

    };

#endif // CRECT_H
