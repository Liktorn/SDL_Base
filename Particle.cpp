#include "Particle.h"
#include <math.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES // for C++
#include <cmath>


namespace bw
{


Particle::Particle()
{
	init();
}


Particle::~Particle()
{
}

void Particle::init()
{
	m_x = 0;
	m_y = 0;

	// initiate a random direction in radiants
	m_direction = (2 * M_PI * rand()) / RAND_MAX;
	// Initiate arandom speed
	m_speed = (0.04 * rand()) / RAND_MAX;

	m_speed *= m_speed;
}

void Particle::update(int interval)
{
	//makes a slight spiral
	m_direction += interval*0.0003;

	double xSpeed = m_speed * cos(m_direction);
	double ySpeed = m_speed * sin(m_direction);
	// xSpeed * interval makes the game runn equally fast on different computers
	m_x += xSpeed * interval;
	m_y += ySpeed * interval;

	// if particles are outside teh window, re initilize them
	if (m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1)
	{
		init();
	}
	// 1/100 chance to randomny initialize
	if (rand() < RAND_MAX / 100)
		init();
}

} /*end of namespace*/