#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include <memory>
#include <vector>
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

    bool adicionarHidrometro(const Configuracao& conf);

    void executar();
    void executarIninterrupto();

protected:
    double tempoTotalSeg;
    double dtSeg;
    int maxHidrometros;
    std::vector<Hidrometro> hidrometros_;
    std::vector<Configuracao> configs_;
    std::vector<std::unique_ptr<Saida>> saidas_; 
    std::vector<int> lastSavedM3s_;
};
}

#endif
