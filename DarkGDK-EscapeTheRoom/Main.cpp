/*Copyright (c) 2011 Robert Rouhani

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
 
Robert Rouhani <robert.rouhani@gmail.com>*/

/*License of Textbox.h and Textbox.cpp unknown, provided by the instructor of
the summer course. Use at your own risk.*/

#include "DarkGDK.h"
#include "TextBox.h"

#include <vector>
#include <string>

using namespace std;

//Globals
int spriteNum = 1000;
bool firstClick = true;
bool initialized = false;

TextBox msgbox = TextBox(14, 414, 574,false, 10, 15, 10000, 10001);

bool items[6] = {false, false, false, false, false, false };
bool itemsClaimed[6] = {false, false, false, false, false, false };
bool itemSpotsAvail[12] = {true, true, true, true, true, true, true, true, true, true, true, true };

////////////////////
//  Items List:   //
//0 - Code piece 1//
//1 - Code piece 2//
//2 - Code piece 3//
//3 - Code piece 4//
//4 - Combined code/
//5 - Screwdriver //
//6 - Exit key    //
////////////////////

const int ItemPosition[12][2] = {
	{615, 15},
	{703, 15},
	{615, 115},
	{703, 115},
	{615, 213},
	{703, 213},
	{615, 309},
	{703, 309},
	{615, 404},
	{703, 404},
	{615, 499},
	{703, 499},
};

enum GameStates { 
	mainMenu, 
	room1Side1, 
	room1Side2, 
	room1Side3, 
	room1Side4, 
	room1Up, 
	room1UnderBed, 
	room1OutsideSafe, 
	room1UncoveredSafe, 
	room1InsideSafe, 
	room1InsideVent, 
	room1Dresser, 
	room1ToolBox,
	room1BehindCurtains,
	//room2Side1, 
	//room2Cabinet, 
	//room2InsideCabinet, 
	//room1DoorClose, 
	winMenu
};

GameStates gs = mainMenu;

struct Item {
	int spriteID;
	int pos_x;
	int pos_y;
	int size_x;
	int size_y;
	int imgID;
	bool scale;
	int itemID;
	string name;

	Item () {
		spriteID = 0;
		pos_x = 0;
		pos_y = 0;
		size_x = 0;
		size_y = 0;
		imgID = 0;
		scale = false;
		name = "";
		itemID = 0;
	}

	Item (int sprID, int p_x, int p_y, int s_x, int s_y, int iID, bool scl, string n, int itID)
	{
		spriteID = sprID;
		pos_x = p_x;
		pos_y = p_y;
		size_x = s_x;
		size_y = s_y;
		imgID = iID;
		scale = scl;
		name = n;
		itemID = itID;
		dbSprite(spriteID, pos_x, pos_y, imgID);
	}

	void CreateSprite()
	{
		dbSprite(spriteID, pos_x, pos_y, imgID);
	}

	bool MouseOver (int mouseX, int mouseY)
	{
		int pos_bx, pos_by;
		pos_bx = pos_x + size_x;
		pos_by = pos_y + size_y;
		if (mouseX >= pos_x && mouseX <= pos_bx && mouseY >= pos_y && mouseY <= pos_by)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ScaleUp ()
	{
		if (scale)
		{
			dbScaleSprite(spriteID, 105);
		}
	}

	void ScaleDown ()
	{
		if (scale)
		{
			dbScaleSprite(spriteID, 100);
		}
	}

	void PickUp ()
	{
		items[itemID] = true;
		char name_c[4096];
		sprintf(name_c, "You just found a %s", name.c_str());
		dbDeleteSprite(spriteID);
		msgbox.addText(name_c);
	}

	void Remove()
	{
		dbDeleteSprite(spriteID);
	}
};


struct Sprite {

	//Variables
	int spriteID;
	int pos_x;
	int pos_y;
	int size_x;
	int size_y;
	int imgID;
	bool scale;
	bool msg;
	string message;
	
