#include <iostream>
#include <string>
#include "vectors.h"

//constructor for an empty Vector
Vector::Vector() {
	this->name = "";
	this->next = nullptr;
	this->size = 0;
	this->elements = nullptr;
}

//constructor for a named Vector
Vector::Vector(std::string name) {
	this->name = name;
	this->next = nullptr;
	std::cout << "Setting vector: " << name << std::endl;
	std::cout << "Enter vector size: ";
	std::cin >> this->size;
	this->elements = new int[this->size];
	for (int i = 0; i < this->size; i++) {
		std::cout << "Enter element " << i << " : ";
		std::cin >> this->elements[i];
	}
}

//destructor for Vector, note that it frees up *elements
Vector::~Vector() {
	std::cout << "Deleted vector: " << this->name << std::endl;
	delete[] this->elements;
}

//constructor for List
List::List() {
	head = nullptr;
	current = nullptr;
}

void List::add_vector(std::string name) {
	// write your code here
}

//removes an item from the List
bool List::remove_vector(std::string name) {
	// write your code here
}

bool List::print_vector(std::string name) {
	// write your code here
}

bool List::append_vector(std::string name) {	
	// write your code here
}
