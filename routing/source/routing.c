/*
	Here we make our routing 



*/



#include "classes.h"

#include "../../Http-Cerver.h" 

#include "../headers/routing.h"

#include <stdlib.h>

route_struct* main_page();
route_struct* postpage();
route_struct* error();
route_struct* trie_postpage();


route_struct* main_page(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/main";
	request->method = "GET";
	request->file = "templates/main.html";

	return request;

}


route_struct* postpage(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/post";
	request->method = "POST";
	request->file = "templates/main.html";

	return request;

}
route_struct* trie_postpage(){
	
	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/trie";
	request->method = "POST";
	request->file = "templates/main.html";
	request->children = malloc(2*sizeof(*request->children));
	request->children[0] = postpage();
	return request;

}

route_struct* error(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/error";
	request->method = "GET";
	request->file = "templates/main.html";

	return request;

}