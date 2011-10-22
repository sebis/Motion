#include "MeshObject.h"
#include "Interpolator.h"
#include "KeyframeAnimator.h"
#include "MainApplication.h"
#include "Texture.h"
#include "TransformRenderer.h"
#include "Trace.h"
#include "Utils.h"

#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"

namespace Interpolation
{
	using namespace Common;

	MainApplication::MainApplication(bool fixedTimeStep, float targetElapsedTime)
		: Base(fixedTimeStep, targetElapsedTime),
		m_camera(glm::vec3(13.0f, 14.0f, -15.0f), glm::vec3(-2.0f, 0.0f, -2.0f)),
		m_currentScene(0),
		m_numScenes(7)
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

		// TODO: Shouldn't need to "initialize" uniforms like this
		Shader * shader = Shader::find("lambert");
		shader->bind();
		shader->setUniform("lightDirection", glm::vec3(1.0f, 0.5f, 0.25f));
		shader->unbind();

		shader = Shader::find("phong");
		shader->bind();
		shader->setUniform("lightDirection", glm::vec3(1.0f, 0.5f, 0.25f));
		shader->unbind();

		setScene(m_currentScene);

		return true;
	}

	void MainApplication::setScene(int scene)
	{
		Trace::info("Switched to scene %d\n", scene);

		// clear old scene
		for (ComponentIterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			delete *it;
		}
		m_components.clear();

		if (scene == 0)
		{
			m_description = "Scene 1:\n"
				"Linear interpolation between control points (big dots).\n"
				"Time between small dots: 500ms.";

			Common::Material * material = new Common::Material(Shader::find("model"));
			material->setTexture(new Texture("resources/space_frigate.bmp"));
			Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/space_frigate.ply"), material);

			obj->m_camera = &m_camera;

			// create a linear interpolator for vec3 type
			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			// create a keyframe animator that animates position
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->transform().position(), false);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 1)
		{
			m_description = "Scene 2:\n"
				"Linear interpolation with reparameterization\n"
				"based on arc-length between control points.\n"
				"Dot interval: 500ms.";

			Common::Material * material = new Common::Material(Shader::find("model"));
			material->setTexture(new Texture("resources/space_frigate.bmp"));
			Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/space_frigate.ply"), material);

			obj->m_camera = &m_camera;

			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->transform().position(), true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 2)
		{
			m_description = "Scene 3:\n"
				"Interpolating the position on a CatmullRom-spline\n"
				"without arc-length parameterization.";

			Common::Material * material = new Common::Material(Shader::find("model"));
			material->setTexture(new Texture("resources/space_frigate.bmp"));
			Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/space_frigate.ply"), material);

			obj->m_camera = &m_camera;

			Interpolator<glm::vec3> * interpolator = new CatmullRomInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->transform().position(), false, true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		} 
		else if (scene == 3)
		{
			m_description = "Scene 4:\n"
				"Interpolating the position on a CatmullRom-spline\n"
				"with arc-length parameterization. Arc length is\n"
				"approximated by summing small linear segments.";

			Common::Material * material = new Common::Material(Shader::find("model"));
			material->setTexture(new Texture("resources/space_frigate.bmp"));
			Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/space_frigate.ply"), material);

			obj->m_camera = &m_camera;

			Interpolator<glm::vec3> * interpolator = new CatmullRomInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->transform().position(), true, true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f));
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f));
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f));
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f));
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f));
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f));
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f));
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f));
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f));
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f));
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f));
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f));

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 4)
		{
			m_description = "Scene 5:\n"
				"Interpolating the position on a Kochanek-Bartels-spline (TCB-spline)\n"
				"with arc-length paramtereization. TCB-values are randomly\n"
				"generated for each control point independently.";

			Utils::Random r;

			Common::Material * material = new Common::Material(Shader::find("model"));
			material->setTexture(new Texture("resources/space_frigate.bmp"));
			Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/space_frigate.ply"), material);

			obj->m_camera = &m_camera;

			Interpolator<glm::vec3> * interpolator = new KochanekBartelsInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->transform().position(), true, true);

			animator->setRenderer(new SplineRenderer(obj, interpolator, animator->keys()));

			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(2000.0f, glm::vec3(3.5f, 0.0f, 4.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(3000.0f, glm::vec3(8.0f, 0.0f, 2.5f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(4000.0f, glm::vec3(7.0f, 0.0f, 0.5f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(7000.0f, glm::vec3(5.0f, 0.0f, -0.5f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(13000.0f, glm::vec3(-0.5f, 3.0f, 6.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(15000.0f, glm::vec3(-9.0f, 0.0f, 3.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(16000.0f, glm::vec3(-6.0f, 2.0f, -8.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(18000.0f, glm::vec3(-3.0f, 0.0f, -8.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(20000.0f, glm::vec3(-4.0f, 0.0f, -13.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(21000.0f, glm::vec3(-1.0f, 0.0f, -12.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(23000.0f, glm::vec3(4.0f, 0.0f, -6.0f), r.rand11(), r.rand11(), r.rand11());
			animator->addKeyframe(24000.0f, glm::vec3(0.0f, 0.0f, 0.0f), r.rand11(), r.rand11(), r.rand11());

			obj->m_animator = animator;

			m_components.push_back(obj);
		}
		else if (scene == 5)
		{
			m_description = "Scene 6:\n"
				"Linear interpolation of orientation using euler angles.\n"
				"Gimbal lock occurs when one degree of freedom is lost when\n"
				"two rotation axes become parallel due to rotation ordering.\n"
				"Also linear interpolation doesn't take into account the\n"
				"continuity of spherical coordinates (eg. interpolating\n"
				"from -350 to 0 will always take the longer route).";

			glm::vec3 translation(-3.0f, 0.0f, -7.5f);
			glm::vec3 scale(5.0f);

			Common::Material * material = new Common::Material(Shader::find("model"));
			material->setTexture(new Texture("resources/spacecraft.bmp"));
			Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/spacecraft.ply"), material);

			obj->m_camera = &m_camera;
			obj->transform().translate(translation);

			Interpolator<glm::vec3> * interpolator = new LinearInterpolator<glm::vec3>;
			KeyframeAnimator<glm::vec3> * animator = new KeyframeAnimator<glm::vec3>(obj, interpolator, obj->transform().rotation(), false, false, false);
			animator->addKeyframe(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            animator->addKeyframe(3000.0f, glm::vec3(0.0f, 0.0f, 90.0f));
            animator->addKeyframe(6000.0f, glm::vec3(0.0f, 90.0f, 90.0f));
			animator->addKeyframe(9000.0f, glm::vec3(0.0f, 90.0f, 90.0f));
			animator->addKeyframe(12000.0f, glm::vec3(-90.0f, 90.0f, 90.0f));
			animator->addKeyframe(13000.0f, glm::vec3(-90.0f, 90.0f, 90.0f));
			animator->addKeyframe(16000.0f, glm::vec3(180.0f, 90.0f, 90.0f));
			animator->addKeyframe(17000.0f, glm::vec3(180.0f, 90.0f, 90.0f));
			animator->addKeyframe(20000.0f, glm::vec3(0.0f, 0.0f, 0.0f));
			obj->m_animator = animator;

			m_components.push_back(obj);

			Common::GameObject * gyroscope = new Common::GameObject;
			gyroscope->m_camera = &m_camera;
			gyroscope->m_renderer = new TransformRenderer(gyroscope, Shader::find("lambert"), &obj->transform());
			gyroscope->transform().translate(translation);
			gyroscope->transform().scale(glm::vec3(5.0f));
			
			m_components.push_back(gyroscope);
		}
		else if (scene == 6)
		{
			m_description = "Scene 7:\n"
				"Spherical interpolation using quaternions.\n"
				"Demonstrating the same animation using\n"
				"  a) linear interpolation on the sphere\n"
				"  b) spline interpolation using a cubic Bezier curve on the sphere.";

			{ // First object using SLERP
				Common::Material * material = new Common::Material(Shader::find("model"));
				material->setTexture(new Texture("resources/spacecraft.bmp"));
				Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/spacecraft.ply"), material);

				obj->m_camera = &m_camera;
				obj->transform().enableQuaternions();
				obj->transform().translate(glm::vec3(4.0f, 0.0f, -1.0f));

				Interpolator<glm::quat> * interpolator = new SphericalLinearInterpolator<glm::quat>;
				KeyframeAnimator<glm::quat> * animator = new KeyframeAnimator<glm::quat>(obj, interpolator, obj->transform().quaternion(), false, true, false);

				glm::quat q = glm::rotate(glm::quat(), 0.0f, glm::vec3(1, 0, 0));
				animator->addKeyframe(0.0f, q);
				q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
				animator->addKeyframe(2000.0f, q);
				q = glm::rotate(q, 90.0f, glm::vec3(0, 1, 0));
				animator->addKeyframe(4000.0f, q);
				q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
				animator->addKeyframe(6000.0f, q);
				q = glm::rotate(q, 180.0f, glm::vec3(0, 1, 0));
				animator->addKeyframe(8000.0f, q);
				q = glm::rotate(q, -90.0f, glm::vec3(0, 0, 1));
				animator->addKeyframe(10000.0f, q);

				obj->m_animator = animator;

				m_components.push_back(obj);
			}

			{ // Second object using Bezier-SLERP
				Common::Material * material = new Common::Material(Shader::find("model"));
				material->setTexture(new Texture("resources/spacecraft.bmp"));
				Common::GameObject * obj = new MeshObject(MeshFactory::FromFile("resources/spacecraft.ply"), material);

				obj->m_camera = &m_camera;
				obj->transform().enableQuaternions();
				obj->transform().translate(glm::vec3(-3.0f, 0.0f, -7.5f));

				Interpolator<glm::quat> * interpolator = new BezierSphericalLinearInterpolator<glm::quat>;
				KeyframeAnimator<glm::quat> * animator = new KeyframeAnimator<glm::quat>(obj, interpolator, obj->transform().quaternion(), false, false, false);

				glm::quat q = glm::rotate(glm::quat(), 0.0f, glm::vec3(1, 0, 0));
				animator->addKeyframe(0.0f, q);
				q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
				animator->addKeyframe(2000.0f, q);
				q = glm::rotate(q, 90.0f, glm::vec3(0, 1, 0));
				animator->addKeyframe(4000.0f, q);
				q = glm::rotate(q, -90.0f, glm::vec3(1, 0, 0));
				animator->addKeyframe(6000.0f, q);
				q = glm::rotate(q, 180.0f, glm::vec3(0, 1, 0));
				animator->addKeyframe(8000.0f, q);
				q = glm::rotate(q, -90.0f, glm::vec3(0, 0, 1));
				animator->addKeyframe(10000.0f, q);

				obj->m_animator = animator;

				m_components.push_back(obj);
			}
		}
		else
		{
			Trace::warning("No scene %d found!\n", scene);
		}
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
			setScene(++m_currentScene % m_numScenes);
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
		case Common::KEY_RESET_1:
			setScene(m_currentScene % m_numScenes);
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
		m_camera.set_projection(glm::perspective(45.0f, float(width)/float(height), 0.1f, 100.0f));

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

		display_text(m_description.c_str(), 10, 25);
	}
}
