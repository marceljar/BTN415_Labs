#include <iostream>
#include <string>
#include "vectors.h"

int main() {

	List list_of_vectors;

	list_of_vectors.add_vector("qwerty");
	list_of_vectors.add_vector("test"); 
	list_of_vectors.add_vector("asd");	

	list_of_vectors.print_vector("test");
	list_of_vectors.remove_vector("test");
	list_of_vectors.append_vector("qwerty");
	list_of_vectors.print_vector("qwerty");
	list_of_vectors.remove_vector("qwerty");
	if (list_of_vectors.print_vector("qwerty")) {
		std::cout << "Vector found" << std::endl;
	} else {
		std::cout << "Vector not found" << std::endl;
	}

	return 0;
}
