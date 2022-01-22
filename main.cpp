#include <string>
#include <iostream>
#include <fstream>

#include "src/memory.h"

using namespace std;

int main(int argc, char* argv[]){
    string algoritmo = argv[1];
    string arquivo = argv[2];
    int sizePagina = stoi(argv[3]);
    int memoria = stoi(argv[4]);
    
    // Imprimindo mensagens de inicialização do programa
    cout << "Executando o simulador...\n";
    cout << "Arquivo de entrada: "   << arquivo    << "\n";
    cout << "Tamanho da memoria: "   << memoria    << " KB\n";
    cout << "Tamanho das páginas: "  << sizePagina << " KB\n";
    cout << "Tecnica de reposicao: " << algoritmo  << "\n";
    
    // Execução do programa
    VirtualMemory virt_mem(sizePagina, memoria, algoritmo);

    int lidas, escritas; // Número de páginas lidas e escritas durante execução do programa
    
    ifstream file;
    int addr;
    char rw;
    
    file.open(arquivo);

    while (file >> hex >> addr) {
        file >> rw;
        
        switch(rw){
            case 'R':
                virt_mem.read_mem(addr);
                break;
            case 'W':
                virt_mem.write_mem(addr,42);
                break;
        } 
    }
    
    file.close();

    // Resultado encontrado
    cout << "Paginas lidas: "    << virt_mem.get_read_times()  << "\n";
    cout << "Paginas escritas: " << virt_mem.get_write_times() << "\n";

    return 0;
}