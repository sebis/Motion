#ifndef COMMON_LSYSTEM
#define COMMON_LSYSTEM

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
		Production(symbol s, string str);

		symbol sym;
		string str;

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

		void addProduction(symbol s, string str);
		void addTerminals(const std::string & str);
	};

	class LSystem
	{
	public:
		LSystem(PlantDefinition * def);

		std::string generate();

	private:
		PlantDefinition * m_def;
	};
}

#endif /* COMMON_LSYSTEM*/