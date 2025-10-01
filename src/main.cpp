// #include <iostream>
// #include "Configuracao.hpp"
// #include "Controlador.hpp"

// using namespace hidrometro;

// int main() 
// {
//     ParseConfigResult res = carregarConfiguracao("configuracao.txt");
//     if (!res.sucesso)
//     {
//         std::cerr << "Erro ao carregar configuração: " << res.erro << std::endl;
//         return 1;
//     }

//     Configuracao conf = res.conf;

//     Controlador controlador;
//     controlador.configurarComConfiguracao(conf);

//     if (conf.getTfs() > 0.0)
//     {
//         controlador.executar();
//     }
//     else
//     {
//         controlador.executarIninterrupto();
//     }

//     return 0;
// }

#include <iostream>
#include <vector>
#include <string>
#include "Controlador.hpp"
#include "Configuracao.hpp"

// Usar o namespace para simplificar
using namespace hidrometro;

int main() 
{
    // Defina a lista de arquivos de configuração aqui.
    // Adicione ou remova linhas para mudar a simulação.
    std::vector<std::string> arquivosDeConfig = {
        "configuracoes/configuracao1.txt",
        "configuracoes/configuracao2.txt",
        "configuracoes/configuracao3.txt",
        "configuracoes/configuracao4.txt",
        "configuracoes/configuracao5.txt"
    };

    if (arquivosDeConfig.empty()) {
        std::cerr << "Erro: Nenhum arquivo de configuração definido na lista em main.cpp." << std::endl;
        return 1;
    }

    // 1. Cria a instância do controlador
    Controlador controlador;
    double tempoSimulacaoGlobal = 0.0;

    // 2. Loop para carregar cada arquivo da lista
    bool primeiroArquivo = true;
    for (const std::string& caminhoArquivo : arquivosDeConfig) 
    {
        std::cout << "[Main] Carregando arquivo: " << caminhoArquivo << std::endl;

        ParseConfigResult res = carregarConfiguracao(caminhoArquivo);
        if (!res.sucesso)
        {
            std::cerr << "Erro ao carregar '" << caminhoArquivo << "': " << res.erro << std::endl;
            continue; // Pula para o próximo arquivo
        }

        // Usaremos o tempo de simulação (TFS) do PRIMEIRO arquivo como o global
        if (primeiroArquivo) {
            tempoSimulacaoGlobal = res.conf.getTfs();
            controlador.configurarTempo(tempoSimulacaoGlobal, 0.1);
            primeiroArquivo = false;
        }

        // Adiciona o hidrômetro configurado ao controlador
        if (!controlador.adicionarHidrometro(res.conf)) {
             std::cerr << "Aviso: Limite de hidrômetros atingido. Ignorando " << caminhoArquivo << std::endl;
        }
    }

    // 3. Decide qual modo de execução usar com base no tempo global
    std::cout << "\n[Main] Todas as configurações foram carregadas. Iniciando simulação..." << std::endl;
    if (tempoSimulacaoGlobal > 0.0)
    {
        controlador.executar();
    }
    else
    {
        controlador.executarIninterrupto();
    }

    std::cout << "[Main] Simulação finalizada." << std::endl;
    return 0;
}