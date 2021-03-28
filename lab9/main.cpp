#include <iostream>
#include <string>

struct new_packet {
    char number;
    unsigned char urg : 1;
    unsigned char ack : 1;
    unsigned char psh : 1;
    unsigned char rst : 1;
    unsigned char syn : 1;
    unsigned char fin : 1;
    unsigned char : 2; //padding
    unsigned int size;
    char* data;
    unsigned char crc;
    unsigned int checksum;
};

void display(unsigned char byte) {
    for (int k = 0; k < 8; k++) {
        std::cout << (int)(byte >> 7);
        byte = byte << 1;
    }
    std::cout << std::endl;
}

unsigned char calculate_packet_crc(new_packet my_packet) {
    //write your code here
}

unsigned int calculate_packet_checksum(new_packet my_packet) {
    //write your code here
}

int main() {

    new_packet my_packet;

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
    strcpy(my_packet.data, DataBuffer.c_str());
    my_packet.crc = calculate_packet_crc(my_packet);
    my_packet.checksum = calculate_packet_checksum(my_packet);
    
    return 0;
}
