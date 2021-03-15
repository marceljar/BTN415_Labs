#include <iostream>
#include <string>
#include "socket.h"

int main() {
	std::string message = "Hello";
	packet my_packet, recovered_packet;
	my_packet = create_packet();

	struct serialized_packet serialized_data = packet_serializer(my_packet);

	Client_TCP client;
	client.create_socket();
	client.connect_socket("127.0.0.1", 27000);
	client.send_packet(my_packet);
	client.receive_message(message);
	std::cout << "Message Received: " << message << std::endl;

	return 0;
}
