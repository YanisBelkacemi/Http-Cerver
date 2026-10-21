/*

	The first step is to initalize the TCP socket and make it able
	to connect and recieve data


NEXT STEP :
	- Change the method in the http request into an Enum of methods (DONE)
	
	- routing (DONE)

	- adding a TRIE routing sys (gotta fix the Parent route being overwritten) (DONE)

	- finish the POST method Routing 

*/
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "Http-Cerver.h"
//typedef struct http_request http_request;


enum methods {
	POST,
	GET,
	PUT,
	ERROR
};








typedef struct {

	size_t capacity;
	route_struct** routes;

} route_buckets ;

route_buckets* bucket;


 enum methods get_http_method(char* m);
 size_t hash(const char* route , size_t capacity);
 char* routing(http_request* request);
 http_request* parse(char* buffer);
 enum methods get_http_method(char* m);
 char* load_html(const char* FileName);

void Socket_initalize();
void router_init();
void store_route(route_struct* request);
void server_reponse();

int server_socket;


void router_init(){
	bucket = malloc(sizeof(route_buckets));
	if(bucket == NULL){
		return ;

	}
	bucket->capacity = 16; 
	bucket->routes = calloc(bucket->capacity , sizeof(*bucket->routes));
	if (bucket->routes == NULL){

		return ;

	}
}

char* bad_request_400(){
	char* respo ="HTTP/1.1 400 BAD\r\n"
			    "Content-Type: text/plain\r\n"
			    "Content-Length: 4\r\n"
				"Connection: close\r\n"
				"\r\n"
				"400!";
			return respo;

}

void Socket_initalize(){
	
	server_socket = socket(	AF_INET, // IPv4_ADDRESS 
								SOCK_STREAM , // For realiable two way connections
								0 );
	if (server_socket < 0){
		printf("Creating Socket Failed error :%d \n", server_socket );
		close(server_socket);
		return ;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4406);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bind(server_socket , (struct sockaddr*)&server_addr , sizeof(server_addr));
	listen(server_socket , 5);
	printf("server listening at %d \n" , htons(server_addr.sin_port));

}

void server_reponse(){
	struct sockaddr_in client_addr;
	socklen_t client_size = sizeof(client_addr);
	
	int client_socket = accept(server_socket , (struct sockaddr *) &client_addr , &client_size);
	if (client_socket < 0){
		printf("accept failed");
		return;
	}
	printf("Server has received a Connection from %s:%d \n",
			inet_ntoa(client_addr.sin_addr),
			ntohs(client_addr.sin_port) );
	fflush(stdout);

//receiving client request
	char buffer[4096];
	//parse(buffer);
	ssize_t server_receive = recv(
		client_socket,
		buffer,
		sizeof(buffer) - 1,
		0
		);

	if (server_receive < 0){

		printf("receive failed");
		close(client_socket);
		return;
	} else if(server_receive == 0)
	{
		printf("client disconnected");
		close(client_socket);
		return;
	}
	buffer[server_receive] = '\0';
	http_request* request = parse(buffer);

	if (request == NULL){
		return ;

	}
	printf("method : %s \n route : %s \n Version : %s \n " , request->method ,
		request->route,
		request->http_ver);
		

	printf("received : \n %s \n" , buffer);
	fflush(stdout);

	//sending the response

	char *response = routing(request);



	if (response == NULL) {
		return ;
	}




	send(client_socket , response , strlen(response) , 0 );


	close(client_socket);
	//free memory from parse
	free(request->route);
	free(request->http_ver);
	free(request->method);
	free(request);
}

http_request* parse(char* buffer){
	//here creating a new buffer so that changes wont be applied to our original buffer
	char *new_buffer = malloc(strlen(buffer) + 1);
	if (new_buffer == NULL || buffer == NULL){
		return NULL ;
	}
	strcpy(new_buffer , buffer);
	// getting just the first line
	char* first_line = strchr(new_buffer , '\r');

 	if (first_line != NULL){

 		*first_line = '\0';

 	}
 	//printf("buffer : \n %s \n" , new_buffer);
 	char* pSpace = strchr(new_buffer , ' ');

 	if (pSpace != NULL){
 		*pSpace = '\0';
 	}
 	char* method = malloc(strlen(new_buffer) + 1);
 	strcpy(method , new_buffer);
 	
 	http_request* request = calloc(1 , sizeof(http_request) );
 	
 	if (pSpace != NULL){
	 	pSpace+= 1;
	 	char* second_space = strchr(pSpace , ' ');
		 if (second_space != NULL){
		 		*second_space = '\0';
		 }
	 	char* route = malloc(strlen(pSpace) + 1);
	 	strcpy(route , pSpace);
 		request->route = route;

	 		if (second_space != NULL){
		 		second_space += 1;
		 		char* http_ver = malloc(strlen(second_space) + 1);
		 		strcpy(http_ver , second_space);
		 		request->http_ver = http_ver;
		 	}
 	}



 	request->method = method;

 	free(new_buffer);
 	return request;


}


