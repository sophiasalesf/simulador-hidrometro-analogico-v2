#include "Saida.hpp"
#include "Display.hpp"
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <direct.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace hidrometro
{
    Saida::Saida(const std::string& matricula_)
        : matricula(matricula_)
    {
        pastaBase = "Medicoes_" + matricula;
        garantirPasta();
    }

    void Saida::garantirPasta()
    {
        struct stat info;
        if (stat(pastaBase.c_str(), &info) != 0)
        {
            _mkdir(pastaBase.c_str());
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
