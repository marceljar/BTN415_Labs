#include <iostream>
#include <string>
#include "socket.h"

int main() {
	std::string message = "Hello";
	packet recovered_packet;

	Server_TCP server("127.0.0.1", 27000);

	server.create_socket();
	server.bind_socket();
	server.listen_for_connections();
	std::cout << "Waiting for a client to connect." << std::endl;
	server.accept_connection();

	server.receive_packet(recovered_packet);
	std::cout << "Packet name: " << recovered_packet.name << std::endl;
	std::cout << "Packet flag: " << recovered_packet.flag << std::endl;
	std::cout << "Packet size: " << recovered_packet.size<< std::endl;
	for (int i = 0; i < recovered_packet.size; i++) {
		std::cout << "Packet letter: " << i << " " << recovered_packet.letters[i] << std::endl;
	}
	server.send_message("Got your message!");
	
	return 0;
}
