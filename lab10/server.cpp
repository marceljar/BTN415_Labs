#include <iostream>
#include <string>
#include "socket.h"

int main() {
	new_packet recovered_packet;

	Server_TCP server("127.0.0.1", 27000);

	server.create_socket();
	server.bind_socket();
	server.listen_for_connections();
	std::cout << "Waiting for a client to connect." << std::endl;
	server.accept_connection();

	server.receive_new_packet(recovered_packet);
	std::cout << "Packet number: " << recovered_packet.number << std::endl;
	std::cout << "Packet ack: " << recovered_packet.ack << std::endl;
	std::cout << "Packet size: " << recovered_packet.size<< std::endl;
	for (int i = 0; i < recovered_packet.size; i++) {
		std::cout << recovered_packet.data[i];
	}
	std::cout << std::endl;
	std::cout << "Packet crc: " << recovered_packet.crc << std::endl;
	server.send_message("Got your message!");
	
	return 0;
}
