#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <vector>
#include <string>
#include "hidrometro.hpp"

namespace hidrometro
{

class Display
{
public:
    Display();

    void setBaseImage(const std::string& path);

    const std::vector<unsigned char>& renderEstado(const Hidrometro& h, int& w, int& hgt, int& comp);

protected:
    std::vector<unsigned char> buffer;   
    std::vector<unsigned char> base;     
    int baseW, baseH;  
};

}

#endif
