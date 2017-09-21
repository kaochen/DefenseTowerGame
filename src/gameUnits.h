#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include <iostream>
#include <string>

class C_GameUnits
{
	public:
	//methods
	C_GameUnits();
	C_GameUnits(std::string name, int rank);
	C_GameUnits(C_GameUnits const& original);
	virtual ~C_GameUnits();

	virtual std::string getName() const;
	virtual void displayStatus() const;
	virtual void receiveDamage(int nbDamage);
	virtual bool alive() const;
	virtual int getRank() const;

	protected:
	//attibuts
	std::string m_name;
	int m_life;
	int m_rank;
};

#endif
