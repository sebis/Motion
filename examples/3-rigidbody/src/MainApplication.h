#ifndef MAIN_APPLICATION
#define MAIN_APPLICATION

#if 0
#include <SDLApplication.h>
typedef Common::SDLApplication Base;
#else
#include <GlutApplication.h>
typedef Common::GlutApplication Base;
#endif

#include "Camera.h"
#include "GameObject.h"
#include "Physics.h"

#include <vector>

namespace RigidBodyDemo
{
	class MainApplication : public Base
	{
	private:

	public:
		MainApplication(const char * title, bool fixedTimeStep = true, float targetElapsedTime = 1.0f/60.0f);
		virtual ~MainApplication();

		bool init(int argc, char * argv[]);

		void keyDown(Common::Key key);
		void keyUp(Common::Key key);
		void mouse(Common::Key key, int x, int y);

		void window_resized(int width, int height);

		void update(float dt);
		void draw();

	private:
		void setupSimulation();
		void reset();

		Common::Camera m_camera;
		Common::Physics * m_physics;

		bool m_started;
		bool m_visualize;
		bool m_hideHelp;

		typedef std::vector<Common::GameObject*> ComponentCollection;
		typedef ComponentCollection::iterator ComponentIterator;
		ComponentCollection m_components;
	};
}

#endif /* MAIN_APPLICATION */
