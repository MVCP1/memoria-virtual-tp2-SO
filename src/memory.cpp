#include "memory.h"
#include<string>
#include<iostream>

using namespace std;

VirtualMemory::VirtualMemory(int pagesize, int memsize, string algorit){
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
    ptable_ = new int(ptable_size_);

    mem_size_ = memsize;
    memory_ = new unsigned char(memsize);
    dirty_ = new bool(memsize);

}

unsigned char VirtualMemory::read_mem(int address){
    int page_addr = address >> s_;
    int offset    = address - (page_addr << s_);

    int mem_addr = convert_to_mem(page_addr);
    return memory_[mem_addr + offset];
}

void VirtualMemory::write_mem(int address, unsigned char val){
    int page_addr = address >> s_;
    int offset    = address - (page_addr << s_);

    int mem_addr = convert_to_mem(page_addr);
    
    memory_[mem_addr + offset] = val;
    dirty_[mem_addr + offset]  = true;
}

int VirtualMemory::get_read_times(){
    return read_times_;
}

int VirtualMemory::get_write_times(){
    return write_times_;
}

int VirtualMemory::convert_to_mem(int page_address){
    return 0;
}

void VirtualMemory::load_2nd(int mem_address){
    read_times_++;
}

void VirtualMemory::save_2nd(int mem_address){
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