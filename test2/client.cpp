#include <iostream>
#include <string>
#include "socket.h"

int main() {

	data_packet my_packet = create_packet();

	//these lines are useful to test your serializer and deserializer
	//without requiring to run two applications
	struct serialized_packet serialized_data = packet_serializer(my_packet);
	data_packet recovered = packet_deserializer(serialized_data.data);

	std::string message;
	Client_TCP client;
	client.create_socket();
	client.connect_socket("127.0.0.1", 27000);
	client.send_data_packet(my_packet);
	client.receive_message(message);
	std::cout << "Message Received: " << message << std::endl;

	return 0;
}
