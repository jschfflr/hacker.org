#include <regex>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "timer.h"
#include "request.h"
#include "simulation.h"

static std::regex r_width("x=(\\d+)");
static std::regex r_height("y=(\\d+)");
static std::regex r_board("board=([^&\"]+)");

static std::regex r_vars("FlashVars=\"(.+?)\"");
static std::regex r_level("Level: (\\d+)");

struct Level {
	int number;
	Board board;
	std::string solution;

	Level(int number, Board board, std::string path) {
		this->number = number;
		this->board = board;
		this->solution = path;
	}
};

Level solve(std::string data) {
	// make request
	//std::string data = "<html><head><title>Bricolage</title>   <META NAME=\"keywords\" CONTENT=\"game, hack, hacking, programming, competition,bricolage\">   <META NAME=\"description\" CONTENT=\"Play and hack a disappearing blocks game.\">	<LINK rel=\"stylesheet\" href=\"/css/style.css\" type=\"text/css\"></head><body><table border=0 cellspacing=0 cellpadding=0><tr valign=center><td> <img src=\"/images/header.png\" width=\"877\" height=\"68\" border=\"0\" alt=\"\" usemap=\"#header_Map\"><map name=\"header_Map\"><area shape=\"rect\" alt=\"\" coords=\"753,41,858,63\" href=\"/forum/profile.php?mode=register\"><area shape=\"rect\" alt=\"\" coords=\"753,14,858,36\" href=\"/forum/login.php\"><area shape=\"rect\" alt=\"\" coords=\"0,22,223,61\" href=\"/\"></map><map name=\"header_Map_li\"><area shape=\"rect\" alt=\"\" coords=\"0,22,223,61\" href=\"/\"></map></td><td align=left style=\"border-bottom: 1px solid #d4f3e1\"><img src=\"logo.png\"></td></tr></table>Level: 3<br><table border=0><tr valign=top><td>		<object classid=\"clsid:d27cdb6e-ae6d-11cf-96b8-444553540000\"			codebase=\"http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,0,0\"			width=\"800\"			height=\"600\"			align=\"middle\"			id=\"main\">			<param name=\"allowScriptAccess\" value=\"sameDomain\" />			<param name=\"movie\" value=\"bricks.swf\" />			<param name=\"quality\" value=\"high\" />			<param name=\"bgcolor\" value=\"#888888\" />			<param name=\"FlashVars\" value=\"x=11&y=7&board=.cccdddcbd....cdcdbdd....b.ccbdb....b..........b..........b..................\" />			<embed src=\"bricks.swf\"			width=\"800\"			height=\"600\"			autostart=\"false\"			quality=\"high\"			bgcolor=\"#888888\"			FlashVars=\"x=11&y=7&board=.cccdddcbd....cdcdbdd....b.ccbdb....b..........b..........b..................\"			name=\"main\"			align=\"middle\"			allowScriptAccess=\"sameDomain\"			type=\"application/x-shockwave-flash\"			pluginspage=\"http://www.macromedia.com/go/getflashplayer\" />			</object>			</td><td><table border=0><tr><th colspan=3>High Scores</th></tr><tr><th>player</th><th>level</th><th>date</th></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=4887\">Yharaskrik</a><img src=\"/img/star.png\"></td><td><a href=\"/util/charthistory.php?userid=4887&game=brick\">1052</td><td>2010-09-24 02:04</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=11236\">mrhaandi</a></td><td><a href=\"/util/charthistory.php?userid=11236&game=brick\">620</td><td>2011-04-21 09:56</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=7914\">miroe</a></td><td><a href=\"/util/charthistory.php?userid=7914&game=brick\">384</td><td>2012-10-27 05:43</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=2816\">tails</a><img src=\"/img/star.png\"></td><td><a href=\"/util/charthistory.php?userid=2816&game=brick\">377</td><td>2009-07-22 02:03</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=19018\">Rat62</a></td><td><a href=\"/util/charthistory.php?userid=19018&game=brick\">277</td><td>2012-11-01 05:38</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=15564\">nikita</a></td><td><a href=\"/util/charthistory.php?userid=15564&game=brick\">225</td><td>2012-06-17 03:48</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=8286\">bellerophon</a></td><td><a href=\"/util/charthistory.php?userid=8286&game=brick\">216</td><td>2012-01-24 15:03</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=27020\">Schlaule</a></td><td><a href=\"/util/charthistory.php?userid=27020&game=brick\">184</td><td>2013-08-05 04:45</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=28962\">Ignorance</a></td><td><a href=\"/util/charthistory.php?userid=28962&game=brick\">179</td><td>2012-11-16 09:46</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=2353\">THINOU</a></td><td><a href=\"/util/charthistory.php?userid=2353&game=brick\">169</td><td>2013-06-01 12:28</td></tr><tr><td colspan=3><a href=\"?showhigh\">See all...</a></td></tr></table><br><table border=0><tr><th colspan=3>Weekly High Scores</th></tr><tr><th>player</th><th>level</th><th>date</th></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=32828\">Brandonme7</a></td><td><a href=\"/util/charthistory.php?userid=32828&game=brick\">15</td><td>2013-11-23 14:19</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=6888\">wortwart</a></td><td><a href=\"/util/charthistory.php?userid=6888&game=brick\">11</td><td>2013-11-19 06:16</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=32747\">revolutia</a></td><td><a href=\"/util/charthistory.php?userid=32747&game=brick\">10</td><td>2013-11-22 23:21</td></tr><tr valign=center><td><a href=\"/forum/profile.php?mode=viewprofile&u=32831\">wally1</a></td><td><a href=\"/util/charthistory.php?userid=32831&game=brick\">10</td><td>2013-11-24 02:24</td></tr></table></td></tr></table></body></html>";

	int level = 0, width = 0, height = 0;
	std::string board;

	std::smatch m;
	std::regex_search(data, m, r_level);
	level = atoi( m.str(1).c_str() );

	if( !std::regex_search(data, m, r_vars) )
		throw std::runtime_error("Invalid Data.");

	std::string match = m.str(1);
	
	std::regex_search(match, m, r_width);
	width = atoi( m.str(1).c_str() );
	
	std::regex_search(match, m, r_height);
	height = atoi( m.str(1).c_str() );
	
	std::regex_search(match, m, r_board);
	board = m.str(1);

	Board b(width, height, board);
	Simulation s(b);
	try {
		std::stringstream path;
		path.setf(std::ios::hex, std::ios::basefield);
		path.setf(std::ios::right, std::ios::adjustfield);
		path.fill('0');

		auto result = s.run();
		//auto steps  = s.run(b, result);

		for( auto it = result.begin(); it != result.end(); it++) {
			path << std::setw(2) << it->first;
			path << std::setw(2) << it->second;
		}

		/*
		for( auto board: steps) {
			std::cout << (std::string)board << std::endl;
		}
		*/

		
		return Level(level, b, path.str());
	} catch(std::exception &e) {
		std::cout << "Something went wrong: " << e.what() << std::endl;
	}
}

int main(int argc, char* argv[]) {
#if _DEBUG
	std::string version = "singlethreaded,custommutex,debug";
#else
	std::string version = "singlethreaded,release";
#endif
	
	try {
		int level = 0;
		std::string data = "";
		while(true) {
			timer t;

			std::stringstream url1;
			url1 << "/brick/index.php?name=hakker1337&password=test1234&gotolevel=" << level++;
			data = request("www.hacker.org", url1.str());
			
			
			Level level = solve(data);
			
			std::stringstream url2;
			url2 << "/brick/index.php?name=hakker1337&password=test1234&gotolevel=";
			url2 << level.number;
			url2 << "&path=" << level.solution;
			data = request("www.hacker.org", url2.str());


			std::stringstream url3;
			url3 << "?lvl=" << level.number;
			url3 << "&time=" << int(t.get() * 1000);
			url3 << "&version=" << version;
			url3 << "&path=" << level.solution;
			

			request("bricolage.exesystem.net", url3.str());
			std::cerr << level.number << "\t" << (std::string)level.board << "\t" << level.solution << std::endl;
		}
	} catch(std::exception e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}

