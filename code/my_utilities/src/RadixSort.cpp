///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains source code from the article "Radix Sort Revisited".
 *	\file		RadixSort.cpp
 *	\author		Fish
 *	\date		July 17, 2011
 *  \brief      Based on Pierre Terdiman's IceRevisitedRadix
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Headers
#include "../RadixSort.h"
#include "../memory_macros.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Some macros
#define INVALIDATE_RANKS	currentSize_|=0x80000000        // current size or equals with 0s
#define VALIDATE_RANKS		currentSize_&=0x7fffffff
#define CURRENT_SIZE		(currentSize_&0x7fffffff)       // current size bit-and with 1111s
#define INVALID_RANKS		(currentSize_&0x80000000)

#define CREATE_HISTOGRAMS(type, buffer)                                         \
    ZeroMemory(histogram_, 256*4*sizeof(uint));                                 \
    ubyte* p   = (ubyte*)input;                                                 \
    ubyte* pe  = &p[nb*4];                                                      \
    uint*  h0  = &histogram_[0];                                                \
    uint*  h1  = &histogram_[256];                                              \
    uint*  h2  = &histogram_[512];                                              \
    uint*  h3  = &histogram_[768];                                              \
    bool alreadySorted = true;                                                  \
    if (INVALID_RANKS)                                                          \
    {                                                                           \
        type* running = (type*)buffer;                                          \
        type  prevVal = *running;                                               \
        while(p!=pe)                                                            \
        {                                                                       \
            type val = *running++;                                              \
            if (val<prevVal)    { alreadySorted = false; break; }               \
            prevVal = val;                                                      \
            h0[*p++]++;                                                         \
            h1[*p++]++;                                                         \
            h2[*p++]++;                                                         \
            h3[*p++]++;                                                         \
        }                                                                       \
        if (alreadySorted)                                                      \
        {                                                                       \
            for (uint i=0; i<nb; ++i)   ranks_[i] = i;                          \
            return *this;                                                       \
        }                                                                       \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        uint* indicies = ranks_;                                                \
        type prevVal = (type)buffer[*indicies];                                 \
        while (p!=pe)                                                           \
        {                                                                       \
            type val = (type)buffer[*indicies++];                               \
            if (val<prevVal)    { alreadySorted = false; break; }               \
            prevVal = val;                                                      \
            h0[*p++]++;                                                         \
            h1[*p++]++;                                                         \
            h2[*p++]++;                                                         \
            h3[*p++]++;                                                         \
        }                                                                       \
        if (alreadySorted)  { return *this; }                                   \
    }                                                                           \
    while (p!=pe)                                                               \
    {                                                                           \
        h0[*p++]++;                                                             \
        h1[*p++]++;                                                             \
        h2[*p++]++;                                                             \
        h3[*p++]++;                                                             \
    }

// If all values have same byte, no need to sort
#define CHECK_PASS_VALIDITY(pass)                                               \
    uint *curCount = &histogram_[pass<<8];                                      \
    bool PerformPass = true;                                                    \
    ubyte uniqueVal = *(((ubyte*)input)+pass);                                  \
    if (curCount[uniqueVal]==nb)    PerformPass = false;

// Constructor
RadixSort::RadixSort() :
    currentSize_(0),
    ranks_(0),
    ranks2_(0),
    totalCalls_(0)
{
    // Allocate input-independent ram
//    histogram_ = new uint[256*4];
//    offset_    = new uint[256];

    // Initialize indicies
    INVALIDATE_RANKS;
}

// Destructor
RadixSort::~RadixSort()
{
//    DELETEARRAY(offset_);
//    DELETEARRAY(histogram_);
    DELETEARRAY(ranks2_);
    DELETEARRAY(ranks_);
}

// Resizes inner lists
bool RadixSort::Resize(uint nb)
{
    // Free previous ram
    DELETEARRAY(ranks_);
    DELETEARRAY(ranks2_);

    // Get fresh ones
    ranks_  = new uint[nb];     // Should check allocation here later
    ranks2_ = new uint[nb];

    return true;
}

