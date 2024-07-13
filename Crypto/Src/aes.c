#include "aes.h"
#include "Utils.h"

uint8_t mul(uint8_t a, uint8_t b)
{
    uint8_t sb[4];
    uint8_t out = 0;
    sb[0] = b;
    for (int i = 1; i < 4; i++) {
        sb[i] = sb[i - 1] << 1;
        if (sb[i - 1] & 0x80) {
            sb[i] ^= 0x1b;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (a >> i & 0x01) {
            out ^= sb[i];
        }
    }
    return out;
}


uint8_t* SubWord(uint8_t* word)
{
    for (int i = 0; i < 4; i++) {
        word[i] = sbox[word[i]];
    }
    return word;
}

uint8_t* RotWord(uint8_t* word)
{
    uint8_t tmp[4];
    memcpy(tmp, word, 4);
    for (int i = 0; i < 4; i++) {
        word[i] = tmp[(i + 1) % 4];
    }
    return word;
}

// Round Ops
void SubBytes(uint8_t state[4][4])
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            state[col][row] = sbox[state[col][row]];
        }
    }
}

void InvSubBytes(uint8_t state[4][4])
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            state[col][row] = invsbox[state[col][row]];
        }
    }
}

void ShiftRows(uint8_t state[4][4])
{
    uint8_t tmp[4];
    for (int row = 1; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            tmp[col] = state[(row + col) % 4][row];
        }
        for (int col = 0; col < 4; col++) {
            state[col][row] = tmp[col];
        }
    }
}

void InvShiftRows(uint8_t state[4][4])
{
    uint8_t tmp[4];
    for (int row = 1; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            tmp[(row + col) % 4] = state[col][row];
        }
        for (int col = 0; col < 4; col++) {
            state[col][row] = tmp[col];
        }
    }
}

void MixColumns(uint8_t state[4][4])
{
    uint8_t tmp[4];
    uint8_t matmul[][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            tmp[row] = state[col][row];
        }
        for (int i = 0; i < 4; i++) {
            state[col][i] = 0x00;
            for (int j = 0; j < 4; j++) {
                state[col][i] ^= mul(matmul[i][j], tmp[j]);
            }
        }
    }
}

void InvMixColumns(uint8_t state[4][4])
{
    uint8_t tmp[4];
    uint8_t matmul[][4] = {
        {0x0e, 0x0b, 0x0d, 0x09},
        {0x09, 0x0e, 0x0b, 0x0d},
        {0x0d, 0x09, 0x0e, 0x0b},
        {0x0b, 0x0d, 0x09, 0x0e}
    };
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            tmp[row] = state[col][row];
        }
        for (int i = 0; i < 4; i++) {
            state[col][i] = 0x00;
            for (int j = 0; j < 4; j++) {
                state[col][i] ^= mul(matmul[i][j], tmp[j]);
            }
        }
    }
}


void AddRoundKey(uint8_t state[4][4], uint8_t * key)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            state[col][row] ^= key[col * 4 + row];
        }
    }
}

// The Cipher
void Cipher(const uint8_t* in, uint8_t* out, uint8_t* w, uint8_t Nk, uint8_t Nr)
{
    uint8_t state[Nk][4];
    memcpy(state, in, 4 * Nk);

    AddRoundKey(state, w);
    for (int round = 0; round < Nr; round++) {
        SubBytes(state);
        ShiftRows(state);
        if (round != (Nr - 1))
            MixColumns(state);
        AddRoundKey(state, (uint8_t*)(w + (round + 1) * 16));
    }
    memcpy(out, state, Nk * 4);
}

void InvCipher(const uint8_t* in, uint8_t* out, uint8_t* w, uint8_t Nk, uint8_t Nr)
{
    uint8_t state[Nk][4];
    memcpy(state, in, 16);

    AddRoundKey(state, w + (Nr * 16));
    for (int round = Nr - 1; round >= 0; round--) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, (uint8_t*)(w + round * 16));
        if (round)
            InvMixColumns(state);
    }
    memcpy(out, state, Nk * 4);
}

// Key Expansion
void KeyExpansion(uint8_t* key, uint8_t* w, uint8_t Nk, uint8_t Nr)
{
    uint8_t tmp[4];
    memcpy(w, key, 4 * Nk);

    for (int i = 4 * Nk; i < 4 * (Nr + 1) * 4; i += 4) {
        memcpy(tmp, w + i - 4, 4);
        if (i % (Nk * 4) == 0) {
            SubWord(RotWord(tmp));
            for (int j = 0; j < 4; j++) {
                tmp[j] ^= Rcon[i / Nk + j];
            }
        } else if (Nk > 6 && (i % (Nk * 4)) == 16) {
            SubWord(tmp);
        }
        for (int j = 0; j < 4; j++)
            w[i + j] = w[i - Nk * 4 + j] ^ tmp[j];
    }
}

// Main Functions
uint8_t* aes_128_encrypt(const uint8_t* in, uint8_t* out, uint8_t* key)
{
    uint8_t* w;
    uint8_t Nk = 4, Nr = 10;
    w = (uint8_t*)malloc(16 * (Nr + 1));
    KeyExpansion(key, w, Nk, Nr);
    Cipher(in, out, w, Nk, Nr);
    free(w);
    return out;
}

uint8_t* aes_128_decrypt(const uint8_t* in, uint8_t* out, uint8_t* key)
{
    uint8_t * w;
    uint8_t Nk = 4, Nr = 10;
    w = (uint8_t*)malloc(16 * (Nr + 1));
    KeyExpansion(key, w, Nk, Nr);
    InvCipher(in, out, w, Nk, Nr);
    free(w);
    return out;
}

