

#ifndef CLASSES_H
#define CLASSES_H

#include <stddef.h>



typedef struct Class Class;
typedef struct Method Method ;
typedef void (*MethodFn)(Class *self ,void *args);

Class *Create_Class(char* name , Class* parent);
void Destroy_class(Class *class);
void call_method(Class *class , char* name, void *args);
void add_method(Class *class , MethodFn function, char* name);

#endif
