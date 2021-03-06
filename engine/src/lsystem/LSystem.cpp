/**
 * Copyright 2011-2012 Sebastian Eriksson.
 * This file is part of Motion.
 *
 * Motion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Motion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Motion. If not, see <http://www.gnu.org/licenses/>.
 */

#include "LSystem.h"
#include "TurtleRenderer.h"
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
		productions.insert(std::make_pair(sym, Production(sym, str)));
	}

	void PlantDefinition::addTerminals(const std::string & str)
	{
		for (unsigned i = 0; i < str.length(); i++)
		{
			char c = str[i];
			productions.insert(std::make_pair(c, Production(c, std::string(1, c))));
		}
	}

	LSystem::LSystem(PlantDefinition * def, float delay)
		: m_def(def), m_time(0.0f), m_delay(delay)
	{
		m_renderer = new TurtleRenderer(this);
	}

	void LSystem::update(float dt)
	{
		m_time += dt;
		transform().update();
	}

	std::string LSystem::generate()
	{
		std::string str = m_def->axiom;
		int iterations = m_def->iterations;

		for (int i = 0; i < iterations; i++) {

			std::string tmp;

			for (unsigned c = 0; c < str.length(); c++) {

				char chr = str[c];

				// Search for valid production
				Productions::iterator it = m_def->productions.find(chr);
				if (it != m_def->productions.end()) {
					Production prod = it->second;
					tmp.append(prod.str);
				} else {
					//Trace::info("No matching rule for symbol: %c\n", chr);
				}
			}

			str = tmp;
		}

		return str;
	}
}
