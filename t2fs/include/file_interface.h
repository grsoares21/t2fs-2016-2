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
	Funcaoo que percorre as pastas em disco para encontrar o record apontado pelo caminho "path"
Entrada:
	path -> caminho para o arquivo cujo record deve ser recuperado
Retorna:
	SUCESSO: ponteiro para uma estrutura t2fs_record contendo o record do arquivo apontado por "path"
	ERRO: NULL
-------------------------------------------------------------*/

struct t2fs_record* getRecord(char *path);

#endif
