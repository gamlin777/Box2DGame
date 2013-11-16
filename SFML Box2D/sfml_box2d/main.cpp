/*
JGamlin Box2d Game

Note: Object ID's are as follows:
	Ship: 5
	n_ball: 6 (balls)
	debris: 3
	bottom court: 2
	top court: 1
	right court: 7
	left court: 8


*/
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#include <ctime>
#include <cstdlib>


b2World* m_world;

struct objectData {
	int id;
	int ballId;
	int bounceCount;
	bool bomb;
};
	
bool startBalls = true;
int numBalls = 0;
void newBall(float x, float y);
b2Vec2 myRand (int strength);
int lives = 3;



class MyListener : public b2ContactListener
{
	void BeginContact(b2Contact *contact)  {
		b2Fixture* GetFixtureA();
		b2Fixture* GetFixtureB();
		objectData* contactA = (objectData*)contact->GetFixtureA()->GetBody()->GetUserData();
		objectData* contactB = (objectData*)contact->GetFixtureB()->GetBody()->GetUserData();
		b2Vec2 splitterA = contact->GetFixtureA()->GetBody()->GetPosition();
		b2Vec2 splitterB = contact->GetFixtureB()->GetBody()->GetPosition();

		//contact a & b's type id
 		int aa = contactA->id;
		int bb = contactB->id;
		
		//contact a & b's bounce count (used for balls only)
		int ac = contactA->bounceCount;
		int bc = contactB->bounceCount;

		if(bb == 5 ){ // ship
			if ( aa == 6){ // ball
				contactA->bomb = true;
				printf("contact A is a bomb\n");
			}
		}
		if(bb == 6){
			if(aa == 5){
				contactB->bomb = true;
				printf("contact B is a bomb\n");
			}
		}
		//ball to court collision tests & ball spawn
		if(bb == 6){ 
			if(aa == 2){ //bounces on bottom court
				contactB->bounceCount += 1;
				if(bc >= 6){
					if(splitterB.x > 300.0f) {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x - 20.0f, splitterB.y);
					} else {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x + 20.0f, splitterB.y);
					}
				}
			}
			if(aa == 1){ //bounces on top court
				contactB->bounceCount += 1;
				if(bc >= 6){
					if(splitterB.x > 300.0f) {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x - 20.0f, splitterB.y);
					} else {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x + 20.0f, splitterB.y);
					}
				}
			}
			if(aa == 7){ // bounces on right court
				contactB->bounceCount += 1;
				if(bc >= 6){
					if(splitterB.y > 300.0f) {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x, splitterB.y - 20.0f);
					} else {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x, splitterB.y + 20.0f);
					}
				}
			}
			if(aa == 8){ // bounces on left
				contactB->bounceCount += 1;
				if(bc >= 6){
					if(splitterB.y > 300.0f) {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x, splitterB.y - 20.0f);
					} else {
						contactB->bounceCount = 0;
						m_world->Step((1.0f / 200.0f), 10, 8);
						newBall(splitterB.x, splitterB.y + 20.0f);
					}
				}
			}
		} 
	}
};

