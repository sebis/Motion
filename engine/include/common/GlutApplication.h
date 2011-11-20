#ifndef COMMON_GLUTAPPLICATION
#define COMMON_GLUTAPPLICATION

#include "Application.h"

namespace Common
{
	class GlutApplication : public Application
	{
	private:
		static GlutApplication * m_instance;

	public:
		GlutApplication(const char * title, bool fixedTimeStep = true, float targetElapsedTime = 1.0f/60.0f);
		virtual ~GlutApplication() {};

		virtual bool init(int argc, char * argv[]);
		int run();

		virtual void keyDown(Key /*key*/) {}
		virtual void keyUp(Key /*key*/) {}
		virtual void mouse(Key /*key*/, int /*x*/, int /*y*/) {}

		virtual void window_resized(int /*width*/, int /*height*/) {}

		virtual void display_text(const char * text, int x, int y);

		virtual void update(float dt);
		virtual void draw();

	private:
		static void updateWrapper() { updateWrapper(0); }
		static void updateWrapper(int value);
		static void drawWrapper();
		static void keyboardWrapper(unsigned char key, int x, int y);
		static void keyboardUpWrapper(unsigned char key, int x, int y);
		static void specialWrapper(int key, int x, int y);
		static void specialUpWrapper(int key, int x, int y);
		static void motionWrapper(int x, int y);
		static void mouseWrapper(int button, int state, int x, int y);
		static void reshapeWrapper(int width, int height);

		const char * m_title;
		bool m_fixedTimeStep;
		float m_targetElapsedTime;
		int m_totalTime;
	};
}

#endif /* COMMON_GLUTAPPLICATION */
