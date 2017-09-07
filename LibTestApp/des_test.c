/*
 * Copyright (c) 2017, Intel Corporation
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Intel Corporation nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <mb_mgr.h>
#include <des.h>

#include "gcm_ctr_vectors_test.h"

#ifndef DIM
#define DIM(x) (sizeof(x) / sizeof(x[0]))
#endif

struct des_vector {
	const uint8_t* K;          /* key */
	const uint8_t* IV;         /* initialization vector */
	const uint8_t* P;          /* plain text */
	uint64_t       Plen;       /* plain text length */
	const uint8_t* C;          /* cipher text - same length as plain text */
};

/* CM-SP-SECv3.1-I07-170111 I.7 */
static const uint8_t K1[] = {
        0xe6, 0x60, 0x0f, 0xd8, 0x85, 0x2e, 0xf5, 0xab
};
static const uint8_t IV1[] = {
        0x81, 0x0e, 0x52, 0x8e, 0x1c, 0x5f, 0xda, 0x1a
};
static const uint8_t P1[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x88, 0x41, 0x65, 0x06
};
static const uint8_t C1[] = {
        0x0d, 0xda, 0x5a, 0xcb, 0xd0, 0x5e, 0x55, 0x67,
        0x9f, 0x04, 0xd1, 0xb6, 0x41, 0x3d, 0x4e, 0xed
};

static const uint8_t K2[] = {
        0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e
};
static const uint8_t IV2[] = {
        0x02, 0xa8, 0x11, 0x77, 0x4d, 0xcd, 0xe1, 0x3b
};
static const uint8_t P2[] = {
        0x05, 0xef, 0xf7, 0x00, 0xe9, 0xa1, 0x3a, 0xe5,
        0xca, 0x0b, 0xcb, 0xd0, 0x48, 0x47, 0x64, 0xbd,
        0x1f, 0x23, 0x1e, 0xa8, 0x1c, 0x7b, 0x64, 0xc5,
        0x14, 0x73, 0x5a, 0xc5, 0x5e, 0x4b, 0x79, 0x63,
        0x3b, 0x70, 0x64, 0x24, 0x11, 0x9e, 0x09, 0xdc,
        0xaa, 0xd4, 0xac, 0xf2, 0x1b, 0x10, 0xaf, 0x3b,
        0x33, 0xcd, 0xe3, 0x50, 0x48, 0x47, 0x15, 0x5c,
        0xbb, 0x6f, 0x22, 0x19, 0xba, 0x9b, 0x7d, 0xf5

};
static const uint8_t C2[] = {
        0xf3, 0x31, 0x8d, 0x01, 0x19, 0x4d, 0xa8, 0x00,
        0xa4, 0x2c, 0x10, 0xb5, 0x33, 0xd6, 0xbc, 0x11,
        0x97, 0x59, 0x2d, 0xcc, 0x9b, 0x5d, 0x35, 0x9a,
        0xc3, 0x04, 0x5d, 0x07, 0x4c, 0x86, 0xbf, 0x72,
        0xe5, 0x1a, 0x72, 0x25, 0x82, 0x22, 0x54, 0x03,
        0xde, 0x8b, 0x7a, 0x58, 0x5c, 0x6c, 0x28, 0xdf,
        0x41, 0x0e, 0x38, 0xd6, 0x2a, 0x86, 0xe3, 0x4f,
        0xa2, 0x7c, 0x22, 0x39, 0x60, 0x06, 0x03, 0x6f
};

static struct des_vector vectors[] = {
        {K2, IV2, P2, sizeof(P2), C2},
        {K1, IV1, P1, sizeof(P1), C1},
};

/* CM-SP-SECv3.1-I07-170111 I.7 */
static const uint8_t DK1[] = {
        0xe6, 0x60, 0x0f, 0xd8, 0x85, 0x2e, 0xf5, 0xab
};
static const uint8_t DIV1[] = {
        0x81, 0x0e, 0x52, 0x8e, 0x1c, 0x5f, 0xda, 0x1a
};
static const uint8_t DP1[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x88, 0x41, 0x65, 0x06
};
static const uint8_t DC1[] = {
        0x0d, 0xda, 0x5a, 0xcb, 0xd0, 0x5e, 0x55, 0x67,
        0x9f, 0x04, 0xd1, 0xb6, 0x41, 0x3d, 0x4e, 0xed
};

