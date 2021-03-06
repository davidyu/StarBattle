////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  Contains a pair class and the includes to the SAP procedure
 *  \file       SAPHelper.h
 *  \author     Fish
 *  \date       July 13, 2011
 *  \brief
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef SAPHELPER_H_
#define SAPHELPER_H_

// Headers
#include "SAP.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //! Generic couple structure
    struct Pair
    {
        inline  Pair()                                       {}
        inline  Pair(uint i0, uint i1) : id0(i0), id1(i1)    {}

        uint    id0;    //!< First index of pair
        uint    id1;    //!< Second index of pair
    };




#endif //SAPHELPER_H_
