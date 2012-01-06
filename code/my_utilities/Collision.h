#ifndef COLLISION_H_
#define COLLISION_H_

#include "mymath.h"
#include "CRigidBody.h"
#include <vector>

using std::vector;

class CRigidBody;

struct TCollisionResult
{
	TCollisionResult();

	// overlaps
	bool	overlapped;
	float	mtvLengthSquared;
	Vec2f	mtv;

	// swept
	bool	willOverlap;
	Vec2f	nEnter;//normals...
	Vec2f	nLeave;
	float	tEnter; //times...
	float	tLeave;
};

struct TSupportPoints
{
    TSupportPoints();

    enum { MAX_SUPPORTS = 4 };
    Vec2f support[MAX_SUPPORTS];
    int count;
};

struct TContactPair
{
    TContactPair();
    TContactPair(const Vec2f& a, const Vec2f& b);

    Vec2f pos[2];
    float distanceSquared;
    float jn; //normal impulse at contact
    float jt; //friction impulse at contact;
};

struct TContactInfo
{
    TContactInfo();

    TContactInfo(const TSupportPoints& supports1, const TSupportPoints& supports2);
    void EdgeEdge(const Vec2f *edge1, const Vec2f *edge2); //takes array of edges
    void EdgeVertex(const Vec2f *edge, const Vec2f& vertex);
    void VertexEdge(const Vec2f& vertex, const Vec2f *edge);
    void VertexVertex(const Vec2f& vertex1, const Vec2f& vertex2);

    enum { MAX_CONTACTS = 8 };
	TContactPair	contact[MAX_CONTACTS];
    int count;
};

struct TCollisionReport
{
    TCollisionReport();

    TCollisionReport(CRigidBody *a, CRigidBody *b);

    void ApplyResponse(float cor, float cof);

    bool             collisionReported;
    CRigidBody       *body[2];
    vector<CPolygon> poly;
    Vec2f            ncoll;
    Vec2f            mtv;
    float            tcoll;
    TCollisionResult collisionResult;
    TContactInfo     contacts;
    Vec2f            vcoll;
};

bool PolygonPolygonCollision(const CPolygon& a, const CPolygon& b);
bool CircleCollision(CRigidBody& bodyA, CRigidBody& bodyB);
bool PointInPolygon(const Vec2f& p, const CPolygon& poly);
bool LineIntersectPolygon(const TLine& line, const CPolygon& poly, TLine& intersect);
bool PointInRect(Vec2f p, CRect r);

#endif //COLLISION_H_
