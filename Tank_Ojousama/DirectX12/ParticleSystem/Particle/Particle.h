#pragma once

class Particle
{
protected:
	Particle();
public:
	virtual ~Particle() = 0;

	void update();

private:
	//ÉRÉsÅ[ã÷é~
	Particle(const Particle&) = delete;
	Particle& operator=(const Particle&) = delete;
	Particle(Particle&&) = delete;
	Particle& operator=(Particle&&) = delete;

private:

};