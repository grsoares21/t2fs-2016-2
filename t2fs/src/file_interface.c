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

int min(int a, int b) {
	if(a < b) {
		return a;
	} else {
		return b;
	}
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

void allocateBlockForInode(int inodeNumber, int blockIndex) {
	int allocatedBlockNumber = allocateBlock();
	struct t2fs_inode* inode = getInode(inodeNumber);

	unsigned char *allocBlckNbrLtlEnd = (unsigned char*)malloc(sizeof(unsigned char) * 4);

	allocBlckNbrLtlEnd[0] = allocatedBlockNumber;
	allocBlckNbrLtlEnd[1] = allocatedBlockNumber >> 8;
	allocBlckNbrLtlEnd[2] = allocatedBlockNumber >> 16;
	allocBlckNbrLtlEnd[3] = allocatedBlockNumber >> 24; //conversao little-endian

	if(blockIndex < 2) {
		//TODO: verificar se o bloco alocado não é -1 (erro)
		inode->dataPtr[blockIndex] = allocatedBlockNumber;
		writeInode(inodeNumber, *inode);
	} else if(blockIndex < 1026) {
		if(blockIndex == 2) {
			inode->singleIndPtr = allocateBlock();
			writeInode(inodeNumber, *inode);
		}
		writeInBlock(inode->singleIndPtr, (blockIndex - 2)*4, allocBlckNbrLtlEnd, 4);
	} else {
		int firstIndIndex = (blockIndex - 1026) / 1024;
		int secondIndIndex = (blockIndex - 1026) - (firstIndIndex * 1024);

		char *firstIndBlock = getBlock(inode->doubleIndPtr);
		int secondIndBlockNumber = getNthIntegerFromBlock(firstIndBlock, firstIndIndex);

		if(blockIndex == 1026) {
			inode->doubleIndPtr = allocateBlock();
			writeInode(inodeNumber, *inode);
		} if((blockIndex - 1026) % 1024 == 0) {
			int secondIndBlockNbr = allocateBlock();

			unsigned char* scndIndBlckNbrLtlEnd = (unsigned char*)malloc(sizeof(char)*4);
			scndIndBlckNbrLtlEnd[0] = secondIndBlockNbr;
			scndIndBlckNbrLtlEnd[1] = secondIndBlockNbr >> 8;
			scndIndBlckNbrLtlEnd[2] = secondIndBlockNbr >> 16;
			scndIndBlckNbrLtlEnd[3] = secondIndBlockNbr >> 24; //conversao little-endian

			writeInBlock(inode->doubleIndPtr, (firstIndIndex)*4, scndIndBlckNbrLtlEnd, 4);
			free(scndIndBlckNbrLtlEnd);
		}
		writeInBlock(secondIndBlockNumber, secondIndIndex*4, allocBlckNbrLtlEnd, 4);
	}
	free(allocBlckNbrLtlEnd);

	return;
}

/*------ FUNCOES DA INTERFACE ------
	Regiao do codigo que possui as funcoes da file_interface e que estao expostas no header.
----------------------------*/
struct t2fs_record* getRecord(char *path) {
	//struct t2fs_inode* rootInode = getInode(0);
	return NULL;
}

int truncateInode(int inodeNumber, int fromBlockIndex) {
	struct t2fs_inode* originalInode = getInode(inodeNumber);
	struct t2fs_inode temporaryInode = *originalInode;
	int currBlockNumber = getNthBlockNumberInInode(*originalInode, fromBlockIndex);

	unsigned char *invalidPtrLtlEnd = (unsigned char*)malloc(sizeof(unsigned char) * 4);

	invalidPtrLtlEnd[0] = INVALID_PTR;
	invalidPtrLtlEnd[1] = INVALID_PTR >> 8;
	invalidPtrLtlEnd[2] = INVALID_PTR >> 16;
	invalidPtrLtlEnd[3] = INVALID_PTR >> 24; //conversao little-endian

	int currBlockIndex = fromBlockIndex;

	while(currBlockNumber != INVALID_PTR) {
		if(currBlockIndex == 1) { //a partir do bloco 2 eh o minimo, pois o primeiro bloco sempre existe
			temporaryInode.dataPtr[1] = INVALID_PTR;
			writeInode(inodeNumber, temporaryInode);
			freeBlock(originalInode->dataPtr[1]);
			freeBlock(currBlockNumber);
		} else if(currBlockIndex < 1026) {
			if(currBlockIndex == 2) {
				temporaryInode.singleIndPtr = INVALID_PTR;
				writeInode(inodeNumber, temporaryInode);
				freeBlock(originalInode->singleIndPtr);
			}

			writeInBlock(originalInode->singleIndPtr, (currBlockIndex - 2)*4, invalidPtrLtlEnd, 4);
			freeBlock(currBlockNumber);
		} else {
			int firstIndIndex = (currBlockIndex - 1026) / 1024;
			int secondIndIndex = (currBlockIndex - 1026) - (firstIndIndex * 1024);

			char *firstIndBlock = getBlock(originalInode->doubleIndPtr);
			int secondIndBlockNumber = getNthIntegerFromBlock(firstIndBlock, firstIndIndex);

			if(currBlockIndex == 1026) {
				temporaryInode.doubleIndPtr = INVALID_PTR;
				writeInode(inodeNumber, temporaryInode);
				freeBlock(originalInode->doubleIndPtr);
			} if((currBlockIndex - 1026) % 1024 == 0) {
				writeInBlock(originalInode->doubleIndPtr, (firstIndIndex)*4, invalidPtrLtlEnd, 4);
				freeBlock(secondIndBlockNumber);
			}

			free(firstIndBlock);
			writeInBlock(secondIndBlockNumber, secondIndIndex*4, invalidPtrLtlEnd, 4);
			freeBlock(currBlockNumber);
		}
		currBlockIndex++;
		currBlockNumber = getNthBlockNumberInInode(*originalInode, currBlockIndex);
	}

	free(invalidPtrLtlEnd);
	return 0;
}

char* readFromInode(int inodeNumber, unsigned int fileHandle, int size) {
	struct t2fs_inode* inode = getInode(inodeNumber);
	int firstBlockIndex = (int)fileHandle / 4096;
	int numberOfBlocks = (size / 4096) + 1;

	char* blocksData = (char*)malloc(sizeof(char)*4096*numberOfBlocks);
	char* finalData = (char*)malloc(sizeof(char)*size);

	int i;
	//le todos os blocos e guarda em blockData
	for(i = firstBlockIndex; i < firstBlockIndex + numberOfBlocks; i++) {
		char* currentBlockData = getNthBlockInInode(*inode, i);
		memcpy(blocksData + (4096*(i - firstBlockIndex)), currentBlockData, 4096);
		free(currentBlockData);
	}
	memcpy(finalData, blocksData + (fileHandle - (4096*firstBlockIndex)), size);

	return finalData;
}

void writeInInode(int inodeNumber, unsigned int fileHandle, unsigned char* data, int size) {
	struct t2fs_inode* inode = getInode(inodeNumber);
	int writtenBytes = 0;

	while(writtenBytes < size) {
		int currentBlockIndex = ((int)fileHandle + writtenBytes) / 4096;
		int currentInBlockIndex = ((int)fileHandle + writtenBytes) - (currentInBlockIndex * 4096);

		int currentBlockNumber = getNthBlockNumberInInode(*inode, currentBlockIndex);
		if(currentBlockNumber == INVALID_PTR) {
			allocateBlockForInode(inodeNumber, currentBlockIndex);
			currentBlockNumber = getNthBlockNumberInInode(*inode, currentBlockIndex);
		}
		char* currentBlock = getNthBlockInInode(*inode, currentBlockIndex);

		int nbOfBytesLeft = size - writtenBytes;
		int spaceLeftInBlock = 4096 - currentInBlockIndex;
		int nbOfBytesToWrite = min(nbOfBytesLeft, spaceLeftInBlock);

		memcpy(currentBlock + currentInBlockIndex, data + writtenBytes, nbOfBytesToWrite);
		writeBlock(currentBlockNumber, currentBlock);
		free(currentBlock);
		writtenBytes += nbOfBytesToWrite;
	}

	return;
}