static const uint8_t DK2[] = {
        0xe6, 0x60, 0x0f, 0xd8, 0x85, 0x2e, 0xf5, 0xab
};
static const uint8_t DIV2[] = {
        0x81, 0x0e, 0x52, 0x8e, 0x1c, 0x5f, 0xda, 0x1a
};
static const uint8_t DP2[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x91,
        0xd2, 0xd1, 0x9f
};
static const uint8_t DC2[] = {
        0x0d, 0xda, 0x5a, 0xcb, 0xd0, 0x5e, 0x55, 0x67,
        0x51, 0x47, 0x46, 0x86, 0x8a, 0x71, 0xe5, 0x77,
        0xef, 0xac, 0x88
};

static const uint8_t DK3[] = {
        0xe6, 0x60, 0x0f, 0xd8, 0x85, 0x2e, 0xf5, 0xab
};
static const uint8_t DIV3[] = {
        0x51, 0x47, 0x46, 0x86, 0x8a, 0x71, 0xe5, 0x77
};
static const uint8_t DP3[] = {
        0xd2, 0xd1, 0x9f
};
static const uint8_t DC3[] = {
        0xef, 0xac, 0x88
};

static struct des_vector docsis_vectors[] = {
        {DK2, DIV2, DP2, sizeof(DP2), DC2},
        {DK3, DIV3, DP3, sizeof(DP3), DC3},
        {DK1, DIV1, DP1, sizeof(DP1), DC1},
};

static int
test_des_many(struct MB_MGR *mb_mgr,
              const uint64_t *ks,
              const void *iv,
              const uint8_t *in_text,
              const uint8_t *out_text,
              unsigned text_len,
              int dir,
              int order,
              JOB_CIPHER_MODE cipher,
              const int in_place,
              const int num_jobs)
{
        struct JOB_AES_HMAC *job;
        uint8_t padding[16];
        uint8_t **targets = malloc(num_jobs * sizeof(void *));
        int i, jobs_rx = 0, ret = -1;

        assert(targets != NULL);

        memset(padding, -1, sizeof(padding));

        for (i = 0; i < num_jobs; i++) {
                targets[i] = malloc(text_len + (sizeof(padding) * 2));
                memset(targets[i], -1, text_len + (sizeof(padding) * 2));
                if (in_place) {
                        /* copy input text to the allocated buffer */
                        memcpy(targets[i] + sizeof(padding), in_text, text_len);
                }
        }

        /* flush the scheduler */
        while ((job = IMB_FLUSH_JOB(mb_mgr)) != NULL)
                ;

        for (i = 0; i < num_jobs; i++) {
                job = IMB_GET_NEXT_JOB(mb_mgr);
                job->cipher_direction = dir;
                job->chain_order = order;
                if (!in_place) {
                        job->dst = targets[i] + sizeof(padding);
                        job->src = in_text;
                } else {
                        job->dst = targets[i] + sizeof(padding);
                        job->src = targets[i] + sizeof(padding);
                }
                job->cipher_mode = cipher;
                job->aes_enc_key_expanded = ks;
                job->aes_dec_key_expanded = ks;
                job->aes_key_len_in_bytes = 8;
                job->iv = iv;
                job->iv_len_in_bytes = 8;
                job->cipher_start_src_offset_in_bytes = 0;
                job->msg_len_to_cipher_in_bytes = text_len;
                job->user_data = (void *)((uint64_t)i);

                job->hash_alg = NULL_HASH;
                job->hashed_auth_key_xor_ipad = NULL;
                job->hashed_auth_key_xor_opad = NULL;
                job->hash_start_src_offset_in_bytes = 0;
                job->msg_len_to_hash_in_bytes = 0;
                job->auth_tag_output = NULL;
                job->auth_tag_output_len_in_bytes = 0;

                job = IMB_SUBMIT_JOB(mb_mgr);
                if (job != NULL) {
                        const int num = (const int)((uint64_t)job->user_data);

                        jobs_rx++;
                        if (job->status != STS_COMPLETED) {
                                printf("%d error status:%d, job %d", __LINE__, job->status, num);
                                goto end;
                        }
                        if (memcmp(out_text, targets[num] + sizeof(padding), text_len)) {
                                printf("%d mismatched\n",num);
                                goto end;
                        }
                        if (memcmp(padding, targets[num], sizeof(padding))) {
                                printf("%d overwrite head\n", num);
                                goto end;
                        }
                        if (memcmp(padding, targets[num] + sizeof(padding) + text_len,  sizeof(padding))) {
                                printf("%d overwrite tail\n", num);
                                goto end;
                        }
                }
        }

        while ((job = IMB_FLUSH_JOB(mb_mgr)) != NULL) {
                const int num = (const int)((uint64_t)job->user_data);

                jobs_rx++;
                if (job->status != STS_COMPLETED) {
                        printf("%d Error status:%d, job %d", __LINE__, job->status, num);
                        goto end;
                }
                if (memcmp(out_text, targets[num] + sizeof(padding), text_len)) {
                        printf("%d mismatched\n",num);
                        goto end;
                }
                if (memcmp(padding, targets[num], sizeof(padding))) {
                        printf("%d overwrite head\n", num);
                        goto end;
                }
                if (memcmp(padding, targets[num] + sizeof(padding) + text_len,  sizeof(padding))) {
                        printf("%d overwrite tail\n", num);
                        goto end;
                }
        }

        if (jobs_rx != num_jobs) {
                printf("Expected %d jobs, received %d\n", num_jobs, jobs_rx);
                goto end;
        }
        ret = 0;

 end:
        for (i = 0; i < num_jobs; i++)
                free(targets[i]);
        free(targets);
        return ret;
}

