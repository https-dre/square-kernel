#ifndef STDINT_H
#define STDINT_H

// Tipos inteiros com tamanhos fixos
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

// Tipos inteiros com tamanhos dependentes de ponteiros
typedef int intptr_t;
typedef unsigned int uintptr_t;

#endif // STDINT_H