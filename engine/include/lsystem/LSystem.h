#ifndef COMMON_LSYSTEM
#define COMMON_LSYSTEM

#include "Component.h"

#include "glm/glm.hpp"

#include <string>
#include <set>

namespace Common
{
	typedef char symbol;
	typedef std::string string;

	struct Production
	{
		Production(const std::string & str);
		Production(symbol s, string str, float time = 0.0f);

		symbol sym;
		string str;
		float time;

		bool operator <(const Production & other) const;
		bool operator==(const Production & other) const;
		bool operator==(const symbol & s) const;
	};

	typedef std::set<Production> Productions;

	struct PlantDefinition
	{
		unsigned iterations;
		glm::vec3 angle;
		float diameter;

		string axiom;

		Productions productions;

		void addProduction(symbol s, string str, float time = 0.0f);
		void addTerminals(const std::string & str);
	};

	class LSystem : public Component
	{
	public:
		LSystem(PlantDefinition * def);

		virtual void update(float dt);

		inline std::string get() const { return m_string; };
		std::string generate();

	private:
		std::string m_string;
		PlantDefinition * m_def;
		float m_time;
	};
}

#endif /* COMMON_LSYSTEM*/