#ifndef MEMORY_H
#define MEMORY_H

#include<string>

using namespace std;

class VirtualMemory{
    public:
        //Constói um simulador de memória virtual dados o tamanho de suas páginas, o tamanho da memória física e um string descrevendo o algoritmo de substituição
        //A memória virtual sempre tem 2^32 entradas, cada uma de um byte endereçadas por 32bits, independente do tamanho da memória física. 
        VirtualMemory(int pagesize, int memsize, string algorit);

        //Lê na memória virtual o byte de endereço indicado por address, o endereço tendo 32bits, e retorna o valor lido.
        unsigned char read_mem(int address);

        //Escreve na memória virtual o byte de valor val e endereço indicado por address, o endereço tendo 32bits.
        void write_mem(int address, unsigned char val);

        //Obtém o número de vezes que um ponto da memória foi carregado da memória secundária.
        int get_read_times();

        //Obtém o número de vezes que um ponto da memória foi excrito na memória secundária.
        int get_write_times();

        //Destrutor da classe
        ~VirtualMemory();

    private:
        //Retorna o endereço do quadro na memória física correspondente ao endereço da página na memória virtual dado
        //Se não houver um quadro alocado, seleciona um quadro segundo o algoritmo selecionado e salva na tabela 
        int convert_to_mem(int page_address);

        //Spies para representar o carregamento e salvamento na memória secundária. Ela não está sendo representada, mas o número de chamadas é registrado.
        //Os incrementos foram implementados como funções separadas meramente para simular de maneira um pouco mais fiel os acessos à memória.
        void load_2nd(int mem_address);
        void save_2nd(int mem_address);

        //Retorna o endereço do quadro da memória a ser substituído (vítima) segundo o algoritmo lru
        int lru();

        //Retorna o endereço do quadro da memória a ser substituído (vítima) segundo o algoritmo fifo
        int fifo();

        //Retorna o endereço do quadro da memória a ser substituído (vítima) segundo um novo algoritmo (random?)
        int newalg();

        //Executa o algoritmo escolhido e retorna seu resultado.
        int get_victim();

        //Variáveis privadas que guardam 
        int read_times_;
        int write_times_;

        //Variáveis privadas usadas para a representação da tabela de páginas
        int ptable_size_;
        unsigned char s_;
        int* ptable_;

        //Variáveis privadas usadas para a representação da memória física
        int mem_size_;
        unsigned char* memory_;
        bool* dirty_;

        //Variáveis privadas usadas para os algoritmos
        string algor_;
        
};

#endif //MEMORY_H