static int
test_des_one(struct MB_MGR *mb_mgr,
             const uint64_t *ks,
             const void *iv,
             const uint8_t *in_text,
             const uint8_t *out_text,
             unsigned text_len,
             int dir,
             int order,
             JOB_CIPHER_MODE cipher,
             const int in_place)
{
        struct JOB_AES_HMAC *job;
        uint8_t padding[16];
        uint8_t *target = malloc(text_len + (sizeof(padding) * 2));
        int ret = -1;

        assert(target != NULL);
        
        memset(target, -1, text_len + (sizeof(padding) * 2));
        memset(padding, -1, sizeof(padding));

        if (in_place) {
                /* copy input text to the allocated buffer */
                memcpy(target + sizeof(padding), in_text, text_len);
        }

        while ((job = IMB_FLUSH_JOB(mb_mgr)) != NULL)
                ;

        job = IMB_GET_NEXT_JOB(mb_mgr);
        job->cipher_direction = dir;
        job->chain_order = order;
        if (!in_place) {
                job->dst = target + sizeof(padding);
                job->src = in_text;
        } else {
                job->dst = target + sizeof(padding);
                job->src = target + sizeof(padding);
        }
        job->cipher_mode = cipher;
        job->aes_enc_key_expanded = ks;
        job->aes_dec_key_expanded = ks;
        job->aes_key_len_in_bytes = 8;
        job->iv = iv;
        job->iv_len_in_bytes = 8;
        job->cipher_start_src_offset_in_bytes = 0;
        job->msg_len_to_cipher_in_bytes = text_len;

        job->hash_alg = NULL_HASH;
        job->hashed_auth_key_xor_ipad = NULL;
        job->hashed_auth_key_xor_opad = NULL;
        job->hash_start_src_offset_in_bytes = 0;
        job->msg_len_to_hash_in_bytes = 0;
        job->auth_tag_output = NULL;
        job->auth_tag_output_len_in_bytes = 0;

        job = IMB_SUBMIT_JOB(mb_mgr);
        if (job) {
                printf("%d Unexpected return from submit_job\n", __LINE__);
                goto end;
        }
        job = IMB_FLUSH_JOB(mb_mgr);
        if (!job) {
                printf("%d Unexpected null return from flush_job\n", __LINE__);
                goto end;
        }
        if (job->status != STS_COMPLETED) {
                printf("%d Error status:%d", __LINE__, job->status);
                goto end;
        }
        if (memcmp(out_text, target + sizeof(padding), text_len)) {
                printf("mismatched\n");
                goto end;
        }
        if (memcmp(padding, target, sizeof(padding))) {
                printf("overwrite head\n");
                goto end;
        }
        if (memcmp(padding, target + sizeof(padding) + text_len,  sizeof(padding))) {
                printf("overwrite tail\n");
                goto end;
        }
        ret = 0;
        while ((job = IMB_FLUSH_JOB(mb_mgr)) != NULL)
                ;
 end:
        free(target);
        return ret;
}

