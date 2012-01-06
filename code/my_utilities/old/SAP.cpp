///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  Sweep and Prune: Box Variant - Broadphase Collision Detection
 *  Implementation for Star Battle
 *  \file       SAP.cpp
 *  \author     Fish
 *  \date       July 17, 2011
 *  \brief      Code based on work by Pierre Terdiman
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
//#include "SAPHelper.h"
#include "SAP.h"
#include "memory_macros.h"
#include <algorithm>
#include <cassert>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


using namespace SAP;

inline void Sort(uint& id0, uint& id1)
{
    if(id0>id1) std::swap(id0, id1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \class       CSAP_Element
 * \brief       Used for managing entires in CSAP_PairManager
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSAP_Element
    {
        public:
                uint			mID;        //!< Index of the element
                CSAP_Element*	mNext;      //!< Pointer to the next entry in the PairManager array

            inline	CSAP_Element()											    {}
            inline	CSAP_Element(uint i, CSAP_Element* n) : mID(i), mNext(n)    {}
            inline 	~CSAP_Element()												{}
    };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  \class      CSAP_EndPoint
 *  \brief      Used for storing the max and min bounds of an object along a certain axis
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSAP_EndPoint
    {
        public:
                CSAP_EndPoint*      mPrev;          //previous endpoint that has value larger than this one's value or null
                CSAP_EndPoint*      mNext;          //next endpoint that has value larger than this one's value or null
                uint                mData;          //index of this point in array | last bit is 0 or 1 indicating a min or max value
                float               mValue;         //the value of this min/max element

            inline	void			SetData(uint box_id, BOOL is_max)		    { mData = (box_id<<1)|is_max;	} // mind
            inline	BOOL			IsMax()								const	{ return mData & 1;				} // =
            inline	uint			GetBoxID()							const	{ return mData>>1;				} // blown

            inline	void InsertAfter(CSAP_EndPoint* element)
            {
                if(this!=element && this!=element->mPrev)
                {
                    // Remove
                    if(mPrev)    mPrev->mNext = mNext; //join mPrevious point with current mNext point
                    if(mNext)	mNext->mPrev = mPrev; //join mNext's mPrev with this points's mPrev

                    // Insert
                    mNext = element->mNext;          //perform similar linking
                    if(mNext)	mNext->mPrev = this;

                    // Relink
                    element->mNext = this;
                    mPrev = element;
                }
            }

            inline	void InsertBefore(CSAP_EndPoint* element)
            {
                if(this!=element && this!=element->mPrev)
                {
                    // Remove
                    if(mPrev)    mPrev->mNext = mNext; //join mPrevious point with current mNext point
                    if(mNext)	mNext->mPrev = mPrev; //join mNext's mPrev with this points's mPrev

                    // Insert
                    mPrev = element->mPrev;          //perform similar linking
                    if(mPrev)	mPrev->mNext = this;

                    // Relink
                    element->mPrev = this;
                    mNext = element;
                }
            }

    };

    class CSAP_BOX
    {
        public:
                CSAP_EndPoint*	min[2];
                CSAP_EndPoint*	max[2];
    };



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  CSAP_PairManager Implementation
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constructor
CSAP_PairManager::CSAP_PairManager() :
    nbElements_(0),
    nbUsedElements_(0),
    elementPool_(0),
    firstFree_(0),
    nbObjects_(0),
    array_(0)
{
}

// Destructor
CSAP_PairManager::~CSAP_PairManager()
{
    Release();
}

// Performs cleanup - No dynamic memory allocated by client so should be ok
void CSAP_PairManager::Release()
{
    nbElements_          = 0;
    nbUsedElements_      = 0;
    nbObjects_           = 0;
    DELETEARRAY(elementPool_);
    DELETEARRAY(array_);
}


// Allocates some space in array_
bool CSAP_PairManager::Init(uint nb_objs)
{
    Release();
    if (!nb_objs) return false;

    array_ = new CSAP_Element*[nb_objs];                // Allocate new array of pointers
    ZeroMemory(array_, nb_objs*sizeof(CSAP_Element*));  // Initialize all memory allocated to zero
    nbObjects_ = nb_objs;

}

inline void Remap(CSAP_Element*& element, uint delta)
{
    if (element) element = (CSAP_Element*)(uint(element) + delta);
}

// Inserts an element into the pool and returns address
CSAP_Element* CSAP_PairManager::GetFreeElem(uint id, CSAP_Element* next, uint* remap)
{
    if (remap)  *remap = 0;

    CSAP_Element* freeElem;

    if (firstFree_) // If there is a free slot
    {
        freeElem = firstFree_;
        firstFree_= firstFree_->mNext;
    }
    else          // No free slots
    {
        if (nbUsedElements_==nbElements_)
        {
            //resize nbElements by factor of 2
            nbElements_ = nbElements_ ? (nbElements_<<1) : 2;

            CSAP_Element* newElems = new CSAP_Element[nbElements_];

            if (nbUsedElements_) CopyMemory(newElems, elementPool_, nbUsedElements_*sizeof(CSAP_Element));

            // Remap everything
            uint delta = uint(newElems) - uint(elementPool_);

            for (uint i=0; i<nbUsedElements_; ++i)  Remap(newElems[i].mNext, delta);
            for (uint i=0; i<nbUsedElements_; ++i)  Remap(array_[i], delta);

            Remap(firstFree_, delta);
            Remap(next, delta);

            if (remap)  *remap = delta;

            DELETEARRAY(elementPool_);
            elementPool_ = newElems;
        }

        freeElem = &elementPool_[nbUsedElements_++];
    }

    freeElem->mID = id;
    freeElem->mNext = next;

    return freeElem;
}

// Flags an element in element pool as overwritable
void CSAP_PairManager::FreeElem(CSAP_Element* elem)
{
    elem->mNext = firstFree_;   // Next free
    firstFree_  = elem;
}

// Adds a pair to the set
void CSAP_PairManager::AddPair(uint id1, uint id2)
{
    // Make id1 the lower of the two
    Sort(id1, id2);

    assert(id1<nbObjects_);
    if (id1>=nbObjects_) return;

    // Delect appropriate list from array_
    CSAP_Element* current = array_[id1];

    if (!current)
    {
        // If this is empty slot create the new element
        array_[id1] = GetFreeElem(id2, 0);
    }
    else if (current->mID > id2)
    {
        // If all the elements are greater that id2, insert id2 at fromt
        array_[id1] = GetFreeElem(id2, array_[id1]);
    }
    else
    {
        // Find correct location in sorted (ascending) list to insert id2
        while (current->mNext)
        {
            if (current->mNext->mID > id2) break;

            current = current->mNext;
        }

        if (current->mID==id2) return; // Pair already exists

        uint delta;
        CSAP_Element* e = GetFreeElem(id2, current->mNext, &delta);

        if(delta)   Remap(current, delta);  // Fix address of current if the elementPool_ was resized

        current->mNext = e;
    }
}

// Delete a pair from the set
void CSAP_PairManager::RemovePair(uint id1, uint id2)
{
    // Make id1 the lower of the two
    Sort(id1, id2);

    if (id1>=nbObjects_) return;

    // Otherwis,e select correct list
    CSAP_Element* current = array_[id1];

    // If this list is empty, pair dne
    if (!current) return;

    // If id2 is first element in list, delete it
    if (current->mID==id2)
    {
        array_[id1] = current->mNext;
        FreeElem(current);
    }
    else
    {
        // If id2 is not first element, search the sorted list
        while (current->mNext)
        {
            // We have passed all potential candidates. Pair dne
            if (current->mNext->mID > id2) return;

            // If pair found, delete id2
            if (current->mNext->mID==id2)
            {
                CSAP_Element* temp = current->mNext;
                current->mNext = temp->mNext;
                FreeElem(temp);
                return;
            }

            current = current->mNext;
        }
    }
}

// Get all pairs
void CSAP_PairManager::DumpPairs(std::vector<Pair>& pairs) const
{
    for (uint i=0; i<nbObjects_; ++i)
    {
        CSAP_Element* current = array_[i];

        while(current)
        {
            assert(current->mID<nbObjects_);

            pairs.push_back(Pair(i, current->mID));
            current = current->mNext;

        }
    }
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  CSAP_SweepAndPrune Implementation
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//// Constructor
//CSweepAndPrune::CSweepAndPrune()
//{
//}
//
//// Destructor
//CSweepAndPrune::~CSweepAndPrune()
//{
//}
//
//// Initializes members
//bool CSweepAndPrune::Init(uint nbObj, const vector<CRect>& boxes)
//{
//    nbObjects_ = nbObjects;
//
//    float* data = new float[nbObj*2];
//
//    for (uint i=0; i<nbObj;++i)
//    {
//        data[i*20] = boxes[i]
//    }
//    return true;
//}
//
//bool CSweepAndPrune::UpdateObject(uint i, const const CRect& box)
//{
//
//}
//
//void CSweepAndPrune::GetPairs(vector<uint> pairs) const;
