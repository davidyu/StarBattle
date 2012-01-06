///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains all memory macros.
 *	\file		memory_macros.h
 *	\author		Fish
 *	\date		July 17, 2011
 *  \brief      Based on Pierre Terdiman's IceMemoryMacros
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef MEMORY_MACROS_H
#define MEMORY_MACROS_H

// Headers
#include <string.h>

#undef ZeroMemory
#undef CopyMemory
#undef MoveMemory
#undef FillMemory


	//!	\fn         Clears a buffer.
	//!	\param		addr	[in] buffer address
	//!	\param		size	[in] buffer length
	inline void ZeroMemory(void* addr, uint size)					    { memset(addr, 0, size);		}

	//!	\fn         Fills a buffer with a given byte.
	//!	\param		addr	[in] buffer address
	//!	\param		size	[in] buffer length
	//!	\param		val		[in] the byte value
	inline void FillMemory(void* dest, uint size, ubyte val)			{ memset(dest, val, size);		}

	//! \fn         Copies a buffer.
	//!	\param		addr	[in] destination buffer address
	//!	\param		addr	[in] source buffer address
	//!	\param		size	[in] buffer length
	inline void CopyMemory(void* dest, const void* src, uint size)	    { memcpy(dest, src, size);		}

	//!	\fn         Moves a buffer.
	//!	\param		addr	[in] destination buffer address
	//!	\param		addr	[in] source buffer address
	//!	\param		size	[in] buffer length
	inline void MoveMemory(void* dest, const void* src, uint size)	    { memmove(dest, src, size);		}

    #define DELETESINGLE(x)		if (x) { delete x;      x = 0; }		//!< Deletes an instance of a class.
	#define DELETEARRAY(x)		if (x) { delete []x;    x = 0; }		//!< Deletes an array.


#endif // MEMORY_MACROS_H
