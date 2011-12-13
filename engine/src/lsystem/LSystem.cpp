#include "LSystem.h"
#include "Trace.h"

#include <algorithm>
#include <cstdio>
#include <sstream>

namespace Common
{
	Production::Production(const std::string & _str)
	{
		// TODO: do this in a safer way
		char key, value[80];
		assert(_str.length() < 80);
		sscanf(_str.c_str(), "%c=%s", &key, (char*)&value);

		sym = key;
		str = value;
	}

	Production::Production(symbol s, string _str, float _time)
		: sym(s), str(_str), time(_time)
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

	void PlantDefinition::addProduction(symbol sym, string str, float time)
	{
		productions.insert(std::make_pair(sym, Production(sym, str, time)));
	}

	void PlantDefinition::addTerminals(const std::string & str)
	{
		for (unsigned i = 0; i < str.length(); i++)
		{
			char c = str[i];
			productions.insert(std::make_pair(c, Production(c, std::string(1, c))));
		}
	}

	LSystem::LSystem(PlantDefinition * def)
		: m_def(def), m_time(0.0f)
	{
	}

	void LSystem::update(float dt)
	{
		m_time += dt;
		m_string = generate();
	}

	std::string LSystem::generate()
	{
		std::string str = m_def->axiom;

		m_queue = std::stack<std::string>();
		m_queue.push(str);


		int iterations = m_def->iterations;

		float dt = 500.0f;

		for (int i = 0; i < iterations; i++) {
			
			std::stringstream ss;

			ss << "k" << i;

			for (unsigned c = 0; c < str.length(); c++) {

				char chr = str[c];

				// Search for valid production
				//std::pair<Productions::iterator, Productions::iterator> it = m_def->productions.equal_range(chr);
				Productions::iterator it = m_def->productions.find(chr);
				if (it != m_def->productions.end()) {
					//Productions::iterator f = it.first;
					Production prod = it->second;
					ss << prod.str;
				} else {
					Trace::info("No matching rule for symbol: %c\n", chr);
				}
			}

			str = ss.str();
			//Trace::info("str: %d\n", str.length());
			m_queue.push(str);
			//Trace::info("queuestr: %d\n", m_queue.top().length());
		}

		return str;
	}
}