int main()
{
	srand((time(NULL)));
	//Creates a window using SFML
	sf::RenderWindow window(sf::VideoMode(600, 600, 32), "J Gamlin Box2D", sf::Style::Default);
	
	//b2d world
	b2Vec2 gravity(0, 0);
	m_world = new b2World(gravity);
	MyListener listener;
	m_world->SetContactListener(&listener);

	//struct data for object type ID's
	objectData* ship = new objectData;
	ship->id = 5;

	objectData* bot_court = new objectData;
	bot_court->id = 2;
	objectData* top_court = new objectData;
	top_court->id = 1;
	objectData* right_court = new objectData;
	right_court->id = 7;
	objectData* left_court = new objectData;
	left_court->id = 8;

	objectData* debris = new objectData;
	debris->id = 3;


	//shape setup for obstacles
	//object1 and setup for 2+
	sf::ConvexShape cShape;
	cShape.setFillColor(sf::Color::Color(125,125,100,255));

	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(285, 40);
	myBodyDef.angle = 0;
	b2Body* dynamicBody1 = m_world->CreateBody(&myBodyDef);
	dynamicBody1->SetUserData(debris);

	b2Vec2 vertices[5];
    vertices[0].Set(-10, 50);
    vertices[1].Set(-10,  0);
    vertices[2].Set( 0, -30);
    vertices[3].Set( 10,  0);
    vertices[4].Set( 10,  50);

	b2PolygonShape polyShape;
	polyShape.Set(vertices, 5);

	b2FixtureDef polyFixtureDef;
	polyFixtureDef.shape = &polyShape;
	polyFixtureDef.density = 1;
	polyFixtureDef.restitution = 1.0f;
	dynamicBody1->CreateFixture(&polyFixtureDef);

	//object2
	myBodyDef.position.Set(430, 120);
	b2Body* dynamicBody2 = m_world->CreateBody(&myBodyDef);
	dynamicBody2->SetUserData(debris);

    vertices[0].Set(10, -20);
    vertices[1].Set(10, 0);
    vertices[2].Set(0, 30);
    vertices[3].Set(-10, 0);
    vertices[4].Set(-10, -10);

	polyShape.Set(vertices, 5);
	dynamicBody2->CreateFixture(&polyFixtureDef);

	
	//player ship
	b2BodyDef shipDef;
	shipDef.type = b2_dynamicBody;
	shipDef.position.Set(150,150);
	shipDef.angle = 0.0f;
	b2Body* dynamicShip = m_world->CreateBody(&shipDef);
	dynamicShip->SetUserData(ship);
	
	
	b2Vec2 shipVerts[8];
	shipVerts[0].Set(30,10);
	shipVerts[1].Set(30,20);
	shipVerts[2].Set(20,30);
	shipVerts[3].Set(10,30);
	shipVerts[4].Set(0,20);
	shipVerts[5].Set(0,10);
	shipVerts[6].Set(10,0);
	shipVerts[7].Set(20,0);


	polyShape.Set(shipVerts, 8);
	dynamicShip->CreateFixture(&polyFixtureDef);



	//court bottom & setup
	b2FixtureDef courtDef;
	courtDef.shape = &polyShape;
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(300,600);
	b2Body* staticBody = m_world->CreateBody(&myBodyDef);
	staticBody->SetUserData(bot_court);


	b2Vec2 courtV[4];
	courtV[0].Set(-298, 1);
	courtV[1].Set(-298, -1);
	courtV[2].Set(298, -1);
	courtV[3].Set(298, 1);

	polyShape.Set(courtV, 4);
	staticBody->CreateFixture(&courtDef);
	
	//court top
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(300,0);
	b2Body* staticBody2 = m_world->CreateBody(&myBodyDef);
	polyShape.Set(courtV, 4);
	staticBody2->CreateFixture(&courtDef);
	staticBody2->SetUserData(top_court);

	//court left & setup
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(0,300);
	b2Body* staticBody3 = m_world->CreateBody(&myBodyDef);
	staticBody3->SetUserData(left_court);

	courtV[0].Set(-1, 298);
	courtV[1].Set(-1, -298);
	courtV[2].Set(1, -298);
	courtV[3].Set(1, 298);

	polyShape.Set(courtV, 4);
	staticBody3->CreateFixture(&courtDef);

	//court right
	myBodyDef.type = b2_staticBody;
	myBodyDef.position.Set(600,300);
	b2Body* staticBody4 = m_world->CreateBody(&myBodyDef);
	polyShape.Set(courtV, 4);
	staticBody4->CreateFixture(&courtDef);
	staticBody4->SetUserData(right_court);

	

	//circle shape setup
	b2FixtureDef circleDef;
	b2CircleShape dynamicCircle;


	//textures
	sf::Texture livesTexture;
	livesTexture.loadFromFile("assets/ship.png");
	sf::Sprite livesSprite;
	livesSprite.setTexture(livesTexture);

	sf::Texture ballTexture;
	ballTexture.loadFromFile("assets/ball3.png");
	ballTexture.setSmooth(true);

	sf::Texture bombTexture;
	bombTexture.loadFromFile("assets/ball2.png");
	
	sf::Sprite bombSprite;
	bombTexture.setSmooth(true);
	bombSprite.setTexture(bombTexture);


	sf::Texture shipTexture;
	sf::Texture shipTexture1;
	sf::Texture shipTexture2;


	shipTexture.loadFromFile("assets/ship.png");
	shipTexture1.loadFromFile("assets/shipforward.png");
	shipTexture2.loadFromFile("assets/shipboost.png");
	sf::Sprite shipSprite;
	shipSprite.setTexture(shipTexture);
	ballTexture.setSmooth(true);

	shipSprite.setOrigin(150,150);
	shipSprite.setScale(0.15f,0.15f);
	b2Vec2 pos; //used for finding the ship's origin

	sf::Texture backdropTexture;
	backdropTexture.loadFromFile("assets/backdrop.png");
	sf::Sprite backdrop;
	backdropTexture.setSmooth(true);
	backdrop.setTexture(backdropTexture);
	backdrop.setPosition(300,300);
	backdrop.setOrigin(300,300);
	

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
		
		/*printf("NumBalls: %i\n", numBalls);*/

		//update the ship sprite
		pos = dynamicShip->GetWorldCenter();
		shipSprite.setPosition(pos.x, pos.y);
		shipSprite.setRotation(dynamicShip->GetAngle() * (180.0f / b2_pi));

		//Space bar to start game
		static bool ballbool = true;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && ballbool) {
			newBall(300,300);

			ballbool = false;
		}
		//boost ship
		static bool shiftDown = true;
		static bool wDown = false;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && shiftDown) {
			shipSprite.setTexture(shipTexture2);
			dynamicShip->SetLinearVelocity(b2Vec2(-40.0f * sinf(dynamicShip->GetAngle()),( 40.0f * cosf(dynamicShip->GetAngle()))    ));
			shiftDown = false;
		} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			shiftDown = true;
			if (!wDown){
				shipSprite.setTexture(shipTexture);
			}
		}

		//ship forward
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			wDown = true;
		} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			wDown = false;
			if (shiftDown){
				shipSprite.setTexture(shipTexture);
			}
		}
		if (wDown){
			shipSprite.setTexture(shipTexture1);
			dynamicShip->ApplyLinearImpulse(b2Vec2(-40.0f * sinf(dynamicShip->GetAngle()),( 40.0f * cosf(dynamicShip->GetAngle()))), dynamicShip->GetWorldCenter());
			if (!shiftDown) {
				shipSprite.setTexture(shipTexture2);
			}
		}

		//rotate ship Counter-Clockwise
		static bool aDown = false;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			aDown = true;
		} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			aDown = false;
		}
		if (aDown){
			dynamicShip->SetAngularVelocity(dynamicShip->GetAngularVelocity() - 0.005f);
		}

		//rotate ship Clockwise
		static bool dDown = false;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			dDown = true;
		} else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			dDown = false;
		}
		if (dDown== true){
			dynamicShip->SetAngularVelocity(dynamicShip->GetAngularVelocity() + 0.005f);
		}

		for(int i = 0; i < numBalls; i++){

		}

		//Clear the window every frame to black
		window.clear(sf::Color::Black);
		
							//All draw functions must be after the draw(backdrop)
		window.draw(backdrop);
		
		
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
					window.draw(shipSprite);

				} else if (f->GetType() == b2CircleShape::e_circle) {
					objectData* new_b = new objectData;
					static sf::Sprite bSprite;
					b2PolygonShape* s = (b2PolygonShape*) f->GetShape();
					
					
					bombSprite.setTexture(bombTexture);
					
					bombSprite.setOrigin(50,50);
					bombSprite.setScale((s->m_radius*0.01)*2,(s->m_radius*0.01)*2);
					bombSprite.setPosition (f->GetBody()->GetPosition().x, f->GetBody()->GetPosition().y);
					window.draw(bombSprite);

					bSprite.setTexture(ballTexture);
					bSprite.setOrigin(50,50);
					//bSprite.setScale((s->m_radius*0.01)/1.5,(s->m_radius*0.01)/1.5);
					bSprite.setScale((s->m_radius*0.01)*2,(s->m_radius*0.01)*2);


					
					bSprite.setPosition (f->GetBody()->GetPosition().x, f->GetBody()->GetPosition().y);
					window.draw(bSprite);
				}
			}
		}
		//lives display
		for (int i = 0; i <= lives; i++){		
			livesSprite.setOrigin(0,0);
			livesSprite.setScale(0.07f,0.07f);
					
			livesSprite.setPosition(24.0f*i, 2.0f);
			window.draw(livesSprite);
		}


		//Displays the window on screen
		window.display();

	}

	return 0;
}
void newBall(float x, float y)
{
	b2BodyDef myBodyDef;
	static b2Body* dynamicBodyCircle;
	static b2FixtureDef circleDef;
	static b2CircleShape dynamicCircle;
	objectData* new_b = new objectData;

	
	
	////////////////////////////////////////////////////////////////////////////
	if (startBalls){
		for(int i = 0; i < 8 - 2; ++i) {
			float radius = 50.0f;
	
			new_b = new objectData;
			new_b->bounceCount = 0;
			new_b->id = 6;
			myBodyDef.type = b2_dynamicBody;
			myBodyDef.position.Set((i*100)+50, 20); //startpos

			dynamicCircle.m_radius = 10;
			dynamicCircle.m_p.Set(0,0);
			circleDef.shape = &dynamicCircle;
			circleDef.restitution = 1.0f;
			circleDef.density = 1.0f;
			circleDef.friction = 0.8f;
			dynamicBodyCircle = m_world->CreateBody(&myBodyDef);
			dynamicBodyCircle->CreateFixture(&circleDef);
			dynamicBodyCircle->SetLinearVelocity(myRand(40));
			new_b->ballId = numBalls;
			dynamicBodyCircle->SetUserData(new_b);
			new_b->bomb = false;
			numBalls++;
		}
		startBalls = false;
	}

	///////////////////////////////////////////////////////////////////////////
	new_b = new objectData;
	new_b->id = 6;
	new_b->bounceCount = 0;
	b2Vec2 nBallPos(x,y);
	float radius = 50.0f;
	new_b->bomb = false;
	
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position.Set(nBallPos.x, nBallPos.y); //startpos

	dynamicCircle.m_radius = 10;
	dynamicCircle.m_p.Set(0,0);
	circleDef.shape = &dynamicCircle;
	circleDef.restitution = 1.0f;
	circleDef.density = 1.0f;
	circleDef.friction = 0.8f;
	dynamicBodyCircle = m_world->CreateBody(&myBodyDef); //breaks
	new_b->ballId = numBalls;
	numBalls++;
	dynamicBodyCircle->SetUserData(new_b);
	dynamicBodyCircle->CreateFixture(&circleDef);
	dynamicBodyCircle->SetLinearVelocity(myRand(40));

};

b2Vec2 myRand (int strength) {
	// Create 2 random floats and treat them as a vector
	float rX = rand() % 200;
	rX *= 0.01f;
	--rX;

	float rY = rand() % 200;
	rY *= 0.01f;
	--rY;

	b2Vec2 v(rX,rY);
	v.Normalize();
	v *= strength;

	return v;
};