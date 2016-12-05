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

/*=============================================================
*
*	record MANAGEMENT: getRecord, getRecordsInDir, createRecord, deleteRecord, updateRecord;
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

struct t2fs_record_list {
	struct t2fs_record* fileRecord;
	struct t2fs_record_list* next;
};

struct t2fs_record* createRecord(char *path, BYTE type);
struct t2fs_record* getRecord(char *path);
char* readFromInode(int inodeNumber, unsigned int fileHandle, int size);
void writeInInode(int inodeNumber, unsigned int fileHandle, char* data, int size);
int truncateInode(int inodeNumber, int fromBlockIndex);
int deleteRecord(char* filePath);
int updateRecord(char *filePath, struct t2fs_record newRecord);

#endif
