#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <memory>
#include "Configuracao.hpp"
#include "Hidrometro.hpp"
#include "Entrada.hpp"
#include "Saida.hpp"

namespace hidrometro
{
class Controlador
{
public:
    Controlador();

    void configurarTempo(double tempoTotalSeg, double dtSeg);
    void configurarEntrada(const Entrada& e);
    void configurarComConfiguracao(const Configuracao& conf);

    void executar();
    void executarIninterrupto();

protected:
    double tempoTotalSeg;
    double dtSeg;
    Hidrometro hidrometro;
    Configuracao conf_;
    std::unique_ptr<Saida> saida_; 
    int lastSavedM3_;
};
}

#endif
