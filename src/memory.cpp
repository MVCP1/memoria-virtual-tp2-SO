#include "memory.h"
#include <string>
#include <iostream>
#include <ctime> 
#include <list>

using namespace std;

//Funções públicas

VirtualMemory::VirtualMemory(int pagesize, int memsize, string algorit): VirtualMemory(pagesize,memsize,algorit,false){}

VirtualMemory::VirtualMemory(int pagesize, int memsize, string algorit, bool debug){
    frame_size_ = pagesize*1024;
    algor_ = algorit;
    read_times_ = 0;
    write_times_ = 0;
    debug_ = debug;

    srand((unsigned)time(0));
    
    //Guarda o número de bits de pagesize em s
    while(pagesize > 1){
        pagesize = pagesize >> 1;
        s_++;
    }

    //Define o tamanho da tabela de páginas da seguinte forma:
    //Dos 32 bits de endereçamento virtual (2^32 B), 10 são retirados pois os dados estão em kB (2^10 B), e s são retirados pois os kB estão em blocos de tamanho pagesize = 2^s
    //O número de páginas é então uma potência de 2 dos bits restantes, o que equivale a um shift para a esquerda nesse número de bits 
    ptable_size_ = 1 << (32 - (s_ + 10));
    ptable_ = new int[ptable_size_]();
    dirty_  = new bool[ptable_size_]();
    valid_  = new bool[ptable_size_]();

    mem_size_ = memsize*1024;
    memory_ = new unsigned char[mem_size_]();

    control_size_ = mem_size_/frame_size_;
}

unsigned char VirtualMemory::read_mem(int address){
    int page_addr = address >> (s_+10);
    int offset    = address - (page_addr << (s_+10));
    
    if(debug_){cout<<"Leitura na memória virtual. Endereço: "<<address<<". Endereço da página: "<<page_addr<<". Offset: "<<offset<<endl;}
    
    int fram_addr = page_to_frame(page_addr);
    return memory_[fram_addr + offset];
}

void VirtualMemory::write_mem(int address, unsigned char val){
    int page_addr = address >> (s_+10);
    int offset    = address - (page_addr << (s_+10));

    if(debug_){cout<<"Escrita na memória virtual. Endereço: "<<address<<". Endereço da página: "<<page_addr<<". Offset: "<<offset<<endl;}

    int fram_addr = page_to_frame(page_addr);

    dirty_[page_addr]  = true;

    if(debug_){cout<<"  Página suja."<<endl;}
}

int VirtualMemory::get_read_times(){
    return read_times_;
}

int VirtualMemory::get_write_times(){
    return write_times_;
}

VirtualMemory::~VirtualMemory(){
    delete[] ptable_;
    delete[] memory_;
    delete[] dirty_;
    delete[] valid_;
}

//Funções privadas

int VirtualMemory::page_to_frame(int page_address){
    int victim, vic_frame;
    
    if(!valid_[page_address]){
        if(control_.size() < control_size_){         //Memória física ainda não está cheia
            control_.push_back(control_.size());
            vic_frame = control_.back()*frame_size_;
            
            if(debug_){cout<<"  Página não alocada. Sem páginas substituídas. Quadro escolhido: "<<vic_frame<<endl;}
        }
        
        else{                                        //Memória física está cheia e uma vítima deve ser escolhida para ser substituída.
            victim = get_victim();
            vic_frame = ptable_[victim];

            if(debug_){cout<<"  Página não alocada. Página substituída: "<<victim<<". Quadro escolhido: "<<vic_frame<<endl;}

            if(dirty_[victim]){
                save_2nd(victim,vic_frame);
            }
            valid_[victim] = false;
            dirty_[victim] = false;

        }

        load_2nd(page_address,vic_frame);
        valid_[page_address] = true;
        ptable_[page_address] = vic_frame;
        

        memory_[vic_frame]   = (page_address & stoi("111111110000000000000000",0,2)) >> 16;
        memory_[vic_frame+1] = (page_address & stoi("000000001111111100000000",0,2)) >> 8;
        memory_[vic_frame+2] = (page_address & stoi("000000000000000011111111",0,2));

    }
    else if(valid_[page_address] && algor_ == "lru"){    //Coloca a página acessada no topo da pilha de controle se o algoritmo for lru e a página for válida.
        int i = ptable_[page_address]/frame_size_;
        list<int>::iterator remove_it;
        for (list<int>::iterator it = control_.begin(); it!=control_.end(); ++it){
            if(*it == i){
                remove_it = it;
            }
        }
        control_.erase(remove_it);
        control_.push_back(i);
    }

    return ptable_[page_address];
}

int VirtualMemory::frame_to_page(int frame_address){
    int temp1 = memory_[frame_address];
    int temp2 = memory_[frame_address+1];
    int temp3 = memory_[frame_address+2];
    return (temp1<<16)+(temp2<<8)+(temp3);
}

void VirtualMemory::load_2nd(int page_address, int mem_address){
    read_times_++;

    if(debug_){cout<<"  Leitura no disco."<<endl;}
}

void VirtualMemory::save_2nd(int page_address, int mem_address){
    write_times_++;

    if(debug_){cout<<"  Escrita no disco."<<endl;}
}

int VirtualMemory::fifo(){
    int frame_idx = control_.front();
    control_.pop_front();
    control_.push_back(frame_idx);
    return frame_to_page(frame_idx*frame_size_);
    
}

int VirtualMemory::lru(){
    int frame_idx = control_.front();
    control_.pop_front();
    control_.push_back(frame_idx);
    return frame_to_page(frame_idx*frame_size_);
}

int VirtualMemory::newalg(){
    int frame_idx = rand()%(control_size_);
    return frame_to_page(frame_idx*frame_size_);
}

int VirtualMemory::get_victim(){
    if(algor_ == "lru"){
        return lru();
    }
    else if(algor_ == "fifo"){
        return fifo();
    }
    else{
        return newalg();
    }
}