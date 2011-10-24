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
#include "Shader.h"
#include "Skeleton.h"

#include <vector>

namespace IK
{
	class Gait;

	class MainApplication : public Base
	{
	private:

	public:
		MainApplication(bool fixedTimeStep = true, float targetElapsedTime = 1.0f/60.0f);
		virtual ~MainApplication();

		bool init(int argc, char * argv[]);

		void keyDown(Common::Key key);
		void keyUp(Common::Key key);
		void mouse(Common::Key key, int x, int y);

		void window_resized(int width, int height);

		void update(float dt);
		void draw();

	private:
		Common::Camera m_camera;

		Common::Skeleton * createBiped(const Gait& gait);
		Common::Skeleton * createQuadrupedal(const Gait& gait);

		typedef std::vector<Common::GameObject*> ComponentCollection;
		typedef ComponentCollection::iterator ComponentIterator;
		ComponentCollection m_components;
	};
}

#endif /* MAIN_APPLICATION */
