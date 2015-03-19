#include "slab.h"
#include <stdlib.h>
#include <stdio.h>


/* global def init */

size_t OBJ_SIZE 	= 0;
size_t PAGE_SIZE 	= 0;
size_t BITMAP_SIZE 	= 0;
size_t BITMAP_TAIL	= 0;

const int SLABS_FULL 	= 0;
const int SLABS_PARTIAL = 1;
const int SLABS_FREE 	= 2;
const int SLAB_COUNT 	= 3;

const int INIT_PAGE_COUNT = 3;
const int INIT_PAGE_INCREMENT = 10;

static uint8_t BITMAP_MASK = 0x01;

/* Global slab cache */

slab_cache_t 	global_cache;

static inline int PAGE_FULL(page_t p){

	int i;
	for(i=0; i < BITMAP_SIZE; i++)
		if(p.bitmap[i] != 0xFF)
			return 0;

	return 1;
}


static page_t page_alloc(){

	page_t page;
	uint8_t* addr = malloc(BITMAP_SIZE + PAGE_SIZE);

	printf("Page alloc\n");

	page.bitmap = addr;
	page.start_address = addr + BITMAP_SIZE;
	page.bitmap[BITMAP_SIZE - 1] |= BITMAP_MASK;

	return page;	
}


void slab_init(size_t sz){

	OBJ_SIZE 	= sz;
	PAGE_SIZE 	= 0x1000;
	PAGE_SIZE  += (PAGE_SIZE % sz);
	BITMAP_SIZE	= ((PAGE_SIZE / OBJ_SIZE) / sizeof(uint8_t)) + 1;
	BITMAP_TAIL = ((PAGE_SIZE / OBJ_SIZE) % sizeof(uint8_t));

	BITMAP_MASK <<= (sizeof(uint8_t) - BITMAP_TAIL);
	BITMAP_MASK -= 1;

	/* Initialize/Allocate slab pages */

	global_cache.slabs[SLABS_FULL].pages = 
				calloc(INIT_PAGE_COUNT,sizeof(page_t));
	global_cache.slabs[SLABS_PARTIAL].pages = 
				calloc(INIT_PAGE_COUNT,sizeof(page_t));
	global_cache.slabs[SLABS_FREE].pages = 
				calloc(INIT_PAGE_COUNT,sizeof(page_t));

	printf("Slab alloc\n");
	printf("Slab alloc\n");
	printf("Slab alloc\n");

	global_cache.slabs[SLABS_FULL].alloc_cnt = INIT_PAGE_INCREMENT;
	global_cache.slabs[SLABS_PARTIAL].alloc_cnt = INIT_PAGE_INCREMENT;
	global_cache.slabs[SLABS_FREE].alloc_cnt = INIT_PAGE_INCREMENT;

	global_cache.slabs[SLABS_FULL].page_cnt = 0;
	global_cache.slabs[SLABS_PARTIAL].page_cnt = 0;
	global_cache.slabs[SLABS_FREE].page_cnt = INIT_PAGE_COUNT;
	
	int i;
	for(i=0; i < INIT_PAGE_COUNT; i++)
		global_cache.slabs[SLABS_FREE].pages[i] = page_alloc();
}

void* slab_alloc(){

	slab_t spartial = global_cache.slabs[SLABS_PARTIAL];
	slab_t sfree = global_cache.slabs[SLABS_FREE];
	slab_t sfull = global_cache.slabs[SLABS_FULL];
	page_t page;

	/* Any pages in the partial slab? */

	if(spartial.page_cnt > 0){

		page = spartial.pages[spartial.page_cnt - 1];
	}


	/* Any pages in the free slab? */

	if(sfree.page_cnt == 0){

	}

	/* Take a free page and move to partial slab */

	page = sfree.pages[sfree.page_cnt - 1];
}

void slab_free(void* addr){

}

void slab_destroy(){

	int i,j;
	for(i=0; i < SLAB_COUNT; i++){

		slab_t slab = global_cache.slabs[i];

		for(j=0; j < slab.page_cnt; j++){
			free(slab.pages[j].bitmap);
			printf("page free\n");
		}

		free(slab.pages);
		printf("Slab free\n");
	}
}
