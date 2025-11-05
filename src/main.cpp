#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include "Controlador.hpp"
#include "Configuracao.hpp"
#include "SimuladorHidrometroFacade.hpp"

// Usar o namespace para simplificar
using namespace hidrometro;
using namespace std;

class Cliente {
    public:
        static int main() {
            double tmp_at, tmp_exe;
            
            cout << "Configurando o simulador..." << endl;
            cout << "Digite o tempo de execucao e o tempo de atualizacao do simulador (em segundos): ";
            cin >> tmp_exe >> tmp_at;

            SimuladorHidrometroFacade& shf = SimuladorHidrometroFacade::getInstancia();
            shf.configSimuladorSHA(tmp_exe, tmp_at);
            
            cout << "Simulador configurado com sucesso!" << endl;
            
            int opc;

            while (opc != 5) {
                cout << "Funcionalidades:" << endl << "[1] criaSHA" << endl << 
                "[2] finalizaSHA" << endl << "[3] modificaVazaoSHA" << endl << "[4] habilitaGeracaoImagemSHA" << 
                endl << "[5] executarSimulador" << endl << endl;
                cout << "Digite a sua opcao: ";
                cin >> opc;
                
                int indice, qnt;
                double nova_vazao;
                std::string s;

                switch (opc) {
                    case 1:
                        cout << "Digite quantos simuladores quer instanciar (Total maximo de 5): ";
                        cin >> qnt;

                        for (int i = 0; i < qnt; i++)
                            shf.criaSHA();
                        break;
                    
                    case 2:
                        cout << "Digite o indice da instancia que quer finalizar: ";
                        cin >> indice;
                        shf.finalizaSHA(indice);
                        break;
                    
                    case 3:
                        cout << "Digite o valor da vazao e o indice do hidrometro: ";
                        cin >> nova_vazao >> indice;
                        shf.modificaVazaoSHA(nova_vazao, indice);
                        break;

                    case 4:
                        cout << "[y] Habilitar" << endl;
                        cout << "[n] Desabilitar" << endl;
                        cout << "Digite sua opcao: ";
                        cin >> s;
                        cout << s;
                        s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
                        std::transform(s.begin(), s.end(), s.begin(), ::tolower);

                        if (s == "y")
                            shf.habilitaGeracaoImagemSHA(true);
                        else if (s == "n")
                            shf.habilitaGeracaoImagemSHA(false);
                        else
                            cout << "Digite uma opcao valida!" << endl;
                        break;
                        
                    case 5:
                        cout << "Iniciando simulador..." << endl;
                        shf.executar();
                        break;

                    default:
                        break;
                    

                    }
                
                if (opc != 5) {
                    std::this_thread::sleep_for(chrono::seconds(2));
                    system("clear");
                }
            }

            return 0;
        }
};

int main() {
   Cliente::main(); 
}