#include "classes.h"
#include "Http-Cerver.h"
#include "routing/headers/routing.h"


int main()
{
	router_init();
	store_route(main_page());
	store_route(postpage());
	Socket_initalize();
	
	while(1){
		server_reponse();
	}
	close(server_socket);
	return 0;
}