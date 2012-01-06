#include "../misc_helper.h"
#include "../CResourceManager.h"

#include <iostream>

using std::pair;

CResourceManager::CResourceManager()
{
    //ctor
}

CResourceManager::~CResourceManager()
{
    //dtor
}

bool CResourceManager::RegisterImage(string fileName, string key)
{
    sf::Image temp;

    if (!temp.LoadFromFile(fileName))
    {
        std::cout << "IMAGE ERROR\n";
        return false;
    }

    images_.insert(pair<string, sf::Image>(miscH::ToLower(key), temp));

    return true;
}

sf::Image* CResourceManager::AccessImage(string key)
{
    key = miscH::ToLower(key);
    if (images_.find(key)==images_.end())   return 0;

    return &images_[key];
}
