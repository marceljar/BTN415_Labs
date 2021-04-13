#include "socket.h"
#include <string>
#include <iostream>
#include <windows.networking.sockets.h>
#include <stdlib.h>
#pragma comment(lib, "Ws2_32.lib")

bool Node::dlls_started = false;
int Node::num_nodes = 0;

void display(unsigned char byte) {
    for (int k = 0; k < 8; k++) {
        std::cout << (int)(byte >> 7);
        byte = byte << 1;
    }
    std::cout << std::endl;
}

unsigned char calculate_new_packet_crc(new_packet my_packet) {
    unsigned char byte, input, feedback, state = 0;
    int size = strlen(my_packet.data);
    for (int i = 0; i < size; i++) {
        byte = my_packet.data[i];
        for (int j = 0; j < 8; j++) {
            input = byte >> 7;
            feedback = ((state & 0x80) >> 7) ^
                ((state & 0x20) >> 5) ^ input;
            state = (state << 1) + feedback;
            byte = byte << 1;
        }
    }
    return state;
}

unsigned int calculate_new_packet_checksum(new_packet my_packet) {
    unsigned int sum = 0;
    unsigned int* ptr = (unsigned int*)my_packet.data;
    for (unsigned int i = 0; i < strlen(my_packet.data) / sizeof(unsigned int); i++) {
        sum += *ptr++;
    }
    int modulus = strlen(my_packet.data) % sizeof(unsigned int);
    int reminder = 0;
    if (modulus != 0) {
        memcpy(&reminder, ptr, modulus);
    }
    sum += reminder;
    return sum;
}

packet create_packet() {
    packet my_packet;
    std::cout << "Enter a packet name: ";
    std::cin >> my_packet.name;
    std::cout << "What is the size of the packet? ";
    std::cin >> my_packet.size;
    if (my_packet.size > 10) {
        my_packet.flag = true;
    }
    else {
        my_packet.flag = false;
    }
    my_packet.letters = new char[my_packet.size];
    for (int i = 0; i < my_packet.size; i++) {
        std::cout << "Enter element " << i << ": ";
        std::cin >> my_packet.letters[i];
    }
    return my_packet;
}

serialized_packet packet_serializer(packet my_packet) {
    //beware of Windows completing groups of four bytes
    char* my_serialized_packet = new char[16 * sizeof(char) + 1 * sizeof(bool) +
        1 * sizeof(int) + my_packet.size * sizeof(char)];
    char* auxptr = my_serialized_packet;
    memcpy(auxptr, my_packet.name, 16 * sizeof(char));
    auxptr += 16 * sizeof(char);
    memcpy(auxptr, &my_packet.flag, 1 * sizeof(bool));
    auxptr += 1 * sizeof(bool);
    memcpy(auxptr, &my_packet.size, 1 * sizeof(int));
    auxptr += 1 * sizeof(int);
    memcpy(auxptr, my_packet.letters, my_packet.size * sizeof(char));

    serialized_packet output;
    output.data = my_serialized_packet;
    output.length = 16 * sizeof(char) + 1 * sizeof(bool) + 1 * sizeof(int) + my_packet.size * sizeof(char);
    return output;
}

packet packet_deserializer(char* serialized_packet) {
    packet deserialized_packet;
    char* auxptr = serialized_packet;
    memcpy(deserialized_packet.name, auxptr, 16 * sizeof(char));
    auxptr += 16 * sizeof(char);
    memcpy(&deserialized_packet.flag, auxptr, sizeof(bool));
    auxptr += 1 * sizeof(bool);
    memcpy(&deserialized_packet.size, auxptr, sizeof(int));
    auxptr += 1 * sizeof(int);
    deserialized_packet.letters = new char[deserialized_packet.size];
    memcpy(deserialized_packet.letters, auxptr, sizeof(char) * deserialized_packet.size);
    return deserialized_packet;
}

Node::Node() {
    start_dlls();
    num_nodes++;
    this->active_socket = INVALID_SOCKET;
    this->protocol = "tcp";
    this->ip = "127.0.0.1";
    this->port = 27000;
}

Node::Node(std::string ip, int port) {
    start_dlls();
    num_nodes++;
    this->active_socket = INVALID_SOCKET;
    this->ip = ip;
    this->port = port;
}

Node::~Node() {
    closesocket(this->active_socket);
    std::cout << "Closing socket" << std::endl;
    num_nodes--;
    if (num_nodes == 0) {
        WSACleanup();
    }
}

void Node::start_dlls() const {
    if (!dlls_started) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cout << "Could not start DLLs" << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            dlls_started = true;
        }
    }
}

