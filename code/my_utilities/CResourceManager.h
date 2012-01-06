///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	\file		CResourceManager.cpp
 *	\author		Fish
 *	\date		Jul 27, 2011
 *  \brief      Manages all images, sounds for game.
 *              Primitive stuff... no handles or anything. Just a fancy container
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include
#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

// Headers
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class CResourceManager
{
    public:
                                        CResourceManager();
    virtual                             ~CResourceManager();

            bool                        RegisterImage(string fileName, string key);
            sf::Image*                  AccessImage(string key);
    protected:
            map<string, sf::Image>      images_;


};

#endif // CRESOURCEMANAGER_H
