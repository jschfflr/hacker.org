#include "common.h"

#include "app.h"
#include "monitor.h"
#include <iostream>

int main(int argc, const char* argv[]) {
#if _DEBUG
	std::string version = "multithreaded,custommutex,customstack,customheap,debug";
#else
	std::string version = "multithreaded,custommutex,customstack,customheap,release";
#endif
	
	monitor::create("events.log");
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try {
		app a(argc, (const char**)argv);
		a.run();
	} catch(std::exception e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	//_CrtDumpMemoryLeaks();

	return 0;
}

