#pragma once
#include <SFML/Graphics.hpp>

#define Max_Items 4

class Startmenu
{
public:

	Startmenu(float width, float height);
	~Startmenu();

	void draw(sf::RenderWindow& window);
	void MoveUp();
	void MoveDown();
	int getpressed() { return selectedindex; }

private:
	
	int selectedindex;
	sf::Font font;
	sf::Text text[Max_Items];

};

