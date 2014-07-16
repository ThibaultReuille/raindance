#pragma once

#include "Core/Scene/NodeVector.hh"

namespace Physics
{
	class IForce
	{
	public:
		virtual ~IForce() = 0;

		virtual void apply(Scene::NodeVector& nodes) = 0;
	};

	IForce::~IForce()
	{
	}

	class GravitationForce : IForce
	{
	public:
		virtual void apply(Scene::NodeVector& nodes)
		{
			const float c_G = 9.80665f; // m / s2
			const float c_NodeMass = 2.0f;

			// TODO : To make a better version of the gravitation force we need time !
			// Right now we just set the node mass really low to see a smooth animation

			glm::vec3 dir;

			Scene::NodeVector::iterator it;
			for (it = nodes.begin(); it != nodes.end(); ++it)
			{
				// NOTE : (0.0, 1.0, 0.0) is considered to be the up vector !
				dir = glm::vec3(0.0, -1.0f * c_G * c_NodeMass, 0.0);

				(*it)->setDirection((*it)->getDirection() + dir, false);
			}
		}
	};
}

