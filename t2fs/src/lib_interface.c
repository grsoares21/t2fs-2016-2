#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/file_interface.h"
#include "../include/lib_interface.h"

struct t2fs_open_dir* listaDir2[20];
struct t2fs_open_file* listaFile2[20];

int identify2(char *name, int size)
{	//TODO add jessica's and bharbara's ids
  	strncpy(name, "Bharbara Cechin - 240430\nGabriel Restori Soares - 217436\nJessica Lorencetti - 228342", (size_t)size);
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

    listaFile2[freeIndex]->filePath = (char*)malloc(sizeof(char)*(strlen(filename)+1));
	memcpy(listaFile2[freeIndex]->filePath, filename, strlen(filename));
	listaFile2[freeIndex]->filePath[strlen(filename)] = '\0';

	return freeIndex;
}

FILE2 open2 (char *filename) {
	int freeIndex = getFreeFileIndex();
	if(freeIndex == -1)
		return -1;

    struct t2fs_record* record = getRecord(filename);
	if(record == NULL)
		return -1;

	listaFile2[freeIndex] = (struct t2fs_open_file*)malloc(sizeof(struct t2fs_open_file));
	listaFile2[freeIndex]->handle = 0;
	listaFile2[freeIndex]->filePath = (char*)malloc(sizeof(char)*(strlen(filename)+1));
	memcpy(listaFile2[freeIndex]->filePath, filename, strlen(filename));
	listaFile2[freeIndex]->filePath[strlen(filename)] = '\0';

	listaFile2[freeIndex]->fileRecord = *record;

	return freeIndex;
}

int close2 (FILE2 handle) {
    if(listaFile2[handle] == NULL) {
        return -1;
    }

	free(listaFile2[handle]);
	listaFile2[handle] = NULL;

	return 0;
}

int read2 (FILE2 handle, char *buffer, int size) {
    if(listaFile2[handle] == NULL) {
        return -1;
    }

    int bytesToRead = min(size, listaFile2[handle]->fileRecord.bytesFileSize - listaFile2[handle]->handle);
	if(bytesToRead < 1)
        return -1;

	char* readBytes = readFromInode(listaFile2[handle]->fileRecord.inodeNumber, listaFile2[handle]->handle, bytesToRead);

	memcpy(buffer, readBytes, bytesToRead);
	free(readBytes);
	listaFile2[handle]->handle = listaFile2[handle]->handle + bytesToRead;
	return bytesToRead;
}

int write2 (FILE2 handle, char *buffer, int size) {
    if(listaFile2[handle] == NULL) {
        return -1;
    }

	writeInInode(listaFile2[handle]->fileRecord.inodeNumber, listaFile2[handle]->handle, buffer, size);
	if(listaFile2[handle]->handle + size > listaFile2[handle]->fileRecord.bytesFileSize) {
        listaFile2[handle]->fileRecord.bytesFileSize = listaFile2[handle]->handle + size;
        listaFile2[handle]->fileRecord.blocksFileSize = ((listaFile2[handle]->handle + size) / 4096) + 1;
        updateRecord(listaFile2[handle]->filePath, listaFile2[handle]->fileRecord);
	}

	listaFile2[handle]->handle = listaFile2[handle]->handle + size;

	return size;
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
	file->fileRecord.blocksFileSize = (file->handle / 4096) + 1;

	truncateInode(file->fileRecord.inodeNumber, file->fileRecord.blocksFileSize);
	return updateRecord(file->filePath, file->fileRecord);
}

int mkdir2 (char *pathname) {
 	if(createRecord(pathname, (BYTE)0x02) != NULL) {
		return 0;
	}
	return -1;
}

int rmdir2 (char *pathname) {
	struct t2fs_record* dirRecord = getRecord(pathname);
	struct t2fs_record_list* currentChildRecord = getRecordsInDir(dirRecord->inodeNumber);
	while(currentChildRecord != NULL) {
		int pathNameSize = strlen(pathname);
		int childRecordNameSize = strlen(currentChildRecord->fileRecord->name);
		char* recordPath = (char*)malloc(sizeof(char)*(pathNameSize + childRecordNameSize + 2));
		memcpy(recordPath, pathname, pathNameSize);
		recordPath[pathNameSize] = '/';
		memcpy(recordPath+pathNameSize+1, currentChildRecord->fileRecord->name, childRecordNameSize);
		recordPath[pathNameSize + childRecordNameSize + 1] = '\0';

		if(currentChildRecord->fileRecord->TypeVal == (BYTE)0x01) {
			delete2(recordPath);
		} else if(currentChildRecord->fileRecord->TypeVal == (BYTE)0x02) {
			rmdir2(recordPath);
		}
		free(recordPath);

		currentChildRecord = currentChildRecord->next;
	}
	return deleteRecord(pathname);
}

DIR2 opendir2(char *pathname) {
	int freeIndex = getFreeDirIndex();
	if(freeIndex == -1)
		return -1;

	listaDir2[freeIndex] = (struct t2fs_open_dir*)malloc(sizeof(struct t2fs_open_dir));
	listaDir2[freeIndex]->currentDirentIndex = 0;

	struct t2fs_record* record = getRecord(pathname);
	if(record == NULL)
		return -1;

	listaDir2[freeIndex]->fileRecord = *record;

	return freeIndex;
}

struct t2fs_record* getNthRecordInRecordList(struct t2fs_record_list* list, int index) {
	int currentIndex = 0;
	while(currentIndex < index) {
		if(list == NULL) return NULL;
		currentIndex++;
		list = list->next;
	}

	if(list == NULL) return NULL;
	return list->fileRecord;
}

int readdir2(DIR2 handle, DIRENT2 *dentry) {
	struct t2fs_record_list* dirEntries = getRecordsInDir(listaDir2[handle]->fileRecord.inodeNumber);
	struct t2fs_record* dentryRecord = getNthRecordInRecordList(dirEntries, (listaDir2[handle]->currentDirentIndex)++);
	if(dentryRecord == NULL) {
		return -1;
	}

	memcpy(dentry->name, dentryRecord->name, strlen(dentryRecord->name));
	dentry->fileType = dentryRecord->TypeVal;
	dentry->fileSize = dentryRecord->bytesFileSize;

	return 0;
}

int closedir2 (DIR2 handle) {
	free(listaDir2[handle]);
	listaDir2[handle] = NULL;

	return 0;
}


//TODO: verificar erros das funções implementadas e documentar os contratos
