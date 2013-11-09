//#include <stdio.h>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <sstream>

int main()
{
	//Creates a window using SFML
	sf::RenderWindow window(sf::VideoMode(600, 600, 32), "SFML Test", sf::Style::Default);

	//Defines gravity and sets up the game world in Box2D
	b2Vec2 gravity(0, 9.8f);
	b2World* m_world = new b2World(gravity);

	//Creates a new shape in SFML which we use later to draw the shapes
	sf::ConvexShape cShape;
	cShape.setFillColor(sf::Color::Red);

	/*To draw objects in Box2D we need a body and a fixture, think of a body as an overall
	container or model of the object with a fixture being the individual parts of the model
	ie a car where the overall car is the body with fixtures such as the wheels and steering wheel
	as part of the overall car body*/

	//Define a new body
	b2BodyDef myBodyDef;
	//Set the body to be dynamic (means it can move through the gameworld and can be knocked around by other objects)
	myBodyDef.type = b2_dynamicBody;
	//Set its position in the world
	myBodyDef.position.Set(300, 10);
	//The starting angle of the body
	myBodyDef.angle = 0;
	//Add this body to the world
	b2Body* dynamicBody1 = m_world->CreateBody(&myBodyDef);

	//Create some vertices to make a shape, in this case it makes a 5 sided polygon
	b2Vec2 vertices[5];
    vertices[0].Set(-10, 50);
    vertices[1].Set(-10,  0);
    vertices[2].Set( 0, -30);
    vertices[3].Set( 10,  0);
    vertices[4].Set( 10,  50);

	//Create a new polygon shape to store these vertices and turn them into a shape
	b2PolygonShape polyShape;
	polyShape.Set(vertices, 5);

	//Create a new fixture where you can store the shape
	b2FixtureDef polyFixtureDef;
	//Set the shape of the fixture to contain the polygon shape
	polyFixtureDef.shape = &polyShape;
	//Set the weight of the object. Note: having no weight means it wont react properly with objects!
	polyFixtureDef.density = 1;
	//Add this fixture to the body we made previously
	dynamicBody1->CreateFixture(&polyFixtureDef);

	//You can use the same definition of a body again, but make sure when you add it to the world it is something different!
	myBodyDef.position.Set(430, 120);
	//Add it to the game world
	b2Body* dynamicBody2 = m_world->CreateBody(&myBodyDef);

	//A reversed set of vertices than before
    vertices[0].Set(10, -20);
    vertices[1].Set(10, 0);
    vertices[2].Set(0, 30);
    vertices[3].Set(-10, 0);
    vertices[4].Set(-10, -10);

	//Set the poly shape to contain these new vertices
	polyShape.Set(vertices, 5);
	//Add the fixture to the body
	dynamicBody2->CreateFixture(&polyFixtureDef);

	//Set the body definition to static this time, meaning that it cant be moved around in the game world
	myBodyDef.type = b2_staticBody;
	//Set its position
	myBodyDef.position.Set(300,500);
	//Add it to the world
	b2Body* staticBody = m_world->CreateBody(&myBodyDef);

	//This is vertices to create a line along the bottom which infact is just an elongated rectangle
	b2Vec2 lineV[4];
	lineV[0].Set(-150, 1);
	lineV[1].Set(-150, -1);
	lineV[2].Set(150, -1);
	lineV[3].Set(150, 1);

	//Set the shape to hold the line
	polyShape.Set(lineV, 4);
	//Add the line to the gameworld
	staticBody->CreateFixture(&polyFixtureDef);

	//circle
	float radius = 50.0f;
	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Red);
	circle.setRadius(radius);

	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(200, -20); //startpos
	b2Body* dynamicBody3 = m_world->CreateBody(&myBodyDef);

	b2FixtureDef circleDef;
	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = 50;
	dynamicCircle.m_p.Set(0,0);
	circleDef.shape = &dynamicCircle;
	circleDef.restitution = 0.5f;
	circleDef.density = 1.0f;
	circleDef.friction = 0.8f;
	dynamicBody3->CreateFixture(&circleDef);

	//texture

	sf::Texture ballTexture;
	ballTexture.loadFromFile("ball.png");
	sf::Sprite ballSprite;
	ballSprite.setTexture(ballTexture);
	ballSprite.setPosition(sf::Vector2f(10,10));
	b2PolygonShape ballShape;
	b2FixtureDef ballFixtureDef;
	ballFixtureDef.shape = &ballShape;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(300, 300);
	b2Body* dynamicBody4 = m_world->CreateBody(&myBodyDef);
	dynamicBody4->CreateFixture(&ballFixtureDef);
	


	//This loops through the window, so while the window is open
	while (window.isOpen())
	{
		//Creates a new event which handles what events are happening to the window
		sf::Event event;
		//So while there is an event
		while (window.pollEvent(event))
		{
			//Check to see if someone has closed the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Clear the window every frame to black
		window.clear(sf::Color::Black);
		//Step through the Box2D world, if we dont do this, box2D would not perform any physics calculations
		//If the world is running to fast or too slow try changing the 500.0f, lower if running to slow or higher if going to fast
		m_world->Step((1.0f / 200.0f), 10, 8);

		//Create a new size object, this is used for drawing the shapes in SFML
		size_t size = 0;
		b2Vec2 v;

		/*Ok so now this code below gets every object that is currently in Box2D and sends it all to SFML to be drawn
		this is so we can actually get the shapes drawn on the screen as Box2D only has very basic draw functions*/

		//This loops through every single body in the game world
		for(b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
		{
			//This loops through every fixture in the current body
			for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				//This checks to see if the type of the fixture is a polygon, if it is then draw the polygon
				if(f->GetType() == b2Shape::e_polygon)
				{
					//Stores a pointer to the shape stored in the fixture
					b2PolygonShape* s = (b2PolygonShape*)f->GetShape();

					//Get the amount of vertices stored in the shape
					size = s->GetVertexCount();
					//Set the size of the object in SFML so it knows how many vertices the shape should have
					cShape.setPointCount(size);
					

					//Loop through the vertices and send them from Box2D to the SFML shape
					for(int i = 0; i < size; i++)
					{
						//Stores the current vertex in v
						v = s->GetVertex(i);
						//Converts the vertex from its local space to where it is in the world
						cShape.setPoint(i, sf::Vector2f(b->GetWorldVector(v).x + b->GetPosition().x, b->GetWorldVector(v).y + b->GetPosition().y));
					
					}

					//Draws the shape onto the window
					window.draw(cShape);

				} else if (f->GetType() == b2CircleShape::e_circle) {
					//b2CircleShape* c = (b2CircleShape*)f->GetShape();
					//size = c->GetVertexCount();
					//circle.setPointCount();
					//for(int i = 0; i < size; i++) {
					//	v = c ->GetVertex(i);
					//	circle.setRadius(radius);
					//}
					b2PolygonShape* s = (b2PolygonShape*) f->GetShape();
					circle.setRadius(s->m_radius);
					circle.setPosition(f->GetBody()->GetPosition().x - circle.getRadius(), f->GetBody()->GetPosition().y - circle.getRadius());
					window.draw(circle);

										static sf::Sprite bSprite;

					bSprite.setScale(0.2f,0.2f);
					bSprite.setPosition (0, 0);
					bSprite.setTexture(ballTexture);
					bSprite.setOrigin(150,150);
					bSprite.setScale((s->m_radius*0.01)/1.5,(s->m_radius*0.01)/1.5);
					
					bSprite.setPosition (f->GetBody()->GetPosition().x, f->GetBody()->GetPosition().y);
					window.draw(bSprite);
				}

				/*TODO
				Add functions to allow lines, boxes and circles to be drawn
				*/
			}
		}

		//Displays the window on screen
		window.display();
	}

	return 0;
}