#ifndef MAIN_APPLICATION
#define MAIN_APPLICATION

#if 0
#include <SDLApplication.h>
typedef Common::SDLApplication Base;
#else
#include <GlutApplication.h>
typedef Common::GlutApplication Base;
#endif

#include "Animator.h"
#include "Camera.h"
#include "Collider.h"
#include "Component.h"

#include <vector>
#include <map>

namespace SoftBodyDemo
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
		void initScene();

		Common::Camera m_camera;
		bool m_started;

		typedef std::vector<Common::Component*> ComponentCollection;
		typedef ComponentCollection::iterator ComponentIterator;
		ComponentCollection m_components;

		Common::Animator * m_ballAnimator;

		ComponentCollection addDrawDebug(Common::MeshCollider * collider, int level);
		std::map<int, ComponentCollection> m_debugComponents;
		typedef std::map<int, ComponentCollection>::iterator DebugIterator;
		int m_currentLevel;
		bool m_debug;
		bool m_constrained;
		bool m_animate;
		bool m_help;
	};
}

#endif /* MAIN_APPLICATION */
