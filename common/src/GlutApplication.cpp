#include "GlutApplication.h"
#include "Trace.h"
#include "WindowParams.h"

#include <GL/freeglut.h>
#include <iostream>

GlutApplication * GlutApplication::m_instance = 0;

GlutApplication::GlutApplication(bool fixedTimeStep, float targetElapsedTime)
	: m_totalTime(0),
	  m_fixedTimeStep(fixedTimeStep),
	  m_targetElapsedTime(targetElapsedTime)
{
}

bool GlutApplication::init(int argc, char * argv[])
{
	m_instance = this;

	glutInitContextVersion(3, 1);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("GlutApplication");

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

	glutMainLoop();

	return 0;
}

void GlutApplication::update(float dt)
{
}

void GlutApplication::draw()
{
}

void GlutApplication::updateWrapper(int value)
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
