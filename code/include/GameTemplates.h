///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		GameTemplates.h
 *	\author		Fish
 *	\date		August 2, 2011
 *  \brief      Contains all the code to initialize all the pretty objects
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef GAMETEMPLATES_H
#define GAMETEMPLATES_H

// Headers
#include "IShipFlyweight.h"
#include "my_utilities/CPolygon.h"

CPolygon InterceptorPolygon()
{
    static CPolygon temp;
    static bool init = false;

    if (!init)
    {
        temp.PushPoint(Vec2f(15, 0));
        temp.PushPoint(Vec2f(-5, -5));
        temp.PushPoint(Vec2f(-8, 0));
        temp.PushPoint(Vec2f(-5, 5));
        temp.BuildPolygon();
        init = true;
    }

    return temp;
}


TShipStats InterceptorStats()
{
    static TShipStats ts;
    static bool init = false;

    if (!init)
    {
        TShipHandling th;
        TShipHandling b;

        // Ship Handling
        th.max_vel          = 2.f;
        th.max_steer        = 0.025f;

        th.thrust_fwd       = 0.03f;
        th.thrust_rev       = 0.01f;
        th.steer            = 0.001f;
        th.steer_clip       = 0.0000001f;

        th.thrust_damp      = 0.05f;
        th.ang_decay_fwd    = 0.05f;
        th.ang_decay_rev    = 0.2f;

        // Burst Handling
        b.max_vel           = 4.f;
        b.max_steer         = 0.3f;

        b.thrust_fwd        = 0.05f;
        b.thrust_fwd        = 0.03f;
        b.steer             = 0.017f;
        b.steer_clip        = 0.0000001f;

        b.thrust_damp       = 0.1f;
        b.ang_decay_fwd     = 0.4f;
        b.ang_decay_rev     = 0.5f;

        // Ship Stats
        ts.handling             = th;
        ts.handling_burst       = b;
        ts.hull_max             = 100.f;
        ts.hull_regen           = 0.f;
        ts.hull_regen_timeout   = 0.f;
        ts.shields_max          = 0.f;
        ts.shields_regen        = 0.f;
        ts.shields_regen_timeout= 0.f;

        init = true;
    }

    return ts;
}

#endif // GAMETEMPLATES_H