	//Constructors
	Sprite () {
		spriteID = 0;
		pos_x = 0;
		pos_y = 0;
		size_x = 0;
		size_y = 0;
		imgID = 0;
		scale = false;
		msg = false;
		message = "";
	}
	Sprite (int sprID, int p_x, int p_y, int s_x, int s_y, int iID, bool scl, bool m, string s_msg) {
		spriteID = sprID;
		pos_x = p_x;
		pos_y = p_y;
		size_x = s_x;
		size_y = s_y;
		imgID = iID;
		scale = scl;
		msg = m;
		message = s_msg;
		dbSprite(spriteID, pos_x, pos_y, imgID);
	}

	//Methods
	void CreateSprite()
	{
		dbSprite(spriteID, pos_x, pos_y, imgID);
	}

	bool MouseOver (int mouseX, int mouseY)
	{
		int pos_bx, pos_by;
		pos_bx = pos_x + size_x;
		pos_by = pos_y + size_y;
		if (mouseX >= pos_x && mouseX <= pos_bx && mouseY >= pos_y && mouseY <= pos_by)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ScaleUp ()
	{
		if (scale)
		{
			dbScaleSprite(spriteID, 105);
		}
	}

	void ScaleDown ()
	{
		if (scale)
		{
			dbScaleSprite(spriteID, 100);
		}
	}

	void Click ()
	{
		if (msg)
		{
			char temp_msg[4096];
			sprintf(temp_msg, "%s", message.c_str());
			msgbox.addText(temp_msg);
		}
	}

	void Remove()
	{
		dbDeleteSprite(spriteID);
	}
};


struct Button {

	//Variables
	int spriteID;
	int pos_x;
	int pos_y;
	int size_x;
	int size_y;
	int imgID;
	bool scale;
	GameStates gs;

	
	//Constructors
	Button() {
		spriteID = 0;
		pos_x = 0;
		pos_y = 0;
		size_x = 0;
		size_y = 0;
		imgID = 0;
		scale = false;
		gs = room1Side1;
	}
	Button(int sprID, int p_x, int p_y, int s_x, int s_y, int iID, bool scl, GameStates state) {
		spriteID = sprID;
		pos_x = p_x;
		pos_y = p_y;
		size_x = s_x;
		size_y = s_y;
		imgID = iID;
		scale = scl;
		gs = state;
		dbSprite(spriteID, pos_x, pos_y, imgID);
	}

	//Methods
	void CreateSprite()
	{
		dbSprite(spriteID, pos_x, pos_y, imgID);
	}

