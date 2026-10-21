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
	request->route = "/posts";
	request->method = "POST";
	request->file = "templates/main.html";

	return request;

}

route_struct* postpage2(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/users";
	request->method = "POST";
	request->file = "templates/main.html";

	return request;

}
route_struct* trie_postpage(){
	
	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/api";
	request->method = "POST";
	request->file = "templates/main.html";
	request->children_num = 2;
	request->children = malloc(sizeof(request->children_num));
	request->children[0] = postpage();
	request->children[1] = postpage2();
	return request;

}

route_struct* error(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/error";
	request->method = "GET";
	request->file = "templates/main.html";

	return request;

}