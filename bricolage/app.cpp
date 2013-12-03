#include <string.h>
#include <stdexcept>

#include "app.h"
#include "monitor.h"
#include "simulation.h"


app::app(int argc, const char** argv) {
	level = 0;
	user = "hakker1337";
	password = "test1234";

	for (int i = 0; i < argc; i++) {
		if( strcmp(argv[i], "--level") == 0) {
			if (i + 1 >= argc)
				throw std::runtime_error("missing level");

			level = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "--user") == 0) {
			if (i + 1 >= argc)
				throw std::runtime_error("missing username");

			user = argv[++i];
		}
		else if (strcmp(argv[i], "--password") == 0) {
			if (i + 1 >= argc)
				throw std::runtime_error("missing password");

			password = argv[++i];
		}
	}
}

void app::parse(std::string data, int& level, int& width, int& height, std::string& board) const {
	std::smatch m;
	std::regex_search(data, m, r_level);
	level = atoi(m.str(1).c_str());

	if (!std::regex_search(data, m, r_vars))
		throw std::runtime_error("Invalid Data.");

	std::string match = m.str(1);

	std::regex_search(match, m, r_width);
	width = atoi(m.str(1).c_str());

	std::regex_search(match, m, r_height);
	height = atoi(m.str(1).c_str());

	std::regex_search(match, m, r_board);
	board = m.str(1);
}

bool app::solve(const board& board, std::string& path) {
	simulation s(board);
	try {
		s.dfs(path);
		return true;
	}
	catch (std::exception e) {
		monitor::_emit(monitor::event("exception") << "process" << e.what());
		return false;
	}

}

void app::parseline(std::fstream& file, int& width, int& height, std::string& data) const {
	char buffer[1024];
	file.getline(buffer, sizeof(buffer));
	std::cmatch m;
	if (!std::regex_search(buffer, m, std::regex("^(\\d+)\\s+(\\d+)\\s+([a-z\\.]+)$")) )
		throw std::runtime_error("could not parse line");

	width = atoi(m.str(1).c_str());
	height = atoi(m.str(2).c_str());
	data = m.str(3);
}

void app::run() {

	int width, height, level = 0;
	std::string data, path;
	std::fstream levels("levels.txt", std::fstream::in);
	while ( !levels.eof() ) {
		parseline(levels, width, height, data);

		timer t;

		solve(board(width, height, data.c_str()), path);

		monitor::_emit(monitor::event("solved") << level++ << int(t.get() * 1000) << path);
	}
}