static int
test_des(struct MB_MGR *mb_mgr,
         const uint64_t *ks,
         const void *iv,
         const uint8_t *in_text,
         const uint8_t *out_text,
         unsigned text_len,
         int dir,
         int order,
         JOB_CIPHER_MODE cipher,
         const int in_place)
{
        int ret = 0;

        ret |= test_des_one(mb_mgr, ks, iv, in_text, out_text, text_len, dir, order, cipher, in_place);
        ret |= test_des_many(mb_mgr, ks, iv, in_text, out_text, text_len, dir, order, cipher, in_place, 32);

        return ret;
}

static int
test_des_vectors(struct MB_MGR *mb_mgr, const int vec_cnt,
                 const struct des_vector *vec_tab, const char *banner,
                 const JOB_CIPHER_MODE cipher)
{
	int vect, errors = 0;
        uint64_t ks[16];

	printf("%s:\n", banner);
	for (vect = 0; vect < vec_cnt; vect++) {
#ifdef DEBUG
		printf("Standard vector %d/%d  PTLen:%d\n",
                       vect + 1, vec_cnt,
                       (int) vec_tab[vect].Plen);
#else
		printf(".");
#endif
                des_key_schedule(ks, vec_tab[vect].K);

                if (test_des(mb_mgr, ks, 
                             vec_tab[vect].IV,
                             vec_tab[vect].P, vec_tab[vect].C,
                             (unsigned) vec_tab[vect].Plen,
                             ENCRYPT, CIPHER_HASH, cipher, 0)) {
                        printf("error #%d encrypt\n", vect + 1);
                        errors++;
                }

                if (test_des(mb_mgr, ks,
                             vec_tab[vect].IV,
                             vec_tab[vect].C, vec_tab[vect].P,
                             (unsigned) vec_tab[vect].Plen,
                             DECRYPT, HASH_CIPHER, cipher, 0)) {
                        printf("error #%d decrypt\n", vect + 1);
                        errors++;
                }

                if (test_des(mb_mgr, ks, 
                             vec_tab[vect].IV,
                             vec_tab[vect].P, vec_tab[vect].C,
                             (unsigned) vec_tab[vect].Plen,
                             ENCRYPT, CIPHER_HASH, cipher, 1)) {
                        printf("error #%d encrypt in-place\n", vect + 1);
                        errors++;
                }

                if (test_des(mb_mgr, ks,
                             vec_tab[vect].IV,
                             vec_tab[vect].C, vec_tab[vect].P,
                             (unsigned) vec_tab[vect].Plen,
                             DECRYPT, HASH_CIPHER, cipher, 1)) {
                        printf("error #%d decrypt in-place\n", vect + 1);
                        errors++;
                }
	}
	printf("\n");
	return errors;
}


int
des_test(const enum arch_type arch,
         struct MB_MGR *mb_mgr)
{
        int errors;

        errors = test_des_vectors(mb_mgr, DIM(vectors), vectors,
                                  "DES standard test vectors", DES);

        errors += test_des_vectors(mb_mgr, DIM(docsis_vectors), docsis_vectors,
                                   "DOCSIS DES standard test vectors", DOCSIS_DES);

	if (0 == errors)
		printf("...Pass\n");
	else
		printf("...Fail\n");
        
	return errors;
}
