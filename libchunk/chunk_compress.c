#define BITS_UNSIGNED_CHAR 8
#define N4095 4095
#include "chunk.h"
#include <stdlib.h>

void small_case(unsigned char* code, int* length, int c, int last_block) {
    code[(*length)] |= (last_block << (BITS_UNSIGNED_CHAR - 2));
    code[(*length)] |= c;
}

void medium_case(unsigned char* code, int* length, int c, int last_block) {
    code[(*length)] |= (last_block << (BITS_UNSIGNED_CHAR - 2));
    code[(*length)] |= (1 << (BITS_UNSIGNED_CHAR - 3));
    code[(*length)] |= (c >> BITS_UNSIGNED_CHAR);
    (*length)++;
    code[(*length)] |= (c & ((1 << BITS_UNSIGNED_CHAR) - 1));
}

void big_case(unsigned char* code, int* length, int c, int last_block) {
    while (c > N4095) {
        code[(*length)] |= (last_block << (BITS_UNSIGNED_CHAR - 2));
        code[(*length)] |= (1 << (BITS_UNSIGNED_CHAR - 3));
        code[(*length)] |= ((1 << (BITS_UNSIGNED_CHAR - 4)) - 1);
        (*length)++;
        code[(*length)] |= ((1 << BITS_UNSIGNED_CHAR) - 1);
        (*length)++;
        c -= N4095;
    }
    int nr_biti = 0, cc = c;
    while (cc != 0) {
        nr_biti++;
        cc >>= 1;
    }
    if (nr_biti < BITS_UNSIGNED_CHAR - 3) {
        small_case(code, length, c, last_block);
    } else if (nr_biti <= BITS_UNSIGNED_CHAR * 2 - 4) {
        medium_case(code, length, c, last_block);
    }
}

unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length) {
        (*length) = 0;
        unsigned char* code = calloc(width * height * depth, sizeof(unsigned char));
        int c = 0;
        int last_block = chunk[0][0][0];
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                for (int x = 0; x < width; x++) {
                    if (chunk[x][y][z] == last_block) {
                        c++;
                    } else {
                        int nr_biti = 0, cc = c;
                        while (cc != 0) {
                            nr_biti++;
                            cc >>= 1;
                        }
                        if (nr_biti <= BITS_UNSIGNED_CHAR - 3) {
                            small_case(code, length, c, last_block);
                        } else if (nr_biti <= BITS_UNSIGNED_CHAR * 2 - 4) {
                            medium_case(code, length, c, last_block);
                        } else {
                            big_case(code, length, c, last_block);
                        }
                        (*length)++;
                        c = 1;
                    }
                    last_block = chunk[x][y][z];
                }
            }
        }
    int nr_biti = 0, cc = c;
    while (cc != 0) {
        nr_biti++;
        cc >>= 1;
    }
    if (nr_biti <= BITS_UNSIGNED_CHAR - 3) {
        small_case(code, length, c, last_block);
    } else if (nr_biti <= BITS_UNSIGNED_CHAR * 2 - 4) {
        medium_case(code, length, c, last_block);
    } else {
        big_case(code, length, c, last_block);
    }
    (*length)++;
    return code;
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {
        char*** chunk = (char***)calloc(width, sizeof(char**));
        for (int x = 0; x < width; x++) {
            chunk[x] = (char**)calloc(height, sizeof(char*));
            for (int y = 0; y < height; y++) {
                chunk[x][y] = (char*)calloc(depth, sizeof(char));
            }
        }
        int c = 0, x = 0, y = 0, z = 0;
        while (y != height) {
            char last_block = (char)(code[c] >> (BITS_UNSIGNED_CHAR - 2));
            if ((code[c] >> (BITS_UNSIGNED_CHAR - 3)) % 2 == 0) {
                for (int i = 0; i < (code[c] & ((1 << (BITS_UNSIGNED_CHAR - 2)) - 1)); i++) {
                    chunk[x][y][z] = last_block;
                    x++;
                    if (x == width) {
                        x = 0;
                        z++;
                    }
                    if (z == depth) {
                        x = 0;
                        z = 0;
                        y++;
                    }
                    if (y == height) {
                        break;
                    }
                }
            } else if ((code[c] >> (BITS_UNSIGNED_CHAR - 3)) % 2 == 1) {
                int c1 = code[c] & ((1 << (BITS_UNSIGNED_CHAR - 3)) - 1);
                c++;
                int c2 = code[c];
                for (int i = 0; i < (c1 << BITS_UNSIGNED_CHAR) + c2; i++) {
                    chunk[x][y][z] = last_block;
                    x++;
                    if (x == width) {
                        x = 0;
                        z++;
                    }
                    if (z == depth) {
                        x = 0;
                        z = 0;
                        y++;
                    }
                    if (y == height) {
                        break;
                    }
                }
            }
            c++;
        }
    return chunk;
}

