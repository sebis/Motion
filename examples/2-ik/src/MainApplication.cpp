/**
 * Copyright 2011-2012 Sebastian Eriksson.
 * This file is part of Motion.
 *
 * Motion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Motion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Motion. If not, see <http://www.gnu.org/licenses/>.
 */

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

	class Gait
	{
	public:
		float strokeMin;
		float strokeMax;
		float heightMin;
		float heightMax;
		float legSpeed;
		float bodySpeed;
	};

	MainApplication::MainApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: Base(title, fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(10.0f), glm::vec3(0.0f)),
		m_currentTarget(0)
	{
	}

	MainApplication::~MainApplication()
	{
	}

	namespace
	{
		void createGaitAnimation(GameObject * target, float phase, const Gait& gait, float offset = 0.0f)
		{
			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(target, interpolator, target->transform().position(), Interpolation::CONNECT);

			// Calculate gait parameters based on Girard 85

			float stroke = gait.strokeMax - gait.strokeMin;
			float supportDuration = stroke / gait.bodySpeed;

			// TODO: how about other splines than linear? Would need to create a separate spline class that doesn't require animation..
			float min2 = gait.strokeMin*gait.strokeMin;
			float max2 = gait.strokeMax*gait.strokeMax;
			float h = gait.heightMax - gait.heightMin;
			float arcLength = std::sqrt(min2 + h*h) + std::sqrt(h*h + max2);

			float transferDuration = arcLength / gait.legSpeed;
			float cycleLength = supportDuration + transferDuration;

			/// Create a keyframe animation based on the gait cycle that animates the end effector position

			// "hack" to create phased animations
			animator->setIdle(cycleLength * phase);
			
			animator->addKeyframe(0.0f, glm::vec3(gait.strokeMax, gait.heightMin, offset));  // foot forward (hits ground)
			animator->addKeyframe(supportDuration * 0.5f, glm::vec3(gait.strokeMin + stroke * 0.5f, gait.heightMin, offset));  // foot straight at center
			animator->addKeyframe(supportDuration, glm::vec3(gait.strokeMin, gait.heightMin, offset)); // foot is just about to lift
			animator->addKeyframe(supportDuration + transferDuration * 0.5f, glm::vec3(0.0f, gait.heightMax, offset)); // transfer phase
			animator->addKeyframe(supportDuration + transferDuration, glm::vec3(gait.strokeMax, gait.heightMin, offset));  // foot hits ground

			target->m_animator = animator;
		}

		// Define a global variable for leg
		Mesh * g_leg;
	}

	Skeleton * MainApplication::createBiped(const Gait& gait)
	{
		/// Create a skeleton

		Skeleton * skeleton = new Skeleton;

		/// Create some materials

		Material * green = new Material(Shader::find("shader"));
		green->setDiffuseColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		
		Material * yellow = new Material(Shader::find("shader"));
		yellow->setDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		Material * red = new Material(Shader::find("shader"));
		red->setDiffuseColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		/// Create leg part meshes

		MeshObject * rightHip = new MeshObject(g_leg, green);
		rightHip->transform().translate(glm::vec3(0.0f, 10.0f, 1.0f));
		rightHip->transform().setParent(skeleton->transform());

		MeshObject * leftHip = new MeshObject(g_leg, green);
		leftHip->transform().translate(glm::vec3(0.0f, 10.0f, -1.0f));
		leftHip->transform().setParent(skeleton->transform());
		
		MeshObject * rightKnee = new MeshObject(g_leg, yellow);
		rightKnee->transform().setParent(rightHip->transform());
		rightKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
		MeshObject * leftKnee = new MeshObject(g_leg, yellow);
		leftKnee->transform().setParent(leftHip->transform());
		leftKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
		GameObject * rightFoot = new GameObject();
		rightFoot->transform().setParent(skeleton->transform());
		
		GameObject * leftFoot = new GameObject();
		leftFoot->transform().setParent(skeleton->transform());

		// Assign animations to the end effectors
		createGaitAnimation(rightFoot, 0.0f, gait);
		createGaitAnimation(leftFoot, 0.5f, gait);

		/// Create bones

		Bone * rightHipBone = new Bone("RightHip", rightHip->transform());
		Bone * leftHipBone = new Bone("LeftHip", leftHip->transform());
		Bone * rightKneeBone = new Bone("RightKnee", rightKnee->transform(), rightHipBone);
		Bone * leftKneeBone = new Bone("LeftKnee", leftKnee->transform(), leftHipBone);
		Bone * rightFootBone = new Bone("RightFoot", rightFoot->transform(), rightKneeBone);
		Bone * leftFootBone = new Bone("LeftFoot", leftFoot->transform(), leftKneeBone);

		// Set constraints based on joint type
		rightHipBone->axis(2)->setConstraints(-90.0f, 90.0f);
		leftHipBone->axis(2)->setConstraints(-90.0f, 90.0f);
		rightKneeBone->axis(2)->setConstraints(-180.0f, 0.0f);
		leftKneeBone->axis(2)->setConstraints(-180.0f, 0.0f);

		// Calculate a local transformations (offset to child bone)
		rightHipBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		rightKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftHipBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));

		// set end effectors
		skeleton->addEndEffector(rightFootBone);
		skeleton->addEndEffector(leftFootBone);

		m_components.push_back(rightHip);
		m_components.push_back(leftHip);
		m_components.push_back(rightKnee);
		m_components.push_back(leftKnee);
		m_components.push_back(rightFoot);
		m_components.push_back(leftFoot);

		return skeleton;
	}

	Skeleton * MainApplication::createQuadrupedal(const Gait& gait, float gaitPhase[4])
	{
		/// Create a skeleton

		Skeleton * skeleton = new Skeleton;

		/// Create some materials

		Material * green = new Material(Shader::find("shader"));
		green->setDiffuseColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		
		Material * yellow = new Material(Shader::find("shader"));
		yellow->setDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		Material * red = new Material(Shader::find("shader"));
		red->setDiffuseColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		/// Create leg part meshes

		GameObject * pelvis = new GameObject;
		pelvis->transform().translate(glm::vec3(-3.0f, 0.0f, 0.0f));
		pelvis->transform().setParent(skeleton->transform());

		GameObject * scapula = new GameObject;
		scapula->transform().translate(glm::vec3(3.0f, 0.0f, 0.0f));
		scapula->transform().setParent(skeleton->transform());

		MeshObject * rightHip = new MeshObject(g_leg, green);
		rightHip->transform().translate(glm::vec3(0.0f, 9.5f, 1.0f));
		rightHip->transform().setParent(pelvis->transform());

		MeshObject * rightShoulder = new MeshObject(g_leg, green);
		rightShoulder->transform().translate(glm::vec3(0.0f, 9.5f, 1.0f));
		rightShoulder->transform().setParent(scapula->transform());

		MeshObject * leftHip = new MeshObject(g_leg, green);
		leftHip->transform().translate(glm::vec3(0.0f, 9.5f, -1.0f));
		leftHip->transform().setParent(pelvis->transform());
		
		MeshObject * leftShoulder = new MeshObject(g_leg, green);
		leftShoulder->transform().translate(glm::vec3(0.0f, 9.5f, -1.0f));
		leftShoulder->transform().setParent(scapula->transform());

		MeshObject * rightBackKnee = new MeshObject(g_leg, yellow);
		rightBackKnee->transform().setParent(rightHip->transform());
		rightBackKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));

		MeshObject * rightFrontKnee = new MeshObject(g_leg, yellow);
		rightFrontKnee->transform().setParent(rightShoulder->transform());
		rightFrontKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
		MeshObject * leftBackKnee = new MeshObject(g_leg, yellow);
		leftBackKnee->transform().setParent(leftHip->transform());
		leftBackKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));

		MeshObject * leftFrontKnee = new MeshObject(g_leg, yellow);
		leftFrontKnee->transform().setParent(leftShoulder->transform());
		leftFrontKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
		GameObject * rightBackFoot = new GameObject();
		rightBackFoot->transform().setParent(pelvis->transform());

		GameObject * rightFrontFoot = new GameObject();
		rightFrontFoot->transform().setParent(scapula->transform());
		
		GameObject * leftBackFoot = new GameObject();
		leftBackFoot->transform().setParent(pelvis->transform());

		GameObject * leftFrontFoot = new GameObject();
		leftFrontFoot->transform().setParent(scapula->transform());

		/// Create gait animations

		createGaitAnimation(leftBackFoot, gaitPhase[0], gait);
		createGaitAnimation(rightBackFoot, gaitPhase[1], gait);
		createGaitAnimation(leftFrontFoot, gaitPhase[2], gait);
		createGaitAnimation(rightFrontFoot, gaitPhase[3], gait);

		/// Create bones and set constraints

		Bone * rightHipBone = new Bone("RightHip", rightHip->transform());
		Bone * leftHipBone = new Bone("LeftHip", leftHip->transform());
		Bone * rightShoulderBone = new Bone("RightShoulder", rightShoulder->transform());
		Bone * leftShoulderBone = new Bone("LeftShoulder", leftShoulder->transform());

		Bone * rightBackKneeBone = new Bone("RightBackKnee", rightBackKnee->transform(), rightHipBone);
		Bone * leftBackKneeBone = new Bone("LeftBackKnee", leftBackKnee->transform(), leftHipBone);
		Bone * rightFrontKneeBone = new Bone("RightFrontKnee", rightFrontKnee->transform(), rightShoulderBone);
		Bone * leftFrontKneeBone = new Bone("LeftFrontKnee", leftFrontKnee->transform(), leftShoulderBone);

		Bone * rightBackFootBone = new Bone("RightBackFoot", rightBackFoot->transform(), rightBackKneeBone);
		Bone * leftBackFootBone = new Bone("LeftBackFoot", leftBackFoot->transform(), leftBackKneeBone);
		Bone * rightFrontFootBone = new Bone("RighFronttFoot", rightFrontFoot->transform(), rightFrontKneeBone);
		Bone * leftFrontFootBone = new Bone("LeftFoot", leftFrontFoot->transform(), leftFrontKneeBone);

		rightHipBone->axis(2)->setConstraints(-90.0f, 90.0f);
		leftHipBone->axis(2)->setConstraints(-90.0f, 90.0f);
		rightShoulderBone->axis(2)->setConstraints(-90.0f, 90.0f);
		leftShoulderBone->axis(2)->setConstraints(-90.0f, 90.0f);

		rightBackKneeBone->axis(2)->setConstraints(0.0f, 180.0f);
		leftBackKneeBone->axis(2)->setConstraints(0.0f, 180.0f);
		rightFrontKneeBone->axis(2)->setConstraints(-180.0f, 0.0f);
		leftFrontKneeBone->axis(2)->setConstraints(-180.0f, 0.0f);

		rightHipBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		rightBackKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		rightShoulderBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		rightFrontKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftHipBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftBackKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftShoulderBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		leftFrontKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));

		skeleton->addEndEffector(rightBackFootBone);
		skeleton->addEndEffector(leftBackFootBone);
		skeleton->addEndEffector(rightFrontFootBone);
		skeleton->addEndEffector(leftFrontFootBone);

		m_components.push_back(rightHip);
		m_components.push_back(leftHip);
		m_components.push_back(rightShoulder);
		m_components.push_back(leftShoulder);
		m_components.push_back(rightBackKnee);
		m_components.push_back(leftBackKnee);
		m_components.push_back(rightFrontKnee);
		m_components.push_back(leftFrontKnee);
		m_components.push_back(rightBackFoot);
		m_components.push_back(leftBackFoot);
		m_components.push_back(rightFrontFoot);
		m_components.push_back(leftFrontFoot);

		return skeleton;
	}

	Skeleton * MainApplication::createCentipede(const Gait& gait, int segments)
	{
		/// Create a skeleton

		Skeleton * centipede = new Skeleton;

		/// Create some materials

		Material * green = new Material(Shader::find("shader"));
		green->setDiffuseColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		
		Material * yellow = new Material(Shader::find("shader"));
		yellow->setDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		Material * red = new Material(Shader::find("shader"));
		red->setDiffuseColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		/// Create leg part meshes

		float rightGaitPhase[3] = { 0.5f, 0.8f, 0.1f };
		float leftGaitPhase[3] = { 0.0f, 0.3f, 0.6f };

		// Create a pair of legs for each segment
		for (int i = 0; i < segments; i++)
		{
			GameObject * vertebra = new GameObject();
			vertebra->transform().translate(float(i) * glm::vec3(2.0f, 0.0f, 0.0f));
			vertebra->transform().setParent(centipede->transform());

			MeshObject * rightHip = new MeshObject(g_leg, green);
			rightHip->transform().translate(glm::vec3(0.0f, 4.0f, 0.5f));
			rightHip->transform().setParent(vertebra->transform());

			MeshObject * leftHip = new MeshObject(g_leg, green);
			leftHip->transform().translate(glm::vec3(0.0f, 4.0f, -0.5f));
			leftHip->transform().setParent(vertebra->transform());
		
			MeshObject * rightKnee = new MeshObject(g_leg, yellow);
			rightKnee->transform().setParent(rightHip->transform());
			rightKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
			MeshObject * leftKnee = new MeshObject(g_leg, yellow);
			leftKnee->transform().setParent(leftHip->transform());
			leftKnee->transform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
		
			GameObject * rightFoot = new GameObject();
			rightFoot->transform().setParent(vertebra->transform());
		
			GameObject * leftFoot = new GameObject();
			leftFoot->transform().setParent(vertebra->transform());

			// Use a repeating wave gait phase
			createGaitAnimation(rightFoot, rightGaitPhase[i % 3], gait, 6.0f);
			createGaitAnimation(leftFoot, leftGaitPhase[i % 3], gait, -6.0f);

			Bone * rightHipBone = new Bone("RightHip", rightHip->transform());
			Bone * leftHipBone = new Bone("LeftHip", leftHip->transform());
			Bone * rightKneeBone = new Bone("RightKnee", rightKnee->transform(), rightHipBone);
			Bone * leftKneeBone = new Bone("LeftKnee", leftKnee->transform(), leftHipBone);
			Bone * rightFootBone = new Bone("RightFoot", rightFoot->transform(), rightKneeBone);
			Bone * leftFootBone = new Bone("LeftFoot", leftFoot->transform(), leftKneeBone);

			rightHipBone->axis(0)->setConstraints(-180.0f, 90.0f);
			//rightHipBone->axis(1)->setConstraints(-90.0f, 90.0f);
			rightHipBone->axis(2)->setConstraints(-90.0f, 90.0f);
			rightKneeBone->axis(0)->setConstraints(0.0f, 180.0f);

			leftHipBone->axis(0)->setConstraints(-90.0f, 180.0f);
			//leftHipBone->axis(1)->setConstraints(-90.0f, 90.0f);
			leftHipBone->axis(2)->setConstraints(-90.0f, 90.0f);
			leftKneeBone->axis(0)->setConstraints(-180.0f, 0.0f);

			rightHipBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
			rightKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
			leftHipBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));
			leftKneeBone->localTransform().translate(glm::vec3(0.0f, -5.0f, 0.0f));

			centipede->addEndEffector(rightFootBone);
			centipede->addEndEffector(leftFootBone);

			m_components.push_back(rightHip);
			m_components.push_back(leftHip);
			m_components.push_back(rightKnee);
			m_components.push_back(leftKnee);
			m_components.push_back(rightFoot);
			m_components.push_back(leftFoot);
		}

		return centipede;
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

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		Common::GameObject::s_camera = &m_camera;

		/// Create floor

		Material * floorMaterial = new Material(Shader::find("shader"));
		floorMaterial->setTexture(new Texture("resources/dirt.bmp"));
		floorMaterial->setDiffuseColor(glm::vec4(0.8f));
		floorMaterial->setSpecularColor(glm::vec4(0.2f));

		MeshObject * floor = new MeshObject(MeshFactory::Plane(glm::vec4(1.0f), 50), floorMaterial);
		floor->transform().scale() = glm::vec3(500.0f);
		
		m_components.push_back(floor);

		// Load the basic leg mesh from a file
		g_leg = MeshFactory::FromFile("resources/leg_part.ply");

		/// Create walking human skeleton

		Gait walking = { -3.0f, 3.0f, 0.0f, 3.5f, 10.0f/1000.0f, 6.0f/1000.0f };

		Skeleton * human = createBiped(walking);

		human->m_animator = new LocationAnimator(human, glm::vec3(walking.bodySpeed, 0.0f, 0.0f));
		human->transform().translate(glm::vec3(0.0f, 0.0f, -10.0f));

		m_components.push_back(human);

		/// Create galloping horses with different gaits

		Gait gallop = { -3.0f, 4.0f, 0.0f, 3.5f, 10.0f/1000.0f, 10.0f/1000.0f };

		float rotaryGallopPhase[4] = { 0.0f, 0.1f, 0.6f, 0.5f };
		Skeleton * horse = createQuadrupedal(gallop, rotaryGallopPhase);
		horse->m_animator = new LocationAnimator(horse, glm::vec3(gallop.bodySpeed, 0.0f, 0.0f));
		horse->transform().translate(glm::vec3(0.0f, 0.0f, 0.0f));
		m_components.push_back(horse);

		float trotPhase[4] = { 0.0f, 0.5f, 0.5f, 0.0f };
		Skeleton * horse2 = createQuadrupedal(gallop, trotPhase);
		horse2->m_animator = new LocationAnimator(horse2, glm::vec3(gallop.bodySpeed, 0.0f, 0.0f));
		horse2->transform().translate(glm::vec3(0.0f, 0.0f, 15.0f));
		m_components.push_back(horse2);

		float amblePhase[4] = { 0.0f, 0.5f, 0.2f, 0.7f };
		Skeleton * horse3 = createQuadrupedal(gallop, amblePhase);
		horse3->m_animator = new LocationAnimator(horse3, glm::vec3(gallop.bodySpeed, 0.0f, 0.0f));
		horse3->transform().translate(glm::vec3(0.0f, 0.0f, -15.0f));
		m_components.push_back(horse3);

		/// Create a centipede with six segments

		Gait wave = { -1.0f, 1.0f, 0.0f, 1.5f, 15.0f/1000.0f, 5.0f/1000.0f };
		Skeleton * centipede = createCentipede(wave, 6);

		centipede->m_animator = new LocationAnimator(centipede, glm::vec3(wave.bodySpeed, 0.0f, 0.0f));

		m_components.push_back(centipede);

		// Set some camera targets
		m_cameraTargets.push_back(&human->transform().position());
		m_cameraTargets.push_back(&horse->transform().position());
		m_cameraTargets.push_back(&centipede->transform().position());

		m_cameraTarget = m_cameraTargets[m_currentTarget];

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
		else if (key == Common::KEY_CONTINUE)
			m_cameraTarget = m_cameraTargets[++m_currentTarget % m_cameraTargets.size()];
		else if (key == Common::KEY_RESET_2)
			m_camera.reset(glm::vec3(13.0f, 14.0f, -15.0f), glm::vec3(-2.0f, 0.0f, -2.0f));
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
		default:
			break;
		}
	}

	void MainApplication::mouse(Common::Key key, int x, int y)
	{
		m_cameraTarget = 0;

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
		if (m_cameraTarget)
			m_camera.reset(*m_cameraTarget + glm::vec3(25.0f), *m_cameraTarget);
		else
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
