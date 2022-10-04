// Copyright (c) 2008-2011 Yannick Tapsoba.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "renderbliss/Macros.h"

namespace renderbliss
{
inline PropertyMap::PropertyMap()
{
}

inline bool PropertyMap::Find(const char* name) const
{
    return properties.find(std::string(name)) != properties.end();
}

inline bool PropertyMap::Find(const std::string& name) const
{
    return properties.find(name) != properties.end();
}

template <typename T>
void PropertyMap::Get(const char* name, const T& defaultValue, T& out) const
{
    Get(std::string(name), defaultValue, out);
}

template <typename T>
void PropertyMap::Get(const std::string& name, const T& defaultValue, T& out) const
{
    std::map<std::string, boost::any>::const_iterator propIter = properties.find(name);
    if (propIter != properties.end())
    {
        try
        {
            out = boost::any_cast<const T&>(propIter->second);
        }
        catch (boost::bad_any_cast&)
        {
            out = defaultValue;
        }
    }
    else
    {
        out = defaultValue;
    }
}

template <typename T>
void PropertyMap::Set(const char* name, const T& value)
{
    if (name)
    {
        Set(std::string(name), value);
    }
}

template <typename T>
void PropertyMap::Set(const std::string& name, const T& value)
{
    if (!name.empty())
    {
        properties[name] = value;
    }
}

inline void PropertyMap::SetFrom(const PropertyMap& p)
{
    for (std::map<std::string, boost::any>::const_iterator propIter = p.properties.begin();
         propIter != p.properties.end();
         ++propIter)
    {
        properties[propIter->first] = propIter->second;
    }
}
}
