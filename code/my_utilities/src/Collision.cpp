#include "../Collision.h"
#include <SFML/Graphics/Shape.hpp>

#include <stdlib.h>

using namespace std;

//Function prototypes
Vec2f GetEdge(CPolygon polygon, unsigned int index);
void ProjectPolygon(Vec2f axis, CPolygon polygon, float& min, float& max);
float IntervalDist(float minA, float maxA, float minB, float maxB);
bool SeparatedByAxisCurrently(const Vec2f& axis, float d0, float d1, TCollisionResult& result);
bool SeparatedByAxisSwept(const Vec2f& axis, float d0, float d1, float v, TCollisionResult& result);
bool SeparatedByAxis(const Vec2f& axis, const CPolygon& polygonA, const CPolygon& polygonB, const Vec2f& delta, TCollisionResult& result);
TCollisionResult GetPolygonCollision(CPolygon& polygonA, CPolygon& polygonB, const Vec2f& delta);

TCollisionResult::TCollisionResult()
{
    tEnter = 0.0f;
    tLeave = 0.0f;
    nEnter = Vec2f(0,0);
    nLeave = Vec2f(0,0);
    overlapped = false;

    willOverlap = false;
    mtv = Vec2f(0, 0);
    mtvLengthSquared = 0.0f;
}

TSupportPoints::TSupportPoints()
{
    count = 0;
}

TContactPair::TContactPair()
{
    pos[0] = pos[1] = Vec2f(0.0f, 0.0f);
    jn = jt = 0.0f;
    distanceSquared = 0.0f;
}

TContactPair::TContactPair(const Vec2f& a, const Vec2f& b)
{
    Vec2f d(b - a);
    pos[0] = a;
    pos[1] = b;
    distanceSquared = d * d;
}

int CompareContacts(const void* v0, const void* v1)
{
    TContactPair *V0 = (TContactPair*) v0;
    TContactPair *V1 = (TContactPair*) v1;
    return (V0->distanceSquared > V1->distanceSquared)? 1 : -1;
}

Vec2f ClosestPointOnEdge(const Vec2f *edge, const Vec2f& v)
{
    Vec2f e(edge[1] - edge[0]);
    Vec2f d(v - edge[0]);
    float t = (e * d) / (e * e);
    t = (t < 0.0f) ? 0.0f : (t > 1.0f)? 1.0f : t;
    return edge[0] + e * t;
}

TContactInfo::TContactInfo()
{
    count = 0;
}


