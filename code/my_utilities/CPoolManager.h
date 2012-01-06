#ifndef CPOOLMANAGER_H
#define CPOOLMANAGER_H

#include "memory_macros.h"
#include "misc_helper.h"

template <class T>
class CPoolManager
{
    public:
                        CPoolManager() : pool_(0), first_free_(0),
                                            pool_size_(0), nb_used_items_(0) {}
                                         
    virtual             ~CPoolManager()
            {
                first_free_         = 0;
                pool_size_          = 0;
                nb_used_items_      = 0;
                DELETEARRAY(pool_);
            }
                                
            void        Init(uint sz)
            {
                pool_ = new T[sz];
                for (uint i=0; i<sz; ++i)   pool_[i] = T();
                
                first_free_ = &pool_[0];
                for (uint i=0; i<sz-1; ++i) pool_[i].SetNext(&pool_[i+1]);
                
                pool_[sz-1].SetNext(0);
                pool_size_ = sz;
                
            }
                        
            T*          CheckoutItem()
            {
                if (first_free_)
                {
                    T* prev_first = first_free_;
                    first_free_ = first_free_->GetNext();
                    ++nb_used_items_;
                    return prev_first;
                }
                return 0;
            }
            void        CheckinItem(T* item)
            {
                item->SetNext(first_free_);
                first_free_ = item;
                --nb_used_items_;
            }
                    
    private:
            T*            pool_;
            T*            first_free_;
            
            uint          pool_size_;
            uint          nb_used_items_;
};

#endif // CPOOLMANAGER_H
