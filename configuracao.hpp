#ifndef CONFIGURACAO_HPP
#define CONFIGURACAO_HPP

#include <string>
#include "SentidoFluxo.hpp"

namespace hidrometro
{
using std::string;

class Configuracao
{
public:
    Configuracao();

    double getTfs() const;
    double getVam() const;
    double getPctAr() const;
    double getBitola() const;
    SentidoFluxo getSentido() const;
    double getTaxaImg() const;
    string getSaidaDir() const;
    string getFormato() const;
    string getBaseImg() const;

    void setTfs(double v);
    void setVam(double v);
    void setPctAr(double v);
    void setBitola(double v);
    void setSentido(SentidoFluxo s);
    void setTaxaImg(double v);
    void setSaidaDir(const string& dir);
    void setFormato(const string& f);
    void setBaseImg(const string& path);

protected:
    double tfs;
    double vam;
    double pctAr;
    double bitola;
    SentidoFluxo sentido;
    double taxaImg;
    string saidaDir;
    string formato;
    string baseImg;
};

struct ParseConfigResult
{
    bool sucesso;        
    Configuracao conf;   
    string erro;    
};

ParseConfigResult carregarConfiguracao(const string& caminho);
bool validarConfiguracao(Configuracao& c, string& warnLog);
SentidoFluxo parseSentido(const string& s);

}

#endif
