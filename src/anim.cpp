/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "anim.h"

using namespace std;

//---------------- C_Anim ------------
C_Anim::C_Anim():
	C_Anim("empty",0,0,50)
{
}



C_Anim::C_Anim(std::string name, int imageStart,int imageEnd,long delay):
	m_name(name),
	m_imageStart(imageStart),
	m_imageEnd(imageEnd),
	m_imageCurrent(imageStart),
	m_started(false),
	m_timeDelay(delay)
{
	m_timeStart = SDL_GetTicks();
	m_timeLast = m_timeStart;
}

void C_Anim::play(){
	long timeCurrent = SDL_GetTicks();
	if( timeCurrent > (m_timeLast + m_timeDelay)){
		m_timeLast = timeCurrent;
		m_imageCurrent++;
		if(m_imageCurrent > m_imageEnd){
			m_imageCurrent = m_imageStart;
		}
	}
}

int C_Anim::getImageNbr(){
	return m_imageCurrent;
}

void C_Anim::reset(){
	m_imageCurrent = m_imageStart;
}

void C_Anim::status(){
	cout << "Anim: " << m_name << " image" << m_imageCurrent << "/" << m_imageEnd << " Delay: "<< m_timeDelay << " Time: " << m_timeLast <<  endl;
}

bool C_Anim::end(){
	bool ret = false;
	long currentTime = SDL_GetTicks();
	int imageNbr = m_imageEnd - m_imageStart;

	if( currentTime > (m_timeStart + imageNbr*m_timeDelay)){
		ret = true;
		m_started = false;
	}
	return ret;
}

void C_Anim::start(){
	if(!m_started){
		m_timeStart = SDL_GetTicks();
		m_started = true;
	};
}
//---------------- C_AnimList ------------
C_AnimList::C_AnimList(){
	m_list["empty"] = C_Anim();
}

void C_AnimList::add(C_Anim anim){
	m_list[anim.getName()] = anim;
};

C_Anim& C_AnimList::get(std::string name){
	if ( m_list.find(name) == m_list.end() ) {
		cerr << "Can not find C_Anim : " << name << " !" << endl;
		return m_list["empty"];
	} else {
		return m_list[name];
	}
}

int C_AnimList::getImageNbr(std::string name){
	return get(name).getImageNbr();
}

void C_AnimList::show(){
	for (auto const& x : m_list) {
		string name = x.first;  // string (key)
		map<string, C_Anim>::iterator search = m_list.find(name);
		if(search == m_list.end()) {
			cout << "\"" << name << "\" not available in the anim map\n";
		} else {
			m_list[name].status();
		}
	}
}

void C_AnimList::play(std::string name){
	for(auto & x : m_list){
		if(x.first == name){
			x.second.play();
		}
	}
};

