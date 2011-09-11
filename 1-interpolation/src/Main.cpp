#include "ExampleApplication.h"

int main(int argc, char * argv[])
{
	ExampleApplication app;

	if(!app.init(argc, argv))
		return 1;

	return app.run();
}