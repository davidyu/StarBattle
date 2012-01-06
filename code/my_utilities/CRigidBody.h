#ifndef CRIGIDBODY_H
#define CRIGIDBODY_H

#include "my_utilities/mymath.h"
#include "my_utilities/Collision.h"

struct TCollisionResult;

class CRigidBody
{
    public:
        //ctors
        CRigidBody();
        CRigidBody(CPolygon poly, Vec2f pos = Vec2f(0.0f, 0.0f), float orientation=0.f, float density=0.85f);//pi/2.f);

        //takes reference to another body and calculates collision effects on both
        void CollisionResponse(CRigidBody& body, const TCollisionResult& result);

        //updates position & similar
        void Step();

        //increments position by pos
        void Move(const Vec2f& pos);

        //increments vel by v
        void IncVel(const Vec2f& v);

        //increments angVel by a;
        void IncAngVel(float a);

        //Setters
        void SetPos(const Vec2f& pos);
        void SetVel(const Vec2f& vel);
        void SetOrient(float orient);
        void SetMass(float mass);
        void SetAngVel(float angVel);
        void SetPolygon(const CPolygon& poly);

        //Getters
        const Vec2f& GetPos() const;
        const Vec2f& GetVel() const;
        float GetOrient() const;
        float GetInvMass() const;
        float GetInvInertia() const;
        float GetAngVel() const;
        CPolygon GetPolygon() const;

        //temporary
        CRect* GetBoundingRect();

    protected:
        CPolygon polygon_;          //!< Collision polygon
        Vec2f    pos_;              //!< Position
        Vec2f    vel_;              //!< Velocity
        float    angVel_;           //!< Angular velocity
        float    orient_;           //!< Angle of body
        float    invMass_;          //!< Inverse mass
        float    invInertia_;       //!< Inverse inertia

        CRect    boundingRect_;      //!< Bounding rectangle for board phase



};

#endif // CRIGIDBODY_H
