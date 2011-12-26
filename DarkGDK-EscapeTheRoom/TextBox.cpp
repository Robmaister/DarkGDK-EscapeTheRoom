#include "assert.h"
#include <iostream>
#include "DarkGDK.h"
#include "TextBox.h"

using namespace std;


TextBox::TextBox (int boxX, int boxY, unsigned int boxWidth,
				  bool showLineNums,
				  unsigned int boxMaxNumLines,
				  unsigned int boxTextSize,
				  unsigned int textBitmapId,
				  unsigned int textIdStart) {
	counter = 0;
	x = boxX;
	y = boxY;
	width = boxWidth;
	maxNumLines = boxMaxNumLines;
	textSize = boxTextSize;
	bitmapId = textBitmapId;
	nextSpriteId = textIdStart;
	lineNums = showLineNums;
}


void TextBox::addText (char *text) {
	dbSetTextSize (textSize);

	TextSprite line;
	line.height = dbTextHeight (text);
	line.id     = nextSpriteId++;

	// Create a temporary bitmap and set its background to transparent.
	dbCreateBitmap (bitmapId, width, line.height);

	// Write the text to the bitmap and grab an image of that text.
	char tmp[1024];
	int success;
	if (lineNums) {
		success = sprintf (tmp, "[%d] %s", counter, text);
	}
	else {
		success = sprintf (tmp, "%s", text);
	}
	assert (success > 0);
	dbText (0, 0, tmp);
	dbGetImage (line.id, 0, 0, width, line.height);

	// Clean up by deleting the temp bitmap and reseting our canvas.
	dbDeleteBitmap (bitmapId);
	dbSetCurrentBitmap (0);

	// Update line queue.
	if (lines.size() >= maxNumLines) {
		// Wipe stale sprites for lines out of bounds.
		assert (lines.size() == maxNumLines);
		dbDeleteSprite (lines.front().id);
		//nextSpriteId = lines.front().id;  // ARGH!  Can't recycle sprite IDs...
		                                    // If you reuse them, text starts
		                                    // getting weirdly stretched!!
		lines.pop_front();
	}
	lines.push_back (line);

	// Put ALL text sprite, one by one, on screen.
	for (int i = 0; i < lines.size(); i++) {
		dbSprite (lines[i].id, x, y+lines[i].height*i, lines[i].id);
	}
	
	counter++;
}


void TextBox::moveBy (int dx, int dy) {
	x += dx;
	y += dy;
	// Draw text line sprites at new location.
	for (int i = 0; i < lines.size(); i++) {
		dbSprite (lines[i].id, x, y+lines[i].height*i, lines[i].id);
	}
}