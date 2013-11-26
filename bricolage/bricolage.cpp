#include <iostream>
#include <string>

class Board {
public:
	enum {
		EMPTY = '.',
	};

private:
	int width;
	int height;
	char* data;

public:
	Board(int width, int height, const char* board) {
		this->width = width;
		this->height = height;
		this->data = new char[width * height];
		memcpy(this->data, board, width * height);
	}

	~Board() {
		delete [] this->data;
	}

	char get(int x, int y) {
		if( !(0 <= x && x < width) ||
			!(0 <= y && y < height) )
			throw new std::range_error("Board Index out of Range");

		return data[x + y * width];
	}

	void set(int x, int y, char c) {
		if( !(0 <= x && x < width) ||
			!(0 <= y && y < height) )
			throw new std::range_error("Board Index out of Range");
		
		data[x + y * width] = c;
	}

	std::string debug() {
		std::string output = "";
		for(int y = height - 1; y >= 0; y--) {
			for(int x = 0; x < width; x++) {
				output += ' ';
				output += get(x, y);
				output += ' ';
			}
			output += '\n';
		}

		return output;
	}

	operator std::string() {
		return debug();
	}

	static Board* Load(std::string src) {
		int width = 0;
		int height = 0;
		char* data = 0;

		throw std::runtime_error("Not Implemented Yet");
	}
};

class Simulation {

	Simulation(Board* startingpoint) {
		// append startingpoint to queue
	}

	std::string run() {
		while(true) {
			Board* pBoard = 0; //Get board from queue

			// fetch all available areas from board
			// if len(areas) == 0 and not empty:
			//	invalid state -> don't process
			//  continue
			// 
			// for each area
			//		simulate click on area
			//		if empty:
			//			SOLUTION FOUND!!!
			//          return its path
			//		else:
			//			append to queue
			//
		}
		return "";
	}
};

int main(int argc, char* argv[]) {
	Board* b = new Board(8, 5, ".bbccbb..b.b.bb......c.......c..........");
	std::cout << b->debug() << std::endl;


	// clear buffer
    fflush(stdin);
    printf("Please enter a character to exit\n");
    char ch;
    ch = getchar();
    
	return 0;
}

