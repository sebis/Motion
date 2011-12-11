#include "LSystem.h"
#include "Trace.h"

#include <algorithm>
#include <cstdio>

namespace Common
{
	Production::Production(const std::string & _str)
	{
		// TODO: do this in a safer way
		char key, value[80];
		assert(_str.length() < 80);
		sscanf(_str.c_str(), "%c=%s", &key, &value);

		sym = key;
		str = value;
	}

	Production::Production(symbol s, string _str)
		: sym(s), str(_str)
	{}

	bool Production::operator <(const Production & other) const
	{
		return sym < other.sym;
	}

	bool Production::operator==(const Production & other) const
	{
		return sym == other.sym;
	}

	bool Production::operator==(const symbol & s) const
	{
		return sym == s;
	}

	void PlantDefinition::addProduction(symbol sym, string str)
	{
		productions.insert(Production(sym, str));
	}

	void PlantDefinition::addTerminals(const std::string & str)
	{
		for (int i = 0; i < str.length(); i++)
		{
			char c = str[i];
			productions.insert(Production(c, std::string(1, c)));
		}
	}

	namespace
	{
		std::string expand(PlantDefinition * def)
		{
			std::string str = def->axiom;

			int iterations = def->iterations;

			for (int i = 0; i < iterations; i++) {
				
				std::string tmp;

				for (int c = 0; c < str.length(); c++) {

					char chr = str[c];

					// Search for valid production
					Productions::iterator it = std::find(def->productions.begin(), def->productions.end(), chr);
					if (it != def->productions.end()) {
						tmp.append(it->str);
					} else {
						Trace::info("No matching rule for symbol: %c\n", chr);
					}
				}

				str = tmp;
			}

			return str;
		}
	}

	LSystem::LSystem(PlantDefinition * def)
		: m_def(def)
	{
	}

	std::string LSystem::generate()
	{
		return expand(m_def);
	}
}