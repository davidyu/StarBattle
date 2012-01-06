///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  Sweep and Prune: Box Variant - Broadphase Collision Detection
 *  Header for Star Battle
 *  \file       SAP.cpp
 *  \author     Fish
 *  \date       July 17, 2011
 *  \brief      Code based on work by Pierre Terdiman
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef SAPUTIL_H_
#define SAPUTIL_H_

// Headers
#include <vector>
#include <string>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// typedefs & defines
    typedef unsigned int		uint;       //!< sizeof(uword)	must be 2
    typedef int	                BOOL;       //!< Another boolean type
    typedef unsigned char		ubyte;		//!< sizeof(ubyte)	must be 1

    #ifndef FALSE
    #define FALSE               0
    #endif

    #ifndef TRUE
    #define TRUE                1
    #endif

    struct Pair
    {
        inline  Pair()                                       {}
        inline  Pair(uint i0, uint i1) : id0(i0), id1(i1)    {}

        uint    id0;    //!< First index of pair
        uint    id1;    //!< Second index of pair
    };


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward class declarations
class CSAP_Element;
class CSAP_EndPoint;
class CSAP_Box;
struct Pair;

using std::vector;


namespace SAP
{

    class CSAP_PairManager
    {
        public:
                                        CSAP_PairManager();
                                        ~CSAP_PairManager();

                bool			        Init(uint nb_objects);

                void                    AddPair(uint id1, uint id2);
                void                    RemovePair(uint id1, uint id2);

                void                    DumpPairs(vector<Pair>& pairs) const;

        private:


                uint                    nbElements_;     //!< Total # of elements in pool
                uint                    nbUsedElements_; //!< # of used elements
                CSAP_Element*           elementPool_;    //!< Array of elements
                CSAP_Element*           firstFree_;      //!< First free element in pool

                uint                    nbObjects_;      //!< Max number of objects we can handle
                CSAP_Element**          array_;          //!< Pointers to pool

            // Internal methods
                void                    FreeElem(CSAP_Element* elem);
                CSAP_Element*           GetFreeElem(uint id, CSAP_Element* next, uint* remap=0);
                void                    Release();
    };

//    class CSweepAndPrune
//    {
//        public:
//                                        CSweepAndPrune();
//                                        ~CSweepAndPrune();
//
//                bool                    Init(uint nbObj, const vector<CRect>& boxes);
//                bool                    UpdateObject(uint i, const const CRect& box);
//
//                void                    GetPairs(vector<uint> pairs)                    const;
//
//        private:
//                uint                    objCount_;
//                CSAP_PairManager        pairs_;
//                CSAP_Box*               boxes_;
//                CSAP_EndPoint*          listPoints_[2];
//
//            //internal methods
//                bool                    CheckListsIntegrity();
//    };
//
}

#endif //SAPUTIL_H_
