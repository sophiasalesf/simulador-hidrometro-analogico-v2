#include "medicao.hpp"

namespace hidrometro
{
Medicao::Medicao()
: totalDecilitros(0), residualLitros(0.0)
{}

void Medicao::acrescentarLitros(double litros)
{
    if (litros <= 0.0) 
        return;

    double dlTotal = litros * 10.0 + residualLitros * 10.0;

    unsigned long long dlInt = static_cast<unsigned long long>(dlTotal);
    totalDecilitros += dlInt;

    residualLitros = (dlTotal - dlInt) / 10.0;
}

void Medicao::reset()
{
    totalDecilitros = 0;
    residualLitros = 0.0;
}

unsigned long long Medicao::getTotalDecilitros() const
{
    return totalDecilitros;
}

unsigned long long Medicao::getTotalLitros() const
{
    return totalDecilitros / 10ULL;
}

double Medicao::getTotalM3() const
{
    const double dL = static_cast<double>(totalDecilitros) + residualLitros * 10.0;
    return dL / 10000.0;
}

std::array<int,6> Medicao::getDigitos() const
{
    unsigned long long dl = totalDecilitros;

    std::array<int,6> d = {0,0,0,0,0,0};

    d[5] = static_cast<int>(dl % 10ULL);
    dl /= 10ULL;

    unsigned long long L = dl % 10ULL;
    d[4] = static_cast<int>(L);
    dl /= 10ULL;

    d[3] = static_cast<int>(dl % 10ULL);
    dl /= 10ULL;

    // m3 dezena
    d[2] = static_cast<int>(dl % 10ULL);
    dl /= 10ULL;

    d[1] = static_cast<int>(dl % 10ULL);
    dl /= 10ULL;

    d[0] = static_cast<int>(dl % 10ULL);

    return d;
}

}