inline void RadixSort::CheckResize(uint nb)
{
    uint curSize = CURRENT_SIZE;
    if (nb!=curSize)
    {
        if(nb>curSize)  Resize(nb);

        currentSize_ = nb;
        INVALIDATE_RANKS;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main sort routine.
 *	This one is for integer values. After the call, ranks_ contains a list of indices in sorted order, i.e. in the order you may process your data.
 *	\param		input	[in] a list of integer values to sort
 *	\param		nb		[in] number of values to sort, must be < 2^31
 *	\param		hint	[in] RADIX_SIGNED to handle negative values, RADIX_UNSIGNED if you know your input buffer only contains positive values
 *	\return		Self-Reference
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RadixSort& RadixSort::Sort(const uint* input, uint nb, RadixHint hint)
{
    // Checkings
    if (!input || !nb || nb&0x80000000) return *this;

    // Stats
    totalCalls_++;

    CheckResize(nb);

    uint histogram_[256*4];
    uint* link[256];

    // Create histograms
    if (hint==RADIX_UNSIGNED)   { CREATE_HISTOGRAMS(uint, input); }
    else                        { CREATE_HISTOGRAMS(int,  input); }

    uint nbNegValues = 0;
    if (hint==RADIX_SIGNED)
    {
        uint* h3 = &histogram_[768];
        for (uint i=128; i<256; ++i)    nbNegValues += h3[i];   // 768 for last histogram, 128 for negative part
    }

    // Radix sort, j is pass number (0=LSB, 3=MSB)
    for (uint j=0; j<4; ++j)
    {
        CHECK_PASS_VALIDITY(j)
        if (PerformPass)
        {
            if(j!=3 || hint==RADIX_UNSIGNED)    // Handle positive values
            {
                link[0] = ranks2_;
                for (uint i=1; i<256; ++i)  link[i] = link[i-1] + curCount[i-1];
            }
            else                                // Handle negative integers, sorted in right order but wrong place
            {
               link[0] = &ranks2_[nbNegValues];
               for (uint i=1; i<128; ++i)   link[i] = link[i-1] + curCount[i-1];

               // Fix the wrong place
               link[128] = ranks2_;
               for (uint i=129; i<256; ++i) link[i] = link[i-1] + curCount[i-1];
            }

            ubyte* inputBytes = (ubyte*)input;
            inputBytes += j;
            if (INVALID_RANKS)
            {
                for (uint i=0; i<nb; ++i)   *link[inputBytes[i<<2]]++ = i;
                VALIDATE_RANKS;
            }
            else
            {
                uint* indicies      = ranks_;
                uint* indiciesEnd   = &ranks_[nb];

                while (indicies!=indiciesEnd)
                {
                    uint id = *indicies++;

                    *link[inputBytes[id<<2]]++ = id;
                }
            }

            // Swap pointers for next pass
            uint* tmp = ranks_;
            ranks_ = ranks2_;
            ranks2_ = tmp;
        }
    }

    return *this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Main sort routine.
 *	This one is for floating-point values. After the call, mRanks contains a list of indices in sorted order, i.e. in the order you may process your data.
 *	\param		input			[in] a list of floating-point values to sort
 *	\param		nb				[in] number of values to sort, must be < 2^31
 *	\return		Self-Reference
 *	\warning	only sorts IEEE floating-point values
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RadixSort& RadixSort::Sort(const float* input2, uint nb)
{
    if (!input2 || !nb || nb&0x80000000)   return *this;

    totalCalls_++;

    uint* input = (uint*)input2;

    CheckResize(nb);

    uint histogram_[256*4];
    uint* link[256];

    { CREATE_HISTOGRAMS(float, input2); }

    uint nbNegValues = 0;
    uint* h3 = &histogram_[768];

    for (uint i=128; i<256; ++i)    nbNegValues += h3[i];

    for (uint j=0; j<4; ++j)
    {
        if (j!=3)
        {
            CHECK_PASS_VALIDITY(j);

            if(PerformPass)
            {
                link[0] = ranks2_;
                for (uint i=1; i<256; ++i)  link[i] = link[i-1] + curCount[i-1];

                ubyte* inputBytes = (ubyte*)input;
                inputBytes += j;

                if(INVALID_RANKS)
                {
                    for (uint i=0; i<nb; ++i)   *link[inputBytes[i<<2]]++ = i;

                    VALIDATE_RANKS;
                }
                else
                {
                    uint* indicies      = ranks_;
                    uint* indiciesEnd   = &ranks_[nb];

                    while(indicies!=indiciesEnd)
                    {
                        uint id = *indicies++;
                        *link[inputBytes[id<<2]]++ = id;
                    }
                }

                uint* tmp = ranks_; ranks_ = ranks2_; ranks2_ = tmp;
            }
        }
        else
        {
            CHECK_PASS_VALIDITY(j);

            if (PerformPass)
            {
                link[0] = &ranks2_[nbNegValues];

                for(uint i=1; i<128; ++i)   link[i] = link[i-1] + curCount[i-1];

                link[255] = ranks2_;
                for(uint i=0; i<127; ++i)   link[254-i] = link[255-i] + curCount[255-i];
                for(uint i=128; i<256; ++i) link[i] += curCount[i];

                if(INVALID_RANKS)
                {
                    for (uint i=0; i<nb; ++i)
                    {
                        uint Radix = input[i]>>24;

                        if (Radix<128)  *link[Radix]++ = i;
                        else            *(--link[Radix])  = i;
                    }

                    VALIDATE_RANKS;
                }
                else
                {
                    for (uint i=0; i<nb; ++i)
                    {
                        uint Radix = input[ranks_[i]]>>24;

                        if (Radix<128)  *link[Radix]++ = ranks_[i];
                        else            *(--link[Radix])  = ranks_[i];
                    }
                }

                uint* tmp = ranks_; ranks_ = ranks2_; ranks2_ = tmp;
            }
            else
            {
                if (uniqueVal>=128)
                {
                    if (INVALID_RANKS)
                    {
                        for (uint i=0; i<nb; ++i)   ranks2_[i] = nb-i-1;

                        VALIDATE_RANKS;
                    }
                    else
                    {
                        for (uint i=0; i<nb; ++i)   ranks2_[i] = ranks_[nb-i-1];
                    }

                    uint* tmp = ranks_; ranks_ = ranks2_; ranks2_ = tmp;
                }
            }
        }

    }

    return *this;
}



