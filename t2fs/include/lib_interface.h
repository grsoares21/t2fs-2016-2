/*
*
*	lib_interface: arquivo de inclusao com os prototipos de funcoes implementadas
*		Contem as funcoes definidas pela biblioteca
*
*	VERSAO ATUAL: 20161127
*
*/

#ifndef __lib_interface__
#define __lib_interface__

struct t2fs_open_file {
	struct t2fs_record fileRecord;
	unsigned int handle;
	char* filePath;
};

struct t2fs_open_dir {
	struct t2fs_record fileRecord;
	int currentDirentIndex;
};


#endif
