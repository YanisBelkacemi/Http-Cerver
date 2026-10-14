



#ifndef HTTP_CERVER_H
#define HTTP_CERVER_H 

//typedef struct http_request http_request;
typedef struct  {
	char* method;
	char* route;
	char* http_ver;
} http_request;

typedef struct {
	char* route ;
	char* method;
	char* file;
} route_struct;


#endif