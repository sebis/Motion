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

	namespace
	{
		void createGautAnimation(GameObject * target, float phase, float bodySpeed)
		{
			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(target, interpolator, target->transform().position(), false, true, false, true);

			// changable parameters
			float strokeMin = -2.0f;
			float strokeMax = 3.0f;
			float heightMin = 0.5f;
			float heightMax = 2.4f;
			float legSpeed = 6.0f/1000.0f; // TODO: this from somewhere

			float stroke = strokeMax - strokeMin;
			float supportDuration = stroke / bodySpeed;
			float arcLength = std::sqrt(strokeMin*strokeMin + (heightMax - heightMin)*(heightMax - heightMin)) + std::sqrt((heightMax - heightMin)*(heightMax - heightMin) + strokeMax*strokeMax);
			float transferDuration = arcLength / legSpeed;
			float cycleLength = supportDuration + transferDuration;

			Trace::info("stroke: %f\n", stroke);
			Trace::info("cycleLength: %f\n", cycleLength);
			Trace::info("supportDuration: %f\n", supportDuration);
			Trace::info("transferDuration: %f\n", transferDuration);

			animator->setIdle(cycleLength * phase);
			// support phase
			animator->addKeyframe(0.0f, glm::vec3(strokeMax, heightMin, 0.0f));  // foot forward (hits ground)
			animator->addKeyframe(supportDuration * 0.5f, glm::vec3(strokeMin + stroke * 0.5f, heightMin, 0.0f));  // foot straight in center
			animator->addKeyframe(supportDuration, glm::vec3(strokeMin, heightMin, 0.0f)); // foot is just about to lift
			animator->addKeyframe(supportDuration + transferDuration * 0.5f, glm::vec3(0.0f, heightMax, 0.0f));

			// dummy needs to repeat first
			animator->addKeyframe(supportDuration + transferDuration, glm::vec3(strokeMax, heightMin, 0.0f));  // foot hits ground

			target->m_animator = animator;
		}
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

		Common::GameObject::s_camera = &m_camera;

		/// Create floor

		Material * floorMaterial = new Material(Shader::find("shader"));
		floorMaterial->setTexture(new Texture("resources/checker.bmp"));

		MeshObject * floor = new MeshObject(MeshFactory::Plane(glm::vec4(1.0f), 5), floorMaterial);
		// TODO: shouldn't need to set camera explicitly
		floor->transform().scale() = glm::vec3(500.0f);
		floor->transform().position() = glm::vec3(0.0f, -1.0f, 0.0f);
		
		m_components.push_back(floor);

		/// Create a skeleton

		Skeleton * skeleton = new Skeleton;
		skeleton->transform().translate(glm::vec3(0.0f, 0.0f, 0.0f));

		float bodySpeed = 6/1000.0f; // 6 units per second
		//skeleton->m_animator = new LocationAnimator(skeleton, glm::vec3(bodySpeed, 0.0f, 0.0f));

		

		/// Create some materials

		Material * green = new Material(Shader::find("shader"));
		green->setDiffuseColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		
		Material * yellow = new Material(Shader::find("shader"));
		yellow->setDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		Material * red = new Material(Shader::find("shader"));
		red->setDiffuseColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		MeshObject * rightHip = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), green);
		rightHip->transform().translate(glm::vec3(0.0f, 9.5f, 1.0f));
		rightHip->transform().setParent(skeleton->transform());

		MeshObject * leftHip = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), green);
		leftHip->transform().translate(glm::vec3(0.0f, 9.5f, -1.0f));
		leftHip->transform().setParent(skeleton->transform());
		
		MeshObject * rightKnee = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), yellow);
		rightKnee->transform().setParent(rightHip->transform());
		rightKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
		MeshObject * leftKnee = new MeshObject(MeshFactory::FromFile("resources/leg_part.ply"), yellow);
		leftKnee->transform().setParent(leftHip->transform());
		leftKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
		MeshObject * rightFoot = new MeshObject(MeshFactory::Cube(false, glm::vec4(1.0f)), red);
		rightFoot->transform().setParent(skeleton->transform());
		
		MeshObject * leftFoot = new MeshObject(MeshFactory::Cube(false, glm::vec4(1.0f)), red);
		leftFoot->transform().setParent(skeleton->transform());

		createGautAnimation(rightFoot, 0.0f, bodySpeed);
		createGautAnimation(leftFoot, 0.5f, bodySpeed);

		Bone * rightHipBone = new Bone("RightHip", rightHip->transform());
		Bone * leftHipBone = new Bone("LeftHip", leftHip->transform());
		Bone * rightKneeBone = new Bone("RightKnee", rightKnee->transform(), rightHipBone);
		Bone * leftKneeBone = new Bone("LeftKnee", leftKnee->transform(), leftHipBone);
		Bone * rightFootBone = new Bone("RightFoot", rightFoot->transform(), rightKneeBone);
		Bone * leftFootBone = new Bone("LeftFoot", leftFoot->transform(), leftKneeBone);

		rightHipBone->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));
		leftHipBone->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));
		rightKneeBone->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));
		leftKneeBone->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));

		rightHipBone->m_localTransform.translate(glm::vec3(0.0f, -5.0f, 0.0f));
		rightKneeBone->m_localTransform.translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftHipBone->m_localTransform.translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftKneeBone->m_localTransform.translate(glm::vec3(0.0f, -5.0f, 0.0f));

		// TEMP START
		/*rightHipBone->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));
		rightHipBone->m_localTransform.translate(glm::vec3(0.0f, -5.0f, 0.0f));
		rightKneeBone->setAxis(glm::vec3(0.0f, 0.0f, 1.0f));
		rightKneeBone->m_localTransform.translate(glm::vec3(0.0f, -5.0f, 0.0f));
		Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
		KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(rightFoot, interpolator, rightFoot->transform().position(), false, false, false, true);
		animator->addKeyframe(0.0f, glm::vec3(1.0f, 2.0f, 0.0f));
		animator->addKeyframe(3000.0f, glm::vec3(10.0f, 2.0f, 0.0f));
		animator->addKeyframe(6000.0f, glm::vec3(1.0f, 2.0f, 0.0f));
		animator->addKeyframe(9000.0f, glm::vec3(-10.0f, 2.0f, 0.0f));
		animator->addKeyframe(12000.0f, glm::vec3(1.0f, 2.0f, 0.0f));
		rightFoot->m_animator = animator;*/
		// TEMP END

		skeleton->addEndEffector(rightFootBone);
		skeleton->addEndEffector(leftFootBone);

		m_components.push_back(rightHip);
		m_components.push_back(leftHip);
		m_components.push_back(rightKnee);
		m_components.push_back(leftKnee);
		m_components.push_back(rightFoot);
		m_components.push_back(leftFoot);

		m_components.push_back(skeleton);

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
