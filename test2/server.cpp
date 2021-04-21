#include <iostream>
#include <string>
#include "socket.h"

int main() {
	data_packet recovered_packet;

	Server_TCP server("127.0.0.1", 27000);

	server.create_socket();
	server.bind_socket();
	server.listen_for_connections();
	std::cout << "Waiting for a client to connect." << std::endl;
	server.accept_connection();

	server.receive_data_packet(recovered_packet);
	std::cout << "data: " ;
	for (int i = 0; i < recovered_packet.length; i++) {
		std::cout << recovered_packet.data[i];
	}
	std::cout << std::endl;
	
	bool data_check = check_crc(recovered_packet);
	if (data_check) {
		std::cout << "Data is fine!" << std::endl;
	} else {
		std::cout << "Data has been corrupted" << std::endl;
	}
	
	server.send_message("Got your message!");

	return 0;
}
