
# square-kernel

Um kernel simples e open-source para aprendizado e hobby.

<img src="docs/print.png">

# Implementações

- Protected-Mode *32 bits*  *(implemented)*

- Vga-Buffer *(implemented)*

- Basic *PM* - Process Manager *(implemented)*

- Basic *MM* - Memory Manager *(implemented)*

- Advanced *PM*

- Hardware Suport (Teclado e etc)

- Basic *FS* - File System

  

## Compilação e Emulação

**Não é recomendado tentar dar boot em uma máquina real, o projeto ainda segue incompleto e não há certeza que a segurança é 100% é garantida**

### Dependências de Compilação e Linkagem

 - nasm - *compilador assembly x86*
 - gcc - *compilador C*
 - ld - *linker*


### Baixe um emulador

Para rodar o projeto é necessário baixar em um emulador compatível com x86, no caso do projeto eu uso o QEMU

### Baixe o projeto do github:

	git clone https://github.com/https-dre/square-kernel/tree/main
	cd square-kernel

### Crie um diretório para arquivos de construção
	mkdir build

*Use o script "install" para realizar a instalação em distribuições linux baseadas em Debian*

	chmod +x install
	./install

### Rode o Makefile

	make

## Licença

Este projeto está licenciado sob a Licença Pública Geral GNU v3.0 - veja o arquivo [LICENSE](LICENSE) para mais detalhes.
