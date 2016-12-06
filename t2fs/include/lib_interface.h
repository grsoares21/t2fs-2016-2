/*
*
*	lib_interface: arquivo de inclusao com as estruturas utilizadas nas funcoes implementadas
*		Contem as funcoes definidas pela biblioteca
*
*	VERSAO ATUAL: 20161127
*
*/

#ifndef __lib_interface__
#define __lib_interface__

/*=============================================================
*
*	ESTRUTURAS: t2fs_open_file, t2fs_open_dir;
*
=============================================================*/
struct t2fs_open_file {
	struct t2fs_record fileRecord;
	unsigned int handle;
	char* filePath;
};

struct t2fs_open_dir {
	struct t2fs_record fileRecord;
	int currentDirentIndex;
};



/*=============================================================
*
*	FUNCOES AUXILIARES: getFreeDirIndex, getFreeFileIndex, getNthRecordInRecordList;
*
=============================================================*/
int getFreeDirIndex();
int getFreeFileIndex();
struct t2fs_record* getNthRecordInRecordList(struct t2fs_record_list* list, int index);

#endif