// route handling
char* routing(http_request* request){
	if (request == NULL){
		printf("invalid request\n");
		return NULL;
	}
	size_t index = hash(request->route , bucket->capacity);

	if (bucket->routes[index]== NULL){


		return bad_request_400();
	}



	switch(get_http_method(request->method)){
		case GET:
			if(strcmp(request->route , bucket->routes[index]->route) == 0 && strcmp(request->method , bucket->routes[index]->method) == 0){
				char* html_response =load_html(bucket->routes[index]->file);
				char main_route[250];
				int header_length = snprintf(
					main_route,
					sizeof(main_route),
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n"
					"Content-Length: %zu\r\n"
					"Connection: close\r\n"
					"\r\n",
					strlen(html_response)
					);
				if (header_length < 0 || (size_t)header_length >= sizeof(main_route)) {
					    free(html_response);
					    return NULL;
					}

				char* response = malloc((size_t)header_length + strlen(html_response));

				if (response == NULL) {
				    free(html_response);
				    return NULL;
				}

				memcpy(response, main_route, (size_t)header_length);
				memcpy(response + header_length, html_response, strlen(html_response));

				free(html_response);
				return response;
				}
				else{
					return bad_request_400();
					}
			break;
		case POST:
			printf("The POST has been accessed\n" );
			if(strcmp(request->route , bucket->routes[index]->route) == 0 && strcmp(request->method , bucket->routes[index]->method) == 0){
				char* main_route =
					    "HTTP/1.1 200 OK\r\n"
					    "Content-Type: text/plain\r\n"
					    "Content-Length: 4\r\n"
					    "Connection: close\r\n"
					    "\r\n"
					    "POST";
					return main_route;
				}
				else{
					return bad_request_400();
					}

			break; 
	default:
		return bad_request_400();
		break;
	}		

	return bad_request_400() ;
}



// turning the Http char method into the enum
enum methods get_http_method(char* m){
	enum methods response;
	if (strcmp(m , "GET")==0){
		response = GET;
		return response;
	}
	else if (strcmp(m , "POST")==0){
		response = POST;
		return response;

	}else{
		response = ERROR;
		return response;

	}
}

// hashing function for the hashing map
size_t hash(const char* route , size_t capacity){
	size_t h = 0;
	while(*route){
		h = h * 31 + (unsigned char)*route;
		route++;
	}

	return h % capacity;
}

//storing routes in a hash map
void store_route(route_struct* request){
	if(request == NULL){
		printf("request in NULL");
		return;
	}


	if (request->children != NULL){

		for (int i = 0 ; i< request->children_num ; ++i){
		route_struct* new_route = malloc(sizeof(*new_route));
		if (new_route == NULL){
			return;
		}
		size_t len = strlen(request->children[i]->route) + strlen(request->route) + 1;
		new_route->route = malloc(len);
		snprintf( new_route->route, len, "%s%s", request->route, request->children[i]->route);
		size_t index = hash(new_route->route , bucket->capacity);
		new_route->method = request->children[i]->method;
		new_route->file = request->children[i]->file;
		bucket->routes[index] = new_route ;
		printf("Registering route: %s\n", new_route->route);
		printf("Method: %s\n", new_route->method);
	}
	}
	route_struct* parent_route = malloc(sizeof(*parent_route));
	if (parent_route == NULL){
			return;
		}
	parent_route->route = request->route;
	parent_route->method = request->method;
	parent_route->file = request->file;
	size_t index = hash(request->route , bucket->capacity);
	bucket->routes[index] = parent_route ;
	printf("Registering route: %s\n", parent_route->route);
	printf("Method: %s\n", parent_route->method);
}


char* load_html(const char* FileName){
	printf("File is : %s \n" , FileName);
	FILE *fptr;
	fptr = fopen(FileName , "rb");
	if (fptr == NULL){
		    perror("fopen");
    	return NULL;

	}

	fseek(fptr , 0 , SEEK_END);
	long size = ftell(fptr);
	fseek(fptr , 0 , SEEK_SET);

	char* buffer = malloc( size + 1);

	if (buffer == NULL){
		printf("unable to read the file ");
		return NULL; 
	}

	size_t elements_f = fread(buffer , 1 , size , fptr );
	buffer[elements_f] = '\0';
	fclose(fptr);
	return buffer;
}


