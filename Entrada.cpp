#include "Entrada.hpp"
#include <stdexcept>

using std::invalid_argument;

namespace hidrometro
{

Entrada::Entrada(double bitola, double vazao, SentidoFluxo sentido, bool arPresente, double arPercentual)
{
    entradaFluxo(bitola, vazao, sentido);
    definirAr(arPresente, arPercentual);
}

void Entrada::entradaFluxo(double novaBitola, double vazaoLpm, SentidoFluxo novoSentido)
{
    if (novaBitola < 0.0) 
        throw invalid_argument("bitola deve ser >= 0");
    if (vazaoLpm  < 0.0) 
        throw invalid_argument("vazao deve ser >= 0 (L/min)");
    bitola  = novaBitola;
    vazao   = vazaoLpm;
    sentido = novoSentido;
}

void Entrada::definirAr(bool presente, double percentual)
{
    arPresente = presente;
    if (!presente)
    {
        arPercentual = 0.0;
        return;
    }
    if (percentual < 0.0)       
        arPercentual = 0.0;
    else if (percentual > 1.0)  
        arPercentual = 1.0;
    else                        
        arPercentual = percentual;
}

double Entrada::getBitola() const 
{ 
    return bitola; 
}
double Entrada::getVazao() const 
{ 
    return vazao; 
}
double Entrada::getArPercentual() const 
{ 
    return arPercentual; 
}
bool   Entrada::getArPresente() const 
{ 
    return arPresente; 
}
SentidoFluxo Entrada::getSentido() const 
{ 
    return sentido; 
}

}