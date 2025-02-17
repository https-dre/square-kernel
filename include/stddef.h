#ifndef STDDEF_H
#define STDDEF_H

// Definição de NULL
#define NULL ((void*)0)

// Tipos para tamanhos e diferenças de ponteiros
typedef unsigned int size_t;
typedef int ptrdiff_t;

// Macro para obter o offset de um membro em uma estrutura
#define offsetof(type, member) __builtin_offsetof(type, member)

#endif // STDDEF_H