#ifndef CBULLETFLYWEIGHT_H
#define CBULLETFLYWEIGHT_H

#include "my_utilities/CAnimManager.h"

class CBulletFlyweight
{
    public:
        friend class CBullet;
        CBulletFlyweight() {}
        virtual ~CBulletFlyweight() {}
    protected:
            float damage_;
            CAnimManager anim_manager_;

};

#endif // CBULLETFLYWEIGHT_H
