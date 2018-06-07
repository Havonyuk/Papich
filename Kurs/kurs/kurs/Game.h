#include <SFML/Graphics.hpp>

using namespace sf;


void menu(RenderWindow & window) {

	Music podrubka;
	podrubka.openFromFile("sounds/podrubka.ogg");
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/222.png");
	menuTexture3.loadFromFile("images/333.png");
	aboutTexture.loadFromFile("images/about.png");
	menuBackground.loadFromFile("images/menu.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 300);
	menu2.setPosition(100, 360);
	menu3.setPosition(100, 420);
	menuBg.setPosition(0, 0);

	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 300, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Yellow); menuNum = 1; }
		if (IntRect(100, 360, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Yellow); menuNum = 2; }
		if (IntRect(100, 420, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Yellow); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) {isMenu = false; };//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
	////////////////////////////////////////////////////
}

