/*
*
*	file_interface: arquivo de inclusao com os prototipos de funcoes implementadas
*		Contem as funcoes necessarias para a interacao com o sistema de arquivos t2fs
*
*	VERSAO ATUAL: 20161127
*
*/

#ifndef __file_interface__
#define __file_interface__

struct t2fs_record_list {
	struct t2fs_record* fileRecord;
	struct t2fs_record_list* next;
};

/*=============================================================
*
*	FUNCOES AUXILIARES: allocateBlockForInode, getNthIntegerFromBlock, getFatherDirInode, min;
*
=============================================================*/

void allocateBlockForInode(int inodeNumber, int blockIndex);
int getNthIntegerFromBlock(char* block, int index);
struct t2fs_inode* getFatherDirInode(char* path);
int min(int a, int b);

/*=============================================================
*
*	RECORD MANAGEMENT: getRecord, getRecordsInDir, createRecord, deleteRecord, updateRecord;
*
=============================================================*/

/*-------------------------------------------------------------
	Funcao que percorre as pastas em disco para encontrar o record apontado pelo caminho "path"
Entrada:
	path -> caminho para o arquivo cujo record deve ser recuperado
Retorna:
	SUCESSO: ponteiro para uma estrutura t2fs_record contendo o record do arquivo apontado por "path"
	ERRO: NULL
-------------------------------------------------------------*/
struct t2fs_record* createRecord(char *path, BYTE type);

/*-------------------------------------------------------------
	Funcao que delete um record de nome fileName, excluindo seus dados de dentro do bloco e marcando como invalido dentro da pasta pai
	OBS: nao se responsabiliza por deletar os arquivos contidos dentro de uma pasta, caso tenho sido passada no parametro (tratar na funcao chamadora)
Entrada:
	fileName -> nome do arquivo que sera deletado
Retorna:
	SUCESSO: 0
	ERRO: qualquer numero diferente de zero
-------------------------------------------------------------*/
int deleteRecord(char* filePath);

/*-------------------------------------------------------------
	Funcao que percorre as pastas em disco para encontrar o record apontado pelo caminho "path"
Entrada:
	path -> caminho para o arquivo cujo record deve ser recuperado
Retorna:
	SUCESSO: ponteiro para uma estrutura t2fs_record contendo o record do arquivo apontado por "path"
	ERRO: NULL
-------------------------------------------------------------*/
struct t2fs_record* getRecord(char *path);

/*-------------------------------------------------------------
	Funcao que cria uma lista de todos os arquivos contidos em um diretorio
Entrada:
	inodeNumber -> número do inode em que o diretorio esta contido
Retorna:
	SUCESSO: ponteiro para a lista de records
	ERRO: NULL
-------------------------------------------------------------*/
struct t2fs_record_list* getRecordsInDir(int inodeNumber);

/*-------------------------------------------------------------
	Funcao que reescreve um record apontado por fileName com os dados contidos em newRecord
Entrada:
	fileName -> record que sera reescrito com os dados de newRecord
	newRecord -> dados guardados para atualizar o record apontado por fileName
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int updateRecord(char *filePath, struct t2fs_record newRecord);

/*=============================================================
*
*	INODE MANAGEMENT: getNthBlockInInode, getNthBlockNumberInInode, readFromInode, truncateInode, writeInInode;
*
=============================================================*/
char* getNthBlockInInode(struct t2fs_inode inode, int index);

/*-------------------------------------------------------------
	Funcao que percorre um inode para retornar o index-esimo bloco deste inode
Entrada:
	inode -> estrutura de inode em que se quer achar o bloco desejado
	index -> número do bloco que se quer achar no inode
Retorna:
	SUCESSO: numero do index-esimo bloco do inode
	ERRO: INVALID_PTR
-------------------------------------------------------------*/
int getNthBlockNumberInInode(struct t2fs_inode inode, int index);

/*-------------------------------------------------------------
	Funcao que le de um inode, percorrendo-o
Entrada:
	inodeNumber -> numero do inode a ser lido
	fileHandle -> quais blocos devem ser lidos
	size -> tamanho da quantidade lida do inode
Retorna:
	SUCESSO: uma string de tamanho size
	ERRO: NULL
-------------------------------------------------------------*/
char* readFromInode(int inodeNumber, unsigned int fileHandle, int size);

/*-------------------------------------------------------------
	Funcao que
Entrada:
	inodeNumber ->
	fromBlockIndex ->
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
int truncateInode(int inodeNumber, int fromBlockIndex);

/*-------------------------------------------------------------
	Funcao que escreve somente em blocos especificos de um inode
Entrada:
	inodeNumber -> numero do inode que contem os blocos que vao ser escritos
	fileHandle ->
	data ->
	size ->
Retorna:
	SUCESSO: 0
	ERRO: qualquer valor diferente de zero
-------------------------------------------------------------*/
void writeInInode(int inodeNumber, unsigned int fileHandle, char* data, int size);


#endif
