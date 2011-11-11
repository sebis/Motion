#include "MainApplication.h"

int main(int argc, char * argv[])
{
	RigidBodyDemo::MainApplication app(true, 1.0f/120.0f);

	if(!app.init(argc, argv))
		return 1;

	return app.run();
}
