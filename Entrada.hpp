#ifndef ENTRADA_HPP
#define ENTRADA_HPP

#include "SentidoFluxo.hpp"

namespace hidrometro
{

class Entrada
{
public:
    Entrada(double bitola = 0.0, double vazao = 0.0, SentidoFluxo sentido = SentidoFluxo::Direto, bool arPresente = false, double arPercentual = 0.0);

    void entradaFluxo(double bitola, double vazao, SentidoFluxo sentido);
    void definirAr(bool arPresente, double arPercentual);

    double getBitola() const;
    double getVazao() const;
    double getArPercentual() const;
    bool getArPresente() const;
    SentidoFluxo getSentido() const;

protected:
    double bitola;
    double vazao;
    SentidoFluxo sentido;
    bool arPresente;
    double arPercentual;
};

}

#endif
