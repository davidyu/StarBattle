///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CCollisionManager.cpp
 *	\author		Fish
 *	\date		August 1, 2011
 *  \brief      Implementation for CCollisionManager
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "CBullet.h"
#include "CGenericObject.h"
#include "CGenericShip.h"
#include "my_utilities/BoxPruner.h"

#include "CCollisionManager.h"

CCollisionManager::CCollisionManager()
{
    for (uint i=0; i<=COL_BULLET; ++i) radix_sorters_.push_back(RadixSort());
}

CCollisionManager::~CCollisionManager()
{
    //dtor
}

void CCollisionManager::PruneObjectObject(vector<CGenericObject*> objects)
{

    TAxes axes; axes.Axis0 = 0; axes.Axis1 = 1;
    vector<CRect*> boundingRects;
    vector<TPair> collision_pairs;

    for (uint i=0; i<objects.size(); ++i)
    {
        boundingRects.push_back(objects[i]->GetBoundingRect());
    }

    CompletePruning(radix_sorters_[COL_OBJ], boundingRects, collision_pairs, axes);

    for (uint i=0; i<collision_pairs.size(); ++i)
    {
        TCollisionReport report(objects[collision_pairs[i].id0], objects[collision_pairs[i].id1]);

        if(report.collisionReported)
        {
            report.ApplyResponse(0.3f, 0.9f);
        }

    }
}

void CCollisionManager::PruneBulletObject(vector<CBullet*> bullets, vector<CGenericObject*> objects)
{
    TAxes axes; axes.Axis0 = 0; axes.Axis1 = 1;
    vector<CRect*> bounding_b;
    vector<CRect*> bounding_s;
    vector<TPair> collision_pairs;

    for (uint i=0; i<objects.size(); ++i)   bounding_s.push_back(objects[i]->GetBoundingRect());
    for (uint i=0; i<bullets.size(); ++i)   bounding_b.push_back(bullets[i]->GetBoundingRect());

    BipartitePruning(radix_sorters_[COL_BULLET], radix_sorters_[COL_OBJ], bounding_b, bounding_s, collision_pairs, axes);

    for (uint i=0; i<collision_pairs.size(); ++i)
    {
        TCollisionReport report(bullets[collision_pairs[i].id0], objects[collision_pairs[i].id1]);

        if(report.collisionReported)
        {
            report.ApplyResponse(0.3f, 0.9f);
        }

    }
}

void CCollisionManager::PruneQueryShip(vector<CGenericShip*> ships, vector<vector<CGenericShip*> >& results)
{
    TAxes axes; axes.Axis0 = 0; axes.Axis1 = 1;
    vector<CRect*> ship_bounding;
    vector<CRect*> ship_query;
    vector<TPair> collision_pairs;

    for (uint i=0; i<ships.size(); ++i)
    {
        ship_bounding.push_back(ships[i]->GetBoundingRect());
        ship_query.push_back(ships[i]->GetQueryRect());
    }

    BipartitePruning(radix_sorters_[COL_SHIP_QUERY], radix_sorters_[COL_SHIP],ship_query, ship_bounding, collision_pairs, axes);

    for (uint i=0; i<collision_pairs.size();)
    {
        uint same = collision_pairs[i].id0;

        while(results.size()<=collision_pairs[i].id0)
            results.push_back(vector<CGenericShip*>());
        while(collision_pairs[i].id0==same && i<collision_pairs.size())
        {
            results.back().push_back(ships[collision_pairs[i++].id1]);
        }
    }


}

void CCollisionManager::OcclusionCull(CRect* camera, vector<CGenericObject*> objects, vector<CBullet*> bullets, vector<CGenericObject*>& objects_in_view)
{
    TAxes axes; axes.Axis0 = 0; axes.Axis1 = 1;
    vector<CRect*> bounding;
    vector<uint> results;

    for (uint i=0; i<objects.size(); ++i)
    {
        bounding.push_back(objects[i]->GetBoundingRect());
    }
    SinglePruning(radix_sorters_[COL_OBJ], camera, bounding, results, axes);

    for (uint i=0; i<results.size(); ++i)
    {
        objects_in_view.push_back(objects[results[i]]);
    }

    bounding.clear();
    results.clear();
    for (uint i=0; i<bullets.size(); ++i)
    {
        bounding.push_back(bullets[i]->GetBoundingRect());
    }
    SinglePruning(radix_sorters_[COL_BULLET], camera, bounding, results, axes);

    for (uint i=0; i<results.size(); ++i)
    {
        objects_in_view.push_back(bullets[results[i]]);
    }


}
