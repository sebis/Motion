#include "GlutApplication.h"
#include "Trace.h"

#include <GL/freeglut.h>
#include <iostream>

namespace Common
{
	GlutApplication * GlutApplication::m_instance = 0;

	GlutApplication::GlutApplication(const char * title, bool fixedTimeStep, float targetElapsedTime)
		: m_title(title),
		  m_fixedTimeStep(fixedTimeStep),
		  m_targetElapsedTime(targetElapsedTime),
		  m_totalTime(0),
		  m_mouseButton(0)
	{
	}

	bool GlutApplication::init(int argc, char * argv[])
	{
		m_instance = this;

		//glutInitContextVersion(3, 1);
		//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
		//glutInitContextProfile(GLUT_CORE_PROFILE);

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize(800, 600);
		glutCreateWindow(m_title);

		reshapeWrapper(800, 600);

		return true;
	}

	int GlutApplication::run()
	{
		if (m_fixedTimeStep) {
			glutTimerFunc(0, updateWrapper, 0);
			glutIdleFunc(drawWrapper);
		}
		else {
			glutIdleFunc(updateWrapper);
		}

		glutKeyboardFunc(keyboardWrapper);
		glutKeyboardUpFunc(keyboardUpWrapper);
		glutSpecialFunc(specialWrapper);
		glutSpecialUpFunc(specialUpWrapper);
		glutMouseFunc(mouseWrapper);
		glutMotionFunc(motionWrapper);
		glutReshapeFunc(reshapeWrapper);

		glutMainLoop();

		return 0;
	}

	void GlutApplication::update(float /*dt*/)
	{
	}

	void GlutApplication::draw()
	{
	}

