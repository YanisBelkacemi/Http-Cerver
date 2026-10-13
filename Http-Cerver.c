/*

	The first step is to initalize the TCP socket and make it able
	to connect and recieve data


NEXT STEP :
	- Change the method in the http request into an Enum of methods (DONE)
	
	- routing

*/
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "classes.h"
enum methods {
	POST,
	GET,
	PUT,
	ERROR
};


typedef struct {
	enum methods method;
	char* route;
	char* http_ver;
} http_request ;





char* routing(http_request* request);
http_request* parse(char* buffer);
enum methods get_http_method(char* m);
void Socket_initalize();

int server_socket;


char* bad_request_400(){
	char* respo ="HTTP/1.1 400 BAD\r\n"
			    "Content-Type: text/plain\r\n"
			    "Content-Length: 4\r\n"
				"Connection: close\r\n";
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
	printf("method : %d \n route : %s \n Version : %s \n " , request->method ,
		request->route,
		request->http_ver);
		

	printf("received : \n %s \n" , buffer);
	fflush(stdout);

	//sending the response

	char *response = routing(request);

	send(client_socket , response , strlen(response) , 0 );


	close(client_socket);
	//free memory from parse
	free(request->route);
	free(request->http_ver);
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



 	request->method = get_http_method(method);

 	free(new_buffer);
 	free(method);
 	return request;


}
char* routing(http_request* request){
	if (request == NULL){
		printf("invalid request\n");
		return NULL;
	}
	switch(request->method){
		case GET:
			if(strcmp(request->route , "/get") == 0){

				char* main_route =
					    "HTTP/1.1 200 OK\r\n"
					    "Content-Type: text/plain\r\n"
					    "Content-Length: 3\r\n"
					    "Connection: close\r\n"
					    "\r\n"
					    "GET";
					return main_route;
				}
			break;
		case POST:
			if(strcmp(request->route , "/post") == 0){
				char* main_route =
					    "HTTP/1.1 200 OK\r\n"
					    "Content-Type: text/plain\r\n"
					    "Content-Length: 4\r\n"
					    "Connection: close\r\n"
					    "\r\n"
					    "POST";
					return main_route;
				}
			break; 
	default:
		return bad_request_400() ;
		break;
	}		

	return bad_request_400() ;
}




enum methods get_http_method(char* m){
	enum methods response = ERROR ;
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
	return response;
	
}




int main()
{
	Socket_initalize();
		
	while(1){
		server_reponse();
	}
	close(server_socket);
	return 0;
}
