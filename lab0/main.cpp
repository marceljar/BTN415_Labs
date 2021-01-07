#include <iostream>


int add(int x, int y) {
	int z;
	z = x + y;
	return z;
}

int main() {

	char message[16] = "Hello World" ;
	std::cout << message << std::endl;

	int x, y, z;
	x = 3;
	y = 2;
	z = add(x, y);
	
	int array[10];
	for (int i = 0; i < 10; i++) {
		array[i] = i;
	}

	return 0;
}
