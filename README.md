# Simulador de Hidrômetro Analógico (SHA) v2.0

O Simulador de Hidrômetro Analógico (SHA) é um software desenvolvido em C++ que reproduz o funcionamento de um hidrômetro de água em ambiente virtual. Ele foi criado para auxiliar em testes e experimentos sem a necessidade de um equipamento físico.

## Ferramenta utilizada para o Diagrama de Classes — SHA 2.0
- O diagrama de classes do SHA 2.0 foi produzido utilizando a ferramenta **draw.io** (https://app.diagrams.net/)

## Funcionalidades
- Simulação contínua do fluxo de água em tempo discreto ou de forma ininterrupta.
- Registro automático de medições em arquivos de imagem (.jpeg), organizados por pasta.
- Interface gráfica simples gerada por software, que lembra um hidrômetro analógico.
- Suporte a parâmetros configuráveis via arquivo configuracao.txt (ex.: tempo total, passo de simulação, base de imagem, diretório de saída).

## Estrutura do código
- `Entrada.hpp/.cpp` → Configuração de vazão, bitola, direção do fluxo e presença de ar.

- `Medicao.hpp/.cpp` → Controle da contagem de litros e m³.

- `Hidrometro.hpp/.cpp` → Núcleo da simulação do hidrômetro.

- `Display.hpp/.cpp` → Geração da imagem do hidrômetro com odômetro, litros e dL.

- `Saida.hpp/.cpp` → Salvamento das medições em arquivos .jpeg.

- `Controlador.hpp/.cpp` → Coordena a execução da simulação de acordo com as configurações.

- `main.cpp` → Ponto de entrada do programa.

## Como compilar
1. Compile o projeto (g++ ou outro compilador C++ compatível).
- No PowerShell (Windows, usando MinGW):
```sh
mkdir bin 2>$null
g++ -std=c++14 -I . .\Entrada.cpp .\Medicao.cpp .\Hidrometro.cpp .\Display.cpp .\Controlador.cpp .\Saida.cpp .\main.cpp -o .\bin\simulador.exe
.\bin\simulador.exe
```
2. Ajuste os parâmetros no arquivo `configuracao.txt`.
3. Rode o programa.
4. As medições geradas estarão disponíveis na pasta `Medicoes_<matrícula>` ou no diretório configurado.

## Exemplo de saída no terminal
<img width="753" height="786" alt="image" src="https://github.com/user-attachments/assets/0f6fd501-e1ff-47ff-9dd1-af9228b233f1" />

## Exemplo de imagem.jpeg gerada
<img width="640" height="360" alt="image" src="https://github.com/user-attachments/assets/4b8ad2a6-15d5-4705-b1a2-3594d2a96cec" />

## Desenvolvedora
Sophia Sales (IFPB - Engenharia de Computação) (https://github.com/Sophia7b)  
