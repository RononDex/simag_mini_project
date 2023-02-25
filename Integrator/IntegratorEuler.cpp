#include "IntegratorEuler.h"

void IntegratorEuler::step(
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
) const
{
	// Simple integration

	if (nSize == 0) return;

	for (int i = 0; i < nSize; i++)
	{
		auto& p = *pos;
		auto& v = *vel;
		auto& m = *mass;
		auto& f = *force;

		// todo students
		// position-update
		// velocity-update

		pos++;
		vel++;
		force++;
		mass++;
	}
}
