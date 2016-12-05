/*
*
*	disk_interface: arquivo de inclus�o com os prot�tipos de fun��es a serem implementadas
*		Cont�m as fun��es necess�rias para a comunica��o com o disco virtual passado pelo professor
*
*	VERS�O ATUAL: 20161120
*
*/

#ifndef __disk_interface__
#define __disk_interface__

/*-------------------------------------------------------------
	Fun��o de inicializa��o da estrutura de superbloco (setor 1 do disco). Deve ser chamada na primeira vez que alguma fun��o da disk_interface for chamada.
Entrada:
	NULL
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int init();

/*=============================================================
*
*	READ MANAGEMENT: getInode, getBlock;
*
=============================================================*/

/*-------------------------------------------------------------
	Fun��o que calcula e retorna um ponteiro do tipo char, que conter� os dados lidos do bloco, cujo n�mero foi passado como par�metro
Entrada:
	blockNumber -> n�mero do bloco a ser retornado
Retorna:
	SUCESSO: ponteiro para a estrutura de dados que onter� os dados lidos do bloco requerido
	ERRO: NULL
-------------------------------------------------------------*/
char* getBlock(int blockNumber);

/*-------------------------------------------------------------
	Fun��o que calcula e retorna um estrutura inode, cujo n�mero foi passado como para�metro
Entrada:
	inodeNumber -> n�mero do inode a ser retornado
Retorna:
	SUCESSO: estrutura t2fs_inode com os seus respectivos valores j� setados
	ERRO: NULL
-------------------------------------------------------------*/
struct t2fs_inode* getInode(int inodeNumber);


/*=============================================================
*
*	WRITE MANAGEMENT: writeBlock, writeInode, writeInBlock;
*
=============================================================*/

/*-------------------------------------------------------------
	Fun��o que reescreve inteiramente o bloco indicado por blockNumber com os dados indicados por data.
Entrada:
	blockNumber -> n�mero do bloco que vai ter suas infoma��es reescritas
	data -> conjunto de dados a ser escrito no bloco indicado
Retorna:
	SUCESSO:
	ERRO:
-------------------------------------------------------------*/
int writeBlock(int blockNumber, char data[4096]);

/*-------------------------------------------------------------
	Fun��o que reescreve parte do bloco indicado por blockNumber com os dados indicados por data
Entrada:
	blockNumber -> n�mero do bloco que vai ter suas informa��es reescritas
	initialByte -> ?
	data -> conjunto de dados a ser escrito no bloco indicado
	size -> ?
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int writeInBlock(int blockNumber, int initialByte, unsigned char* data, int size);

/*-------------------------------------------------------------
	Fun��o que atualiza um inode, cujo n�mero � passado por inodeNumber, com as informa��es de inode
Entrada:
	inodeNumber -> n�mero do inode que ser� atualizado
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
	Fun��o que atualiza o bit indicado por blockNumber para um, ou seja, aloca o bloco
Entrada:
	blockNumber -> n�mero do bloco que ser� atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int allocateBlock();

/*-------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Fun��o que atualiza o bit indicado por inodeNumber para um, ou seja, aloca o inode
Entrada:
	inodeNumber -> n�mero do inode que ser� atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
int allocateInode();

/*-------------------------------------------------------------
	Fun��o que atualiza o bit indicado por blockNumber para zero, ou seja, libera o bloco
Entrada:
	blockNumber -> n�mero do bloco que ser� atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int freeBlock(int blockNumber);

/*-------------------------------------------------------------
	Fun��o que atualiza o bit indicado por inodeNumber para zero, ou seja, libera o inode
Entrada:
	inodeNumber -> n�mero do inode que ser� atualizado/liberado
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int freeInode(int inodeNumber);


#endif
