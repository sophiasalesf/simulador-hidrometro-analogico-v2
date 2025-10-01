#ifndef SAIDA_HPP
#define SAIDA_HPP

#include <string>
#include "Hidrometro.hpp"
#include "Configuracao.hpp"

namespace hidrometro
{

using std::string;

class Saida
{
public:
    Saida(const std::string& matricula, int indiceHidrometro);
    void salvarMedicao(int m3, const Configuracao& conf, const Hidrometro& h);

protected:
    int indiceHidrometro;
    std::string matricula;
    std::string pastaBase;
    void garantirPasta();
};

}

#endif
