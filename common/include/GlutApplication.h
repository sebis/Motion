#ifndef COMMON_GLUTAPPLICATION
#define COMMON_GLUTAPPLICATION

#include "Application.h"

class GlutApplication : public Application
{
private:
	static GlutApplication * m_instance;

public:
	GlutApplication(bool fixedTimeStep = true, float targetElapsedTime = 1.0f/60.0f);
	virtual ~GlutApplication() {};

	virtual bool init(int argc, char * argv[]);
	int run();

	virtual void update(float dt);
	virtual void draw();

protected:
	virtual void input(unsigned char key);

private:
	static void keyboardWrapper(unsigned char key, int x, int y);
	static void updateWrapper() { updateWrapper(0); }
	static void updateWrapper(int value);
	static void drawWrapper();

	bool m_fixedTimeStep;
	float m_targetElapsedTime;
	int m_totalTime;
};

#endif /* COMMON_GLUTAPPLICATION */