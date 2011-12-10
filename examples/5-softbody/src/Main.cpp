#include "MainApplication.h"

int main(int argc, char * argv[])
{
	SoftBodyDemo::MainApplication app("Soft Body Simulation Demo", true, 1.0f/60.0f);

	if(!app.init(argc, argv))
		return 1;

	return app.run();
}
