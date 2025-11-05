#ifndef SIMULADORHIDROMETROFACADE_HPP
#define SIMULADORHIDROMETROFACADE_HPP

#include "Controlador.hpp"
#include <thread>
#include <atomic>

// Fachada Singleton para o SHA
namespace hidrometro {

    class SimuladorHidrometroFacade {
    
        private:
            Controlador controlador;
            std::thread threadSimulador;

            bool parar;
    
            SimuladorHidrometroFacade(const SimuladorHidrometroFacade&) = delete;
            SimuladorHidrometroFacade& operator=(const SimuladorHidrometroFacade&) = delete;
    
            SimuladorHidrometroFacade() {}
    
        public:
            static SimuladorHidrometroFacade& getInstancia();
    
            bool configSimuladorSHA(double, double);
            bool criaSHA();
            bool finalizaSHA(int);
            bool modificaVazaoSHA(double, int);
            void habilitaGeracaoImagemSHA(bool);

            void executar();
    };

}

#endif