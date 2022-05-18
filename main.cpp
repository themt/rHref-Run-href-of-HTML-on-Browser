#include <windows.h>
#include "rHref.h"



int WINAPI WinMain(HINSTANCE instance, HINSTANCE preInstance, LPSTR argv, INT showStyle) {
	rHref rhref;

	rhref.initialize();
	rhref.showTime(argv);
	rhref.uninitialize();

	return 0;
}
