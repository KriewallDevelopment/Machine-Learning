#ifndef _SLAB_ALLOC_HEADER
#define _SLAB_ALLOC_HEADER

#ifdef __cplusplus
extern "C" {
#endif


#include "stddef.h"
#include <inttypes.h>


#ifndef NULL
#define NULL ((void*)0)
#endif


extern size_t OBJ_SIZE;


/* Virtual Page representation */

extern size_t PAGE_SIZE;
extern size_t BITMAP_SIZE;
extern size_t BITMAP_TAIL;

typedef struct {

	uint8_t* start_address;
	uint8_t* bitmap;

} page_t;


/* Slab representation */

const int INIT_PAGE_COUNT;
const int INIT_PAGE_INCREMENT;

typedef struct {

	/* list of pages in this slab */

	page_t* pages;

	/* Size of the array */

	int alloc_cnt;

	/* Actual pages in use */

	int page_cnt;

} slab_t;


/* A cache containing 3 slabs indexed by constants */

extern const int SLABS_FULL;
extern const int SLABS_PARTIAL;
extern const int SLABS_FREE;
extern const int SLAB_COUNT;

typedef struct {

	slab_t slabs[3];

} slab_cache_t;


/* Init the slab allocator with cached object size */

void slab_init(size_t sz);

/* Get a pointer to a block of memory of size OBJ_SIZE */

void* slab_alloc();

/* Function to free an allocated object */

void slab_free(void*);

/* clean up */

void slab_destroy();


#ifdef __cplusplus
}
#endif

#endif	/* _SLAB_ALLOC_HEADER */
