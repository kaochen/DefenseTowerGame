#include "weapons.h"
#include <cmath>

using namespace std;

C_Weapon::C_Weapon():m_name("CANON"),
		m_damage(10),
		m_fireRate(500),
		m_fireRange(200),
		m_x_screen(0),
		m_y_screen(0),
		m_shooting(false),
		m_lastShootTime(0),
		m_dist(80),
		m_direction(UNKNOWN)
{
}

C_Weapon::C_Weapon(std::string name, int damage, int fireRate, int fireRange):
	m_name(name),
	m_damage(damage),
	m_fireRate(fireRate),
	m_fireRange(fireRange),
	m_x_screen(0),
	m_y_screen(0),
	m_shooting(false),
	m_lastShootTime(0),
	m_dist(80),
	m_direction(UNKNOWN),
	m_strDirection("EE")
{
}

C_Weapon::~C_Weapon()
{
}

void C_Weapon::change(string name, int damage, int fireRate, int fireRange)
{
 	m_name = name;
 	m_damage = damage;
 	m_fireRate = fireRate;
 	m_fireRange = fireRange;
}

void C_Weapon::displayStatus() const
{
 	cout << "\tWeapon: " << m_name << " (Damage: "<< m_damage << ", firerate: " << m_fireRate << ")" << endl;
}

int C_Weapon::getDamage() const
{
	return m_damage;
}

int C_Weapon::getFireRate() const
{
	return m_fireRate;
}

int C_Weapon::getFireRange() const
{
	return m_fireRange;
}

int C_Weapon::getXScreen() const
{
	return m_x_screen;
}

int C_Weapon::getYScreen() const
{
	return m_y_screen;
}

bool C_Weapon::getShooting() const
{
	return m_shooting;
}

long C_Weapon::getLastShootTime() const
{
	return m_lastShootTime;
}

void C_Weapon::setShooting(bool status)
{
	m_shooting = status;
}

int C_Weapon::getDirection() const
{
	return m_direction;
}
string C_Weapon::getStrDirection() const
{
	return m_strDirection;
}

bool C_Weapon::shoot(C_GameUnits &shooter, C_GameUnits &target){
			int x_s_target = target.getXScreen();
			int y_s_target = target.getYScreen();
			int x_s_shooter = shooter.getXScreen();
			int y_s_shooter = shooter.getYScreen() + 30;
			int ab = x_s_target - x_s_shooter;
			int bc = y_s_target - y_s_shooter;
			int hyp = sqrt((ab*ab + bc*bc));
			hyp -= hyp*m_dist/100;
			double angle = atan2(ab,bc);
			int newA = hyp*sin(angle);
			int newB = hyp*cos(angle);
			angle = angle *180/3.14159265359;
			string direction;
			if (angle > -22.5 && angle <= 22.5){
				m_direction = SOUTH;
				m_strDirection = "SS";
				}
			else if (angle > 22.5 && angle <= 67.5){
				m_direction = SOUTH_EAST;
				m_strDirection = "SE";
			}
			else if(angle > 67.5 && angle <=112.5){
				m_direction = EAST;
				m_strDirection = "EE";
				}
			else if(angle > 112.5 && angle <=157.5){
				m_direction = NORTH_EAST;
				m_strDirection = "NE";
				}
			else if((angle > 157.5 && angle <=180) && (angle > -180 && angle <= -157.5)){
				m_direction = NORTH;
				m_strDirection = "NN";
				}
			else if(angle > -157.5 && angle <= -112.5){
				m_direction = NORTH_WEST;
				m_strDirection = "NW";
				}
			else if(angle > -112.5 && angle <=-67.5){
				m_direction = WEST;
				m_strDirection = "WW";
				}
			else if(angle > -67.5 && angle <=-22.5){
				m_direction = SOUTH_WEST;
				m_strDirection = "SW";
				}
			else{
				m_direction = UNKNOWN;
				//m_strDirection = "EE";
				}
			//cout << "Angle:"<< angle << " Direction: " << direction << endl;

			m_x_screen = x_s_shooter + newA;
			m_y_screen = y_s_shooter + newB;
			m_dist -= 10;
			if (m_dist < 20){
				m_dist = 80;
				m_lastShootTime = SDL_GetTicks();
				return true;
				}
			return false;
}
