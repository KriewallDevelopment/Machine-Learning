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

static inline int PAGE_EMPTY(page_t p){

	int i;
	for(i=0; i < BITMAP_SIZE - 1; i++)
		if(p.bitmap[i] != 0x00)
			return 0;

	return p.bitmap[BITMAP_SIZE - 1] == BITMAP_MASK;
}

static inline void set_bit(uint8_t* bm, int idx, int val){

	int arridx = idx / 8;
	uint8_t mask = 0x01 << (7 - idx);
	
	if(val)
		bm[arridx] |= mask;
	else
		bm[arridx] &= (~mask);
}

static page_t page_alloc(){

	page_t page;
	uint8_t* addr = malloc(BITMAP_SIZE + PAGE_SIZE);

	//printf("Page alloc: %p, size %i\n", addr, BITMAP_SIZE + PAGE_SIZE);

	page.bitmap = addr;
	page.start_address = addr + BITMAP_SIZE;
	page.bitmap[BITMAP_SIZE - 1] |= BITMAP_MASK;

	return page;	
}

static inline void add_page_to_slab(slab_t* s, page_t* p){

	s->pages[s->page_cnt] = *p;
	s->page_cnt++;
}

static inline void remove_page_from_slab(slab_t* s, page_t* p){


	page_t* itr = s->pages;
	int idx = 0;

	while(itr++ != p && idx++ < s->page_cnt);

	if(idx == s->page_cnt)
		return;
	else if(idx != s->page_cnt - 1){

		int i;
		for(i=idx; i<s->page_cnt-1; i++)
			s->pages[i] = s->pages[i+1];

		p = s->pages + (s->page_cnt - 1);
	}

	s->page_cnt--;
	p->start_address = NULL;
	p->bitmap = NULL;
}

static void assert_slab_alloc(slab_t* s){

	size_t alloc_sz = INIT_PAGE_INCREMENT * sizeof(page_t);
	size_t old_sz = s->alloc_cnt * sizeof(page_t);

	if(s->page_cnt == s->alloc_cnt){

		s->pages = realloc(s->pages, old_sz + alloc_sz);
		s->alloc_cnt += INIT_PAGE_INCREMENT;

		//printf("Slab Realloc: Grow\n");
	}
	else if(s->page_cnt + INIT_PAGE_INCREMENT < s->alloc_cnt){

		s->pages = realloc(s->pages, old_sz - alloc_sz);
		s->alloc_cnt -= INIT_PAGE_INCREMENT;

		//printf("Slab Realloc: Shrink\n");
	}
}


