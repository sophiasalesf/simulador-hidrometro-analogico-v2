#include "Hidrometro.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

using std::invalid_argument;

namespace hidrometro
{


Hidrometro::Hidrometro() : input_(), medicao_() 
{}


void Hidrometro::setEntrada(const Entrada& e) { input_ = e; }
const Entrada& Hidrometro::getEntrada() const { return input_; }

void Hidrometro::medir(double dtSeg)
{
    if (dtSeg <= 0.0) 
        throw invalid_argument("medir: dtSeg deve ser > 0");

    if (input_.getSentido() != SentidoFluxo::Direto) 
        return;

    const double vazaoLpm = input_.getVazao();

    double fracAr = 0.0;
    if (input_.getArPresente())
    {
        fracAr = input_.getArPercentual();
        if (fracAr < 0.0) fracAr = 0.0;
        if (fracAr > 1.0) fracAr = 1.0;
    }

    const double vazaoLiquidaLpm   = vazaoLpm * (1.0 - fracAr);
    const double litrosNoIntervalo = vazaoLiquidaLpm * (dtSeg / 60.0);

    
    if (litrosNoIntervalo > 0.0)
    {
        medicao_.acrescentarLitros(litrosNoIntervalo);
    }
    
}

const Medicao& Hidrometro::getMedicao() const { return medicao_; }


std::string Hidrometro::apresentacaoMedicao() const
{
    const auto d = medicao_.getDigitos();

    const int m3Milhar  = d[0];
    const int m3Centena = d[1];
    const int m3Dezena  = d[2];
    const int m3Unidade = d[3];
    const int litroUnid = d[4]; 
    const int decilitro = d[5]; 

    std::ostringstream texto;
    texto << "odometro:[" << m3Milhar << m3Centena << m3Dezena << m3Unidade
          << "] L=" << litroUnid << " dL=" << decilitro
          << " total_m3=" << std::fixed << std::setprecision(4) << medicao_.getTotalM3();

    return texto.str();
}


}