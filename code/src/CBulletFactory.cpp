#include "CBulletFlyweight.h"

#include "CBulletFactory.h"

using std::pair;

map<EBulletType, CBulletFlyweight*>  CBulletFactory::bullets_;

CBulletFactory::CBulletFactory()
{
    //ctor
}

CBulletFactory::~CBulletFactory()
{
    for(std::map<EBulletType, CBulletFlyweight*>::iterator it = bullets_.begin(); it != bullets_.end(); ++it)
    {
        CBulletFlyweight* Value = (*it).second;
        delete Value;
    }
}

CBulletFlyweight* CBulletFactory::GetBulletBase(EBulletType type)
{
    CBulletFlyweight* new_bullet;

    if (bullets_.find(type)==bullets_.end())
    {
        switch (type)
        {
            case MACHINE:
                new_bullet = new CBulletFlyweight();
            default:
                new_bullet = new CBulletFlyweight();
        }

        bullets_.insert(pair<EBulletType, CBulletFlyweight*>(type, new_bullet));
    }

    return bullets_[type];
}
