#include "memory.h"
#include<string>
#include<iostream>

using namespace std;

//Funções públicas

VirtualMemory::VirtualMemory(int pagesize, int memsize, string algorit){
    frame_size_ = pagesize;
    algor_ = algorit;
    read_times_ = 0;
    write_times_ = 0;

    //Guarda o número de bits de pagesize em s
    while(pagesize > 1){
        pagesize = pagesize >> 1;
        s_++;
    }

    //Define o tamanho da tabela de páginas da seguinte forma:
    //Dos 32 bits de endereçamento virtual (2^32 B), 10 são retirados pois os dados estão em kB (2^10 B), e s são retirados pois os kB estão em blocos de tamanho pagesize = 2^s
    //O número de páginas é então uma potência de 2 dos bits restantes, o que equivale a um shift para a esquerda nesse número de bits 
    ptable_size_ = 1 << (22 - s_);
    ptable_ = new int[ptable_size_]();
    dirty_  = new bool[ptable_size_]();
    valid_  = new bool[ptable_size_]();

    mem_size_ = memsize;
    memory_ = new unsigned char[memsize]();

}

unsigned char VirtualMemory::read_mem(int address){
    int page_addr = address >> s_;
    int offset    = address - (page_addr << s_);

    int fram_addr = page_to_frame(page_addr);
    return memory_[fram_addr + offset];
}

void VirtualMemory::write_mem(int address, unsigned char val){
    int page_addr = address >> s_;
    int offset    = address - (page_addr << s_);

    int fram_addr = page_to_frame(page_addr);

    dirty_[page_addr]  = true;
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
        victim = get_victim();
        vic_frame = ptable_[victim];
        
        if(dirty_[victim]){
            save_2nd(victim,vic_frame);
        }
        valid_[victim] = false;
        dirty_[victim] = false;

        load_2nd(page_address,vic_frame);
        valid_[page_address] = true;
        ptable_[page_address] = vic_frame;

        memory_[vic_frame]   = (page_address & stoi("111111110000000000000000",0,2)) >> 16;
        memory_[vic_frame+1] = (page_address & stoi("000000000111111110000000",0,2)) >> 8;
        memory_[vic_frame+2] = (page_address & stoi("000000000000000011111111",0,2));

    }
    return ptable_[page_address];
}

int VirtualMemory::frame_to_page(int frame_address){
    return (memory_[frame_address]<<16)+(memory_[frame_address+1]<<8)+(memory_[frame_address+2]);
}

void VirtualMemory::load_2nd(int page_address, int mem_address){
    read_times_++;
}

void VirtualMemory::save_2nd(int page_address, int mem_address){
    write_times_++;
}

int VirtualMemory::fifo(){
    return 0;
}

int VirtualMemory::lru(){
    return 0;
}

int VirtualMemory::newalg(){
    return 0;
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