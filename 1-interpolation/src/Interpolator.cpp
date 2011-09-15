#include "Interpolator.h"
#include "Transform.h"

#include "glm/glm.hpp"

namespace Interpolation
{
	void LinearInterpolator<glm::vec3>::interpolate2(glm::vec3& out, glm::vec3 first, glm::vec3 second, float t)
	{
		out = first + t * (second - first);
	}

	glm::vec3 LinearInterpolator<glm::vec3>::interpolate(glm::vec3 first, glm::vec3 second, float t)
	{
		return first + t * (second - first);
	}

	Common::Transform LinearInterpolator<Common::Transform>::interpolate(Common::Transform first, Common::Transform second, float t)
	{
		Common::Transform final;
		final.translate(first.position() + t * (second.position() - first.position()));

		return final;
	}
};