#pragma once
#include <SFML/Graphics.hpp>

class Widget;

class gui
{
private:
	std::vector<Widget> widgets;

public:
	void handle_event(const sf::Event&);
	void add();
	void remove();
	void draw();
};

class Widget
{
	protected:
		sf::Vector2i position;
		unsigned height, weight;

	private:
		void virtual draw() = 0;

	friend gui;
};
