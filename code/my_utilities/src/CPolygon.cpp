#include "../CPolygon.h"
#include "../Collision.h"

CPolygon::CPolygon() : radius_(0)
{

}

CPolygon::CPolygon(unsigned int count, float radius)
{
    for(unsigned int i = 0; i < count; ++i)
    {
        float a = 2.0f * pi * (i / (float) count);
        points_.push_back(Vec2f(cos(a), sin(a)) * radius);
    }

    BuildPolygon();

}

TSupportPoints CPolygon::GetSupports(const Vec2f& axis)
{
	TSupportPoints supports;

	float min = -1.0f;
	const float threshold = 1.0E-1f;

	int num = points_.size();
	int i;

	for(i = 0; i < num; ++i)
	{
		float t = (axis * points_[i]);
		if(t < min || i == 0)
			min = t;
	}

	for(i = 0; i < num; ++i)
	{
		float t = (axis * points_[i]);

		if(t < min+threshold)
		{
			supports.support[supports.count++] = points_[i];
			if (supports.count == 2)
                break;
		}
	}
	return supports;
}

float CPolygon::CalcMass(float density)
{
    if (points_.size() < 2)
        return 5.0f * density;

    float mass = 0.0f;
    for (unsigned int j = points_.size()-1, i = 0; i < points_.size(); j = i++)
    {
        mass += (float) fabs(points_[i] ^ points_[j]);
    }

    mass *= density * 0.5f;
    return mass;
}

float CPolygon::CalcInertia()
{
    if (points_.size() == 1)
        return 0.0f;

    float denom = 0.0f;
    float numer = 0.0f;

    for (unsigned int j = points_.size()-1, i = 0; i < points_.size(); j = i++)
    {
        float a = (float) fabs(points_[i] ^ points_[j]);
        float b = points_[j].LengthSquared() + points_[j] * points_[i] + points_[i].LengthSquared();

        denom += a * b;
        numer += a;
    }

    float inertia = (denom / numer) * (1.0f / 6.0f);
    return inertia;
}

//adds a vertex
void CPolygon::PushPoint(const Vec2f& v)
{
    points_.push_back(v);
}

//changes value of existing vertex at index
void CPolygon::SetPoint(unsigned int i, const Vec2f& v)
{
    assert(i < points_.size());
    points_[i] = v;
}

//gets value of vertex at index
Vec2f CPolygon::GetPoint(unsigned int i) const
{
    assert(i < points_.size());
    return points_[i];
}

//get number of points in polygon
unsigned int CPolygon::GetPointCount() const
{
    return points_.size();
}

//deletes all the points
void CPolygon::ClearPoints()
{
    points_.clear();
}


//builds the edges --call when all the points are set
void CPolygon::BuildPolygon()
{
    center_ = CalcCenter();

    for (unsigned int i = 0; i < points_.size(); ++i)
    {
        points_[i] -= center_;
    }

    CalcRadius();
}

Vec2f CPolygon::CalcCenter()
{
    Vec2f center(0.0f, 0.0f);

    for (unsigned int i = 0; i < points_.size(); ++i)
    {
        center += points_[i];
    }
    return (center / (float)points_.size());
}

//gets value of edge at index --BuildEdges must have been called first
Vec2f CPolygon::GetEdge(unsigned int i) const
{
    uint j = i+1;

    if (j==points_.size())
        j = 0;

    return points_[j] - points_[i];
}
//returns center point
Vec2f& CPolygon::GetCenter()
{
    return center_;
}

void CPolygon::CalcRadius()
{
    if (points_.empty())     return;

    Vec2f p = center_ - points_[0];
    float temp, max = p.LengthSquared();

    for (uint i=1; i<points_.size(); ++i)
    {
        p = center_ - points_[i];
        temp = p.LengthSquared();
        if (temp > max)    max = temp;
    }

    radius_ = sqrt(max);
}

void CPolygon::SetRadius(float r)
{
    radius_ = r;
}
//returns tight rectangle around polygon
CRect CPolygon::GetBoundingRect()
{
    return CRect(center_.y - radius_, center_.x + radius_, center_.y + radius_, center_.x - radius_);
}

//Adds all the points by the given vector
void CPolygon::Translate(const Vec2f& v)
{
    for (unsigned int i = 0; i < points_.size(); ++i)
    {
        points_[i] += v;
    }
}

void CPolygon::Transform(const Vec2f& v, float angle)
{
    for (unsigned int i = 0; i < points_.size(); ++i)
    {
        points_[i].Transform(v, angle);
    }
}

CPolygon CPolygon::Transformed(const Vec2f& v, float angle) const
{
    CPolygon tempP;

    for (unsigned int i = 0; i < points_.size(); ++i)
    {
        tempP.PushPoint(points_[i].Transformed(v, angle));
    }
    tempP.SetCenter(center_+v);
    tempP.SetRadius(radius_);

    return tempP;
}
////sets the point of rotation of the polygon
//void CPolygon::SetOrigin(Vec2f o)
//{
//    origin_ = o;
//}
//
////sets the orientation of the polygon
//void CPolygon::SetOrientation(float o)
//{
//    while(o > 360.0f)
//    {
//        o -= 360.0f;
//    }
//    orientation_ = o;
//}
//
////gets the orientation of the polygon
//float CPolygon::GetOrientation()
//{
//    return orientation_;
//}
//
////rotates the polygon
//void CPolygon::Rotate(float angle)
//{
//    orientation_ += angle;
//    if (orientation_ > 360.0f)
//        orientation_ -= 360.0f;
//}
