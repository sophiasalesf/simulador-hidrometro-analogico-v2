#include <iostream>
#include "Configuracao.hpp"
#include "Controlador.hpp"

using namespace hidrometro;

int main() 
{
    ParseConfigResult res = carregarConfiguracao("configuracao.txt");
    if (!res.sucesso)
    {
        std::cerr << "Erro ao carregar configuração: " << res.erro << std::endl;
        return 1;
    }

    Configuracao conf = res.conf;

    Controlador controlador;
    controlador.configurarComConfiguracao(conf);

    if (conf.getTfs() > 0.0)
    {
        controlador.executar();             
    }
    else
    {
        controlador.executarIninterrupto(); 
    }

    return 0;
}