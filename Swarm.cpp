#include "Swarm.h"



namespace bw
{
Swarm::Swarm() : lastTime(0)
{
	m_pParticles = new Particle[NPARTICLES];
}


Swarm::~Swarm()
{
	delete[] m_pParticles;
}

void Swarm::update(int elapsed)
{
	//compare the time since the last update
	int interval = elapsed - lastTime;

	// itterate throug all the particles and uppdate them indiviually
	for (int i = 0; i < Swarm::NPARTICLES; i++)
		m_pParticles[i].update(interval);

	lastTime = elapsed;
}

}