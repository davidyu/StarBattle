///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CCollisionManager.h
 *	\author		Fish
 *	\date		August 1, 2011
 *  \brief      Keeps track of colliding pairs and stuff
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

// Headers
#include "my_utilities/BoxPruner.h"
#include <vector>

using std::vector;
using std::pair;

class TPair;
class CGenericObject;
class CBullet;

enum TCollisionBody { COL_OBJ = 0, COL_SHIP, COL_SHIP_QUERY, COL_BULLET };

class CCollisionManager
{
    public:
                                        CCollisionManager();
    virtual                             ~CCollisionManager();

    // Calculate Collisions
            void                        PruneObjectObject(vector<CGenericObject*> objects);
            void                        PruneBulletObject(vector<CBullet*> bullets, vector<CGenericObject*> objects);
            void                        PruneQueryShip(vector<CGenericShip*> ships, vector<vector<CGenericShip*> >& results);
            void                        OcclusionCull(CRect* camera, vector<CGenericObject*> objects, vector<CBullet*> bullets, vector<CGenericObject*>& objects_in_view);

    // Get Results
    private:
            vector<RadixSort>           radix_sorters_;
            vector<CGenericObject*>     collided_objects_;    //!< A list of all the ships that are potentiall colliding
};

#endif // CCOLLISIONMANAGER_H
