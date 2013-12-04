#include "common.h"

#include "app.h"
#include "monitor.h"
#include <iostream>

#if _DEBUG
	const char* version = "multithreaded,custommutex,customstack,customheap,debug";
#else
	const char* version = "multithreaded,custommutex,customstack,customheap,release";
#endif

int main(int argc, const char* argv[]) {	
	WSAData data;
	if (WSAStartup(MAKEWORD(2, 0), &data) != 0)
		throw std::runtime_error("WSAStartUp failed.");

	monitor* m = monitor::create("events.log");
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try {
		app a(argc, (const char**)argv);
		a.run();
	} catch(std::exception e) {
		std::cout << e.what() << std::endl;
		return -1;
	}


	m->release();
	WSACleanup();
	_CrtDumpMemoryLeaks();

	return 0;
}

