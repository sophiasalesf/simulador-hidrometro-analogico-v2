#ifndef MEDICAO_HPP
#define MEDICAO_HPP

#include <array>

namespace hidrometro
{
class Medicao
{
public:
    Medicao();

    void acrescentarLitros(double litros); 
    void reset();

    unsigned long long getTotalDecilitros() const;
    unsigned long long getTotalLitros() const; 
    double getTotalM3() const;                
    std::array<int,6> getDigitos() const;      

protected:
    unsigned long long totalDecilitros; 
    double residualLitros;              
};
}

#endif
