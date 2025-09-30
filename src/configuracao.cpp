#include "configuracao.hpp"
#include <fstream>   
#include <sstream>   

namespace hidrometro
{

Configuracao::Configuracao()
    : tfs(0.0),
        vam(0.0),
        pctAr(0.0),
        bitola(25.0),
        sentido(SentidoFluxo::Direto),
        taxaImg(0.0),
        saidaDir(""),
        formato("ambos"),
        baseImg("")
{}

double Configuracao::getTfs() const 
{ 
    return tfs; 
}
double Configuracao::getVam() const
{ 
    return vam; 
}
double Configuracao::getPctAr() const 
{ 
    return pctAr; 
}
double Configuracao::getBitola() const 
{ 
    return bitola; 
}
SentidoFluxo Configuracao::getSentido() const 
{ 
    return sentido; 
}
double Configuracao::getTaxaImg() const 
{ 
    return taxaImg; 
}
std::string Configuracao::getSaidaDir() const 
{ 
    return saidaDir; 
}
std::string Configuracao::getFormato() const 
{
        return formato; 
}
std::string Configuracao::getBaseImg() const 
{ 
    return baseImg; 
}

void Configuracao::setTfs(double v) 
{ 
    tfs = v; 
}
void Configuracao::setVam(double v) 
{ 
    vam = v; 
}
void Configuracao::setPctAr(double v) 
{ 
    pctAr = v; 
}
void Configuracao::setBitola(double v) 
{ 
    bitola = v; 
}
void Configuracao::setSentido(SentidoFluxo s) 
{ 
    sentido = s; 
}
void Configuracao::setTaxaImg(double v) 
{ 
    taxaImg = v; 
}
void Configuracao::setSaidaDir(const std::string& dir) 
{ 
    saidaDir = dir; 
}
void Configuracao::setFormato(const std::string& f) 
{ 
    formato = f; 
}
void Configuracao::setBaseImg(const std::string& path) 
{ 
    baseImg = path; 
}

ParseConfigResult carregarConfiguracao(const std::string& caminho)
{
    ParseConfigResult result;
    result.sucesso = false;

    std::ifstream file(caminho.c_str());
    if (!file.is_open())
    {
        result.erro = "Erro: não foi possível abrir " + caminho;
        return result;
    }

    Configuracao conf;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string chave = line.substr(0, pos);
        std::string valor = line.substr(pos + 1);

        auto trim = [](std::string& s) 
        {
            size_t start = s.find_first_not_of(" \t");
            size_t end   = s.find_last_not_of(" \t");
            if (start == std::string::npos) { s = ""; return; }
            s = s.substr(start, end - start + 1);
        };

        trim(chave);
        trim(valor);

        if (chave == "TFS") conf.setTfs(std::stod(valor));
        else if (chave == "VAM") conf.setVam(std::stod(valor));
        else if (chave == "PCT_AR") conf.setPctAr(std::stod(valor));
        else if (chave == "BITOLA") conf.setBitola(std::stod(valor));
        else if (chave == "SENTIDO") {
            conf.setSentido((valor == "Reverso") ? SentidoFluxo::Reverso : SentidoFluxo::Direto);
        }
        else if (chave == "TAXA_IMG") conf.setTaxaImg(std::stod(valor));
        else if (chave == "SAIDA_DIR") conf.setSaidaDir(valor);
        else if (chave == "FORMATO") conf.setFormato(valor);
        else if (chave == "BASE_IMG") conf.setBaseImg(valor);
    }

    result.sucesso = true;
    result.conf = conf;
    return result;
}

} 