void slab_init(size_t sz){

	OBJ_SIZE 	= sz;
	PAGE_SIZE 	= 0x1000;

	if(OBJ_SIZE > PAGE_SIZE){
		fprintf(stderr, "Error: Object size cannot exceed page size\n");
		exit(EXIT_FAILURE);
	}

	PAGE_SIZE  += (PAGE_SIZE % sz);
	BITMAP_SIZE	= ((PAGE_SIZE / OBJ_SIZE) / 8 * sizeof(uint8_t)) + 1;
	BITMAP_TAIL = ((PAGE_SIZE / OBJ_SIZE) % (8 * sizeof(uint8_t)));

	BITMAP_MASK <<= ((sizeof(uint8_t) * 8) - BITMAP_TAIL);
	BITMAP_MASK -= 1;

	//printf("OBJ_SIZE: %i\n", OBJ_SIZE);
	//printf("PAGE_SIZE: %i\n", PAGE_SIZE);
	//printf("BITMAP_SIZE: %i\n", BITMAP_SIZE);
	//printf("BITMAP_TAIL: %i\n", BITMAP_TAIL);
	//printf("BITMAP_MASK: %p\n", (void*)BITMAP_MASK);

	/* Initialize/Allocate slab pages */

	global_cache.slabs[SLABS_FULL].pages = 
				calloc(INIT_PAGE_INCREMENT,sizeof(page_t));
	global_cache.slabs[SLABS_PARTIAL].pages = 
				calloc(INIT_PAGE_INCREMENT,sizeof(page_t));
	global_cache.slabs[SLABS_FREE].pages = 
				calloc(INIT_PAGE_INCREMENT,sizeof(page_t));

	//printf("Slab alloc\n");
	//printf("Slab alloc\n");
	//printf("Slab alloc\n");

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

	slab_t* spartial = global_cache.slabs + SLABS_PARTIAL;
	slab_t* sfree = global_cache.slabs + SLABS_FREE;
	slab_t* sfull = global_cache.slabs + SLABS_FULL;
	page_t* page;
	void* addr = NULL;

	/* Any pages in the partial slab? */

	if(spartial->page_cnt > 0){

		page = spartial->pages + (spartial->page_cnt - 1);

		//printf("Partial Slab Object Alloc:\n");
		//printf("\tOn page %i\n", spartial->page_cnt - 1);

		int i,j;
		uint8_t mask;
		
		for(i=0; i < BITMAP_SIZE; i++){

			uint8_t val = page->bitmap[i];

			if(val != 0xFF){

				j=0;
				for(mask=0x80; mask != 0; mask>>=1){

					if(!(mask & val)){

						page->bitmap[i] |= mask;
						break;
					}

					j++;
				}

				int idx = (i * sizeof(uint8_t)) + j;
				
				//printf("\tObject index is %i\n",idx);

				idx *= OBJ_SIZE;
				addr = (page->start_address + idx);
				break;
			}
		}

		if(PAGE_FULL(*page)){

			//printf("ADDING PAGE TO FULL SLAB\n");

			assert_slab_alloc(sfull);
			add_page_to_slab(sfull,page);
			remove_page_from_slab(spartial,page);
			assert_slab_alloc(spartial);
		}

		return addr;
	}


	/* Any pages in the free slab? */

	if(sfree->page_cnt == 0){
		printf("Zero free pages? wtf?\n");
		exit(EXIT_FAILURE);	
	}

	/* Take a free page and move to partial slab */

	//printf("Taking Free page at index %i\n",sfree->page_cnt -1);

	page = sfree->pages + (sfree->page_cnt - 1);
	set_bit(page->bitmap,0,1);
	addr = page->start_address;

	assert_slab_alloc(spartial);
	add_page_to_slab(spartial,page);
	remove_page_from_slab(sfree,page);
	assert_slab_alloc(sfree);

	return addr;
}

void slab_free(void* addr){

	slab_t* spartial = global_cache.slabs + SLABS_PARTIAL;
	slab_t* sfree = global_cache.slabs + SLABS_FREE;
	slab_t* sfull = global_cache.slabs + SLABS_FULL;
	slab_t* from = NULL;
	slab_t* to = NULL;
	page_t* page = NULL;
	int i;

	/* Is this object on a full page? */

	for(i=0; i < sfull->page_cnt; i++){

		page_t* p = sfull->pages + i;
		uint8_t* lb = p->start_address;

		if((uint8_t*)addr >= lb && (uint8_t*)addr < (lb + PAGE_SIZE)){
			page = p;
			from = sfull;
			to = spartial;
			//printf("found freed address in FULL page\n");
			break;
		}
	}

	if(!page){

		/* Must be on a partial page */

		from = spartial;

		for(i=0; i < spartial->page_cnt; i++){

			page_t* p = spartial->pages + i;
			uint8_t* lb = p->start_address;

			if((uint8_t*)addr >= lb && (uint8_t*)addr < (lb + PAGE_SIZE)){
				page = p;
				//printf("found freed address in PARTIAL page\n");
				break;
			}
		}
	}


	if(!page){

		/* Well then this was a bogus address */

		fprintf(stderr, "Error: invalid address passed to free\n");
		exit(EXIT_FAILURE);
	}

	int idx = ((uint32_t)(((uint8_t*)addr) - 
				page->start_address)) / OBJ_SIZE;

	set_bit(page->bitmap,idx,0);

	if(PAGE_EMPTY(*page))
		to = sfree;

	if(to){

		assert_slab_alloc(to);
		add_page_to_slab(to,page);
		remove_page_from_slab(from,page);
		assert_slab_alloc(from);
	}
}

void slab_destroy(){

	int i,j;
	for(i=0; i < SLAB_COUNT; i++){

		slab_t slab = global_cache.slabs[i];

		for(j=0; j < slab.page_cnt; j++){
			free(slab.pages[j].bitmap);
			//printf("page free\n");
		}

		free(slab.pages);
		//printf("Slab free\n");
	}
}