	bool MouseOver (int mouseX, int mouseY)
	{
		int pos_bx, pos_by;
		pos_bx = pos_x + size_x;
		pos_by = pos_y + size_y;
		if (mouseX >= pos_x && mouseX <= pos_bx && mouseY >= pos_y && mouseY <= pos_by)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ScaleUp ()
	{
		if (scale)
		{
			dbScaleSprite(spriteID, 105);
		}
	}

	void ScaleDown ()
	{
		if (scale)
		{
			dbScaleSprite(spriteID, 100);
		}
	}

	void Remove()
	{
		dbDeleteSprite(spriteID);
	}
};


std::vector<Sprite> AllSprites;
std::vector<Button> AllButtons;
std::vector<Item> AllItems;

//Prototypes
void Game();
void Initialize();
void ItemCheck();
void CombineItems();
int returnImgIDFromArray(int num);

//Main

void DarkGDK ( void )
{
	dbSetDisplayMode(800,600,32);
	dbSyncOn   ( );
	dbSyncRate ( 60 );
	
	dbLoadImage("ETR-mainMenu.png", 1);
	dbLoadImage("ETR-gameBack.png", 2);
	dbLoadImage("ETR-baseRoom.png", 3);
	dbLoadImage("ETR-Larrow.png", 4);
	dbLoadImage("ETR-Rarrow.png", 5);
	dbLoadImage("ETR-doorOut.png", 6);
	dbLoadImage("ETR-table.png", 7);
	dbLoadImage("ETR-bed.png", 8);
	dbLoadImage("ETR-paint1.png", 9);
	dbLoadImage("ETR-outsideSafe.png", 10);
	dbLoadImage("ETR-SafeCovered.png", 11);
	dbLoadImage("ETR-SafeCover.png", 12);
	dbLoadImage("ETR-Darrow.png", 13);
	dbLoadImage("ETR-Uarrow.png", 14);
	dbLoadImage("ETR-room1ceil.png", 15);
	dbLoadImage("ETR-vent.png", 16);
	dbLoadImage("ETR-InsideVent.png", 17);
	dbLoadImage("ETR-items-torn1.png", 18);
	dbLoadImage("ETR-items-torn2.png", 19);
	dbLoadImage("ETR-items-torn3.png", 20);
	dbLoadImage("ETR-items-torn4.png", 21);
	dbLoadImage("ETR-items-code.png", 22);
	dbLoadImage("ETR-paper-scrap.png", 23);
	dbLoadImage("ETR-toolbox.png", 24);
	dbLoadImage("ETR-InsideToolBox.png", 25);
	dbLoadImage("ETR-screwdriver.png", 26);
	dbLoadImage("ETR-items-screwdrv.png", 27);
	dbLoadImage("ETR-UnderBed.png", 28);
	dbLoadImage("ETR-paint2.png", 29);
	dbLoadImage("ETR-dresser.png", 30);
	dbLoadImage("ETR-InsideDresser.png", 31);
	dbLoadImage("ETR-curtains.png", 32);
	dbLoadImage("ETR-BehindCurtains.png", 33);
	dbLoadImage("ETR-behindSafe.png", 34);
	dbLoadImage("ETR-key.png", 35);
	dbLoadImage("ETR-items-key.png", 36);
	dbLoadImage("ETR-WinMenu.png", 37);


	while ( LoopGDK ( ) )
	{
		Game();
		CombineItems();
		ItemCheck();
		dbSync ( );
	}

	return;
}

//Everything Else
void Game()
{
	switch(gs)
	{
		case mainMenu:
			dbSprite(1, 0, 0, 1);
			if (dbKeyState(28))
			{
				dbSprite(1, 0, 0, 2);
				dbSprite(2, 0, 0, 3);
				gs = room1Side1;
			}
			break;

		case winMenu:
			break;

		default:
			if (!initialized)
			{
				Initialize();
				initialized = true;
			}
			else
			{
				for (int i = 0; i < AllButtons.size(); i++)
				{
					if (AllButtons[i].MouseOver(dbMouseX(), dbMouseY()))
					{
						AllButtons[i].ScaleUp();
						if (dbMouseClick() && firstClick)
						{
							firstClick = false;
							initialized = false;
							gs = AllButtons[i].gs;
							Initialize();
						}
						if (!dbMouseClick())
						{
							firstClick = true;
						}
					}
					else
					{
						AllButtons[i].ScaleDown();
					}
				}

				for (int i = 0; i < AllItems.size(); i++)
				{
					if (AllItems[i].MouseOver(dbMouseX(), dbMouseY()))
					{
						AllItems[i].ScaleUp();

						if (dbMouseClick() && firstClick)
						{
							firstClick = false;
							AllItems[i].PickUp();
							AllItems.erase(AllItems.begin() +i);
						}

						if (!dbMouseClick())
						{
							firstClick = true;
						}
					}
					else
					{
						AllItems[i].ScaleDown();
					}
				}

				for (int i = 0; i < AllSprites.size(); i++)
				{
					if (AllSprites[i].MouseOver(dbMouseX(), dbMouseY()))
					{
						AllSprites[i].ScaleUp();

						if (dbMouseClick() && firstClick)
						{
							firstClick = false;
							AllSprites[i].Click();
						}

						if (!dbMouseClick())
						{
							firstClick = true;
						}
					}
					else
					{
						AllSprites[i].ScaleDown();
					}
				}
			}

			break;
	}
}

void Initialize()
{
	for (int i = 0; i < AllItems.size(); i++)
	{
		AllItems[i].Remove();
	}
	for (int i = 0; i < AllButtons.size(); i++)
	{
		AllButtons[i].Remove();
	}
	for (int i = 0; i < AllSprites.size(); i++)
	{
		AllSprites[i].Remove();
	}
	
	AllItems.clear();
	AllButtons.clear();
	AllSprites.clear();

	switch (gs)
	{
		case room1Side1:
			dbSprite(2, 0, 0, 3);
			AllButtons.push_back(Button(3, 5, 175, 64, 48, 4, true, room1Side4));
			AllButtons.push_back(Button(4, 523, 175, 64, 48, 5, true, room1Side2));
			AllButtons.push_back(Button(5, 262, 5, 48, 64, 14, true, room1Up));
			if (!items[5])
			{
				AllSprites.push_back(Sprite(6, 381, 93, 119, 253, 6, false, true, "The door is locked."));
			}
			else
			{
				AllButtons.push_back(Button(6, 381, 93, 119, 253, 6, true, winMenu));
			}
			AllButtons.push_back(Button(7, 141, 110, 126, 108, 9, true, room1OutsideSafe));
			break;

		case room1Side2:
			dbSprite(2, 0, 0, 3);
			AllButtons.push_back(Button(3, 5, 175, 64, 48, 4, true, room1Side1));
			AllButtons.push_back(Button(4, 523, 175, 64, 48, 5, true, room1Side3));
			AllButtons.push_back(Button(5, 262, 5, 48, 64, 14, true, room1Up));
			AllButtons.push_back(Button(6, 203, 71, 206, 293, 32, false, room1BehindCurtains));
			break;

		case room1Side3:
			dbSprite(2, 0, 0, 3);
			AllButtons.push_back(Button(3, 5, 175, 64, 48, 4, true, room1Side2));
			AllButtons.push_back(Button(4, 523, 175, 64, 48, 5, true, room1Side4));
			AllButtons.push_back(Button(5, 262, 5, 48, 64, 14, true, room1Up));
			AllSprites.push_back(Sprite(6, 327, 69, 181, 161, 29, false, true, "A painting."));
			AllButtons.push_back(Button(7, 14, 243, 219, 147, 30, true, room1Dresser));
			break;

		case room1Side4:
			dbSprite(2, 0, 0, 3);
			AllButtons.push_back(Button(3, 5, 175, 64, 48, 4, true, room1Side3));
			AllButtons.push_back(Button(4, 523, 175, 64, 48, 5, true, room1Side1));
			AllButtons.push_back(Button(5, 262, 5, 48, 64, 14, true, room1Up));
			AllSprites.push_back(Sprite(6, 11, 270, 180, 128, 7, false, false, ""));
			AllButtons.push_back(Button(7, 185, 193, 271, 207, 8, false, room1UnderBed));
			AllButtons.push_back(Button(8, 75, 210, 106, 76, 24, true, room1ToolBox));
			break;

		case room1Up:
			dbSprite(2, 0, 0, 15);
			AllButtons.push_back(Button(3, 276, 331, 48, 64, 13, true, room1Side1));
			if (items[5])
			{
				AllButtons.push_back(Button(4, 214, 119, 86, 74, 16, true, room1InsideVent));
			}
			else
			{
				AllSprites.push_back(Sprite(4, 214, 119, 86, 74, 16, false, true, "The vent is screwed shut."));
			}
			break;

		case room1InsideVent:
			dbSprite(2, 0, 0, 17);
			AllButtons.push_back(Button(3, 276, 331, 48, 64, 13, true, room1Up));
			if (!items[0])
			{
				AllItems.push_back(Item(4, 450, 290, 50, 50, 23, true, "scrap of paper!", 0));
			}
			break;

		case room1OutsideSafe:
			dbSprite(2, 0, 0, 3);
			AllSprites.push_back(Sprite(4, 0, 0, 600, 400, 11, false, false, ""));
			if (!items[4])
			{
				AllButtons.push_back(Button(5, 71, 6, 455, 390, 12, true, room1UncoveredSafe));
			}
			else
			{
				AllButtons.push_back(Button(5, 71, 6, 455, 390, 12, true, room1InsideSafe));
			}
			AllButtons.push_back(Button(3, 5, 331, 48, 96, 13, true, room1Side1));
			break;

		case room1UncoveredSafe:
			dbSprite(2, 0, 0, 3);
			AllSprites.push_back(Sprite(4, 0, 0, 600, 400, 10, false, false, ""));
			AllButtons.push_back(Button(3, 5, 331, 48, 96, 13, true, room1OutsideSafe));
			break;

		case room1ToolBox:
			dbSprite(2, 0, 0, 25);
			AllButtons.push_back(Button(3, 276, 331, 48, 64, 13, true, room1Side4));
			if (!items[5])
			{
			AllItems.push_back(Item(4, 175, 147, 266, 100, 26, true, "screwdriver!", 5));
			}
			break;

		case room1UnderBed:
			dbSprite(2, 0, 0, 28);
			AllButtons.push_back(Button(3, 262, 5, 48, 64, 14, true, room1Side4));
			if (!items[1])
			{
				AllItems.push_back(Item(4, 100, 310, 50, 50, 23, true, "scrap of paper!", 1));
			}
			break;

		case room1Dresser:
			dbSprite(2, 0, 0, 31);
			AllButtons.push_back(Button(3, 276, 331, 48, 64, 13, true, room1Side3));
			if (!items[2])
			{
				AllItems.push_back(Item(4, 530, 310, 50, 50, 23, true, "scrap of paper!", 2));
			}
			break;

		case room1BehindCurtains:
			dbSprite(2, 0, 0, 33);
			AllButtons.push_back(Button(3, 276, 331, 48, 64, 13, true, room1Side2));
			if (!items[3])
			{
				AllItems.push_back(Item(4, 350, 210, 50, 50, 23, true, "scrap of paper!", 3));
			}
			break;

		case room1InsideSafe:
			dbSprite(2, 0, 0, 34);
			AllButtons.push_back(Button(3, 276, 331, 48, 64, 13, true, room1OutsideSafe));
			if (!items[6])
			{
				AllItems.push_back(Item(4, 157, 48, 307, 298, 35, true, "key!", 6));
			}
			break;

		case winMenu:
			bool won = false;
			dbSprite(1, 0, 0, 37);
			for (int i = 2; i <=7; i++)
			{
				dbDeleteSprite(i);
			}
			for (int i = 1000; i <= 1007; i++)
			{
				dbDeleteSprite(i);
			}

			for (int i = 10000; i <= 10100; i++)
			{
				dbDeleteSprite(i);
			}

			while (!won)
			{

				if (dbKeyState(28))
				{
					won = true;
					for (int i = 0; i < AllItems.size(); i++)
					{
						AllItems[i].Remove();
					}
					for (int i = 0; i < AllButtons.size(); i++)
					{
						AllButtons[i].Remove();
					}
					for (int i = 0; i < AllSprites.size(); i++)
					{
						AllSprites[i].Remove();
					}
					AllItems.clear();
					AllButtons.clear();
					AllSprites.clear();
					for (int i = 0; i < 6; i++)
					{
						items[i] = false;
						itemsClaimed[i] = false;
					}
					for (int i = 0; i < 12; i++)
					{
						itemSpotsAvail[i] = true;
					}
					gs = room1Side1;

				}
				dbSync();
			}
			dbSprite(1, 0, 0, 2);
			break;
	}
}

void ItemCheck()
{
	for (int i = 0; i <= 6; i++)
	{
		if (items[i] && !itemsClaimed[i])
		{
			for (int j = 0; j < 12; j++)
			{
				if (itemSpotsAvail[j])
				{
					dbSprite(spriteNum, ItemPosition[j][0], ItemPosition[j][1], returnImgIDFromArray(i));
					itemsClaimed[i] = true;
					itemSpotsAvail[j] = false;
					spriteNum++;
					return;
				}
			}
		}
	}
}

int returnImgIDFromArray(int num)
{
	switch (num)
	{
	case 0:
		return 18;
	case 1:
		return 19;
	case 2:
		return 20;
	case 3:
		return 21;
	case 4:
		return 22;
	case 5:
		return 27;
	case 6:
		return 36;
	default:
		return 18;
	}
}

void CombineItems()
{
	if (items[0] && items[1] && items[2] && items[3])
	{
		items[4] = true;
	}
}