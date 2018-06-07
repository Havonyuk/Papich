#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "view.h"
#include <iostream>
#include <sstream>

#include "iostream"
#include "level.h"
#include <vector>
#include <list>
#include "LifeBar.h"
#include "Game.h"

using namespace sf;
////////////////////////////////////Общий класс-родитель//////////////////////////
class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}

	virtual void update(float time) = 0;
};


////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay, attack, hurt, die } state;
	int playerScore;
	float CurrentFrame; bool Hurt; bool win;
	Player(Image &image, String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		win = false;
		playerScore = 0; state = stay; obj = lev.GetAllObjects();
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
	}

	void control() {
		state = stay;
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			state = left; speed = 0.1;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			state = right; speed = 0.1;
		}

		if ((Keyboard::isKeyPressed(Keyboard::W)) && (onGround)) {
			state = jump; dy = -1; onGround = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::S)) {
			state = down;
		}


		if (Hurt == true)
		{
			state = hurt;
		}
		if (win == true)
		{
			state = stay;
		}
		if (health <= 0)
		{
			speed = 0.00;
			state = die;
			life = false;
		}

	}
	void update(float time)
	{
		if (life) {
			control();
			switch (state)
			{
			case right:dx = speed; break;
			case left:dx = -speed; break;
			case up: break;
			case down: dx = 0; break;
			case jump:
				break;
			case attack:
				break;
			case die:
				dx = 0;
				dy = 0;
				break;
			case stay:
				dx = 0;
				break;
			case hurt:
				dx = 0;
				break;
			}
			x += dx * time;
			checkCollisionWithMap(dx, 0);
			y += dy * time;
			checkCollisionWithMap(0, dy);
			speed = 0;
			sprite.setPosition(x + w / 2, y + h / 2);
			dy = dy + 0.0015*time;
			setPlayerCoordinateForView(x, y);
		}
	}


	void checkCollisionWithMap(float Dx, float Dy)
	{

		for (int i = 0; i<obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}

				if (obj[i].name == "water")
				{
					health -= 100;
				}
				if (obj[i].name == "Win") {
					win = true;
				}

			}


	}


};



class Enemy :public Entity {
public:
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(0, 300, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				//if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
				if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				//	}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			checkCollisionWithMap(dx, 0);
			x += dx*time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};


class MovingPlatform : public Entity {
public:
	MovingPlatform(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		sprite.setTextureRect(IntRect(127, 32, W, H));
		dx = 0.08;
	}

	void update(float time)
	{
		x += dx * time;
		moveTimer += time;
		if (moveTimer>2000) { dx *= -1; moveTimer = 0; }
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};

bool startGame()
{
	RenderWindow window(VideoMode(1334, 750), "Papich");
	menu(window);
	view.reset(FloatRect(0, 0, 1334, 750));

	Font fontdie;
	fontdie.loadFromFile("Trajan.ttf");
	Text textdie("", fontdie, 90);
	//	textdie.setColor(Color::Red);
	textdie.setStyle(Text::Bold | Text::Underlined);

	Font fontdamage;
	fontdamage.loadFromFile("Trajan.ttf");
	Text textdamge("", fontdie, 10);
	textdie.setStyle(Text::Bold);

	Level lvl;
	lvl.LoadFromFile("level1.tmx");

	SoundBuffer shootbuff;
	shootbuff.loadFromFile("sounds/shoot.ogg");
	Sound shoot(shootbuff);

	Music podrubka;
	podrubka.openFromFile("sounds/podrubka.ogg");

	Music main_song;
	main_song.openFromFile("sounds/main_song.ogg");




	podrubka.play();

	main_song.play();
	main_song.setLoop(true);






	Image heroImage;
	heroImage.loadFromFile("images/2.png");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/fang.png");

	Image movePlatfrormImage;
	movePlatfrormImage.loadFromFile("images/goodly-2x.png");

	std::list<Entity*> entities;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;
	std::vector<Object> e = lvl.GetObjects("EasyEnemy");

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 34, 62));

	Object player = lvl.GetObject("player");

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 100, 185);

	e = lvl.GetObjects("MovingPlatform");
	for (int i = 0; i < e.size(); i++) entities.push_back(new MovingPlatform(movePlatfrormImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 97, 33));

	LifeBar lifeBarPlayer;

	Clock clock;
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 300;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		p.update(time);
		if (p.life == false) {
			textdie.setString("YOU DIED");
			textdie.setPosition(view.getCenter().x - 200, view.getCenter().y - 100);
		}

		if (p.win == true) {
			main_song.stop();
		}

		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { it = entities.erase(it); delete b; }
			else it++;
		}


		for (it = entities.begin(); it != entities.end(); it++)
		{
			if (((*it)->name == "MovingPlatform") && ((*it)->getRect().intersects(p.getRect())))
			{
				Entity * movPlat = *it;
				if ((p.dy > 0) || (p.onGround == false))
					if (p.y + p.h < movPlat->y + movPlat->h) {
						p.y = movPlat->y - p.h + 5; p.x += movPlat->dx*time;
						p.dy = 0;
						p.onGround = true;
					}
			}

			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "EasyEnemy") {
					Entity *enemy1 = *it;
					if ((p.dy > 0) && (p.onGround == false)) { enemy1->dx = 0; p.dy = -0.2; enemy1->health = 0; }
					else {
						p.health -= 5;
					}
				}

			}
		}


		window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);

		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
		}


		lifeBarPlayer.update(p.health);
		//	p.update(time);
		view.setCenter(p.x, p.y);
		window.draw(textdie);

		window.draw(p.sprite);
		lifeBarPlayer.draw(window);
		window.display();

		if (Keyboard::isKeyPressed(Keyboard::R)) return true;
		if (Keyboard::isKeyPressed(Keyboard::Escape)) return false;
	}
}

void gameRunning()
{
	if (startGame()) { gameRunning(); }
}



int main()
{
	gameRunning();
	return 0;
}