#include <iostream>
#include <string>

#include "socket.h"

int main() {
	std::string message = "Hello";
  //uncomment these lines, and comment lines 20 to 25 to test the server part
	/*Server_TCP server("127.0.0.1", 28000);
	
	server.create_socket();
	server.bind_socket();
	server.listen_for_connections();
	server.accept_connection();
	
	server.receive_message(message);
	std::cout << "Message Received: " << message << std::endl;
	server.send_message("Got your message!");*/

	Client_TCP client;
	client.create_socket();
	client.connect_socket("127.0.0.1", 28000);
	client.send_message("Here it goes!");
	client.receive_message(message);
	std::cout << "Message Received: " << message << std::endl;

	return 0;
}
