#include <SFML/Graphics.hpp>
using namespace sf;

View view;

void setPlayerCoordinateForView(float x, float y) { 
	float tempX = x, tempY = y; 
	//if (x < 320) tempX = 320;
	//if (y < 240) tempY = 240;
//	if (y > 980) tempY = 980;


	
	view.setCenter(tempX, tempY);
//	return view;

}

