#ifndef COMMON_APPLICATION
#define COMMON_APPLICATION

class Application
{
public:
	Application() {};
	virtual ~Application() {};

	virtual bool init(int argc, char * argv[]) = 0;
	virtual int run() = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
};

#endif /* COMMON_APPLICATION */