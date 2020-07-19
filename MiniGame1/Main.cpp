#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

using namespace std;
using namespace sf;

#define M_PI 3.14159265359


//Class of car
class Car : public Drawable, public Transformable
{
	ConvexShape carRoof;
	RectangleShape carBody;
	CircleShape backWheel, frontWheel;

	public:
		Car();
		virtual void draw(RenderTarget& target, RenderStates states) const //used for displaying car on screen
		{
			states.transform = getTransform();
			target.draw(carRoof,states);
			target.draw(carBody, states);
			target.draw(backWheel, states);
			target.draw(frontWheel, states);
		}

		RectangleShape getCarBody()
		{
			return carBody;
		}

		void rotateWheels(float angle)
		{
			backWheel.rotate(angle);
			frontWheel.rotate(angle);
		}
};

Car::Car()
{
	carRoof.setPointCount(4);
	carRoof.setOutlineThickness(4);
	carRoof.setPoint(0, Vector2f(38, 55));
	carRoof.setPoint(1, Vector2f(50, 45));
	carRoof.setPoint(2, Vector2f(80, 45));
	carRoof.setPoint(3, Vector2f(92, 55));
	carRoof.setOrigin(27, 10);
	carRoof.setFillColor(Color::Black);
	carRoof.setPosition(27, 10);

	carBody.setOutlineThickness(4);
	carBody.setSize(Vector2f(90, 14));
	carBody.setOrigin(45, 7);
	carBody.setPosition(65, 70);
	carBody.setFillColor(Color::Black);

	backWheel.setRadius(6);
	backWheel.setPointCount(100);
	backWheel.setPosition(35, 78);
	backWheel.setOrigin(6, 6);
	backWheel.setOutlineThickness(4);
	backWheel.setFillColor(sf::Color::Black);

	frontWheel.setRadius(6);
	frontWheel.setPointCount(100);
	frontWheel.setPosition(80, 78);
	frontWheel.setOrigin(6, 6);
	frontWheel.setOutlineThickness(4);
	frontWheel.setFillColor(sf::Color::Black);
}
int main()
{
	RenderWindow window(VideoMode(1500, 600), "SFML Window", Style::Default);

	window.setVerticalSyncEnabled(true);

	VertexArray curve(LinesStrip);
	int pathIndex = 0;

	for (int x = 0; x <= 1500; x+=50)
	{
		curve.append(Vertex(Vector2f(x, 300 - cos(x)*200)));
	}


	auto derivative = [](float i)
	{
		return sin(i);
	};


	Car car;

	car.setOrigin(60, 66);
	cout << car.getTransform().getMatrix() << endl;

	car.setPosition(curve.operator[](pathIndex).position - Vector2f(0, 18.0f));

	car.setRotation(-atan(derivative(curve.operator[](pathIndex).position.x)) * 180 / M_PI);


	Event event;

	//Game loop

	while (window.isOpen())
	{
		while(window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
			{
				if (pathIndex < curve.getVertexCount() - 1)
				{
					pathIndex++;
					
					float angle = atan(derivative(curve.operator[](pathIndex).position.x));

					angle *= 180 / M_PI;

					std::cout << "angle = " << angle << endl;

					car.setRotation(-angle);
					car.rotateWheels(5.0f);

					if(pathIndex >= 1 && pathIndex <= 2 || pathIndex >= 22 && pathIndex <= 25)
						car.setPosition(curve.operator[](pathIndex).position - Vector2f(0, 18.0f));
					else
						car.setPosition(curve.operator[](pathIndex).position - Vector2f(0, 27.5f));
				}	
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
			{
				if (pathIndex > 0)
				{
					pathIndex--;

					float angle = atan(derivative(curve.operator[](pathIndex).position.x));

					angle *= 180 / M_PI;	//Converts angle from radians to degrees

					//Uncomment the line below to display the angle values of the car at each position on the mountain.
					//std::cout << "angle = " << angle << endl;	

					car.setRotation(-angle);	//Rotates car counterclockwise
					car.rotateWheels(-5.0f);	//Rotates car by 5 degrees counterclockwise

					//Makes sure the car follows the trajectory on the points below
					if (pathIndex >= 0 && pathIndex <= 2 || pathIndex >= 22 && pathIndex <= 25)
						car.setPosition(curve.operator[](pathIndex).position - Vector2f(0, 18.0f));
					else car.setPosition(curve.operator[](pathIndex).position - Vector2f(0, 27.5f));

				}
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
			{
				car.rotate(1);	//Rotates car clockwise
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::L)
			{
				car.rotate(-1);	//Rotates car counterclockwise
			}
		}

		window.clear();

		window.draw(curve);
		window.draw(car);

		window.display();

	}
	return 0;
}



