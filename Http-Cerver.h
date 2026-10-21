



#ifndef HTTP_CERVER_H
#define HTTP_CERVER_H 

//typedef struct http_request http_request;

typedef struct route_struct route_struct;

typedef struct  {
	char* method;
	char* route;
	char* http_ver;
} http_request;

struct route_struct {
	char* route ;
	char* method;
	char* file;
	route_struct** children;
};


extern int server_socket;

void Socket_initalize();
void router_init();
void store_route(route_struct* request);
void server_reponse();


#endif