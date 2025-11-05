#include "Controlador.hpp"
#include <chrono>
#include <iostream>
#include <string>

namespace hidrometro
{
using Clock = std::chrono::steady_clock;
using std::cout;
using std::endl;

Controlador::Controlador()
    : tempoTotalSeg(0.0), dtSeg(0.1), maxHidrometros(5), gerarImagem(true)
{}

// Configura o tempo global da simulação, que se aplica a todos os hidrômetros.
void Controlador::configurarTempo(double tempoTotalSeg_, double dtSeg_)
{
    tempoTotalSeg = (tempoTotalSeg_ < 0.0 ? 0.0 : tempoTotalSeg_);
    dtSeg = (dtSeg_ > 0.0 ? dtSeg_ : 0.1);
}

bool Controlador::setPasso(double dtSeg) {
    if (dtSeg < 0)
        return false;

    this->dtSeg = dtSeg;
    return true;
}

bool Controlador::setMaxHidrometros(int maxHidrometros) {
    if (maxHidrometros < 0 || maxHidrometros > 5)
        return false;

    this->maxHidrometros = maxHidrometros;
    return true;
}

void Controlador::setGeracaoImagem(bool config) {
    this->gerarImagem = config;
}

bool Controlador::setVazaoHidrometro(double vazao, int pos) {
    if (vazao < 0 || pos < 0 || pos >= hidrometros_.size())
        return false;

    Entrada e = hidrometros_[pos].getEntrada();
    e.entradaFluxo(e.getBitola(), vazao, e.getSentido());
    hidrometros_[pos].setEntrada(e);
    return true;
}

bool Controlador::adicionarHidrometro(const Configuracao& conf) {
    if (hidrometros_.size() >= this->maxHidrometros)
    {
        cout << "[Controlador] Erro: Limite de " << this->maxHidrometros << " hidrometros atingido." << endl;
        return false;
    }

    // Guarda a configuração para uso posterior (ex: no salvamento)
    configs_.push_back(conf);

    // Cria e configura a Entrada
    Entrada e;
    e.entradaFluxo(conf.getBitola(), conf.getVam(), conf.getSentido());
    e.definirAr(conf.getPctAr() > 0.0, conf.getPctAr() / 100.0);

    // Cria e configura o Hidrômetro
    Hidrometro novoHidrometro;
    novoHidrometro.setEntrada(e);
    hidrometros_.push_back(novoHidrometro);

    string matricula = "202311250024";
    int proximoIndice = hidrometros_.size(); 
    saidas_.push_back(std::make_unique<Saida>(matricula, proximoIndice));
    
    // Adiciona o estado inicial de salvamento para este hidrômetro
    lastSavedM3s_.push_back(-1);

    return true;
}

bool Controlador::removerHidrometro(int pos) {
    if (pos < 0 || pos >= this->hidrometros_.size()) {
        cout << "[Controlador] Erro: Nao existe hidrometro na posicao " << pos << '.' << endl;
        return false;
    }

    hidrometros_.erase(hidrometros_.begin() + pos);
    configs_.erase(configs_.begin() + pos);
    saidas_.erase(saidas_.begin() + pos);
    lastSavedM3s_.erase(lastSavedM3s_.begin() + pos);
    return true;
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
    
    auto tPrev = Clock::now();
    double acc = 0.0;
    
    // Vetor para controlar o próximo log de cada hidrômetro
    std::vector<double> proximosLogs(hidrometros_.size(), 1.0);

    while (simTime < tempoTotalSeg)
    {
        auto now = Clock::now();
        double dtReal = std::chrono::duration_cast<std::chrono::duration<double>>(now - tPrev).count();
        tPrev = now;
        acc += dtReal;

        // Loop de passo de tempo fixo
        while (acc >= passo)
        {
            for (size_t i = 0; i < hidrometros_.size(); ++i)
            {
                hidrometros_[i].medir(passo);

                int m3Atual = static_cast<int>(hidrometros_[i].getMedicao().getTotalLitros() / 1000);
                if (saidas_[i] && m3Atual > lastSavedM3s_[i] && gerarImagem)
                {
                    saidas_[i]->salvarMedicao(m3Atual, configs_[i], hidrometros_[i]);
                    lastSavedM3s_[i] = m3Atual;
                }

                if (simTime + 1e-9 >= proximosLogs[i])
                {
                    cout << "[tick] H" << i+1 << " t=" << static_cast<int>(proximosLogs[i]) << "s"
                         << " | " << hidrometros_[i].apresentacaoMedicao() << endl;
                    proximosLogs[i] += 1.0;
                }
            }

            simTime += passo;
            acc -= passo;

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
    
    // Vetor para controlar o tempo de log de cada hidrômetro
    std::vector<double> tLogs(hidrometros_.size(), 0.0);

    while (true)
    {
        auto now = Clock::now();
        double dtReal = std::chrono::duration_cast<std::chrono::duration<double>>(now - tPrev).count();
        tPrev = now;
        acc += dtReal;

        while (acc >= passo)
        {
            for (size_t i = 0; i < hidrometros_.size(); ++i)
            {
                hidrometros_[i].medir(passo);
                
                int m3Atual = static_cast<int>(hidrometros_[i].getMedicao().getTotalLitros() / 1000);
                if (saidas_[i] && m3Atual > lastSavedM3s_[i] && this->gerarImagem)
                {
                    saidas_[i]->salvarMedicao(m3Atual, configs_[i], hidrometros_[i]);
                    lastSavedM3s_[i] = m3Atual;
                }

                tLogs[i] += passo;
                if (tLogs[i] >= 2.0)
                {
                    cout << "[tick] H" << i+1 << " +2s | " << hidrometros_[i].apresentacaoMedicao() << endl;
                    tLogs[i] = 0.0;
                }
            }

            acc -= passo;
        }
    }
}
}
