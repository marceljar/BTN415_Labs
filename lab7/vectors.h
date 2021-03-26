#pragma once
#include <iostream>
#include <string>

class Vector;

class List {
private:
	Vector* head;
	Vector* current;
public:
	List();
	void add_vector(std::string);
	bool remove_vector(std::string);
	bool append_vector(std::string);
	bool print_vector(std::string);
};

class Vector {
private:
	std::string name;
	Vector* next;
	int size;
	int* elements;
public:
	Vector();
	Vector(std::string);
	~Vector();
	friend void List::add_vector(std::string);
	friend bool List::remove_vector(std::string);
	friend bool List::append_vector(std::string);
	friend bool List::print_vector(std::string);
};

