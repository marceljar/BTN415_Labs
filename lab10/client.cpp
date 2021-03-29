#include <iostream>
#include <string>
#include "socket.h"

int main() {
    //create a new packet
    my_packet.number = 1;
    my_packet.urg = 0;
    my_packet.ack = 1;
    my_packet.psh = 0;
    my_packet.rst = 1;
    my_packet.syn = 0;
    my_packet.fin = 1;
    std::cout << "Enter your message " << std::endl;
    std::string DataBuffer;
    std::getline(std::cin, DataBuffer);
    my_packet.data = new char[DataBuffer.length()];
    //you need to allow Visual Studio to compile with strcpy. Go to
    //"Configuration Properties"->"C/C++"->"Preprocessor"->"Preprocessor definitions"
    //and add _CRT_SECURE_NO_WARNINGS
    strcpy(my_packet.data, DataBuffer.c_str());
    my_packet.crc = calculate_new_packet_crc(my_packet);
    my_packet.checksum = calculate_new_packet_checksum(my_packet);
    my_packet.size = DataBuffer.length();
    
    //send the packet
    Client_TCP client;
    client.create_socket();
    client.connect_socket("127.0.0.1", 27000);
    client.send_new_packet(my_packet);
    client.receive_message(message);
    std::cout << "Message Received: " << message << std::endl;

    return 0;
}