	void GlutApplication::display_text(const char * text, int x, int y)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, m_width, m_height, 0);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glRasterPos3f(float(x), float(y), 0.0f);
		glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)text);
	}

	void GlutApplication::motionWrapper(int x, int y)
	{
		int middleX = int(m_instance->m_width/2);
		int middleY = int(m_instance->m_height/2);

		if (middleX != x || middleY != y)
		{
			if (m_instance->m_mouseButton == GLUT_LEFT_BUTTON) {
				m_instance->mouse(KEY_MOUSE_LEFT, middleX - x, middleY - y);
				glutWarpPointer(middleX, middleY);
			} else {
				m_instance->mouse(KEY_MOUSE_RIGHT, x, y);
			}
		}
	}

	void GlutApplication::mouseWrapper(int button, int state, int /*x*/, int /*y*/)
	{
		if (state == GLUT_UP) {
			m_instance->m_mouseButton = 0;
			return;
		}

		m_instance->m_mouseButton = button;

		if (button == GLUT_LEFT_BUTTON)
			glutWarpPointer(int(m_instance->m_width/2), int(m_instance->m_height/2));
	}

	void GlutApplication::keyboardWrapper(unsigned char key, int /*x*/, int /*y*/)
	{
		if (key == 'q' || key == 'Q' || key == 27)
			glutLeaveMainLoop();
		else if (key == 'a' || key == 'A')
			m_instance->keyDown(KEY_MOVE_LEFT);
		else if (key == 'd' || key == 'D')
			m_instance->keyDown(KEY_MOVE_RIGHT);
		else if (key == 'w' || key == 'W')
			m_instance->keyDown(KEY_MOVE_FORWARD);
		else if (key == 's' || key == 'S')
			m_instance->keyDown(KEY_MOVE_BACKWARD);
		else if (key == 'c' || key == 'C')
			m_instance->keyDown(KEY_RESET_2);
		else if (key == 'h' || key == 'H')
			m_instance->keyDown(KEY_HELP);
		else if (key == '+')
			m_instance->keyDown(KEY_NEXT);
		else if (key == '-')
			m_instance->keyDown(KEY_PREV);
		else if (key == 'v' || key == 'V')
			m_instance->keyDown(KEY_VERBOSE);
		else if (key == '0')
			m_instance->keyDown(KEY_0);
		else if (key == '1')
			m_instance->keyDown(KEY_1);
		else if (key == '2')
			m_instance->keyDown(KEY_2);
		else if (key == '3')
			m_instance->keyDown(KEY_3);
		else if (key == '4')
			m_instance->keyDown(KEY_4);
		else if (key == '5')
			m_instance->keyDown(KEY_5);
		else if (key == '6')
			m_instance->keyDown(KEY_6);
		else if (key == '7')
			m_instance->keyDown(KEY_7);
		else if (key == '8')
			m_instance->keyDown(KEY_8);
		else if (key == '9')
			m_instance->keyDown(KEY_9);
		else if (key == 13) // return
			m_instance->keyDown(KEY_RESET_1);
		else if (key == 32) // space
			m_instance->keyDown(KEY_CONTINUE);
	}

	void GlutApplication::keyboardUpWrapper(unsigned char key, int /*x*/, int /*y*/)
	{
		if (key == 'q' || key == 'Q' || key == 27)
			glutLeaveMainLoop();
		else if (key == 'a' || key == 'A')
			m_instance->keyUp(KEY_MOVE_LEFT);
		else if (key == 'd' || key == 'D')
			m_instance->keyUp(KEY_MOVE_RIGHT);
		else if (key == 'w' || key == 'W')
			m_instance->keyUp(KEY_MOVE_FORWARD);
		else if (key == 's' || key == 'S')
			m_instance->keyUp(KEY_MOVE_BACKWARD);
		else if (key == 'c' || key == 'C')
			m_instance->keyUp(KEY_RESET_2);
		else if (key == 'h' || key == 'H')
			m_instance->keyUp(KEY_HELP);
		else if (key == '+')
			m_instance->keyUp(KEY_NEXT);
		else if (key == '-')
			m_instance->keyUp(KEY_PREV);
		else if (key == 'v' || key == 'V')
			m_instance->keyUp(KEY_VERBOSE);
		else if (key == '0')
			m_instance->keyUp(KEY_0);
		else if (key == '1')
			m_instance->keyUp(KEY_1);
		else if (key == '2')
			m_instance->keyUp(KEY_2);
		else if (key == '3')
			m_instance->keyUp(KEY_3);
		else if (key == '4')
			m_instance->keyUp(KEY_4);
		else if (key == '5')
			m_instance->keyUp(KEY_5);
		else if (key == '6')
			m_instance->keyUp(KEY_6);
		else if (key == '7')
			m_instance->keyUp(KEY_7);
		else if (key == '8')
			m_instance->keyUp(KEY_8);
		else if (key == '9')
			m_instance->keyUp(KEY_9);
		else if (key == 13) // return
			m_instance->keyUp(KEY_RESET_1);
		else if (key == 32) // space
			m_instance->keyUp(KEY_CONTINUE);
	}

	void GlutApplication::specialWrapper(int key, int /*x*/, int /*y*/)
	{
		if (key == GLUT_KEY_LEFT)
			m_instance->keyDown(KEY_MOVE_LEFT);
		else if (key == GLUT_KEY_RIGHT)
			m_instance->keyDown(KEY_MOVE_RIGHT);
		else if (key == GLUT_KEY_UP)
			m_instance->keyDown(KEY_MOVE_FORWARD);
		else if (key == GLUT_KEY_DOWN)
			m_instance->keyDown(KEY_MOVE_BACKWARD);
	}

	void GlutApplication::specialUpWrapper(int key, int /*x*/, int /*y*/)
	{
		if (key == GLUT_KEY_LEFT)
			m_instance->keyUp(KEY_MOVE_LEFT);
		else if (key == GLUT_KEY_RIGHT)
			m_instance->keyUp(KEY_MOVE_RIGHT);
		else if (key == GLUT_KEY_UP)
			m_instance->keyUp(KEY_MOVE_FORWARD);
		else if (key == GLUT_KEY_DOWN)
			m_instance->keyUp(KEY_MOVE_BACKWARD);
	}

	void GlutApplication::reshapeWrapper(int width, int height)
	{
		m_instance->window_resized(width, height);
	}

	void GlutApplication::updateWrapper(int /*value*/)
	{
		if (m_instance->m_fixedTimeStep)
			glutTimerFunc(int(1000 * m_instance->m_targetElapsedTime), updateWrapper, 0);

		// calculate elapsed time since last update call
		int currentTime = glutGet(GLUT_ELAPSED_TIME);
		int elapsedTime = currentTime - m_instance->m_totalTime;
		m_instance->m_totalTime = currentTime;

		// call update logic
		m_instance->update(float(elapsedTime));

		// request a redisplay
		if (!m_instance->m_fixedTimeStep)
			drawWrapper();
	}

	void GlutApplication::drawWrapper()
	{
		m_instance->draw();

		glutSwapBuffers();
	}
}
