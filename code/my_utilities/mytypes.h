////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  Contains a pair class and the includes to the SAP procedure
 *  \file       mytypes.h
 *  \author     Fish
 *  \date       July 13, 2011
 *  \brief
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef MYTYPES_H
#define MYTYPES_H
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// typedefs & defines


    typedef unsigned int		uint;       //!< sizeof(uword)	must be 2
    typedef int	                BOOL;       //!< Another boolean type
    typedef unsigned char		ubyte;		//!< sizeof(ubyte)	must be 1

    struct TPair
    {
        inline          TPair()                                       {}
        inline          TPair(uint i0, uint i1) : id0(i0), id1(i1)    {}

        uint   id0;    //!< First index of pair
        uint   id1;    //!< Second index of pair
    };



    #ifndef FALSE
    #define FALSE               0
    #endif

    #ifndef TRUE
    #define TRUE                1
    #endif

    struct TAxes
    {
        uint Axis0; // X axis
        uint Axis1; // Y axis
    };




#endif //MYTYPES_H
