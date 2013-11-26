#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <stack>

struct Area {
	char color;
	std::list<std::pair<int,int>> points;

	Area(char color) {
		this->color = color;
	}

	int size() {
		return points.size();
	}
};

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
	Board() {
		width = height = 0;
		data = 0;
	}

	Board(int width, int height, const char* board) {
		this->width = width;
		this->height = height;
		this->data = new char[width * height];
		memcpy(this->data, board, width * height);
	}

	Board Board::operator =(const Board &b) {
		if( this->data)
			delete [] this->data;
		
		this->width = b.width;
		this->height = b.height;
		this->data = new char[width * height];
		memcpy(this->data, b.data, width * height);
		return *this;
	}

	~Board() {
		if( this->data )
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

	bool empty() {
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				if( get(x, y) != '.')
					return false;
			}
		}
		return true;
	}

	// Get all areas in the current field
	std::list<Area> areas() {
		std::stack<std::pair<int,int>> stack;
		std::list<Area> areas;
		char* tmp = new char[width * height];
		memcpy(tmp, data, width * height);

		char color;
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {

				color = tmp[x + y * width];
				if( color == 0 || color == '.' )
					continue;

				Area area(color);
				stack.push(std::pair<int,int>(x, y));

				while(!stack.empty()) {
					auto c = stack.top();
					stack.pop();

					if( !(0 <= c.first && c.first < width) ||
						!(0 <= c.second && c.second < height) )
						continue; // Out of range

					if( tmp[c.first + c.second * width] != color )
						continue;

					tmp[c.first + c.second * width] = 0;
					area.points.push_back(c);
				
					stack.push(std::pair<int,int>(c.first, c.second+1));
					stack.push(std::pair<int,int>(c.first, c.second-1));
					stack.push(std::pair<int,int>(c.first+1, c.second));
					stack.push(std::pair<int,int>(c.first-1, c.second));
				}

				areas.push_back( area );
			}
		}

		delete [] tmp;
		return areas;
	}

	std::string serialize() {
		std::string tmp = "";
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				tmp += get(x, y);
			}
		}
		return tmp;
	}

	static Board Clone(Board& board) {
		std::string tmp = board.serialize();
		return Board(board.width, board.height, tmp.c_str());
	}

	static Board Load(std::string src) {
		int width = 0;
		int height = 0;
		char* data = 0;

		throw std::runtime_error("Not Implemented Yet");
	}
};

class State {
public:
	Board board;
	std::list<std::pair<int,int>> clicks;

	State(Board board) {
		this->board = board;
	}

	State(Board board, std::list<std::pair<int,int>> clicks) {
		this->board = board;
		this->clicks = clicks;
	}

	void click(int x, int y) {

	}

	State copy() {
		std::list<std::pair<int,int>> tmp;
		tmp.splice(tmp.begin(), clicks);

		return State(Board::Clone(board), tmp);
	}
};

class Simulation {
private:
	std::queue<State> queue;

public:
	Simulation(Board& board) {
		State start(board);
		queue.push(start);
	}

	std::list<std::pair<int,int>> run() {
		while(!queue.empty()) {
			State state = (State)queue.front();
			queue.pop();

			auto areas = state.board.areas();

			// fetch all available areas from board
			// if len(areas) == 0 and not empty:
			//	invalid state -> don't process
			//  continue
			//
			if( areas.size() == 0 && !state.board.empty())
				continue; // 
			
			// for each area
			//		simulate click on area
			//		if empty:
			//			SOLUTION FOUND!!!
			//          return its path
			//		else:
			//			append to queue
			//
			for(auto it = areas.begin(); it != areas.end(); it++) {
				State test = state.copy();
				auto point = it->points.front();
				test.click(point.first, point.second);
				if( test.board.empty() ) {
					// SOLUTION
					return test.clicks;
				} else {
					queue.push(test);
				}
			}
		}

		throw std::logic_error("Invalid State");
	}
};

int main(int argc, char* argv[]) {
	Board b(8, 5, ".bbccbb..b.b.bb......c.......c..........");
	std::cout << b.debug() << std::endl;

	Simulation s(b);
	try {
		s.run();
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