bool Node::create_socket() {
    if (this->protocol == "tcp") {
        this->active_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else {
        this->active_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    if (this->active_socket == INVALID_SOCKET) {
        std::cout << "Could not create socket" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

void Node::display_info() const {
    std::cout << "IP: " << this->ip << std::endl;
    std::cout << "port: " << this->port << std::endl;
    std::cout << "protocol: " << this->protocol << std::endl;
    std::cout << "dlls: " << dlls_started << std::endl;
}

Server_TCP::Server_TCP() : Node() {
    this->role = "server";
    this->client_socket = INVALID_SOCKET;
}

Server_TCP::Server_TCP(std::string ip, int port) : Node(ip, port) {
    this->protocol = "tcp";
    this->role = "server";
    this->client_socket = INVALID_SOCKET;
}

Server_TCP::~Server_TCP() {
    closesocket(this->client_socket);
    std::cout << "Closing client socket" << std::endl;
}

bool Server_TCP::bind_socket() {
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    SvrAddr.sin_port = htons(this->port);
    if (bind(this->active_socket, (struct sockaddr*)&SvrAddr,
        sizeof(SvrAddr)) == SOCKET_ERROR) {
        std::cout << "Could not bind socket to address" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

bool Server_TCP::listen_for_connections() {
    if (listen(this->active_socket, 1) == SOCKET_ERROR) {
        std::cout << "Could not start to listen" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

bool Server_TCP::accept_connection() {
    this->client_socket = accept(this->active_socket, NULL, NULL);
    if (this->client_socket == INVALID_SOCKET) {
        std::cout << "Failed to accept connection" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

int Server_TCP::send_message(std::string message) {
    return send(this->client_socket, message.c_str(), message.length(), 0);
}

int Server_TCP::send_packet(packet message) {
    struct serialized_packet serialized = packet_serializer(message);
    return send(this->active_socket, serialized.data, serialized.length, 0);
}

int Server_TCP::receive_message(std::string& message) {
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->client_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    message = RxBuffer;
    return num_bytes;
}

int Server_TCP::receive_packet(packet& my_packet) {
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->client_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    my_packet = packet_deserializer(RxBuffer);
    return num_bytes;
}

void Server_TCP::close_connection() {
    closesocket(this->client_socket);
}

Client_TCP::Client_TCP() : Node() {
    this->protocol = "tcp";
    this->role = "client";
}

bool Client_TCP::connect_socket(std::string ip, int port) {
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(port);
    SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    if ((connect(this->active_socket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
        std::cout << "Failed to connect to server" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

int Client_TCP::send_message(std::string message) {
    return send(this->active_socket, message.c_str(), message.length(), 0);
}

int Client_TCP::send_packet(packet message) {
    struct serialized_packet serialized = packet_serializer(message);
    return send(this->active_socket, serialized.data, serialized.length, 0);
}

int Client_TCP::receive_message(std::string& message) {
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->active_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    message = RxBuffer;
    return num_bytes;
}

int Client_TCP::receive_packet(packet& my_packet) {
    char RxBuffer[MAX_BUFFER_SIZE] = {};
    memset(RxBuffer, 0, MAX_BUFFER_SIZE);
    int num_bytes = recv(this->active_socket, RxBuffer, MAX_BUFFER_SIZE, 0);
    my_packet = packet_deserializer(RxBuffer);
    return num_bytes;
}

Client_UDP::Client_UDP() : Node() {
    this->protocol = "udp";
    this->role = "client";
}

int Client_UDP::send_message(std::string message, std::string ip, int port) {
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(port);
    SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    return sendto(this->active_socket, message.c_str(), message.length(), 0,
        (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
}

struct sockaddr_in Client_UDP::receive_message(std::string& message) {
    struct sockaddr_in SvrAddr;
    int addr_len = sizeof(SvrAddr);

    char RxBuffer[128] = {};
    recvfrom(this->active_socket, RxBuffer, sizeof(RxBuffer), 0,
        (struct sockaddr*)&SvrAddr, &addr_len);
    message = RxBuffer;
    return SvrAddr;
}

Server_UDP::Server_UDP() : Node() {
    this->protocol = "udp";
    this->role = "server";
}

Server_UDP::Server_UDP(std::string ip, int port) : Node(ip, port) {
    this->protocol = "udp";
    this->role = "server";
}

bool Server_UDP::bind_socket() {
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());
    SvrAddr.sin_port = htons(this->port);
    if (bind(this->active_socket, (struct sockaddr*)&SvrAddr,
        sizeof(SvrAddr)) == SOCKET_ERROR) {
        std::cout << "Could not bind socket to address" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

int Server_UDP::send_message(std::string message, std::string ip, int port) {
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(port);
    SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    return sendto(this->active_socket, message.c_str(), message.length(), 0,
        (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
}

struct sockaddr_in Server_UDP::receive_message(std::string& message) {
    struct sockaddr_in SvrAddr;
    int addr_len = sizeof(SvrAddr);

    char RxBuffer[128] = {};
    recvfrom(this->active_socket, RxBuffer, sizeof(RxBuffer), 0,
        (struct sockaddr*)&SvrAddr, &addr_len);
    message = RxBuffer;
    return SvrAddr;
}

