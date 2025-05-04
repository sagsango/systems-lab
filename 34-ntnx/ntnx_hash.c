#include "ntnx_hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>

#define NTNX_HASH_GET_API_VERSION 0
#define NTNX_HASH_COMPUTE         1
#define NTNX_SUPPORTED_API_VERSION 1
#define NTNX_DEVICE_PATH "/dev/ntnx_hash"

/*
 * struct ntnx_hash_compute  - Arguments for NTNX_HASH_COMPUTE ioctl
 * @buf: pointer to the input buffer containing the data to be hashed
 * @len: length of the data to be hashed, present in buf
 * @hash: pointer to the 33 bytes buffer which contains the hash value
 *        terminated by the NULL, it is allocated on the heap by this
 *        library and expected to be freed by the user application
 */
struct ntnx_hash_compute {
	void *buf;
	size_t len;
	void *hash;
};

/*
 * struct ntnx_hash_context - Context return during ntnx_hash_setup()
 * @fd: file descripter for /dev/ntnx_hash device
 * @lock: mutex used to serialize the ioctl access on given context
 *
 * Context return during ntnx_set_hash
 *
 * XXX: "The device driver expects invocations of this ioctl() to be serialised
 * within a context." - This is given so we dont bother the serialization
 * across the context.
 *
 *
 * This is intersting because multiple threads will share the same virual memory
 * and FD-table too, and lock will be present in the heap.
 * [multiple task_struct sharing the same mm_struct].
 *
 * (Because device deriver dont want the ioctl to be serialized accross the
 * context the problem is much esier because within a context eveyone have same
 * pthread_mutex lock in the heap. Otherwise we could have used named semaphore
 * so that evey process & thread calling ioctl will be serialised)
 */
struct ntnx_hash_context {
	int fd;
	pthread_mutex_t lock;
};

/*
 * ntnx_hash_setup - setup the library for hash generation
 *
 * An application must first create a context by calling ntnx_hash_setup()
 * Multithreaded applications should be able to share a context across threads
 *
 * Return : Pointer to device context which can be shared between multiple threads
 *	    or NULL in case of failure.
 */
ntnx_hash_t *ntnx_hash_setup(void) {
	int _errno;
	ntnx_hash_t *ctx = malloc(sizeof(ntnx_hash_t));
	if (!ctx) {
		/* glibc will set errno on malloc failure */
		return NULL;
	}

	ctx->fd = open(NTNX_DEVICE_PATH, O_RDWR);
	if (ctx->fd < 0) {
		/* glibc will set errno on open failure */
		free(ctx); /* free() does not modify the errno */
		return NULL;
	}

	unsigned int version = 0;
	if (ioctl(ctx->fd, NTNX_HASH_GET_API_VERSION, &version) < 0) {
		/* glibc will set errno on ioctl failure */
		_errno = errno;
		close(ctx->fd);
		free(ctx);
		errno = _errno;
		return NULL;
	}


	if (version != NTNX_SUPPORTED_API_VERSION) {
		close(ctx->fd);
		free(ctx);
		errno = ENOTSUP; /* ENOTSUP 95 Operation not supported */
		return NULL;
	}

	if (pthread_mutex_init(&ctx->lock, NULL) != 0) {
		_errno = errno; /* glibc will set errno on pthread_mutex_init failure */
		close(ctx->fd);
		free(ctx);
		errno = _errno;
		return NULL;
	}

	return ctx;
}

/*
 * ntnx_hash_compute - compute the hash of the given buffer
 * @ctx: device context pointer return from ntnx_hash_setup
 * @buf: pointer to data buffer to hash
 * @len: length of data buffer
 *
 * Return: pointer to 33 bytes null terminated hash buffer,
 *         or NULL on failure.
 */
char *ntnx_hash_compute(ntnx_hash_t * ctx, void * buf, size_t len) {
	int _errno;
	if (!ctx || !buf || len == 0 || ctx->fd < 0) {
		errno = EINVAL; /* EINVAL 22 Invalid argument */
		return NULL;
	}

	/* device will create 32 byte string + NULL at the end */
	char *hash = malloc(NTNX_HASH_BUF_LEN);
	/* NOTE: device will terminate the hash buffer by NULL, so no need to
	 * init the hash buffer here */
	if (!hash) {
		/* glibc will set errno on malloc failure */
		return NULL;
	}

	struct ntnx_hash_compute req = {
		.buf = buf,
		.len = len,
		.hash = hash
	};

	if (pthread_mutex_lock(&ctx->lock) != 0) {
		/* glibc will set errno on pthread_mutex_lock failure */
		free(hash); /* free() does not modify the errno */
		return NULL;
	}

	if (ioctl(ctx->fd, NTNX_HASH_COMPUTE, &req) < 0) {
		_errno = errno; /* glibc will set errno on ioctl failure */
		pthread_mutex_unlock(&ctx->lock);
		free(hash);
		errno = _errno;
		return NULL;
	}

	if (pthread_mutex_unlock(&ctx->lock) != 0) {
		/* glibc will set errno on pthread_mutex_unlock failure */
		free(hash); /* free() does not modify the errno */
		return NULL;
	}
	return hash;
}

/*
 * ntnx_hash_destroy - destroy the device context returned by ntnx_hash_setup()
 * @ctx - device context pointer returned by ntnx_hash_setup()
 *
 * Return: 0 on sucess -1 on failure
 */
int ntnx_hash_destroy(ntnx_hash_t *ctx) {
	if (!ctx || ctx->fd < 0) {
		errno = EINVAL;	 /* EINVAL 22 Invalid argument */
		return -1;
	}

	if (close(ctx->fd) < 0) {
		/* glibc will set errno on close failure */
		return -1;
	}

	if (pthread_mutex_destroy(&ctx->lock) != 0) {
		/* glibc will set errno on pthread_mutex_destroy failure */
		return -1;
	}
	free(ctx);
	return 0;
}
