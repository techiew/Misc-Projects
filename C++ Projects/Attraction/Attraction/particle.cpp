#include "particle.h"

Particle::Particle(int posX, int posY) {
	pos = std::make_pair(posX, posY); // Position
	vel = std::make_pair(0.0, 0.0); // Velocity
	acc = std::make_pair(0.0, 0.0);  // Acceleration
	target = std::make_pair(0.0, 0.0); // The thing that is attracting the particle
	dir = std::make_pair(0.0, 0.0); // Direction to the target
	result = std::make_pair(0.0, 0.0);
	force = 0;
	dsquared = 0;
	G = 1; // Strength of the gravitational pull
}

void Particle::Update(int width, int height) {
	vel = std::make_pair(vel.first * 0.999 + acc.first, vel.second * 0.999 + acc.second);

	if (pos.first + vel.first > width || pos.first + vel.first < 0)
	{
		vel.first = -vel.first;
	}

	if (pos.second + vel.second > height || pos.second + vel.second < 0)
	{
		vel.second = -vel.second;
	}

	pos = std::make_pair(pos.first + vel.first, pos.second + vel.second);
	acc = std::make_pair(0.0, 0.0); // Reset acceleration each frame, otherwise it increases forever
}

void Particle::Attract(std::pair <double, double> target) {

	/* Gravitational attraction formula:
	* F = G * (M1 * M2) / (d*d) 
	* 
	* The mass of each object is 1 in this case so we can ignore (M1 * M2)
	*/

	target = std::make_pair(target.first, target.second); // Attract to this
	dir = std::make_pair(target.first - pos.first, target.second - pos.second); // Get direction to target
	dsquared = sqrt(((pos.first-target.first)*(pos.first-target.first)) + ((pos.second-target.second)*(pos.second-target.second))); // Distance squared

	if (dsquared > 10000) { // Limit dsquared so that the difference in force over distance isn't too big
		dsquared = 10000;
	} else if (dsquared < 1) { // We don't want it to become 0
		dsquared = 1;
	}

	force = G / (dsquared * dsquared); // The formula mentioned above

	force *= 50; // Increase the force a bit...

	result = std::make_pair(dir.first * force, dir.second * force); // The resulting vector

	if (dsquared < 50.0) { // When the particle is close, slow it down
		//vel = std::make_pair(-vel.second * -1.1, vel.first * -1.1); // Optional code to bounce particles away
		vel = std::make_pair(vel.first / 10, vel.second / 10); // These numbers (10 and 2) were choosen randomly
		acc = std::make_pair(acc.first / 2, acc.second / 2);
		result = std::make_pair(result.first / 2, result.second / 2);
	}

	acc = std::make_pair(acc.first + result.first, acc.second + result.second);
}

Particle::~Particle() {

}
