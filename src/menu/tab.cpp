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

#include "tab.h"
#include "../message.h"
#include "../window.h"
#include "../textureList.h"
#include "../locator.h"

using namespace std;

std::vector<std::string> C_Page::getListOfVisibleItems()
{
	std::vector <std::string> list;
	for(auto const& x : m_itemsList) {
		list.push_back(x.first);
	}
	return list;
}

int C_Tab::m_id = -1;

C_Tab::C_Tab(std::string title)
    : C_Page(title)
{
	m_id++;
	m_name = "tab_" + to_string(m_id);
	m_title = title;
	C_Settings& settings=C_Locator::getSettings();
	m_width = 900;
	m_height = 400;
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
	m_flagScreen.x = m_screen.x - 295;
	m_flagScreen.y = m_screen.y - 145;
	m_flagOffset = 35;


	m_tabSize = 120;
	int x = m_screen.x - ((m_tabSize *3) / 2);

	m_itemsList[m_name] = new C_MB_TabSelect(m_name,m_title,18, x + (m_tabSize*m_id) ,m_screen.y - 220);
	if(m_itemsList[m_name] != nullptr) {
		m_itemsList[m_name]->setCommand(new C_ChangeTab);
		if( m_itemsList[m_name]->getCommand() != nullptr)
			m_itemsList[m_name]->getCommand()->setNbr(m_id);
	}
}

C_Page::~C_Page()
{
    m_itemsList.clear(); //items are allready delete
}

void C_Tab::displayTab(bool open)
{
	if(open) {
		C_TextureList& t= C_Locator::getTextureList();
		t.renderTexture("Menu_01_background", m_screen.x,m_screen.y,CENTER);
		t.renderTexture("Menu_01_parchment", m_screen.x + 225,m_screen.y,CENTER);
	}
}

void C_Tab::fillWithClosedFlags(){
    size_t start = m_itemsList.size();
    if(start > 0){
        start--;
    }
    for(size_t i = start; i < 10 ; i++){
        std::string name = "closedFlag" + std::to_string(i);
     	m_itemsList[name] = new C_MB_1Line("","",m_flagScreen.x,m_flagScreen.y + m_flagOffset*i);
    }
}



std::vector<string> C_Tab::getListOfVisibleItems()
{
	std::vector <std::string> list;
	for(auto const& x : m_itemsList) {
		if(x.first != m_name)
			list.push_back(x.first);
	}
	return list;
}

C_Tab_Settings::C_Tab_Settings()
	:C_Tab("Settings")
{
	std::string name = "Resolution";
	C_Settings& settings=C_Locator::getSettings();
	std::string width = std::to_string(settings.getWindowWidth());
	std::string height = std::to_string(settings.getWindowHeight());
	std::string text = width + "x" + height;
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x,m_flagScreen.y);

	name = "Grid Size";
	text =  std::to_string(settings.getGridWidth())+ "x" +std::to_string(settings.getGridHeight());
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x,m_flagScreen.y + m_flagOffset);
    fillWithClosedFlags();
}

