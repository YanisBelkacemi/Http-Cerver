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



route_struct* main_page(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/main";
	request->method = "GET";


	return request;

}

route_struct* postpage(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = "/post";
	request->method = "POST";


	return request;

}

route_struct* error(){

	route_struct* request =  malloc(sizeof(route_struct));
	request->route = " ";
	request->method = "ERROR";


	return request;

}