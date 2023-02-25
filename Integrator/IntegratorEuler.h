#pragma once

#include "IIntegrator.h"

class IntegratorEuler : public CIntegrator
{

public:

	virtual const char* name() const override { return "IntegratorEuler"; }

	virtual int noOfPasses() const override { return 1;	}

	virtual void step(
		int passNumber,
		float dt,
		glm::vec3* pos,
		glm::vec3* vel,
		glm::vec3* force,
		float* mass,
		glm::vec3* temp0,
		glm::vec3* temp1,
		glm::vec3* temp2,
		glm::vec3* temp3,
		int nSize
	) const override;
	// ------------------------------------------------------------------------

	IntegratorEuler() = default;
	virtual ~IntegratorEuler() = default;

};