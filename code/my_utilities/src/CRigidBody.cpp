#include "../CRigidBody.h"

CRigidBody::CRigidBody(CPolygon poly, Vec2f pos, float orientation, float density) :
    polygon_(poly),
    pos_(pos),
    orient_(orientation)
{
    vel_          = Vec2f(0.0f, 0.0f);
    angVel_       = 0.0f;

    float mass    = (density == 0.0f)? 0.0f : polygon_.CalcMass(density);
    float inertia = (density == 0.0f)? 0.0f : polygon_.CalcInertia();

    invMass_	  = (mass > 0.0f)? 1.0f / mass : 0.0f;
    invInertia_	  = (inertia > 0.0f)? 1.0f / (inertia * mass) : 0.0f;

    Vec2f center = pos_;
    float radius = poly.GetRadius();
    boundingRect_ = CRect(center.y - radius, center.x - radius, center.y + radius, center.x + radius);
}

CRigidBody::CRigidBody() :
    polygon_(CPolygon()),
    pos_(Vec2f(0.0f, 0.0f)),
    vel_(Vec2f(0.0f, 0.0f)),
    orient_(90.0f),
    angVel_(0.0f)
{
}

//takes reference to another body and calculates collision effects on both
void CRigidBody::CollisionResponse(CRigidBody& body, const TCollisionResult& result)
{
    if (result.overlapped)
    {
        pos_     += result.mtv * (invMass_ / (invMass_ + body.GetInvMass()));
        body.SetPos(body.GetPos() - result.mtv * (body.GetInvMass() / (invMass_ + body.GetInvMass())));
    }

    float tcoll;//time of collision;
    Vec2f ncoll(0.0f, 0.0f);

    //move bodies to collision time
    if (result.willOverlap && result.tEnter > 0.0f)
    {
        tcoll = result.tEnter;
        ncoll = result.nEnter;
    }
    else
    {
        if (result.mtvLengthSquared < TOL)
            return;

        tcoll = 0.0f;
        ncoll = result.mtv.Normalized();
    }

    pos_ += vel_ * tcoll;
    body.SetPos(body.GetPos() + body.GetVel() * tcoll);

    Vec2f vcoll = vel_ - body.GetVel();
    float vn    = vcoll * ncoll;

    if (vn > 0.0f)
        return;

    float CoR    = 1.0f;
    float numer  = -(1.0f + CoR) * vn;
    float denom  = (invMass_ + body.GetInvMass());
    Vec2f j      = ncoll * (numer / denom);

    vel_ += j * invMass_;
    body.SetVel(body.GetVel() - j * body.GetInvMass());

}

CRect* CRigidBody::GetBoundingRect()
{
    boundingRect_.SetPos(pos_);
    return &boundingRect_;
}

//updates position & similar
void CRigidBody::Step()
{
    orient_ += angVel_;
    pos_    += vel_;

}

void CRigidBody::Move(const Vec2f& pos)
{
    pos_ += pos;
}

//increments vel by v
void CRigidBody::IncVel(const Vec2f& v)
{
    vel_ += v;
}

//increments angVel by a;
void CRigidBody::IncAngVel(float a)
{
    angVel_ += a;
}

//Setters

void CRigidBody::SetPos(const Vec2f& pos)
{
    pos_ = pos;
}

void CRigidBody::SetVel(const Vec2f& vel)
{
    vel_ = vel;
}

void CRigidBody::SetOrient(float orient)
{
    orient_ = orient;
}

void CRigidBody::SetMass(float mass)
{
    invMass_ = (mass <= 0.0f)? 0.0f : 1.0f / mass;
}

void CRigidBody::SetAngVel(float angVel)
{
    angVel_ = angVel;
}

void CRigidBody::SetPolygon(const CPolygon& poly)
{
    polygon_ = poly;
}

//Getters

const Vec2f& CRigidBody::GetPos() const
{
    return pos_;
}

const Vec2f& CRigidBody::GetVel() const
{
    return vel_;
}

float CRigidBody::GetOrient() const
{
    return orient_;
}

float CRigidBody::GetInvMass() const
{
    return invMass_;
}
float CRigidBody::GetInvInertia() const
{
    return invInertia_;
}

float CRigidBody::GetAngVel() const
{
    return angVel_;
}

CPolygon CRigidBody::GetPolygon() const
{
    return polygon_.Transformed(pos_, orient_);
}
