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
		PlantDefinition() : iterations(2), angle(0.0f), diameter(0.1f), length(1.0f), thinning(1.0f), size(0.15f), growtime(200.0f) {}
		unsigned iterations;
		float angle;
		float diameter;
		float length;
		float thinning;
		float size;
		float growtime;

		std::string barkTexture;
		std::string leafTexture;

		string axiom;

		Productions productions;

		void addProduction(symbol s, string str);
		void addTerminals(const std::string & str);
	};

	class LSystem : public GameObject
	{
	public:
		LSystem(PlantDefinition * def, float delay = 0.0f);

		virtual void update(float dt);

		inline std::string get() const { return m_string; }
		std::string generate();

		inline const PlantDefinition * definition() const { return m_def; }
		inline float time() const { return std::max(0.0f, m_time - m_delay); }

	private:
		std::string m_string;
		PlantDefinition * m_def;
		float m_time;
		float m_delay;
	};
}

#endif /* COMMON_LSYSTEM*/