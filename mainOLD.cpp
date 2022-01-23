#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//#include "src/memory.h"

using namespace std;

int main(){
    string programa, algoritmo, arquivo;
    int sizePagina, memoria;
    int lidas, escritas; // Número de páginas lidas e escritas durante execução do programa
    
    // Leitura das variáveis de input
    cout << "prompt> ";
    cin >> programa >> algoritmo >> arquivo;
    cin >> sizePagina >> memoria;
    
    // Se o programa certo foi chamado
    if(programa == "tp2virtual"){
        
        // Imprimindo mensagens de inicialização do programa
        // cout << "prompt> " << programa << " " << algoritmo << " " << file << " " << sizePagina << " " << memoria << "\n";
        cout << "Executando o simulador...\n";
        cout << "Arquivo de entrada: " << arquivo << "\n";
        cout << "Tamanho da memoria: " << memoria << " KB\n";
        cout << "Tamanho das páginas: " << sizePagina << " KB\n";
        cout << "Tecnica de reposicao: " << algoritmo << "\n";

        // Execução do programa
        // lidas = 520;
        // escritas = 352;
        FILE *file = fopen(arquivo.c_str(), "r");
        unsigned addr;
        char rw;
        lidas = 0;
        escritas = 0;
        while (fscanf(file,"%x %c",&addr,&rw) != EOF) {
            if(rw == 'R'){
                lidas += 1;
            }else if(rw == 'W'){
                escritas += 1;
            }
        }
        
        fclose(file);

        // Resultado encontrado
        cout << "Paginas lidas: " << lidas << "\n";
        cout << "Paginas escritas: " << escritas << "\n";


    // Retorna erro se outro programa foi chamado
    }else{
        cout << "Erro: Programa solicitado não reconhecido.\n";
    }

    return 0;
}