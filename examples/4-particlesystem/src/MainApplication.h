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
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

#include <vector>

namespace ParticlePhysicsDemo
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
		void initScene(int scene);

		Common::Camera m_camera;

		int m_currentScene;

		std::string text;

		Common::TrailParticleEmitter * m_fireTrailEmitter;
		Common::TrailParticleEmitter * m_smokeTrailEmitter;

		typedef std::vector<Common::Component*> ComponentCollection;
		typedef ComponentCollection::iterator ComponentIterator;
		ComponentCollection m_components;
	};
}

#endif /* MAIN_APPLICATION */
