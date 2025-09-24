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
    Saida(const std::string& matricula);
    void salvarMedicao(int m3, const Configuracao& conf, const Hidrometro& h);

protected:
    std::string matricula;
    std::string pastaBase;
    void garantirPasta();
};

}

#endif
