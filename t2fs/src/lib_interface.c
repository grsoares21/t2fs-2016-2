#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/file_interface.h"
#include "../include/lib_interface.h"

struct t2fs_record* listaDir2[20];
struct t2fs_open_file* listaFile2[20];

int identify2(char *name, int size)
{	//TODO add jessica's and bharbara's ids
  	strncpy(name, "Bharbara Cechin - 240430\nGabriel Restori Soares - 217436\nJessica Lorencetti - XXXXXX", (size_t)size);
    return 0;
}

int getFreeDirIndex() {
	int i;
	for(i = 0; i < 20; i++){
		if(listaDir2[i] == NULL) {
			return i;
		}
	}
	return -1;
}

int getFreeFileIndex() {
	int i;
	for(i = 0; i < 20; i++){
		if(listaFile2[i] == NULL) {
			return i;
		}
	}
	return -1;
}

FILE2 create2 (char *filename) {
	int freeIndex = getFreeFileIndex();
	if(freeIndex == -1)
		return -1;

	listaFile2[freeIndex] = (struct t2fs_open_file*)malloc(sizeof(struct t2fs_open_file));
	listaFile2[freeIndex]->handle = 0;
	//TODO: verificar se o arquivo ja existe
	struct t2fs_record* newRecord = createRecord(filename, (BYTE)0x01);
	listaFile2[freeIndex]->fileRecord = *newRecord;

	return freeIndex;
}

FILE2 open2 (char *filename) {
	int freeIndex = getFreeFileIndex();
	if(freeIndex == -1)
		return -1;

	listaFile2[freeIndex] = (struct t2fs_open_file*)malloc(sizeof(struct t2fs_open_file));
	listaFile2[freeIndex]->handle = 0;

	struct t2fs_record* record = getRecord(filename);
	if(record == NULL)
		return -1;

	listaFile2[freeIndex]->fileRecord = *record;
	listaFile2[freeIndex]->filePath = filename;

	return freeIndex;
}

int close2 (FILE2 handle) {
	free(listaFile2[handle]);
	listaFile2[handle] = NULL;

	return 0;
}

int read2 (FILE2 handle, char *buffer, int size) {
	char* readBytes = readFromInode(listaFile2[handle]->fileRecord.inodeNumber, listaFile2[handle]->handle, size);
	memcpy(buffer, readBytes, size);
	free(readBytes);
	listaFile2[handle]->handle = listaFile2[handle]->handle + size;

	return 0;
}

int write2 (FILE2 handle, char *buffer, int size) {
	writeInInode(listaFile2[handle]->fileRecord.inodeNumber, listaFile2[handle]->handle, buffer, size);
	listaFile2[handle]->handle = listaFile2[handle]->handle + size;

	return 0;
}

int seek2(FILE2 handle, unsigned int offset) {
	listaFile2[handle]->handle = offset;

	return 0;
}

int delete2 (char *filename) {
	return deleteRecord(filename);
}

int truncate2(FILE2 handle) {
	struct t2fs_open_file* file = listaFile2[handle];
	file->fileRecord.bytesFileSize = file->handle;
	file->fileRecord.blocksFileSize = file->handle / 4096;

	truncateInode(file->fileRecord.inodeNumber, file->fileRecord.blocksFileSize);
	return updateRecord(file->filePath, file->fileRecord);
}

//TODO: verificar erros das funções implementadas e documentar os contratos
