#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/classes.h"
typedef struct Class Class;
typedef struct Method Method ;
typedef void (*MethodFn)(Class *self ,void *args);


Class *Create_Class(char* name , Class* parent);
void Destroy_class(Class *class);
void call_method(Class *class , char* name, void *args);
void add_method(Class *class , MethodFn function, char* name);

struct Method {
	char* name;
	MethodFn function;
	Method* next;
};

struct Class {
	char* name;
	Method **bucket;
	size_t capacity;
	Class *parent;
};


size_t hash(const char* name, size_t capacity){
	size_t h = 0;
	while(*name){
		h = h * 31 + (unsigned char)*name;
		name++;

	}

	return h % capacity;
}

Class *Create_Class(char* name , Class* parent){
	//Basic class creation
	Class *class = malloc(sizeof(Class));
	class->name = name;
	class->capacity = 16;
	class->bucket = calloc(class->capacity,sizeof(Method *));
	class->parent = parent;
	return class;
}
//this one needs a for loop to destroy each method inside the bucket class
void Destroy_class(Class *class){
	for (size_t i = 0 ; i < class->capacity ; ++i){
		Method *method= class->bucket[i];
		while(method){
			Method* next = method->next;
			free(method);
			method = next;
		}
	}
	free(class->bucket);
	free(class);
}

void add_method(Class *class , MethodFn function, char* name){

	size_t index = hash(name, class->capacity);
	Method *method = malloc(sizeof(Method));
	method->name = name;
	method->function = function;
	method->next = class->bucket[index];

	class->bucket[index] = method;
}	
 
void call_method(Class *class , char* name, void *args){
	
		Class* current = class;
	while(current){
		
		size_t str = hash(name , current->capacity);
		Method *method = current->bucket[str];
		while(method){
			if (strcmp(method->name, name)==0){
				method->function(class, args);
				return;
			}
			method = method->next;
		}
		current = current->parent;
	}
}



