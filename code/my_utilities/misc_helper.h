#ifndef MISCHELPER_H
#define MISCHELPER_H

#include <string>
#include <algorithm>
#include <map>
#include <vector>

namespace miscH
{

    using namespace std;

    template<class A, class B>
    struct DeleteMapFntor
    {
        // Overloaded () operator.
        // This will be called by for_each() function.
        bool operator()(pair<A,B> x) const
        {
            // Assuming the second item of map is to be
            // deleted. Change as you wish.
            delete x.second;
            return true;
        }
    };

    template <class C> void FreeClear(C& cntr)
    {
        for (typename C::iterator it = cntr.begin(); it != cntr.end(); ++it)
        {
            delete *it;
        }
        cntr.clear();
    }

    template <class C, class D> bool VectorFind(C& v, D& tofind)
    {
        typename C::iterator it;
        if (find(v.begin(), v.end(), tofind) != v.end())
            return true;

        return false;
    }

    template<class key, class val> void DeleteMap(map<key, val*>& m)
    {
        for_each(m.begin(), m.end(), DeleteMapFntor<key,val*>());
        m.clear();
    }

    inline string ToLower(string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);

        return str;
    }


}

#endif //MISCHELPER_H
