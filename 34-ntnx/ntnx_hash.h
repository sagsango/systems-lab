#ifndef NTNX_HASH_H
#define NTNX_HASH_H

#include <stddef.h>

#define NTNX_HASH_BUF_LEN 33
#define NTNX_HASH_LEN 32
/*
 * Device context returned during ntnx_hash_setup() call
 */
typedef struct ntnx_hash_context ntnx_hash_t;

/*
 * ntnx_hash_setup - setup the library for hash generation
 *
 * Setup the context before the use
 *
 * Return: device context pointer (ntnx_hash_t*)
 */
ntnx_hash_t *ntnx_hash_setup(void);

/*
 * ntnx_hash_compute - compute the hash of the given buffer
 * @ctx: device context pointer return from ntnx_hash_setup
 * @buf: pointer to data buffer to hash
 * @len: length of data buffer
 *
 * User program will call this with the context
 * returned during the ntnx_hash_setup
 * check ntnx_hash.c function comments
 *
 * Return: pointer to 33 bytes null terminated hash buffer,
 *         or NULL on failure.
 */
char *ntnx_hash_compute(ntnx_hash_t * ctx, void * buf, size_t len);

/*
 * ntnx_hash_destroy - destroy the device context returned by ntnx_hash_setup()
 * @ctx - device context pointer returned by ntnx_hash_setup()
 *
 * After the use user program will destroy the context
 * check ntnx_hash.c function definition comments for more details
 *
 * Return: 0 on sucess -1 on failure
 */
int ntnx_hash_destroy(ntnx_hash_t *ctx);

#endif
