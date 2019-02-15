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
/*!
 * \file popup.h
 */

#ifndef POPUP_H
#define POPUP_H

#include <iostream>
#include <string>
#include "../coord.h"

/*! \class C_Popup
 * \brief C_Popup display informations when passing over a Tower.
 */

class C_Popup{
public:
	C_Popup();
	~C_Popup(){};
	void render(S_Coord screen);
private:
	static int id;
	std::string m_name;
};

#endif
