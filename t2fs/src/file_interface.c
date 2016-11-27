#include "../include/file_interface.h"
#include "../include/disk_interface.h"
#include "../include/t2fs.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

/*------ FUNCOES AUXILIARES-------
	Regiao do codigo que possui apenas funcoes auxiliares utilizadas exclusivamente pela file_interface.
	Estas funcoes nao sao expostas no header.
 ----------------------------------*/
int getNthIntegerFromBlock(char* block, int index) {
	char* integerBytes = (char*)malloc(sizeof(char)*4);

	integerBytes = (char*)memcpy(integerBytes, block + (index*4), 4);
	uint32_t integer = (uint32_t)(integerBytes[0]) | (uint32_t)(integerBytes[1] << 8)
							| (uint32_t)(integerBytes[2] << 16) | (uint32_t)(integerBytes[3] << 24);
	//this order works considering the bytes are in little-endian format
	free(integerBytes);

	return integer;
}

int getNthBlockNumberInInode(struct t2fs_inode inode, int index) {
	if(index < 2) {
		return inode.dataPtr[index];

	} else if(index < 1026) {
		if(inode.singleIndPtr != INVALID_PTR) {
			char* singleIndBlock = getBlock(inode.singleIndPtr);
			int singleIndIndex = index - 2;
			int blockNumber = getNthIntegerFromBlock(singleIndBlock, singleIndIndex);
			//this order works considering the bytes are in little-endian format
			free(singleIndBlock);

			return blockNumber;
		} else {
			return INVALID_PTR;
		}
	} else {
		if(inode.doubleIndPtr != INVALID_PTR) {
			int firstIndIndex = (index - 1026) / 1024;
			int secondIndIndex = (index - 1026) - (firstIndIndex * 1024);

			char* firstIndBlock = getBlock(inode.doubleIndPtr);
			int secondIndBlockNumber = getNthIntegerFromBlock(firstIndBlock, firstIndIndex);
			free(firstIndBlock);
			if(secondIndBlockNumber != INVALID_PTR) {
				char* secondIndBlock = getBlock(secondIndBlockNumber);
				int blockNumber = getNthIntegerFromBlock(secondIndBlock, secondIndIndex);

				free(secondIndBlock);
				return blockNumber;
			} else {
				return INVALID_PTR;
			}
		} else {
			return INVALID_PTR;
		}
	}
}

char* getNthBlockInInode(struct t2fs_inode inode, int index) {
	int blockNumber = getNthBlockNumberInInode(inode, index);
	if(blockNumber != INVALID_PTR) {
		return getBlock(blockNumber);
	}
	return NULL;
}

/*------ FUNCOES DA INTERFACE ------
	Regiao do codigo que possui as funcoes da file_interface e que estao expostas no header.
----------------------------*/
struct t2fs_record* getRecord(char *path) {
	//struct t2fs_inode* rootInode = getInode(0);
	return NULL;
}
