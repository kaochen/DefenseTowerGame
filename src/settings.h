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


#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <string>
#include <queue>

#define WINDOW_WIDTH 1280
#define ASPECT_RATIO 9/16
#define FRAMERATE 30
#define TILE_HALF_HEIGHT 24
#define TILE_HALF_WIDTH 48

#define MAX_TARGETS 3

struct S_Coord{
	int x;
	int y;
};

struct S_Size{
  int w;
  int h;
};

struct S_NodeCoord{
	S_Coord screen; /*!< position of the center of the tile in px*/
  S_Coord delta; /*!< difference between the center of the tile and the place of the object. A boat is not always placed in the center of the tile*/
	S_Coord grid; /*!< position of the tile in grid unit*/
};


enum Direction {NORTH, NORTH_EAST, NORTH_WEST,SOUTH, SOUTH_EAST, SOUTH_WEST,EAST,WEST,UNKNOWN};
enum GroundType {GROUND_01,GROUND_02};
enum Speed {VERY_SLOW,SLOW, NORMAL,FAST,VERY_FAST};
enum Status {ALIVE,DEAD};
enum buttonType {ACTION,DRAGUNIT,STATUS};
enum texture_align {LEFT,CENTER,RIGHT,CENTER_TILE};
enum game {PLAY,PAUSE};
enum level {ONGOING,LOOSE,WIN};



class C_Settings
{
public:
	C_Settings(std::vector<std::string> args);
	virtual ~C_Settings(){};
	//window
	int getWindowWidth() {return m_windowWidth;};
	int getWindowHeight() {return m_windowHeight;};
	S_Coord getCameraPosition(){return m_cameraPos;}; /*!< get the position of the window over the level map*/
	void setCameraPosition(const S_Coord &pos);
	void centerCameraPosition();
	void cameraOnAPoint(S_Coord grid);
	void moveCameraPosition(const int &rigth,const int &left,const int &down,const int &up);
	void moveCameraPosition(const int &x,const int &y);

	//grid
	int getGridWidth() {return m_windowWidth / TILE_HALF_WIDTH;};
	int getGridHeight() {return m_windowHeight / TILE_HALF_HEIGHT;};
	S_Size getNbrOfTilesToDisplay();
  	//debug
	bool getVerboseMode() {return m_verboseMode;};
	void setDebugMode();
	bool getDebugMode() {return m_debugMode;};
  	void setDebugPathMode();
	bool getDebugPathMode() {return m_debugPath;};
	void displayDebugMode();
  	//images
	void initTSXfileList();
	std::queue<std::string>* getTSXfileList(){return &m_tsxFileList;};
	std::string getImgFolder(){return m_imgFolder;};
	std::string getThemePath(){return m_imgFolder + m_theme;};
	//level
	int setCurrentLevelNbr(int nbr);
	int getCurrentLevelNbr(){return m_currentLevel;};
	int getNbrOfLevels(){return m_nbrOfLevels;};
	std::string getLevelFolder(){return m_levelFolder;};

	//game
	int getPlaying(){return m_playing;};
	void setPlaying();
	void setPlaying(int state);

private:
	std::string absolutePath(std::string &path);
	bool getArg(std::string pattern, std::vector<std::string> args);
	bool fileExist(const std::string &file);
	void loadPrefFile();
	void extractWindowSize();
	void calcGridSize();
	bool extractIntFromINI(int &nbr, const std::string &name, const std::string &filename);
	void setNbrOfLevels();

	//pref file:
	std::string m_prefFile;

	//window
	int m_windowWidth;
	int m_windowHeight;
	S_Coord m_cameraPos; /*!< get the position of the window over the level map*/
	//grid
	size_t m_gridSize; //the grid is a square even if the screen is non-square.

	//debug
	bool m_verboseMode;
	bool m_debugMode;
	bool m_debugPath;
	//images
	std::string m_imgFolder;
	std::string m_theme;
	std::queue<std::string> m_tsxFileList;
	//levels
	int m_currentLevel;
	int m_nbrOfLevels;
	std::string m_levelFolder;

	//game
	int m_playing;
};

#endif
