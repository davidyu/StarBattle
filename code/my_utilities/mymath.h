/*
 *   mymath.h
 *   Harry Yu
 */


#ifndef MYMATH_H_
#define MYMATH_H_

#include "Vec2.h"
#include "CPolygon.h"
#include "CRect.h"
#include "mytypes.h"


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Rect.hpp>

const float TOL = 0.0000001f;

struct TLine
{
    inline          TLine() {}
    inline          TLine(const Vec2f& i1, const Vec2f& i2) : p0(i1), p1(i2) {}

    Vec2f p0, p1;
};

inline float sign(float x)
{
    return (x < 0.0f)? -1.0f : 1.0f;
}

inline void fswap(float& x, float& y)
{
    float t = y;
    y = x;
    x = t;
}

inline float mysign(float f) {
    if (f > 0) return 1.f;
    return (f == 0.f) ? 0.f : -1.f;
}


inline float min(float a, float b)
{
    return (a > b) ? b : a;
}

inline float max(float a, float b)
{
    return (a > b) ? a : b;
}

inline sf::Vector2f Vec2fToSFVec(Vec2f& v)
{
    return sf::Vector2f(v.x, v.y);
}

inline sf::Vector2f Vec2fToSFVec(const Vec2f& v)
{
    return sf::Vector2f(v.x, v.y);
}

inline Vec2f SFVecToVec2f(const sf::Vector2f& v)
{
    return Vec2f(v.x, v.y);
}

inline sf::Shape CPolyToSFPoly(const CPolygon& p)
{
    sf::Shape tempP;
    sf::Vector2f v;
    Vec2f vv;
    for (unsigned int i = 0; i < p.GetPointCount(); ++i)
    {
        vv =  p.GetPoint(i);
        v = Vec2fToSFVec(vv);
        tempP.AddPoint(v.x, v.y);
    }
    return tempP;
}

inline sf::Shape CRectToSFRect(const CRect& r)
{
    sf::Shape tempP = sf::Shape::Rectangle(r.top_left_.x, r.top_left_.y, r.bottom_right_.x, r.bottom_right_.y, sf::Color(0, 0, 0));

    return tempP;
}

inline sf::Shape CRectToSFPoly(const CRect& p)
{
    sf::Shape tempP;
    Vec2f vv;

    vv = p.GetTopLeft();
    tempP.AddPoint(vv.x, vv.y);

    vv = p.GetTopRight();
    tempP.AddPoint(vv.x, vv.y);

    vv = p.GetBottomRight();
    tempP.AddPoint(vv.x, vv.y);

    vv = p.GetBottomLeft();
    tempP.AddPoint(vv.x, vv.y);

    tempP.SetColor(sf::Color(0.0f, 200.0f, 0.0f));

    return tempP;
}

inline CRect SFRectToCRect(const sf::FloatRect& r)
{
    return CRect(r.Top, r.Left, r.Bottom, r.Right);
}

inline bool QuadraticSolver(float a, float b, float c, float& r1, float& r2)
{

    float discrim = b*b-4*a*c;
    if (discrim<0)  return false;

    float quadRoot = sqrt(discrim);

    r1 = (-b + quadRoot)/(2*a);
    r2 = (-b - quadRoot)/(2*a);

    return true;
}

inline void Cap(float& a, float cap)
{
    if (a > cap)    a = cap;
    else if (a < -cap)  a = -cap;
}

inline float Capped(float a, float cap)
{
    if (a > cap)    return cap;
    else if (a < -cap)  return -cap;
    return a;
}



#endif //MYMATH_H_
