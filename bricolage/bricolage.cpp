#include <iostream>

#include "simulation.h"


int main(int argc, char* argv[]) {
	Board b(8, 5, ".bbccbb..b.b.bb......c.......c..........");
	std::cout << b.debug() << std::endl;

	Simulation s(b);
	try {
		auto path = s.run();
		for( auto it = path.begin(); it != path.end(); it++)
			std::cout << std::hex << it->first << ":" << it->second << std::endl;

	} catch(std::exception &e) {
		std::cout << "Something went wrong: " << e.what() << std::endl;
	}

	// clear buffer
    fflush(stdin);
    printf("Please enter a character to exit\n");
    char ch;
    ch = getchar();
    
	return 0;
}

