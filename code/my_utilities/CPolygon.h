#ifndef CPOLYGON_H_
#define CPOLYGON_H_

#include "Vec2.h"
#include "CRect.h"
#include <cassert>
#include <vector>

using std::vector;

struct TSupportPoints;

    class CPolygon
    {
    public:

            //creates instance of polygon. After points are set, BuildPolygon() must be called
            CPolygon();

            //creates centered perfect polygon
            CPolygon(unsigned int count, float radius);


            TSupportPoints GetSupports(const Vec2f& axis);

            float CalcMass(float density);
            float CalcInertia();

            //adds a vertex
            void PushPoint(const Vec2f& v);

            //changes value of existing vertex at index
            void SetPoint(unsigned int i, const Vec2f& v);

            //gets value of vertex at index
            Vec2f GetPoint(unsigned int i) const;

            //get number of points in polygon
            unsigned int GetPointCount() const;

            //deletes all the points
            void ClearPoints();

            //builds the edges as well as offsets all the points for proper rotation.
            //Must be called after all the points are set if the default ctor is used
            void BuildPolygon();

            // Sets rough radius for quick bounding rectangle
            void CalcRadius();

            void SetRadius(float r);

            float GetRadius()                    { return radius_; }

            //gets value of edge at index --BuildPolygon must be called first
            Vec2f GetEdge(unsigned int i) const;

            //returns the average position of all the points
            Vec2f& GetCenter();

            inline void SetCenter(const Vec2f& c) { center_ = c; }

            //returns a tight rectangle around polygon
            CRect GetBoundingRect();

            //translation of all points
            void Translate(const Vec2f& v);

            //translation then rotation
            void Transform(const Vec2f& v, float angle);

            //transformed polygon
            CPolygon Transformed(const Vec2f& v, float angle) const;

        private:
            vector<Vec2f> points_;
            Vec2f center_;
            float radius_;

            //finds the center of the polygon
            Vec2f CalcCenter();


    };

#endif // CPOLYGON_H_
