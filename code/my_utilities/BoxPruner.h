///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for box pruning.
 *	\file		BoxPruner.h
 *	\author		Fish
 *	\date		Jul 18, 2011
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef BOXPRUNER_H
#define BOXPRUNER_H

// Headers
#include "RadixSort.h"
#include "CRect.h"
#include "mytypes.h"
#include <vector>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using std::vector;
using std::pair;

class TPair;

/**
 *  Complete box pruning. Returns a list of overlapping pairs of boxes,
 *  each box of the pair belongs to the same set.
 *	\param		sorter  [in] a radix sorter that computes increasing values
 *	\param		list	[in] list of boxes
 *	\param		pairs	[out] list of overlapping pairs
 *	\param		axes	[in] projection order
 *	\return		true if success.
 */
bool CompletePruning(RadixSort& sorter, const vector<CRect*>& list,
                     vector<TPair>& pairs, const TAxes& axes);
                    
/**
 *  Bipartite version of box pruning
 *	\param		sorter  [in] a radix sorter that computes increasing values
 *	\param		list0	[in] list of boxes
 *  \param		list1	[in] list of boxes
 *	\param		pairs	[out] list of overlapping pairs
 *	\param		axes	[in] projection order
 *	\return		true if success.
 */
bool BipartitePruning(RadixSort& rs0, RadixSort& rs1, const vector<CRect*>& list0,
                      const vector<CRect*>& list1, vector<TPair>& pairs, const TAxes& axes);


/**
 *  Returns overlapping pairs for with single rectangle. Sorter must be already sorted
 *	\param		sorter  [in] a radix sorter that is already sorted
 *	\param		queryBox[in] list of boxes
 *	\param		results	[out] list of overlapping elements
 *	\param		axes	[in] projection order
 */
void SinglePruning(RadixSort& entities, CRect* queryBox, const vector<CRect*>& list,
                   vector<uint>& results, const TAxes& axes, bool unsorted=false);  


#endif // BOXPRUNER_H