C_Tab_Levels::C_Tab_Levels()
	:C_Tab("Levels")
{
	C_Settings& settings=C_Locator::getSettings();
	std::string name = "Number of Levels";
	std::string text = std::to_string(settings.getNbrOfLevels());
	m_itemsList[name] = new C_MB_1Line(name,text,m_flagScreen.x,m_flagScreen.y);
    fillWithClosedFlags();

    m_currentCardLevelNbr = settings.getCurrentLevelNbr();
	name = "Card_Level";
	m_itemsList[name] = new C_MB_LevelCard(m_currentCardLevelNbr,"Card_" + to_string(m_currentCardLevelNbr),m_screen.x + 50,m_screen.y + 20);

	std::string arrowLeft = "Level_Change_Arrow_Left";
	m_itemsList[arrowLeft]  = new C_MB_Arrows(arrowLeft,GO_LEFT,m_screen.x +20,m_screen.y);
	m_itemsList[arrowLeft]->setCommand(new C_ChangeLevelLeft());

	std::string arrowRight = "Level_Change_Arrow_Right";
	m_itemsList[arrowRight]  = new C_MB_Arrows(arrowRight,GO_RIGHT,m_screen.x + 320,m_screen.y);
	m_itemsList[arrowRight]->setCommand(new C_ChangeLevelRight());

	std::string load = "Level_Load";
	m_itemsList[load]  = new C_MB_CardButton(load, m_screen.x + 60 , m_screen.y + 130);
	if(m_itemsList[load]!= nullptr){
	    m_itemsList[load]->setText("Load");
	    C_LoadALevel *command = new C_LoadALevel();
	    m_itemsList[load]->setCommand(command);
	    m_itemsList[load]->getCommand()->setNbr(m_currentCardLevelNbr);
    }

}

void C_Tab_Levels::go(int direction){
    if(direction == GO_LEFT){
        m_currentCardLevelNbr--;
    } else if (direction == GO_RIGHT){
        m_currentCardLevelNbr++;
    }
    //limits
    if(m_currentCardLevelNbr < 1){
        m_currentCardLevelNbr = 1;
    }
    C_Window& win= C_Locator::getWindow();
    int last = win.getLastLevelNbr();
    if(m_currentCardLevelNbr > last){
        m_currentCardLevelNbr = last;
    }

    std::string name = "Card_" + to_string(m_currentCardLevelNbr);

    m_itemsList.erase("Card_Level");
    m_itemsList["Card_Level"] = new C_MB_LevelCard(m_currentCardLevelNbr,name,m_screen.x + 50,m_screen.y + 20);
    m_itemsList["Level_Load"]->getCommand()->setNbr(m_currentCardLevelNbr);
}


C_Tab_endGame::C_Tab_endGame(std::string name)
	:C_Page(name)
{
	C_Settings& settings=C_Locator::getSettings();
	m_screen.x = (settings.getWindowWidth())/2;
	m_screen.y = (settings.getWindowHeight())/2;
	m_levelStatus = ONGOING;

    std::string replay = "EndGame_Replay";
    int current = settings.getCurrentLevelNbr();
	m_itemsList[replay]  = new C_MB_CardButton(replay, m_screen.x - 160, m_screen.y +100);
	if(m_itemsList[replay] != nullptr){
	    m_itemsList[replay]->setText("Replay Level " + to_string(current));
	    C_LoadALevel *command = new C_LoadALevel();
	    m_itemsList[replay]->setCommand(command);
	    m_itemsList[replay]->getCommand()->setNbr(current);
	}

	std::string next = "EndGame_Next";
	m_itemsList[next]  = new C_MB_CardButton(next, m_screen.x - 160, m_screen.y + 150);
	if(m_itemsList[next]!= nullptr){
		m_itemsList[next]->setText("Next Level " + to_string(current + 1));
	    C_LoadALevel *command1 = new C_LoadALevel();
	    m_itemsList[next]->setCommand(command1);
	    m_itemsList[next]->getCommand()->setNbr(current + 1);
	}

	m_itemsList["EndGameResultText"]  = new C_MenuText("EndGameResultText","Winner", m_screen.x - 20 , m_screen.y + 40);
}


void C_Tab_endGame::render()
{
	C_TextureList& t= C_Locator::getTextureList();
	t.renderTexture("Menu_01_parchment", m_screen.x, m_screen.y,CENTER);
	if(m_levelStatus == WIN)
			m_itemsList["EndGameResultText"]->setText("Your castle is safe for now");
		else if(m_levelStatus == LOSE)
			m_itemsList["EndGameResultText"]->setText("You lost this castle");
		else if(m_levelStatus == ONGOING)
			m_itemsList["EndGameResultText"]->setText("The game continue");
}
