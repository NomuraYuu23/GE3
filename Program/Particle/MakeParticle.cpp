#include "MakeParticle.h"
#include <cassert>

MakeParticle* MakeParticle::GetInstance()
{
    return nullptr;
}

Particle* MakeParticle::Run(uint32_t paeticleName, const Vector3& position, const Vector3& size)
{
	
	Particle* particle = nullptr;

	switch (paeticleName)
	{
	case kDefault:
		particle = new Particle();
		particle->Initialize(position, size);
		break;
	case kCountOfPaeticleName:
	default:
		assert(0);
		break;
	}

	return particle;

}
