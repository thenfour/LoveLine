/*
    CBuffer is a wrapper for malloc() and free() basically.  This class is actually
    very useful and functional.  It provides a granularity system and assurance that
    the memory will be freed, as long as the class is freed.
*/



//#include "aberrors.h"
//#include <iostream>


// This buffer class also has a granularity mechanism so it will attempt to allocate more than needed
// to reduce the number of OS heap calls.
#define CBUFFER_DEFAULT_GRANULARITY  200
#define CONVENIENT_BUFFER_SIZE  8192


template<typename T>
class CCEXPORT CBuffer
{
public:
    CBuffer();
    ~CBuffer();

    bool AssignFrom(CBuffer* pSrc);
    bool AssignFrom(const T* p, size_t nElements);

    const T* buf();// returns 0 if nothing has been allocated
    T* Lock();// returns 0 if nothing has been allocated.  You can't lock the same object more than once.  It will return 0.
    bool Unlock();// returns false if someone else has it locked - ALWAYS CHECK THIS!
    bool SetGranularity(size_t n);

    bool Alloc(size_t nNewSize);
    bool Realloc(size_t nNewSize);
    bool Free();

    size_t GetSize();
    size_t GetElementSize();
    size_t GetByteSize(size_t nElements);

    T* locked_value;// for convenience.

private:
    //T m_OptBuf[MAX_PATH*2];// <-- since this will be alloc'ed right along with the CBuffer, then
    //                       // this optimizes all buffers below MAX_PATH size.

    // The "convenient buffer" is allocated right along with this object, so
    // up to CONVENIENT_BUFFER_SIZE, it saves any allocations.
    T m_pConvenientBuffer[CONVENIENT_BUFFER_SIZE];
    bool m_bConvenientBufferUsage;// are we currently using the convenient buffer?

    size_t m_nGranularity;
    bool m_bLocked;
    T* m_sz;// THE BUFFER
    size_t m_nSize;// Current size of the buffer.
};


template<typename T>
CBuffer<T>::CBuffer() :
    m_nGranularity(CBUFFER_DEFAULT_GRANULARITY),
    m_nSize(0),
    m_sz(0),
    m_bLocked(false),
    locked_value(0),
    m_bConvenientBufferUsage(true)
{
}

template<typename T>
CBuffer<T>::~CBuffer()
{
    Unlock();
    Free();
}

template<typename T>
bool CBuffer<T>::AssignFrom(CBuffer* pSrc)
{
    bool r = false;
    Free();

    m_nGranularity = pSrc->m_nGranularity;
    if(Realloc(pSrc->m_nSize))
    {
        m_bLocked = pSrc->m_bLocked;
        r = true;
    }

    return r;
}

template<typename T>
bool CBuffer<T>::AssignFrom(const T* p, size_t nElements)
{
    bool r = false;

    if(Realloc(nElements))
    {
        RtlCopyMemory(m_sz, p, GetByteSize(nElements));
    }

    return r;
}


template<typename T>
bool CBuffer<T>::SetGranularity(size_t n)
{
    m_nGranularity = n;
    return true;
}

template<typename T>
const T* CBuffer<T>::buf()
{
    return m_sz;
}

template<typename T>
T* CBuffer<T>::Lock()
{
    if(m_bLocked == true) return 0;

    // Allocate something if we need to.
    if(!m_sz)
    {
        Alloc(1);
        if(m_sz) m_sz[0] = 0;
    }

    locked_value = m_sz;
    m_bLocked = true;

    return m_sz;
}

template<typename T>
bool CBuffer<T>::Unlock()
{
    m_bLocked = false;
    locked_value = 0;
    return true;
}

template<typename T>
bool CBuffer<T>::Alloc(size_t nNewSize)
{
    if(m_bLocked == true) return false;
    if(nNewSize <= m_nSize)// determine if we need to allocate any more than we already have?
    {
        if(m_sz)// fake it into thinking we allocated.
        {
            m_sz[0] = 0;
            return true;
        }
    }

    // we are here because we know we need to allocate memory.

    // free our current buffer
    if(Free() == false) return false;

    // the actual amount to allocate.
    nNewSize = ((nNewSize/m_nGranularity)+1)*m_nGranularity;

    if(nNewSize > CONVENIENT_BUFFER_SIZE)
    {
        // allocate on the heap.
        m_sz = (T*)malloc(nNewSize*GetElementSize());
        m_bConvenientBufferUsage = false;

        if(!m_sz)
        {
            m_nSize = 0;
            return false;
        }
    }
    else
    {
        // allocate using the convenient buffer.
        m_sz = m_pConvenientBuffer;
        m_bConvenientBufferUsage = true;
    }

    m_sz[0] = 0;
    m_nSize = nNewSize;

    return true;
}

template<typename T>
bool CBuffer<T>::Realloc(size_t nNewSize)
{
    T* szNew = 0;

    if(m_bLocked == true) return false;
    if(nNewSize <= m_nSize) return true;

    if(!m_sz)
    {
        return Alloc(nNewSize);
    }

    nNewSize = ((nNewSize/m_nGranularity)+1)*m_nGranularity;

    if(m_bConvenientBufferUsage)
    {
        if(nNewSize > CONVENIENT_BUFFER_SIZE)
        {
            // stack->heap
            // they're using the convenient buffer and they're reallocing to larger than the buffer size.
            m_sz = (T*)malloc(nNewSize*GetElementSize());
            if(!szNew)
            {
                return false;
            }
            RtlCopyMemory(m_sz, m_pConvenientBuffer, GetByteSize(m_nSize));
            m_nSize = nNewSize;
        }
        else
        {
            // stack->stack
            // they're simply reallocating the convenent-buffer... do nothing.
            m_nSize = nNewSize;
        }
    }
    else
    {
        // normal reallocation (heap -> heap)
        szNew = (T*)realloc(m_sz, nNewSize*GetElementSize());
        if(!szNew)
        {
            return false;
        }
        m_sz = szNew;
        m_nSize = nNewSize;
    }

    return true;
}

template<typename T>
bool CBuffer<T>::Free()
{
    if(m_bLocked == true)
    {
        return false;
    }
    if(m_sz)
    {
        if(m_bConvenientBufferUsage == false)
        {
            free(m_sz);
        }

        m_sz = 0;
        m_nSize = 0;
    }
    return true;
}


template<typename T>
size_t CBuffer<T>::GetSize()
{
    return m_nSize;
}


template<typename T>
size_t CBuffer<T>::GetElementSize()
{
    return sizeof(T);
}


template<typename T>
size_t CBuffer<T>::GetByteSize(size_t nElements)
{
    return nElements*GetElementSize();
}

