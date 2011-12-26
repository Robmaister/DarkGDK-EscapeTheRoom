/* Create debug text box.
   To make a text box object, see Main.cpp for a demo.
   The arguments to the constructor are:
     - x coord
     - y coord
     - box width in pixels (everything above that will get cut off)
     - do you want to show line numbers or not?
     - max num lines in text box
     - text size
     - text bitmap ID -- pick any number, but make sure it doesn't
       clash with any existing sprite IDs !!!
     - starting sprite ID; make sure that you DO NOT HAVE ANY SPRITES
       equal to or above this number!  (including other text box sprites --
	   leave A LOT of room!)

   If you make a text box and run the game long enough, odds are sprite
   numbers will overflow or start to clash.
   Right now, there is no good way around this -- sorry, Dark GDK does not
   allow recycling of sprite IDs for text nicely.
*/

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <deque>

struct TextSprite {
	unsigned int id;      // sprite ID and image ID;
	unsigned int height;  // height in pixels;
};

class TextBox {
public:
	TextBox (int boxX=0, int boxY=0, unsigned int boxWidth=600,
		     bool showLineNums=true,
			 unsigned int boxMaxNumLines=10,
			 unsigned int boxTextSize=20,
			 unsigned int textBitmapId=50000,
			 unsigned int textIdStart=50001);
	void addText (char *text);
	void moveBy (int dx, int dy);
private:
	std::deque<TextSprite> lines;
	int x, y;
	unsigned int counter, width, maxNumLines, textSize, bitmapId, nextSpriteId;
	bool lineNums;
};

#endif
