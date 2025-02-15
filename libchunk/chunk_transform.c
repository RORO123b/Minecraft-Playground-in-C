#include "chunk.h"
#include <limits.h>
#include <stdlib.h>
#define MAX_SIZE 100000

char*** chunk_rotate_y(char*** chunk, int width, int height, int depth) {
    char*** cchunk = (char***)malloc(depth * sizeof(char**));
    for (int x = 0; x < depth; x++) {
        cchunk[x] = (char**)malloc(height * sizeof(char*));
        for (int y = 0; y < height; y++) {
            cchunk[x][y] = (char*)malloc(width * sizeof(char));
        }
    }

    for (int x = 0; x < depth; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < width; z++) {
                cchunk[x][y][z] = chunk[z][y][depth - x - 1];
            }
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(chunk[x][y]);
        }
        free(chunk[x]);
    }
    free(chunk);

    return cchunk;
}

void copyChunk(char*** chunk, char*** chunk_dest, int width, int height, int depth) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                chunk_dest[x][y][z] = chunk[x][y][z];
            }
        }
    }
}

typedef struct {
    int x, y, z;
    char type;
} block;

void markMegaObject(char*** chunk, int width, int height, int depth, int x, int y, int z,
    block blocks[], int *nr_blocks) {
        if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth)
            return;
        if (chunk[x][y][z] != BLOCK_AIR && chunk[x][y][z] != CHAR_MAX - 1) {
            blocks[*nr_blocks].x = x;
            blocks[*nr_blocks].y = y;
            blocks[*nr_blocks].z = z;
            blocks[*nr_blocks].type = chunk[x][y][z];
            (*nr_blocks)++;
            chunk[x][y][z] = CHAR_MAX - 1;
            markMegaObject(chunk, width, height, depth, x + 1, y, z, blocks, nr_blocks);
            markMegaObject(chunk, width, height, depth, x - 1, y, z, blocks, nr_blocks);
            markMegaObject(chunk, width, height, depth, x, y + 1, z, blocks, nr_blocks);
            markMegaObject(chunk, width, height, depth, x, y - 1, z, blocks, nr_blocks);
            markMegaObject(chunk, width, height, depth, x, y, z + 1, blocks, nr_blocks);
            markMegaObject(chunk, width, height, depth, x, y, z - 1, blocks, nr_blocks);
        }
}

void markObject(char*** chunk, int width, int height, int depth, int x, int y, int z,
    char oldBlock, block blocks[], int *nr_blocks) {
        if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth)
            return;
        if (chunk[x][y][z] == oldBlock) {
            blocks[*nr_blocks].x = x;
            blocks[*nr_blocks].y = y;
            blocks[*nr_blocks].z = z;
            blocks[*nr_blocks].type = chunk[x][y][z];
            (*nr_blocks)++;
            chunk[x][y][z] = CHAR_MAX - 1;
            markObject(chunk, width, height, depth, x + 1, y, z, oldBlock, blocks, nr_blocks);
            markObject(chunk, width, height, depth, x - 1, y, z, oldBlock, blocks, nr_blocks);
            markObject(chunk, width, height, depth, x, y + 1, z, oldBlock, blocks, nr_blocks);
            markObject(chunk, width, height, depth, x, y - 1, z, oldBlock, blocks, nr_blocks);
            markObject(chunk, width, height, depth, x, y, z + 1, oldBlock, blocks, nr_blocks);
            markObject(chunk, width, height, depth, x, y, z - 1, oldBlock, blocks, nr_blocks);
        }
}

void moveObject(char*** chunk, int width, int height, int depth, block blocks[], int nr_blocks) {
    for (int i = 0; i < nr_blocks; i++) {
        chunk[blocks[i].x][blocks[i].y][blocks[i].z] = BLOCK_AIR;
    }
    for (int i = 0; i < nr_blocks; i++) {
        int x = blocks[i].x;
        int y = blocks[i].y;
        int z = blocks[i].z;
        chunk[x][y - 1][z] = blocks[i].type;
    }
}

void esteSubMegaObject(char*** chunk, int* rez, block blocks[], int nr_blocks) {
    for (int i = 0; i < nr_blocks; i++) {
        int y = blocks[i].y;
        if (y == 0) {
            *rez = 0;
            return;
        }
    }
}

void esteSubObject(char*** chunk, int* rez, block blocks[], int nr_blocks) {
    for (int i = 0; i < nr_blocks; i++) {
        int x = blocks[i].x;
        int y = blocks[i].y;
        int z = blocks[i].z;
        int type = blocks[i].type;
        if (y == 0) {
            *rez = 0;
            return;
        }
        if (y >= 1 && chunk[x][y - 1][z] != BLOCK_AIR && chunk[x][y - 1][z] != type) {
            *rez = 0;
            return;
        }
    }
}

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {
        char*** cchunk = (char***)malloc(width * sizeof(char**));
        for (int x = 0; x < width; x++) {
            cchunk[x] = (char**)malloc(height * sizeof(char*));
            for (int y = 0; y < height; y++) {
                cchunk[x][y] = (char*)malloc(depth * sizeof(char));
            }
        }
        copyChunk(chunk, cchunk, width, height, depth);

        int cazut = 0;
        while (cazut == 0) {
            cazut = 1;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    for (int z = 0; z < depth; z++) {
                        if (chunk[x][y][z] == BLOCK_AIR || chunk[x][y][z] > BLOCK_STONE)
                            continue;
                        int rez = 1;
                        block blocks[MAX_SIZE];
                        int nr_blocks = 0;
                        markMegaObject(cchunk, width, height, depth, x, y, z, blocks, &nr_blocks);
                        copyChunk(chunk, cchunk, width, height, depth);
                        esteSubMegaObject(chunk, &rez, blocks, nr_blocks);
                        if (rez == 1) {
                            cazut = 0;
                            moveObject(chunk, width, height, depth, blocks, nr_blocks);
                            copyChunk(chunk, cchunk, width, height, depth);
                        }
                    }
                }
            }
        }

        cazut = 0;
        while (cazut == 0) {
            cazut = 1;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    for (int z = 0; z < depth; z++) {
                        if (chunk[x][y][z] == BLOCK_AIR || chunk[x][y][z] > BLOCK_STONE)
                            continue;
                        int rez = 1;
                        block blocks[MAX_SIZE];
                        int nr_blocks = 0;
                        markObject(cchunk, width, height, depth, x, y, z, cchunk[x][y][z], blocks, &nr_blocks);
                        copyChunk(chunk, cchunk, width, height, depth);
                        esteSubObject(chunk, &rez, blocks, nr_blocks);
                        if (rez == 1) {
                            cazut = 0;
                            moveObject(chunk, width, height, depth, blocks, nr_blocks);
                            copyChunk(chunk, cchunk, width, height, depth);
                        }
                    }
                }
            }
        }
        int ok = 1;
        while (ok == 1) {
            for (int x = 0; x < width; x++) {
                for (int z = 0; z < depth; z++) {
                    if (chunk[x][(*new_height) - 1][z] != BLOCK_AIR) {
                        ok = 0;
                    }
                }
            }
            if (ok == 1) {
                (*new_height)--;
            }
        }
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                free(cchunk[x][y]);
            }
            free(cchunk[x]);
        }
        free(cchunk);
        return chunk;
}

