#ifndef COMMON_LSYSTEM
#define COMMON_LSYSTEM

#include "Component.h"

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
		Production(symbol s, string str, float time = 0.0f);

		symbol sym;
		string str;
		float time;
		float probability;

		inline void setTime(float _time) { time = _time; }

		bool operator <(const Production & other) const;
		bool operator==(const Production & other) const;
		bool operator==(const symbol & s) const;
	};

	typedef std::multimap<symbol, Production> Productions;

	struct PlantDefinition
	{
		unsigned iterations;
		float angle;
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