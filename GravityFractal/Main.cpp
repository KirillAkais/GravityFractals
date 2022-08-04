#include "SFML/Graphics.hpp"

using namespace sf;
using namespace std;

#define WINDOW_W 2560
#define WINDOW_H 1440
#define ASP_RAT 1.7777777778
#define PART_COUNT 6

Vector2f mouse_pos(RenderWindow &window)
{
	return Vector2f((float(Mouse::getPosition(window).x) / WINDOW_W - 0.5) * ASP_RAT, 0.5 - float(Mouse::getPosition(window).y) / WINDOW_H);
}

Vector2f operator* (Vector2f v, double x)
{
	v.x *= x;
	v.y *= x;
	return v;
}

Vector2f operator/ (Vector2f v, double x)
{
	v.x /= x;
	v.y /= x;
	return v;
}

void main()
{
	//Load the shader
	Shader shader;
	shader.loadFromFile("shader.frag", Shader::Fragment);
	RectangleShape rect;
	rect.setFillColor(Color::Green);
	rect.setSize(Vector2f(WINDOW_W, WINDOW_H));
	
	double scale = 0.1;
	double dist = 1;
	Vector2f* p_pos = new Vector2f[PART_COUNT];

	//Set mass
	float* p_mass = new float[PART_COUNT];
	p_mass[0] = 3;
	p_mass[1] = 2;
	p_mass[2] = 2;
	p_mass[3] = 4;
	p_mass[4] = -2;
	p_mass[5] = -2;

	Vector2f m_pos_pr;
	Vector2f pos_pr;
	Vector2f pos = Vector2f(0, 0);

	//Create window
	RenderWindow window(VideoMode(WINDOW_W, WINDOW_H), "window", Style::None);
	window.setVerticalSyncEnabled(true);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case(Event::Closed):
				window.close();
				break;
			default:
				break;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		//Change the distance with W and S keys
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			dist += 0.01;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S))
		{
			dist -= 0.01;
		}
		//Scale the image with arrows
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			scale *= 1.01;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			scale /= 1.01;
		}
		//Move the image with mouse
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			pos = pos_pr;
			pos += (mouse_pos(window) - m_pos_pr) / scale;
		}
		else
		{
			m_pos_pr = mouse_pos(window);
			pos_pr = pos;
		}
		//Set points in a hexagon shape
		p_pos[0] = Vector2f(0, 0.57735026919 * dist) + pos;
		p_pos[1] = Vector2f(-0.5 * dist, -0.28867513459 * dist) + pos;
		p_pos[2] = Vector2f(0.5 * dist, -0.28867513459 * dist) + pos;
		p_pos[3] = Vector2f(0, -0.57735026919 * dist) + pos;
		p_pos[4] = Vector2f(-0.5 * dist, 0.28867513459 * dist) + pos;
		p_pos[5] = Vector2f(0.5 * dist, 0.28867513459 * dist) + pos;
		//Set uniforms for the shader
		shader.setUniform("iResolution", Vector2f(WINDOW_W, WINDOW_H));
		shader.setUniform("iScale", float(scale));
		shader.setUniformArray("iPartPos", p_pos, PART_COUNT);
		shader.setUniformArray("iPartMass", p_mass, PART_COUNT);
		//Draw everything
		window.clear(Color::Black);
		window.draw(rect, &shader);
		window.display();
		sleep(seconds(1.0 / 60.0));
	}
}