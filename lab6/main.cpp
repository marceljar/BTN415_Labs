#include <iostream>
#include <string>
#include "socket.h"

int main() {
	std::string message = "Hello";

	/*Client_UDP client;
	client.create_socket();
	client.send_message("Here it goes!", "127.0.0.1", 27000);
	struct sockaddr_in SvrAddr;
	SvrAddr = client.receive_message(message);
	std::cout << "Message Received: " << message << std::endl;*/

	Server_UDP server("127.0.0.1", 27000);
	server.create_socket();
	server.bind_socket();
	struct sockaddr_in SvrAddr;
	std::cout << "Waiting for a message." << std::endl;
	SvrAddr = server.receive_message(message);
	std::cout << "Message Received: " << message << std::endl;
	char* ip = inet_ntoa(SvrAddr.sin_addr);
	//Note that SvrAddr.sin_port is already in network format
	server.send_message("Here it goes!!", ip, ntohs(SvrAddr.sin_port));

	return 0;
}
