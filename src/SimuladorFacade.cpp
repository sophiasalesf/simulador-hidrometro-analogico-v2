#include "SimuladorHidrometroFacade.hpp"

namespace hidrometro {

    SimuladorHidrometroFacade& SimuladorHidrometroFacade::getInstancia() {
        static SimuladorHidrometroFacade instancia;
        return instancia;
    }

    bool SimuladorHidrometroFacade::configSimuladorSHA(double tempo, double passo) {
        if (passo < 0 || tempo < 0)
            return false;
        controlador.configurarTempo(tempo, passo);
        return true;
    }

    bool SimuladorHidrometroFacade::criaSHA() {
        ParseConfigResult config = carregarConfiguracao("configuracoes/configuracao_padrao.txt");
        int res = controlador.adicionarHidrometro(config.conf);
        return res;
    }

    bool SimuladorHidrometroFacade::finalizaSHA(int pos) {
        int res = controlador.removerHidrometro(pos);
        return res;
    }

    bool SimuladorHidrometroFacade::modificaVazaoSHA(double vazao, int pos) {
        int res = controlador.setVazaoHidrometro(vazao, pos);
        return res;
    }

    void SimuladorHidrometroFacade::habilitaGeracaoImagemSHA(bool config) {
        controlador.setGeracaoImagem(config);
    }

    void SimuladorHidrometroFacade::executar() {
        threadSimulador = std::thread(&Controlador::executar, &controlador);

        if (threadSimulador.joinable())
            threadSimulador.join();
    }
}   