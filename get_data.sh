#!/bin/bash

make 
#touch data1.csv

#echo "entry,mem_size,pag_size,alg,reads,writes" > data1.csv

for alg in "fifo" "lru" "newalg"
    do
        for src in "logs/compilador.log" "logs/compressor.log" "logs/matriz.log" "logs/simulador.log"    
        do
            for mem in {7..14}
            do
                let s=2**$mem
#                ./tp2virtual $alg $src 4 $s
                
            done
        done
    done