TContactInfo::TContactInfo(const TSupportPoints& supports1, const TSupportPoints& supports2)
{
    if (supports1.count == 1)
    {
        if (supports2.count == 1)
        {
            VertexVertex(supports1.support[0], supports2.support[0]);
        }
        else if (supports2.count == 2)
        {
            VertexEdge(supports1.support[0], supports2.support);
        }
        else
        {
            assert(false);
        }
    }
    else if (supports1.count == 2)
    {
        if (supports2.count == 1)
        {
            EdgeVertex(supports1.support, supports2.support[0]);
        }
        else if (supports2.count == 2)
        {
            EdgeEdge(supports1.support, supports2.support);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        assert(false);
    }
}

void TContactInfo::EdgeEdge(const Vec2f *edge1, const Vec2f *edge2)
{
    contact[0] = TContactPair(edge1[0], ClosestPointOnEdge(edge2, edge1[0]));
    contact[1] = TContactPair(edge1[1], ClosestPointOnEdge(edge2, edge1[1]));
    contact[2] = TContactPair(ClosestPointOnEdge(edge1, edge2[0]), edge2[0]);
    contact[3] = TContactPair(ClosestPointOnEdge(edge1, edge2[1]), edge2[1]);

    //sort contact pairs by distance value
    qsort(contact, 4, sizeof(contact[0]), CompareContacts);

    //take closest 2
    count = 2;


}

void TContactInfo::VertexVertex(const Vec2f& vertex1, const Vec2f& vertex2)
{
    contact[0] = TContactPair(vertex1, vertex2);
    count = 1;
}

void TContactInfo::EdgeVertex(const Vec2f *edge, const Vec2f& vertex)
{
    contact[0] = TContactPair(ClosestPointOnEdge(edge, vertex), vertex);
    count      = 1;
}

void TContactInfo::VertexEdge(const Vec2f& vertex, const Vec2f *edge)
{
    contact[0] = TContactPair(vertex, ClosestPointOnEdge(edge, vertex));
    count = 1;
}

TCollisionReport::TCollisionReport()
{
    collisionReported = false;
    body[0]           = NULL;
    body[1]           = NULL;
    ncoll             = Vec2f(0.0f, 0.0f);
    mtv               = Vec2f(0.0f, 0.0f);
    tcoll             = 0.0f;
}

TCollisionReport::TCollisionReport(CRigidBody* a, CRigidBody* b)
{
    collisionReported  = false;
    body[0]            = a;
    body[1]            = b;
    contacts           = TContactInfo();
    ncoll              = Vec2f(0.0f, 0.0f);
    mtv                = Vec2f(0.0f, 0.0f);
    tcoll              = 0.0f;

    //get polygons in world space
    poly.push_back(a->GetPolygon());
    poly.push_back(b->GetPolygon());

    //find collision
    Vec2f delta        = (a->GetVel() - b->GetVel());
    collisionResult    = GetPolygonCollision(poly[0], poly[1], delta);
    collisionReported  = (collisionResult.overlapped || collisionResult.willOverlap);

    if (!collisionReported)
        return;

    //convert collisionResult into collision plane info
    if (collisionResult.overlapped)
    {
        if (collisionResult.mtvLengthSquared <= TOL)
        {
            collisionReported = false;
            return;
        }

        ncoll = collisionResult.mtv / collisionResult.mtv.Length();
        tcoll = 0.0f;
        mtv = collisionResult.mtv;
    }
    else if (collisionResult.willOverlap)
    {
        ncoll = collisionResult.nEnter;
        tcoll = collisionResult.tEnter;
    }

    poly[0].Translate(a->GetVel() * tcoll);
    poly[1].Translate(b->GetVel() * tcoll);

    TSupportPoints asup = poly[0].GetSupports(ncoll);
    TSupportPoints bsup = poly[1].GetSupports(-ncoll);

    contacts = TContactInfo(asup, bsup);

}

void TCollisionReport::ApplyResponse(float cor, float cof)
{
    if (!collisionReported)
        return;

    CRigidBody *a = body[0];
    CRigidBody *b = body[1];

    //overlapped, then separate
    a->Move(  mtv * (a->GetInvMass() / (a->GetInvMass() + b->GetInvMass())));
    b->Move(-(mtv * (b->GetInvMass() / (a->GetInvMass() + b->GetInvMass()))));

    if (mtv.LengthSquared() < 0.001)
        mtv = mtv;

    //move to time of collision
    a->Move(a->GetVel() * tcoll);
    b->Move(b->GetVel() * tcoll);

    for (int i = 0; i < contacts.count; ++i)
    {
        Vec2f pa = contacts.contact[i].pos[0]; //position of a
        Vec2f pb = contacts.contact[i].pos[1]; // "" b
        Vec2f ra = pa - a->GetPos();
        Vec2f rb = pb - b->GetPos();
        Vec2f va = a->GetVel() + ra.GetNormal() * a->GetAngVel();
        Vec2f vb = b->GetVel() + rb.GetNormal() * b->GetAngVel();
        Vec2f v  = va - vb;
        Vec2f vt = v - (v * ncoll) * ncoll;
        Vec2f nf = -vt; //friction normal
        nf.Normalize();
        Vec2f nc = ncoll; //collision normal


        //contact points separating, no impulses
        if (v * nc > 0.0f)
            continue;

        //collision impulse
        float jc = (v * nc) / ( (a->GetInvMass() + b->GetInvMass()) +
                                (ra ^ nc) * (ra ^ nc) * a->GetInvInertia() +
                                (rb ^ nc) * (rb ^ nc) * b->GetInvInertia() );

        //friction impulse
        float jf = (v * nf) / ( (a->GetInvMass() + b->GetInvMass()) +
                                (ra ^ nf) * (ra ^ nf) * a->GetInvInertia() +
                                (rb ^ nf) * (rb ^ nf) * b->GetInvInertia() );

        //clamp friction
        if (fabs(jf) > fabs(jc * cof))
            jf = fabs(cof) * sign(jc);

        //total impuse restitued
        Vec2f impulse = nc * (jc * -(cor)) + nf * (jf * -1.0f);

        a->IncVel( (impulse * a->GetInvMass()));
        b->IncVel(-(impulse * b->GetInvMass()));

        a->IncAngVel( ((ra ^ impulse) * a->GetInvInertia()));
        b->IncAngVel(-((rb ^ impulse) * b->GetInvInertia()));
    }

}

//bool PolygonPolygonCollision(const CPolygon& polygonA, const CPolygon& polygonB)
//{
//    int edgeCountA = polygonA.GetPointCount();
//    int edgeCountB = polygonB.GetPointCount();
//    Vec2f edge;
//
//    for (int edgeIndex=0; edgeIndex<edgeCountA+edgeCountB; ++edgeIndex)
//    {
//        if (edgeIndex<edgeCountA)
//            edge = polygonA.GetEdge(edgeIndex);
//        else
//            edge = polygonB.GetEdge(edgeIndex-edgeCountA);
//
//
//        Vec2f axis = edge.GetNormal();
//        if (SeparatedByAxis(axis, polygonA, polygonB, delta, result))
//        {
//            return false;
//        }
//    }
//    
//    return true;
//}

TCollisionResult GetPolygonCollision(CPolygon& polygonA, CPolygon& polygonB, const Vec2f& delta)
{
    TCollisionResult result;

    result.overlapped       = true;
    result.willOverlap      = true;
    result.mtvLengthSquared = -1.0f;
    result.tEnter           = 1.0f;
    result.tLeave           = 0.0f;

    int edgeCountA = polygonA.GetPointCount();
    int edgeCountB = polygonB.GetPointCount();
    Vec2f edge;

    for (int edgeIndex=0; edgeIndex<edgeCountA+edgeCountB; ++edgeIndex)
    {
        if (edgeIndex<edgeCountA)
            edge = polygonA.GetEdge(edgeIndex);
        else
            edge = polygonB.GetEdge(edgeIndex-edgeCountA);


        Vec2f axis = edge.GetNormal();
        if (SeparatedByAxis(axis, polygonA, polygonB, delta, result))
        {
            return TCollisionResult();
        }
    }

    // sanity checks
    result.overlapped  &= (result.mtvLengthSquared >= 0.0f);
    result.willOverlap &= (result.tEnter <= result.tLeave);

    result.nEnter.Normalize();
    result.nLeave.Normalize();

    return result;
}

bool CircleCollision(CRigidBody& bodyA, CRigidBody& bodyB)
{
    Vec2f dist = bodyA.GetPolygon().GetCenter() - bodyB.GetPolygon().GetCenter();
    if (dist.LengthSquared() <= 200.0f)
        return true;

    return false;
}

bool PointInRect(Vec2f p, CRect r)
{
    float left   = r.GetLeft();
    float right  = r.GetRight();
    float top    = r.GetTop();
    float bottom = r.GetBottom();

    return (left <= p.x && right >= p.x &&
            top <= p.y && bottom >= p.y);
}

void ProjectPolygon(Vec2f axis, CPolygon polygon, float& min, float& max)
{
    float dotProduct = axis * polygon.GetPoint(0);
    min = dotProduct;
    max = dotProduct;

    for (int i=1; i<polygon.GetPointCount(); ++i)
    {
        dotProduct = axis * polygon.GetPoint(i);
        if (dotProduct<min)
            min = dotProduct;
        else if (dotProduct>max)
            max = dotProduct;
    }
}

bool SeparatedByAxis(const Vec2f& axis, const CPolygon& polygonA, const CPolygon& polygonB, const Vec2f& delta, TCollisionResult& result)
{
    float minA, maxA, minB, maxB;

    //calculate polygon projections along the seperation axis we are testing
    ProjectPolygon(axis, polygonA, minA, maxA);
    ProjectPolygon(axis, polygonB, minB, maxB);

    float v  = axis * delta; //project delta on axis for sweep test
    float d0 = maxB - minA; //potential left collision
    float d1 = minB - maxA; //... right...


    bool sepCurrent = SeparatedByAxisCurrently(axis, d0, d1, result);
    bool sepSwept   = SeparatedByAxisSwept    (axis, d0, d1, v, result);

    return (sepCurrent && sepSwept);

}

bool SeparatedByAxisCurrently(const Vec2f& axis, float d0, float d1, TCollisionResult& result)
{
    if (!result.overlapped)
        return true;

    // intervals do not overlap so no overlap possible
    if (d0 < 0.0f || d1 > 0.0f)
    {
        result.overlapped = false;
        return true;
    }

    //left overlap or right?
    float overlap = (d0 < -d1) ? d0 : d1;

    //assert(axisLengthSquared > 0.000001f);

    //mtv vector for axis
    Vec2f sep = axis * (overlap / axis.LengthSquared());

    float sepLengthSquared = sep.LengthSquared();

    //if sepLengthSquared is smaller than mtv, update mtv
    if (sepLengthSquared < result.mtvLengthSquared || result.mtvLengthSquared < 0.0f)
    {
        result.mtvLengthSquared = sepLengthSquared;
        result.mtv              = sep;

    }
    return false;

}

bool SeparatedByAxisSwept(const Vec2f& axis, float d0, float d1, float v, TCollisionResult& result)
{
    //if will overlap or the projection is too small
    if (!result.willOverlap)
        return true;
    if (fabs(v) < TOL)
    {
        return true;
    }

    Vec2f n0 = axis;
    Vec2f n1 = -axis;
    float t0 = d0 / v; //estimated time of collision to left side
    float t1 = d1 / v; //...  right

    //sort values on axis to get a valid swept interval... ?
    if (t0 > t1)
    {
        fswap(t0, t1);
        Vec2f tempV = n0;
        n0 = n1;
        n1 = tempV;
    }

    //swept interval outside [0, 1] boundaries -> polygons too far apart
    if (t0 > 1.0f || t1 < 0.0f)
    {
        result.willOverlap = false;
        return true;
    }

    //swept interval of collision hasn't been performed yet
    if (result.tEnter > result.tLeave)
    {
        result.tEnter = t0;
        result.tLeave = t1;
        result.nEnter = n0;
        result.nLeave = n1;
        return false;
    }
    else
    {
        if (t0 > result.tLeave || t1 < result.tEnter)
        {
            result.willOverlap = false;
            return true;
        }

        //reduce collision interval to minima
        if (t0 > result.tEnter)
        {
            result.tEnter = t0;
            result.nEnter = n0;
        }
        if (t1 < result.tLeave)
        {
            result.tLeave = t1;
            result.nLeave = n1;
        }
        return false;
    }
}

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

bool PointInPolygon(const Vec2f& p, const CPolygon& poly)
{
  uint counter = 0;
  float xintersection;
  Vec2f p1, p2;
  int n = poly.GetPointCount();

  p1 = poly.GetPoint(0);
  for (uint i=1; i<=n; ++i)
  {
    p2 = poly.GetPoint(i % n);
    if (p.y > MIN(p1.y,p2.y)   && (p.y <= MAX(p1.y,p2.y)) &&
       (p.x <= MAX(p1.x,p2.x)) &&  p1.y != p2.y)
    {
            xintersection = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
            if (p1.x == p2.x || p.x <= xintersection)
              counter++;

    }
    p1 = p2;
  }

  if (counter % 2 == 0)
    return(false);
  else
    return(true);
}

bool LineIntersectPolygon(const TLine& line, const CPolygon& poly, TLine& intersect)//(, Point* V, int n, Segment* IS)
{
    if (line.p0 == line.p1)
    {
        intersect = line;
        return PointInPolygon(line.p0, poly);
    }

    float  tE   = 0.f;                  // the maximum entering segment parameter
    float  tL   = 1.f;                  // the minimum leaving segment parameter
    Vec2f dS    = line.p1 - line.p0;    // the segment direction vector
    Vec2f e;                            // edge vector
    float  t, N, D;                     // intersect parameter t = N / D

    for (uint i=0; i<poly.GetPointCount(); ++i)  // process polygon edge V[i]V[i+1]
    {
        e = poly.GetEdge(i);
        N = -(e ^ (line.p0 - poly.GetPoint(i))); // = -dot(ne, S.P0-V[i])
        D = (e ^ dS);       // = dot(ne, dS)

        if (fabs(D) < TOL) // line is nearly parallel to this edge
        {
            if (N < 0.f)             // P0 is outside this edge, so
                return false;      // S is outside the polygon
                                   // S cannot cross this edge, so
            continue;          // ignore this edge
        }

        t = N / D;
        if (D<0.f && t>tE) // segment S is entering across this edge
        {
                tE = t;
                if (tE > tL)   // S enters after leaving polygon
                    return false;
        }
        else if(D>=0.f &&t<tL)
        {                 // segment S is leaving across this edge
                tL = t;
                if (tE > tL)   // S enters after leaving polygon
                    return false;
        }


    }

    // tE <= tL implies that there is a valid intersection subsegment
    intersect.p0 = line.p0 + tE * dS;   // = P(tE) = point where S enters polygon
    intersect.p1 = line.p0 + tL * dS;   // = P(tL) = point where S leaves polygon

    return true;
}

bool PointCollideBody(const Vec2f& point, const Vec2f& vel, const CRigidBody& body)
{
    TLine line_swept = TLine(point, point + vel - body.GetVel()); 
    TLine l;
    
    return LineIntersectPolygon(line_swept, body.GetPolygon(), l);
}
