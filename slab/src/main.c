#include "slab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){

	slab_init(40);

	void* obj0;
	void* obj1;
	void* obj2;
	void* obj3;
	void* obj4;
	void* obj5;
	void* obj6;
	void* obj7;
	void* obj8;
	void* obj9;

	int i;
	for(i=0; i<500000; i++){

		obj0 = slab_alloc();
		obj1 = slab_alloc();
		obj2 = slab_alloc();
		obj3 = slab_alloc();
		obj4 = slab_alloc();
		obj5 = slab_alloc();
		obj6 = slab_alloc();
		obj7 = slab_alloc();
		obj8 = slab_alloc();
		obj9 = slab_alloc();

		//obj0 = malloc(100);
		//obj1 = malloc(100);
		//obj2 = malloc(100);
		//obj3 = malloc(100);
		//obj4 = malloc(100);
		//obj5 = malloc(100);
		//obj6 = malloc(100);
		//obj7 = malloc(100);
		//obj8 = malloc(100);
		//obj9 = malloc(100);

		slab_free(obj1);
		slab_free(obj3);
		slab_free(obj5);
		slab_free(obj7);
		slab_free(obj9);

		//free(obj1);
		//free(obj3);
		//free(obj5);
		//free(obj7);
		//free(obj9);

		obj1 = slab_alloc();
		obj3 = slab_alloc();

		//obj1 = malloc(100);
		//obj3 = malloc(100);

		slab_free(obj0);
		slab_free(obj2);
		slab_free(obj4);
		slab_free(obj6);
		slab_free(obj8);

		//free(obj0);
		//free(obj2);
		//free(obj4);
		//free(obj6);
		//free(obj8);

		obj6 = slab_alloc();

		//obj6  = malloc(100);

		slab_free(obj1);
		slab_free(obj6);
		slab_free(obj3);

		//free(obj1);
		//free(obj6);
		//free(obj3);
	}

	slab_destroy();

	return 0;
}
