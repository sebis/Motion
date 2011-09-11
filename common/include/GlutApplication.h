#ifndef COMMON_GLUTAPPLICATION
#define COMMON_GLUTAPPLICATION

#include "Application.h"

class GlutApplication : public Application
{
private:
	static GlutApplication * m_instance;

public:
	GlutApplication();
	virtual ~GlutApplication() {};

	virtual bool init(int argc, char * argv[]);
	int run();

	virtual void update(float dt);
	virtual void draw();

private:
	static void updateWrapper() { updateWrapper(0); }
	static void updateWrapper(int value);
	static void drawWrapper();

	bool m_fixedTimeStep;
	int m_totalTime;
	int m_targetElapsedTime;
};

#endif /* COMMON_GLUTAPPLICATION */