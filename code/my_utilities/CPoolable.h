#ifndef CPOOLABLE_H
#define CPOOLABLE_H

template <class T>
class CPoolable
{
    public:
                            CPoolable<T>() {}
                            CPoolable<T>(const CPoolable<T>& c) { next_ = c.next_; }
    virtual                 ~CPoolable() {}

            T*&             GetNext() { return next_; }
            void            SetNext(T* n)   { next_ = n;    }
    protected:
            T*              next_;


};

#endif // CPOOLABLE_H
