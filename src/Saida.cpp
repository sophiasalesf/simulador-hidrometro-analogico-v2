#include "Saida.hpp"
#include "Display.hpp"
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <direct.h>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace hidrometro
{
    Saida::Saida(const std::string& matricula_, int indiceHidrometro)
        : matricula(matricula_), indiceHidrometro(indiceHidrometro)
    {
        // pastaBase = "Medicoes_" + matricula;
        pastaBase = "Medicoes_" + matricula_ + "/hidrometro" + std::to_string(this->indiceHidrometro);
        garantirPasta();
    }

    void Saida::garantirPasta()
    {
        // Define o caminho da pasta principal
        std::string pastaPrincipal = "Medicoes_" + this->matricula;

        // Tenta criar a pasta principal
        if (_mkdir(pastaPrincipal.c_str()) != 0 && errno != EEXIST) 
        {
            // Constrói uma mensagem de erro detalhada
            std::stringstream msg;
            msg << "Falha ao criar a pasta principal '" << pastaPrincipal << "': " << strerror(errno);
            
            // Lança a exceção com a mensagem
            throw std::runtime_error(msg.str());
        }

        // Tenta criar a subpasta
        if (_mkdir(this->pastaBase.c_str()) != 0 && errno != EEXIST) 
        {
            // Constrói uma mensagem de erro detalhada
            std::stringstream msg;
            msg << "Falha ao criar a subpasta '" << this->pastaBase << "': " << strerror(errno);

            // Lança a exceção com a mensagem
            throw std::runtime_error(msg.str());
        }
    }

    void Saida::salvarMedicao(int m3, const Configuracao& conf, const Hidrometro& h)
    {
        garantirPasta();

        int idx = m3 % 99;
        if (idx == 0) idx = 1;

        std::ostringstream caminho;
        caminho << pastaBase << "/"
                << std::setw(2) << std::setfill('0') << idx
                << ".jpeg";

        Display disp;
        disp.setBaseImage(conf.getBaseImg());

        int w=0, hgt=0, comp=0;
        const std::vector<unsigned char>& rgb = disp.renderEstado(h, w, hgt, comp);

        stbi_write_jpg(caminho.str().c_str(), w, hgt, comp, rgb.data(), 90);
    }
}
