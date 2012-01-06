///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for box pruning implementation.
 *	\file		BoxPruner.cpp
 *	\author		Fish
 *	\date		Jul 18, 2011
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Header
#include "../BoxPruner.h"
#include "../memory_macros.h"

class CGenericShip;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  Complete box pruning. Returns a list of overlapping pairs of boxes,
 *  each box of the pair belongs to the same set.
 *	\param		sorter  [in] a radix sorter that computes increasing values
 *	\param		list	[in] list of boxes
 *	\param		pairs	[out] list of overlapping pairs
 *	\param		axes	[in] projection order
 *	\return		true if success.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 bool CompletePruning(RadixSort& sorter, const vector<CRect*>& list,
                                 vector<TPair>& pairs, const TAxes& axes)
{
    uint nb    = list.size();
    if(!nb)    return false;

    // Catch axes
    uint axis0 = axes.Axis0;
    uint axis1 = axes.Axis1;

    // Temporary data
    float* posList = new float[nb];

    // Build main list using primary axis
    for (uint i=0; i<nb; ++i)   posList[i] = list[i]->GetMin(axis0);

    // Sort list
    uint* sorted = sorter.Sort(posList, nb).GetRanks();

    // Prune list
    const uint* const lastSorted = &sorted[nb];
    const uint* runningAddress = sorted;
    uint index0, index1;

    while (runningAddress<lastSorted && sorted<lastSorted)
    {
        index0 = *sorted++;

        while (runningAddress<lastSorted && posList[*runningAddress++]<posList[index0]) ; // Do nothing!

        const uint* runningAddress2 = runningAddress;

        while (runningAddress2<lastSorted && posList[index1 = *runningAddress2++]<=list[index0]->GetMax(axis0))
        {
            if (index0!=index1)
            {
                if (list[index0]->Intersects(list[index1], axis1) && list[index0]->Intersects(list[index1], axis0) )
                {
                    pairs.push_back(TPair(index0, index1));
                }
            }
        }

    }

    DELETEARRAY(posList);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  Bipartite version of box pruning
 *	\param		sorter  [in] a radix sorter that computes increasing values
 *	\param		list0	[in] list of boxes
 *  \param		list1	[in] list of boxes
 *	\param		pairs	[out] list of overlapping pairs
 *	\param		axes	[in] projection order
 *	\return		true if success.
 */
//////////////////////////////////////////////////////////////////////////////////////////////////
bool BipartitePruning(RadixSort& rs0, RadixSort& rs1, const vector<CRect*>& list0,
                                 const vector<CRect*>& list1, vector<TPair>& pairs, const TAxes& axes)
{
    uint nb0    = list0.size();
    uint nb1    = list1.size();
    uint axis0  = axes.Axis0;
    uint axis1  = axes.Axis1;

    if (!nb0 || !nb1)   return false;

    float* minPosList0 = new float[nb0];
    float* minPosList1 = new float[nb1];

    for (uint i=0; i<nb0; ++i)  minPosList0[i] = list0[i]->GetMin(axis0);
    for (uint i=0; i<nb1; ++i)  minPosList1[i] = list1[i]->GetMin(axis0);

    uint* sorted0 = rs0.Sort(minPosList0, nb0).GetRanks();
    uint* sorted1 = rs1.Sort(minPosList1, nb1).GetRanks();

    uint index0, index1;

    const uint* const lastSorted0 = &sorted0[nb0];
    const uint* const lastSorted1 = &sorted1[nb1];
    const uint* runningAddress0   = sorted0;
    const uint* runningAddress1   = sorted1;

    while (runningAddress1<lastSorted1 && sorted0<lastSorted0)
    {
        index0 = *sorted0++;

        while (runningAddress1<lastSorted1 && minPosList1[*runningAddress1]<minPosList0[index0])    runningAddress1++;

        const uint* runningAddress2_1 = runningAddress1;

        while(runningAddress2_1<lastSorted1 && minPosList1[index1 = *runningAddress2_1++]<=list0[index0]->GetMax(axis0))
        {
            if(list0[index0]->Intersects(list1[index1], axis0) && list0[index0]->Intersects(list1[index1], axis1))
			{
                pairs.push_back(TPair(index0, index1));
            }
        }
    }

    while (runningAddress0<lastSorted0 && sorted1<lastSorted1)
    {
        index0 = *sorted1++;

        while (runningAddress0<lastSorted0 && minPosList0[*runningAddress0]<=minPosList1[index0])   runningAddress0++;

        const uint* runningAddress2_0 = runningAddress0;

        while (runningAddress2_0<lastSorted0 && minPosList0[index1 = *runningAddress2_0++]<=list1[index0]->GetMax(axis0))
        {
            if(list0[index1]->Intersects(list1[index0], axis0) && list0[index1]->Intersects(list1[index0], axis1))
			{
                pairs.push_back(TPair(index1, index0));
            }
        }
    }

    DELETEARRAY(minPosList0);
    DELETEARRAY(minPosList1);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  Returns overlapping pairs for with single rectangle. Sorter must be already sorted
 *	\param		sorter  [in] a radix sorter that is already sorted
 *	\param		queryBox[in] list of boxes
 *	\param		results	[out] list of overlapping elements
 *	\param		axes	[in] projection order
 *  \param      unsorted[in] is the list sorted already? Default is true.
 */ //////////////////////////////////////////////////////////////////////////////////////////////////
void SinglePruning(RadixSort& entities, CRect* queryBox, const vector<CRect*>& list,
                   vector<uint>& results, const TAxes& axes, bool unsorted)
{
    uint nb    = list.size();
    if(!nb)    return;
    
    // Catch axes
    uint axis0 = axes.Axis0;
    uint axis1 = axes.Axis1;
    
    // Temporary data
    float* posList = new float[nb];
    float qmin = queryBox->GetMin(axis0);
    float qmax = queryBox->GetMax(axis0);
    
    // Build main list using primary axis
    for (uint i=0; i<nb; ++i)   posList[i] = list[i]->GetMin(axis0);
    
     // Sort list
    if (unsorted)   entities.Sort(posList, nb);
    
    uint* sorted = entities.GetRanks();
    
    // Prune list
    const uint* const lastSorted = &sorted[nb];
    uint index0;

    while (sorted<lastSorted)
    {
        index0 = *sorted++;
        
        if (qmax<posList[index0]) continue;
        
        if (list[index0]->Intersects(queryBox, axis1) && list[index0]->Intersects(queryBox, axis0) )
        {
            results.push_back(index0);
        }
        else if (qmax<list[index0]->GetMin(axis0))   break;
            
    }
    
    DELETEARRAY(posList);
}
