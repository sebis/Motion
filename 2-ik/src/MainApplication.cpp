#include "Animator.h"
#include "GameObject.h"
#include "MainApplication.h"
#include "Material.h"
#include "MeshObject.h"
#include "Skeleton.h"
#include "Texture.h"
#include "Trace.h"
#include "Utils.h"

#include "KeyframeAnimator.h"
#include "Interpolator.h"

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"

namespace IK
{
	using namespace Common;
	using namespace Interpolation;

	class LocationAnimator : public Animator
	{
	public:
		LocationAnimator(GameObject * gameObject, glm::vec3 velocity)
			: Animator(gameObject), m_velocity(velocity)
		{
		}

		void update(float dt)
		{
			m_gameObject->transform().translate(m_velocity * dt);
		}

	private:
		glm::vec3 m_velocity;
	};

	namespace 
	{
		GameObject * createLeg()
		{
			return 0;
		}
	}

	MainApplication::MainApplication(bool fixedTimeStep, float targetElapsedTime)
		: Base(fixedTimeStep, targetElapsedTime),
		//m_camera(glm::vec3(13.0f, 14.0f, -15.0f), glm::vec3(-2.0f, 0.0f, -2.0f))
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f))
	{
	}

	MainApplication::~MainApplication()
	{
	}

	bool MainApplication::init(int argc, char * argv[])
	{
		if (!Base::init(argc, argv))
			return false;

		GLenum err = glewInit();
		if (err != GLEW_OK) {
			Trace::error("Could not initialize GLEW: %s\n", glewGetErrorString(err));
			return false;
		}

		Trace::info("OpenGL context: %s\n", glGetString(GL_VERSION));
		Trace::info("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		Trace::info("GLEW Version: %s\n", glewGetString(GLEW_VERSION));

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glLineWidth(1.5f);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Material * material = new Material(Shader::find("shader"));
		material->setTexture(new Texture("resources/checker.bmp"));

		MeshObject * floor = new MeshObject(MeshFactory::Plane(glm::vec4(1.0f), 5), material);
		// TODO: shouldn't need to set camera explicitly
		floor->m_camera = &m_camera;

		floor->transform().scale() = glm::vec3(500.0f);
		floor->transform().position() = glm::vec3(0.0f, -1.0f, 0.0f);
		
		m_components.push_back(floor);

		Skeleton * skeleton = new Skeleton;
		skeleton->transform().translate(glm::vec3(0.0f, 0.0f, 0.0f));

		skeleton->m_animator = new LocationAnimator(skeleton, glm::vec3(6/1000.0f, 0.0f, 0.0f));

		m_components.push_back(skeleton);

		{
		Material * green = new Material(Shader::find("shader"));
		green->setDiffuseColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		MeshObject * pelvis = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), green);
		pelvis->m_camera = &m_camera;
		pelvis->transform().translate(glm::vec3(0.0f, 10.0f, -1.0f));
		pelvis->transform().setParent(skeleton->transform());

		//pelvis->m_animator = new LocationAnimator(pelvis, glm::vec3(6/1000.0f, 0.0f, 0.0f));

		m_components.push_back(pelvis);

		Bone * pelvisBone = new Bone(pelvis->transform());

		{
			Material * yellow = new Material(Shader::find("shader"));
			yellow->setDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

			MeshObject * knee = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), yellow);
			knee->transform().setParent(pelvis->transform());
			knee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));

			knee->m_camera = &m_camera;

			m_components.push_back(knee);

			Bone * kneeBone = new Bone(knee->transform(), pelvisBone);

			{
				Material * material = new Material(Shader::find("shader"));
				material->setDiffuseColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

				MeshObject * foot = new MeshObject(MeshFactory::Cube(false, glm::vec4(1.0f)), material);
				//GameObject * foot = new GameObject();
				foot->m_camera = &m_camera;
				foot->transform().setParent(skeleton->transform());
				//foot->transform().position() = glm::vec3(-2.0f, 0.0f, 7.5f);
				m_components.push_back(foot);

				Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
				KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(foot, interpolator, foot->transform().position(), false, true, false, true);

				//animator->setRenderer(new SplineRenderer(foot, interpolator, animator->keys()));

				/*animator->addKeyframe(0.0f, glm::vec3(2.5f, -9.5f, 0.0f));  // foot hits ground
				animator->addKeyframe(500.0f, glm::vec3(0.0f, -9.5f, 0.0f)); 
				animator->addKeyframe(1000.0f, glm::vec3(-3.5f, -8.5f, 0.0f)); // foot off ground
				animator->addKeyframe(1500.0f, glm::vec3(-2.0f, -7.6f, 0.0f));
				animator->addKeyframe(2000.0f, glm::vec3(2.5f, -9.6f, 0.0f));  // foot hits ground*/


				// changable parameters
				float strokeMin = -2.0f;
				float strokeMax = 3.0f;
				float heightMin = 0.5f;
				float heightMax = 2.4f;
				//float cycleLength = 3000.0f; // TODO: this from somewhere
				float bodySpeed = 6.0f/1000.0f; // TODO: this from somewhere
				float legSpeed = 10.0f/1000.0f; // TODO: this from somewhere

				float stroke = strokeMax - strokeMin;
				float supportDuration = stroke / bodySpeed;
				float arcLength = std::sqrt(strokeMin*strokeMin + (heightMax - heightMin)*(heightMax - heightMin)) + std::sqrt((heightMax - heightMin)*(heightMax - heightMin) + strokeMax*strokeMax);
				float transferDuration = arcLength / legSpeed;
				float cycleLength = supportDuration + transferDuration;


				Trace::info("stroke: %f\n", stroke);
				Trace::info("cycleLength: %f\n", cycleLength);
				Trace::info("supportDuration: %f\n", supportDuration);
				Trace::info("transferDuration: %f\n", transferDuration);

				float phase = cycleLength * 0.5f;
				animator->setIdle(phase);
				// support phase
				animator->addKeyframe(0.0f, glm::vec3(strokeMax, heightMin, 0.0f));  // foot forward (hits ground)
				animator->addKeyframe(supportDuration * 0.5f, glm::vec3(strokeMin + stroke * 0.5f, heightMin, 0.0f));  // foot straight in center
				animator->addKeyframe(supportDuration, glm::vec3(strokeMin, heightMin, 0.0f)); // foot is just about to lift
				animator->addKeyframe(supportDuration + transferDuration * 0.5f, glm::vec3(0.0f, heightMax, 0.0f));

				// dummy needs to repeat first
				animator->addKeyframe(supportDuration + transferDuration, glm::vec3(strokeMax, heightMin, 0.0f));  // foot hits ground

				/* SAVE
				animator->addKeyframe(0.0f, glm::vec3(4.0f, 0.5f, 0.0f));  // foot hits ground
				animator->addKeyframe(500.0f, glm::vec3(0.0f, 0.5f, 0.0f)); 
				animator->addKeyframe(1000.0f, glm::vec3(-3.0f, 1.5f, 0.0f)); // foot off ground
				animator->addKeyframe(1500.0f, glm::vec3(-2.0f, 2.4f, 0.0f));
				animator->addKeyframe(2000.0f, glm::vec3(4.0f, 0.5f, 0.0f));  // foot hits ground*/

				foot->m_animator = animator;

				Bone * footBone = new Bone(foot->transform(), kneeBone);

				skeleton->addEndEffector(footBone);
			}
		}
		}

		{
		Material * green = new Material(Shader::find("shader"));
		green->setDiffuseColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		MeshObject * pelvis = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), green);
		pelvis->transform().translate(glm::vec3(0.0f, 10.0f, 1.0f));
		pelvis->transform().setParent(skeleton->transform());
		pelvis->m_camera = &m_camera;

		m_components.push_back(pelvis);

		Bone * pelvisBone = new Bone(pelvis->transform());
		pelvisBone->name = "PelvisBone";

		{
			Material * yellow = new Material(Shader::find("shader"));
			yellow->setDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

			MeshObject * knee = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), yellow);
			knee->m_camera = &m_camera;
			knee->transform().setParent(pelvis->transform());
			knee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));

			m_components.push_back(knee);

			Bone * kneeBone = new Bone(knee->transform(), pelvisBone);
			kneeBone->name = "KneeBone";

			{
				Material * material = new Material(Shader::find("shader"));
				material->setDiffuseColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

				MeshObject * foot = new MeshObject(MeshFactory::Cube(false, glm::vec4(1.0f)), material);
				//GameObject * foot = new GameObject();
				foot->m_camera = &m_camera;
				foot->transform().setParent(skeleton->transform());
				//foot->transform().position() = glm::vec3(-2.0f, 0.0f, 7.5f);
				m_components.push_back(foot);

				Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
				KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(foot, interpolator, foot->transform().position(), false, true, false, true);

				// changable parameters
				float strokeMin = -2.0f;
				float strokeMax = 3.0f;
				float heightMin = 0.5f;
				float heightMax = 2.4f;
				//float cycleLength = 3000.0f; // TODO: this from somewhere
				float bodySpeed = 6.0f/1000.0f; // TODO: this from somewhere
				float legSpeed = 10.0f/1000.0f; // TODO: this from somewhere

				float stroke = strokeMax - strokeMin;
				float supportDuration = stroke / bodySpeed;
				float arcLength = std::sqrt(strokeMin*strokeMin + (heightMax - heightMin)*(heightMax - heightMin)) + std::sqrt((heightMax - heightMin)*(heightMax - heightMin) + strokeMax*strokeMax);
				float transferDuration = arcLength / legSpeed;
				float cycleLength = supportDuration + transferDuration;

				Trace::info("stroke: %f\n", stroke);
				Trace::info("supportDuration: %f\n", supportDuration);
				Trace::info("transferDuration: %f\n", transferDuration);

				float phase = 0.0f;
				animator->setIdle(phase);
				// support phase
				animator->addKeyframe(0.0f, glm::vec3(strokeMax, heightMin, 0.0f));  // foot forward (hits ground)
				animator->addKeyframe(supportDuration * 0.5f, glm::vec3(strokeMin + stroke * 0.5f, heightMin, 0.0f));  // foot straight in center
				animator->addKeyframe(supportDuration, glm::vec3(strokeMin, heightMin, 0.0f)); // foot is just about to lift
				animator->addKeyframe(supportDuration + transferDuration * 0.5f, glm::vec3(0.0f, heightMax, 0.0f));

				// dummy needs to repeat first
				animator->addKeyframe(supportDuration + transferDuration, glm::vec3(strokeMax, heightMin, 0.0f));  // foot hits ground


				 /*// in world space
				animator->addKeyframe(0.0f, glm::vec3(-3.0f, 1.5f, 0.0f)); // foot off ground
				animator->addKeyframe(500.0f, glm::vec3(-2.0f, 2.4f, 0.0f));
				animator->addKeyframe(1000.0f, glm::vec3(3.0f, 0.5f, 0.0f));  // foot hits ground
				animator->addKeyframe(1500.0f, glm::vec3(0.0f, 0.5f, 0.0f)); 
				animator->addKeyframe(2000.0f, glm::vec3(-3.0f, 1.5f, 0.0f)); // foot off ground*/

				foot->m_animator = animator;

				Bone * footBone = new Bone(foot->transform(), kneeBone);
				footBone->name = "FootBone";

				skeleton->addEndEffector(footBone);
			}
		}
		}



		///// 

		/*Material * red = new Material(Shader::find("shader"));
		red->setDiffuseColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		MeshObject * arm = new MeshObject(MeshFactory::FromFile("resources/space_frigate.ply"), red);
		arm->m_camera = &m_camera;

		Bone * armBone = new Bone(pelvis);
		armBone->transform().translate(glm::vec3(-5.0f, 0.0f, 0.0f));
		//arm->setBone(armBone);

		m_components.push_back(base);
		m_components.push_back(arm);*/

		return true;
	}

	void MainApplication::keyDown(Common::Key key)
	{
		if (key == Common::KEY_MOVE_FORWARD)
			m_camera.raiseFlag(Common::Camera::FORWARD);
		else if (key == Common::KEY_MOVE_BACKWARD)
			m_camera.raiseFlag(Common::Camera::BACKWARD);
		else if (key == Common::KEY_MOVE_LEFT)
			m_camera.raiseFlag(Common::Camera::LEFT);
		else if (key == Common::KEY_MOVE_RIGHT)
			m_camera.raiseFlag(Common::Camera::RIGHT);
	}

	void MainApplication::keyUp(Common::Key key)
	{
		switch (key)
		{
		case Common::KEY_MOVE_FORWARD:
			m_camera.dropFlag(Common::Camera::FORWARD);
			break;
		case Common::KEY_MOVE_BACKWARD:
			m_camera.dropFlag(Common::Camera::BACKWARD);
			break;
		case Common::KEY_MOVE_LEFT:
			m_camera.dropFlag(Common::Camera::LEFT);
			break;
		case Common::KEY_MOVE_RIGHT:
			m_camera.dropFlag(Common::Camera::RIGHT);
			break;
		case Common::KEY_RESET_2:
			m_camera.reset(glm::vec3(13.0f, 14.0f, -15.0f), glm::vec3(-2.0f, 0.0f, -2.0f));
			break;
		}
	}

	void MainApplication::mouse(Common::Key key, int x, int y)
	{
		if (key == Common::KEY_MOUSE_LEFT)
			m_camera.turn(glm::vec2(x, y));
	}

	void MainApplication::window_resized(int width, int height)
	{
		glViewport(0, 0, width, height);
		m_camera.set_projection(glm::perspective(45.0f, float(width)/float(height), 0.1f, 1000.0f));

		m_width = width;
		m_height = height;
	}

	void MainApplication::update(float dt)
	{
		m_camera.update(dt);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->update(dt);
		}

		// check for GL errors
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
			Trace::error("OpenGL error: %d\n", err);
	}

	void MainApplication::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->draw();
		}
	}
}
