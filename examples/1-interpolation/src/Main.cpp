#include "MainApplication.h"

int main(int argc, char * argv[])
{
	Interpolation::MainApplication app("Interpolation Demo", true, 1.0f/60.0f);

	if(!app.init(argc, argv))
		return 1;

	return app.run();
}