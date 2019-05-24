// Copyright (c) 2018, The NERVA Project
// Copyright (c) 2018, The Masari Project
// Copyright (c) 2014-2018, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "hash-ops.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "int-util.h"
#include "oaes_lib.h"

#if defined(_MSC_VER)
#define THREADV __declspec(thread)
#else
#define THREADV __thread
#endif

#define MEMORY 131072
#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE 32
#define INIT_SIZE_BLK 8

#define U64(x) ((uint64_t *)(x))

static size_t e2i(const uint8_t *a, size_t count) { return (*((uint64_t *)a) / AES_BLOCK_SIZE) & (count - 1); }

extern void aesb_single_round(const uint8_t *in, uint8_t *out, const uint8_t *expandedKey);
extern void aesb_pseudo_round(const uint8_t *in, uint8_t *out, const uint8_t *expandedKey);

static void mul(const uint8_t *a, const uint8_t *b, uint8_t *res)
{
    uint64_t a0, b0;
    uint64_t hi, lo;

    a0 = SWAP64LE(((uint64_t *)a)[0]);
    b0 = SWAP64LE(((uint64_t *)b)[0]);
    lo = mul128(a0, b0, &hi);
    ((uint64_t *)res)[0] = SWAP64LE(hi);
    ((uint64_t *)res)[1] = SWAP64LE(lo);
}

static void sum_half_blocks(uint8_t *a, const uint8_t *b)
{
    uint64_t a0, a1, b0, b1;

    a0 = SWAP64LE(((uint64_t *)a)[0]);
    a1 = SWAP64LE(((uint64_t *)a)[1]);
    b0 = SWAP64LE(((uint64_t *)b)[0]);
    b1 = SWAP64LE(((uint64_t *)b)[1]);
    a0 += b0;
    a1 += b1;
    ((uint64_t *)a)[0] = SWAP64LE(a0);
    ((uint64_t *)a)[1] = SWAP64LE(a1);
}

static void copy_block(uint8_t *dst, const uint8_t *src)
{
    memcpy(dst, src, AES_BLOCK_SIZE);
}

static void swap_blocks(uint8_t *a, uint8_t *b)
{
    uint64_t t[2];
    U64(t)[0] = U64(a)[0];
    U64(t)[1] = U64(a)[1];
    U64(a)[0] = U64(b)[0];
    U64(a)[1] = U64(b)[1];
    U64(b)[0] = U64(t)[0];
    U64(b)[1] = U64(t)[1];
}

static void xor_blocks(uint8_t *a, const uint8_t *b)
{
    size_t i;
    for (i = 0; i < AES_BLOCK_SIZE; i++)
        a[i] ^= b[i];
}

#pragma pack(push, 1)
union cn_slow_hash_state {
    union hash_state hs;
    struct
    {
        uint8_t k[64];
        uint8_t init[128];
    };
};
#pragma pack(pop)

void cn_slow_hash(const void *data, size_t length, char *hash, int prehashed)
{
    uint8_t* hp_state = (uint8_t *)malloc(MEMORY);
    uint32_t init_size_blk = INIT_SIZE_BLK;
    union cn_slow_hash_state state;
    uint32_t init_size_byte = (init_size_blk * AES_BLOCK_SIZE);
    uint8_t *text = (uint8_t *)malloc(init_size_byte);
    uint8_t a[AES_BLOCK_SIZE];
    uint8_t b[AES_BLOCK_SIZE];
    uint8_t c1[AES_BLOCK_SIZE];
    uint8_t c2[AES_BLOCK_SIZE];
    uint8_t d[AES_BLOCK_SIZE];
    size_t i, j;
    uint8_t aes_key[AES_KEY_SIZE];
    oaes_ctx *aes_ctx;
    static void (*const extra_hashes[4])(const void *, size_t, char *) = {
    hash_extra_blake, hash_extra_groestl, hash_extra_jh, hash_extra_skein};

    if (prehashed)
        memcpy(&state.hs, data, length);
    else
        hash_process(&state.hs, data, length);
    
    memcpy(text, state.init, init_size_byte);
    memcpy(aes_key, state.hs.b, AES_KEY_SIZE);
    aes_ctx = (oaes_ctx *) oaes_alloc();

    oaes_key_import_data(aes_ctx, aes_key, AES_KEY_SIZE);
    for (i = 0; i < MEMORY / init_size_byte; i++) {
        for (j = 0; j < INIT_SIZE_BLK; j++) {
            aesb_pseudo_round(&text[AES_BLOCK_SIZE * j], &text[AES_BLOCK_SIZE * j], aes_ctx->key->exp_data);
        }
        memcpy(&hp_state[i * init_size_byte], text, init_size_byte);
    }

    for (i = 0; i < AES_BLOCK_SIZE; i++)
    {
        a[i] = state.k[i] ^ state.k[AES_BLOCK_SIZE * 2 + i];
        b[i] = state.k[AES_BLOCK_SIZE + i] ^ state.k[AES_BLOCK_SIZE * 3 + i];
    }

    for (i = 0; i < 0x10000; i++)
    {
        j = e2i(a, MEMORY / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        copy_block(c1, &hp_state[j]);
        aesb_single_round(c1, c1, a);
        copy_block(&hp_state[j], c1);
        xor_blocks(&hp_state[j], b);
        j = e2i(c1, MEMORY / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        copy_block(c2, &hp_state[j]);
        mul(c1, c2, d);
        swap_blocks(a, c1);
        sum_half_blocks(c1, d);
        swap_blocks(c1, c2);
        xor_blocks(c1, c2);
        copy_block(&hp_state[j], c2);
        copy_block(b, a);
        copy_block(a, c1);
    }

    memcpy(text, state.init, init_size_byte);          
    oaes_key_import_data(aes_ctx, &state.hs.b[32], AES_KEY_SIZE);
    for (i = 0; i < MEMORY / init_size_byte; i++)
    {
        for (j = 0; j < init_size_blk; j++)            
        {
            xor_blocks(&text[j * AES_BLOCK_SIZE], &hp_state[i * init_size_byte + j * AES_BLOCK_SIZE]);
            aesb_pseudo_round(&text[AES_BLOCK_SIZE * j], &text[AES_BLOCK_SIZE * j], aes_ctx->key->exp_data);
        }
    }
    memcpy(state.init, text, init_size_byte);          
    hash_permutation(&state.hs);                       
    extra_hashes[state.hs.b[0] & 3](&state, 200, hash);
    oaes_free((OAES_CTX **)&aes_ctx);
    free(text);
    free(hp_state);
}
