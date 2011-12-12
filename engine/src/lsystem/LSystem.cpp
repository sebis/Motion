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
		productions.insert(Production(sym, str, time));
	}

	void PlantDefinition::addTerminals(const std::string & str)
	{
		for (unsigned i = 0; i < str.length(); i++)
		{
			char c = str[i];
			productions.insert(Production(c, std::string(1, c)));
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
			
			std::string tmp;

			for (unsigned c = 0; c < str.length(); c++) {

				char chr = str[c];

				// Search for valid production
				Productions::iterator it = std::find(m_def->productions.begin(), m_def->productions.end(), chr);
				if (it != m_def->productions.end()) {
					//Trace::info("Time: %f / %f\n", time, 1000.0f * i);
					//if (1000.0f * i < t) {
						tmp.append(it->str);
						//time = time+dt - t_branch;

						// TODO: not really smart
						Common::Production & p = const_cast<Common::Production &>(*it);
						p.setTime(m_time+dt - it->time);
					//}
				} else {
					Trace::info("No matching rule for symbol: %c\n", chr);
				}
			}

			str = tmp;
			//Trace::info("str: %d\n", str.length());
			m_queue.push(str);
			//Trace::info("queuestr: %d\n", m_queue.top().length());
		}

		return str;
	}
}
