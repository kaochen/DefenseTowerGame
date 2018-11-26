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
 *Functions to extract data from tmx and tsx files
 */

#ifndef XML_H
#define XML_H

#include <iostream>
#include <string>

/*! \class C_Xml
 * \brief functions to extract data from tmx and tsx files
 */

class C_Xml
{
	public:
    C_Xml();
    ~C_Xml();
    std::string extractStrProperty(std::string const &File_Path,std::string const &name);

  protected:

	private:

};
#endif
