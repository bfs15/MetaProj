#ifndef BYTES_H
#define BYTES_H

#define mod(X,Y) ((((X) % (Y)) + (Y)) % Y)
#define Lower_Multiple(SZ,D) ((SZ) - ((SZ) % D))

#define CACHE_LINE_SIZE (64) // likwid-topology: Cache line size:	64
#define L1_LINE_DN (CACHE_LINE_SIZE/sizeof(double)) // how many doubles in a line
// likwid-topology: Size
#define CACHE_L1_SIZE (32*1024/2)
#define CACHE_L2_SIZE (256*1024/2)
#define CACHE_L3_SIZE (3*1024*1024/2)
// divided by 2 because we wont be able to fill L1 completely without throwing
// useful values out

// 2048
#define L1_DN (CACHE_L1_SIZE/sizeof(double)) // how many doubles in L1 cache
#define L2_DN (CACHE_L2_SIZE/sizeof(double)) // how many doubles in L1 cache
#define L3_DN (CACHE_L3_SIZE/sizeof(double)) // how many doubles in L1 cache

// size of the block to fit one matrix in L1
#define MAX_BL1 ((long)sqrt(L1_DN))
// align to cache line
#define BL1 (Lower_Multiple(MAX_BL1, L1_LINE_DN))
// 40 % 8 == 0, (40*40 < 2048)

// to fit 3 matrixes
#define MAX_B3L1 ((long)sqrt(L1_DN/3))
#define B3L1 (Lower_Multiple(MAX_B3L1, L1_LINE_DN))

#define MAX_B3L2 ((long)sqrt(L2_DN/3))
#define B3L2 (Lower_Multiple(MAX_B3L2, B3L1))

#define MAX_B3L3 ((long)sqrt(L3_DN/3))
#define B3L3 (Lower_Multiple(MAX_B3L3, B3L2))

#define REG_SZ (32) // how many bytes in a register

#define regDN (REG_SZ/sizeof(double)) // how many doubles is a register
#define regFN (REG_SZ/sizeof(float)) // how many floats is a register

#endif