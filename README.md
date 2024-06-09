# square-kernel

Um kernel simples e open-source para aprendizado e hobby.

Implementações

- bootloader [x]
- protected-mode [x]
- suporte a vga [x]
- gerenciador de processos (15 processos) [x]
- alocador de memória dinâmica
- gerenciador de processos avançado
- suporte a outros hardwares (teclado e etc)
- sistema de arquivos

## Como rodar o kernel?

Para rodar o kernel em um emulador é necessário baixar o código fonte e compilar uma versão para rodar com o qemu.
Use o script "install" para realizar a instalação dos compiladores e do emulador.
Rode o comando make para realizar build e rodar o kernel.

    git clone https://github.com/https-dre/square-kernel/tree/main
    cd square-kernel
    mkdir build
    chmod +x install
    ./install
    make build_and_run
