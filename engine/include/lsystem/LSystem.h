#ifndef COMMON_LSYSTEM
#define COMMON_LSYSTEM

#include "GameObject.h"

#include "glm/glm.hpp"

#include <string>
#include <map>
#include <stack>

namespace Common
{
	typedef char symbol;
	typedef std::string string;

	struct Production
	{
		Production(const std::string & str);
		Production(symbol s, string str);

		symbol sym;
		string str;
		float probability;

		bool operator <(const Production & other) const;
		bool operator==(const Production & other) const;
		bool operator==(const symbol & s) const;
	};

	typedef std::multimap<symbol, Production> Productions;

	struct PlantDefinition
	{
		PlantDefinition() : iterations(2), angle(0.0f), diameter(0.1f), length(1.0f), thinning(1.0f) {}
		unsigned iterations;
		float angle;
		float diameter;
		float length;
		float thinning;

		string axiom;

		Productions productions;

		void addProduction(symbol s, string str);
		void addTerminals(const std::string & str);
	};

	class LSystem : public GameObject
	{
	public:
		LSystem(PlantDefinition * def);

		virtual void update(float dt);

		inline std::string get() const { return m_string; }
		inline std::stack<std::string> getQueue() const { return m_queue; }
		std::string generate();

		inline const PlantDefinition * definition() const { return m_def; }
		inline float time() const { return m_time; }

	private:
		std::stack<std::string> m_queue;

		std::string m_string;
		PlantDefinition * m_def;
		float m_time;
	};
}

#endif /* COMMON_LSYSTEM*/