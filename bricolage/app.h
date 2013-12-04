#ifndef __APP_H__
#define __APP_H__

#include <regex>
#include "board.h"

class app {
	int level;
	const char* username;
	const char* password;

	const std::regex r_width = std::regex("x=(\\d+)");
	const std::regex r_height = std::regex("y=(\\d+)");
	const std::regex r_board = std::regex("board=([^&\"]+)");

	const std::regex r_vars = std::regex("FlashVars=\"(.+?)\"");
	const std::regex r_level = std::regex("Level: (\\d+)");

private:
	bool solve(const board& board, std::string* path);
	void parse(std::string& data, int* level, int* width, int* height, std::string* board) const;
	void parseline(std::fstream& file, int& width, int& height, std::string& data) const;
	

public:
	app(int argc, const char** argv);
	void run();
};

#endif