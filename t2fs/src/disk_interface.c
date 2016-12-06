#include <stdlib.h>
#include <string.h>
#include "../include/apidisk.h"
#include "../include/bitmap2.h"
#include "../include/t2fs.h"
#include "../include/disk_interface.h"

int interfaceInited = 0;
struct t2fs_superbloco superBlock;

unsigned char* intToChar4LtlEnd(int number) {
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)*4);
	result[0] = number;
	result[1] = number >> 8;
	result[2] = number >> 16;
	result[3] = number >> 24;

	return result;
}

DWORD charToInt4LtlEnd(unsigned char* array) {
	return (DWORD)(array[0]) | (DWORD)(array[1] << 8)
							| (DWORD)(array[2] << 16) | (DWORD)(array[3] << 24);
}

unsigned char* shortIntToChar2LtlEnd(short int number) {
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)*2);
	result[0] = number;
	result[1] = number >> 8;

	return result;
}

WORD char2ToShortIntLtlEnd(unsigned char* array) {
	return (WORD)(array[0]) | (WORD)(array[1] << 8);
}

int init() {
    if(!interfaceInited) {
        interfaceInited = 1;
        unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * 256);
        if(read_sector((unsigned int)0, buffer) != 0)
            return -1;

        memcpy(superBlock.id, buffer, 4);
        superBlock.version = char2ToShortIntLtlEnd(buffer+4);
        superBlock.superblockSize = char2ToShortIntLtlEnd(buffer+6);
        superBlock.freeBlocksBitmapSize = char2ToShortIntLtlEnd(buffer+8);
        superBlock.freeInodeBitmapSize = char2ToShortIntLtlEnd(buffer+10);
        superBlock.inodeAreaSize = char2ToShortIntLtlEnd(buffer+12);
        superBlock.blockSize = char2ToShortIntLtlEnd(buffer+14);
        superBlock.diskSize = charToInt4LtlEnd((buffer+16));

        free(buffer);
    }

    return 0;
}

char* getBlock(int blockNumber) {
    if(init() != 0)
        return NULL;

    unsigned int inDiskBlockIndex = (superBlock.superblockSize +
            superBlock.freeBlocksBitmapSize +
            superBlock.freeInodeBitmapSize + superBlock.inodeAreaSize)*256 + blockNumber*4096;
    unsigned int sectorIndex = inDiskBlockIndex / 256;
    unsigned char* resultingBlock = (unsigned char*)malloc(sizeof(unsigned char)*4096);
    unsigned int i;

    for(i = 0; i < superBlock.blockSize; i++) {
        if(read_sector(sectorIndex+i, resultingBlock+(i*256)) != 0)
            return NULL;
    }

    return (char*)resultingBlock;
}

struct t2fs_inode* getInode(unsigned int inodeNumber) {
    if(init() != 0)
        return NULL;

    unsigned int inDiskInodeIndex = (superBlock.superblockSize +
            superBlock.freeBlocksBitmapSize +
            superBlock.freeInodeBitmapSize)*256 + inodeNumber*16;
    unsigned int sectorIndex = inDiskInodeIndex / 256;
    unsigned int inSectorIndex = inDiskInodeIndex - (sectorIndex*256);

    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*256);
    if(read_sector(sectorIndex, buffer) != 0)
        return NULL;

    struct t2fs_inode* inode = (struct t2fs_inode*)malloc(sizeof(struct t2fs_inode));
    inode->dataPtr[0] = (int)charToInt4LtlEnd(buffer+inSectorIndex);
    inode->dataPtr[1] = (int)charToInt4LtlEnd(buffer+inSectorIndex+4);
    inode->singleIndPtr = (int)charToInt4LtlEnd(buffer+inSectorIndex+8);
    inode->doubleIndPtr = (int)charToInt4LtlEnd(buffer+inSectorIndex+12);

    free(buffer);
    return inode;
}

int writeBlock(int blockNumber, char data[4096]) {
    if(init() != 0)
        return -1;

    unsigned int inDiskBlockIndex = (superBlock.superblockSize +
            superBlock.freeBlocksBitmapSize +
            superBlock.freeInodeBitmapSize + superBlock.inodeAreaSize)*256 + blockNumber*4096;
    unsigned int sectorIndex = inDiskBlockIndex / 256;
    unsigned int i;

    for(i = 0; i < superBlock.blockSize; i++) {
        if(write_sector(sectorIndex+i, (unsigned char*)(data+256*i)) != 0)
            return -1;
    }

    return 0;
}

int writeInode(int inodeNumber, struct t2fs_inode inode) {
    if(init() != 0)
        return -1;

    unsigned int inDiskInodeIndex = (superBlock.superblockSize +
            superBlock.freeBlocksBitmapSize +
            superBlock.freeInodeBitmapSize)*256 + inodeNumber*16;
    unsigned int sectorIndex = inDiskInodeIndex / 256;
    unsigned int inSectorIndex = inDiskInodeIndex - (sectorIndex*256);

    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*256);
    if(read_sector(sectorIndex, buffer) != 0)
        return -1;

    unsigned char* dataPtr0 = intToChar4LtlEnd(inode.dataPtr[0]);
    memcpy(buffer+inSectorIndex, dataPtr0, 4);
    free(dataPtr0);

    unsigned char* dataPtr1 = intToChar4LtlEnd(inode.dataPtr[1]);
    memcpy(buffer+inSectorIndex+4, dataPtr1, 4);
    free(dataPtr1);

    unsigned char* dataSingleInd = intToChar4LtlEnd(inode.singleIndPtr);
    memcpy(buffer+inSectorIndex+8, dataSingleInd, 4);
    free(dataSingleInd);

    unsigned char* dataDoubleInd = intToChar4LtlEnd(inode.doubleIndPtr);
    memcpy(buffer+inSectorIndex+12, dataDoubleInd, 4);
    free(dataDoubleInd);

    if(write_sector(sectorIndex, buffer) != 0)
        return -1;

    free(buffer);
    return 0;
}

int writeInBlock(int blockNumber, int initialByte, unsigned char* data, int size) {
    char* block = getBlock(blockNumber);
    if(block == NULL)
        return -1;

    memcpy(block+initialByte, data, size);
    return writeBlock(blockNumber, block);
}

int allocateBlock() {
    int allocBlockNumber = searchBitmap2(1,0);
    // se der problema pode ser aqui
    if(allocBlockNumber >= 0)
        setBitmap2(1,allocBlockNumber,1);

    return allocBlockNumber;
}

int allocateInode() {
    int allocInodeNumber = searchBitmap2(0,0);
    // se der problema pode ser aqui
    if(allocInodeNumber >= 0)
        setBitmap2(0,allocInodeNumber,1);

    return allocInodeNumber;
}

int freeBlock(int blockNumber) {
    return (setBitmap2(1,blockNumber,0));
}

int freeInode(int inodeNumber) {
    return (setBitmap2(0,inodeNumber,0));
}
