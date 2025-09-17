#ifndef HIDROMETRO_HPP
#define HIDROMETRO_HPP

#include <string>
#include "Entrada.hpp"
#include "Medicao.hpp"

namespace hidrometro
{

class Hidrometro
{
public:
    Hidrometro();

    void setEntrada(const Entrada& e);
    const Entrada& getEntrada() const;

    void medir(double dtSeg);

    const Medicao& getMedicao() const;

    std::string apresentacaoMedicao() const;

private:
    Entrada  input_;
    Medicao  medicao_;
};

}

#endif