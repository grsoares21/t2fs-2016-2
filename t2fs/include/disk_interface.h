/*
*
*	disk_interface: arquivo de inclusao com os prototipos de funcoes a serem implementadas
*		Contem as funcoes necessarias para a comunicacao com o disco virtual passado pelo professor
*
*	VERSAO ATUAL: 20161205
*
*/

#ifndef __disk_interface__
#define __disk_interface__

/*=============================================================
*
*	FUNCOES AUXILIARES: charToInt4LtlEnd, char2ToShortIntLtlEnd, intToChar4LtlEnd, shortIntToChar2LtlEnd, init;
*
=============================================================*/

DWORD charToInt4LtlEnd(unsigned char* array);
WORD char2ToShortIntLtlEnd(unsigned char* array);
unsigned char* intToChar4LtlEnd(int number);
unsigned char* shortIntToChar2LtlEnd(short int number);


/*-------------------------------------------------------------
	Funcao de inicializaco da estrutura de superbloco (setor 1 do disco). Deve ser chamada na primeira vez que alguma Funcao da disk_interface for chamada.
Entrada:
	NULL
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int init();

/*=============================================================
*
*	READ MANAGEMENT: getBlock, getInode;
*
=============================================================*/

/*-------------------------------------------------------------
	Funcao que calcula e retorna um ponteiro do tipo char, que contera os dados lidos do bloco, cujo numero foi passado como parametro
Entrada:
	blockNumber -> numero do bloco a ser retornado
Retorna:
	SUCESSO: ponteiro para a estrutura de dados que ontera os dados lidos do bloco requerido
	ERRO: NULL
-------------------------------------------------------------*/
char* getBlock(int blockNumber);

/*-------------------------------------------------------------
	Funcao que calcula e retorna um estrutura inode, cujo numero foi passado como paraametro
Entrada:
	inodeNumber -> numero do inode a ser retornado
Retorna:
	SUCESSO: estrutura t2fs_inode com os seus respectivos valores ja setados
	ERRO: NULL
-------------------------------------------------------------*/
struct t2fs_inode* getInode(unsigned int inodeNumber);


/*=============================================================
*
*	WRITE MANAGEMENT: writeBlock, writeInode, writeInBlock;
*
=============================================================*/

/*-------------------------------------------------------------
	Funcao que reescreve inteiramente o bloco indicado por blockNumber com os dados indicados por data.
Entrada:
	blockNumber -> numero do bloco que vai ter suas infomaaaes reescritas
	data -> conjunto de dados a ser escrito no bloco indicado
Retorna:
	SUCESSO: 0
	ERRO: -1
-------------------------------------------------------------*/
int writeBlock(int blockNumber, char data[4096]);


/*-------------------------------------------------------------
	Funcao que reescreve parte do bloco indicado por blockNumber com os dados indicados por data
Entrada:
	blockNumber -> numero do bloco que vai ter suas informaaaes reescritas
	initialByte -> ?
	data -> conjunto de dados a ser escrito no bloco indicado
	size -> ?
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int writeInBlock(int blockNumber, int initialByte, unsigned char* data, int size);


/*-------------------------------------------------------------
	Funcao que atualiza um inode, cujo numero sera passado por inodeNumber, com as informaaaes de inode
Entrada:
	inodeNumber -> numero do inode que sera atualizado
	inode -> estrutura do tipo inode
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int writeInode(int inodeNumber, struct t2fs_inode inode);


/*=============================================================
*
*	ALLOCATION MANAGEMENT: freeInode, allocateInode, freeBlock, allocateBlock;
*
=============================================================*/

/*-------------------------------------------------------------
	Funcao que atualiza o bit indicado por blockNumber para um, ou seja, aloca o bloco
Entrada:
	blockNumber -> numero do bloco que sera atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int allocateBlock();

/*-------------------------------------------------------------
	Funcao que atualiza o bit indicado por inodeNumber para um, ou seja, aloca o inode
Entrada:
	inodeNumber -> numero do inode que sera atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int allocateInode();

/*-------------------------------------------------------------
	Funcao que atualiza o bit indicado por blockNumber para zero, ou seja, libera o bloco
Entrada:
	blockNumber -> numero do bloco que sera atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int freeBlock(int blockNumber);

/*-------------------------------------------------------------
	Funcao que atualiza o bit indicado por inodeNumber para zero, ou seja, libera o inode
Entrada:
	inodeNumber -> numero do inode que sera atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int freeInode(int inodeNumber);

#endif
