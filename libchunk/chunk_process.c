#define DIRECTIONS 18
#define MAX_CHUNK_SIZE 1000
#include "chunk.h"

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {
        int dx[DIRECTIONS] = {1, -1, 0, 0, 0, 0, 1, 1, 1, 1, -1, -1, -1, -1, 0, 0, 0, 0};
        int dy[DIRECTIONS] = {0, 0, 1, -1, 0, 0, 1, -1, 0, 0, 1, -1, 0, 0, 1, 1, -1, -1};
        int dz[DIRECTIONS] = {0, 0, 0, 0, 1, -1, 0, 0, 1, -1, 0, 0, 1, -1, 1, -1, 1, -1};

        typedef struct {
            int x, y, z;
        } block;
        block target_blocks[MAX_CHUNK_SIZE];
        int c = 0;

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < depth; k++) {
                    if (chunk[i][j][k] == target_block) {
                        target_blocks[c].x = i;
                        target_blocks[c].y = j;
                        target_blocks[c].z = k;
                        c++;
                    }
                }
            }
        }

        for (int i = 0; i < c; i++) {
            for (int l = 0; l < DIRECTIONS; l++) {
                int x = target_blocks[i].x + dx[l];
                int y = target_blocks[i].y + dy[l];
                int z = target_blocks[i].z + dz[l];
                if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
                    if (chunk[x][y][z] != target_block) {
                        chunk[x][y][z] = shell_block;
                    }
                }
            }
        }
        return chunk;
}

void fillxz(char*** chunk, int depth, int width, int x, int y, int z, char oldBlock, char newBlock) {
    if (x < 0 || x >= width || z < 0 || z >= depth)
        return;
    if (chunk[x][y][z] == oldBlock) {
        chunk[x][y][z] = newBlock;
        fillxz(chunk, depth, width, x + 1, y, z, oldBlock, newBlock);
        fillxz(chunk, depth, width, x - 1, y, z, oldBlock, newBlock);
        fillxz(chunk, depth, width, x, y, z + 1, oldBlock, newBlock);
        fillxz(chunk, depth, width, x, y, z - 1, oldBlock, newBlock);
    }
}

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
        fillxz(chunk, depth, width, x, y, z, chunk[x][y][z], block);
        return chunk;
}

void fill3D(char*** chunk, int width, int height, int depth, int x, int y, int z, char oldBlock, char newBlock) {
    if (x < 0 || x >= width || z < 0 || z >= depth || y < 0 || y >= height)
        return;
    if (chunk[x][y][z] == oldBlock) {
        chunk[x][y][z] = newBlock;
        fill3D(chunk, width, height, depth, x + 1, y, z, oldBlock, newBlock);
        fill3D(chunk, width, height, depth, x - 1, y, z, oldBlock, newBlock);
        fill3D(chunk, width, height, depth, x, y + 1, z, oldBlock, newBlock);
        fill3D(chunk, width, height, depth, x, y - 1, z, oldBlock, newBlock);
        fill3D(chunk, width, height, depth, x, y, z + 1, oldBlock, newBlock);
        fill3D(chunk, width, height, depth, x, y, z - 1, oldBlock, newBlock);
    }
}
char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    fill3D(chunk, width, height, depth, x, y, z, chunk[x][y][z], block);
    return chunk;
}
