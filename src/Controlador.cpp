#include "Controlador.hpp"
#include <chrono>
#include <iostream>

namespace hidrometro
{
using Clock = std::chrono::steady_clock;
using std::cout;
using std::endl;

Controlador::Controlador()
    : tempoTotalSeg(0.0), dtSeg(0.1), saida_(), lastSavedM3_(-1)
{}

void Controlador::configurarTempo(double tempoTotalSeg_, double dtSeg_)
{
    tempoTotalSeg = (tempoTotalSeg_ < 0.0 ? 0.0 : tempoTotalSeg_);
    dtSeg = (dtSeg_ > 0.0 ? dtSeg_ : 0.1);
}

void Controlador::configurarEntrada(const Entrada& e)
{
    hidrometro.setEntrada(e);
}

void Controlador::configurarComConfiguracao(const Configuracao& conf)
{
    conf_ = conf; 

    configurarTempo(conf.getTfs(), 0.1);

    Entrada e;
    e.entradaFluxo(conf.getBitola(), conf.getVam(), conf.getSentido());
    e.definirAr(conf.getPctAr() > 0.0, conf.getPctAr() / 100.0);
    configurarEntrada(e);

    if (!saida_)
    {
        saida_.reset(new Saida("202311250024"));
    }

    lastSavedM3_ = -1; 
}


void Controlador::executar()
{
    if (tempoTotalSeg <= 0.0)
    {
        cout << "[Controlador] TFS=0 => nada a executar aqui. Use executarIninterrupto()." << endl;
        return;
    }

    const double passo = dtSeg;
    double simTime = 0.0;

    auto t0   = Clock::now();
    auto tPrev = t0;
    double acc = 0.0;

    double proximoLog = 1.0;

    while (simTime < tempoTotalSeg)
    {
        auto now = Clock::now();
        double dtReal = std::chrono::duration_cast<std::chrono::duration<double>>(now - tPrev).count();
        tPrev = now;

        acc += dtReal;

        while (acc >= passo)
        {
            hidrometro.medir(passo);
            simTime += passo;
            acc -= passo;

            int m3Atual = static_cast<int>(hidrometro.getMedicao().getTotalLitros() / 1000);
            if (saida_ && m3Atual > lastSavedM3_)
            {
                saida_->salvarMedicao(m3Atual, conf_, hidrometro);
                lastSavedM3_ = m3Atual;
            }

            if (simTime + 1e-9 >= proximoLog)
            {
                cout << "[tick] t=" << static_cast<int>(proximoLog) << "s"
                     << " | " << hidrometro.apresentacaoMedicao() << endl;
                proximoLog += 1.0;
            }

            if (simTime >= tempoTotalSeg) break;
        }
    }

    cout << "[Controlador] EXECUTAR finalizado. t=" << simTime << "s" << endl;
}


void Controlador::executarIninterrupto()
{
    const double passo = dtSeg;
    auto tPrev = Clock::now();
    double acc = 0.0;
    double tLog = 0.0;

    while (true)
    {
        auto now = Clock::now();
        double dtReal = std::chrono::duration_cast<std::chrono::duration<double>>(now - tPrev).count();
        tPrev = now;
        acc += dtReal;

        while (acc >= passo)
        {
            hidrometro.medir(passo);
            acc -= passo;

            int m3Atual = static_cast<int>(hidrometro.getMedicao().getTotalLitros() / 1000);
            if (saida_ && m3Atual > lastSavedM3_) 
            {
                saida_->salvarMedicao(m3Atual, conf_, hidrometro);
                lastSavedM3_ = m3Atual;
            }

            tLog += passo;
            if (tLog >= 2.0)
            {
                std::cout << "[tick] +2s | " << hidrometro.apresentacaoMedicao() << std::endl;
                tLog = 0.0;
            }
        }
    }

}
}