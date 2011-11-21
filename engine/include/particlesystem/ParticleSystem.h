#ifndef COMMON_PARTICLESYSTEM
#define COMMON_PARTICLESYSTEM

#include "Component.h"
#include "FlowPrimitive.h"
#include "ParticleRenderer.h"

#include "glm/glm.hpp"

#include <vector>

namespace Common
{
	struct Particle
	{
		glm::vec3 position;
		glm::vec3 velocity;
		
		glm::vec4 color;

		float startSize;
		float endSize;
		float size;

		float age;

		float ageRandom;
		float colorRandom;
	};

	struct ParticleSettings
	{
		unsigned maxParticles;

		std::string texture;

		glm::vec3 gravity;
		float duration;

		glm::vec3 minVelocity;
		glm::vec3 maxVelocity;

		float minStartSize;
		float maxStartSize;

		float minEndSize;
		float maxEndSize;

		glm::vec4 minColor;
		glm::vec4 maxColor;

		GLenum srcBlend;
		GLenum dstBlend;
	};

	class ParticleSystem : public Component
	{
	public:
		ParticleSystem(const ParticleSettings & settings);
		virtual ~ParticleSystem();

		void update(float dt);
		void draw();

		void addParticle(const glm::vec3 & position, const glm::vec3 & velocity);
		void addFlow(FlowPrimitive * flow);

		inline void setShader(Shader * shader) { m_renderer->setShader(shader); }

	private:
		const ParticleSettings m_settings;

		unsigned firstActiveParticle;
		unsigned firstFreeParticle;

		Particle * m_particles;
		ParticleRenderer * m_renderer;

		typedef std::vector<FlowPrimitive*> Flows;
		typedef Flows::iterator FlowIterator;
		Flows m_flows;
	};
}

#endif /* COMMON_PARTICLESYSTEM */
