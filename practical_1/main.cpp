#include  <SFML/Graphics.hpp>
#include <string> 
//edited with vim again again 
using  namespace  sf;
using  namespace  std;

const  Keyboard::Key  controls[4] = {
	Keyboard::A,    //  Player1  UP
	Keyboard::Z,    //  Player1  Down
	Keyboard::Up,   //  Player2  UP
	Keyboard::Down  //  Player2  Down
};

const  Vector2f  paddleSize(25.f, 100.f);
const  float  ballRadius = 10.f;
const  int  gameWidth = 800;
const  int  gameHeight = 600;
const  float  paddleSpeed = 400.f;

Vector2f  ballVelocity;
bool  server = false;

int scoreLeft = 0;
int scoreRight = 0;

Text text;
Font font;

CircleShape  ball;
RectangleShape  paddles[2];

void Reset() {
	//  reset  paddle  position
	paddles[0].setPosition(10 + paddleSize.x / 2, gameHeight / 2);
	paddles[1].setPosition(790 - paddleSize.x / 2, gameHeight / 2);

	//  reset  Ball  Position
	ball.setPosition(400 - ballRadius, gameHeight / 2);
	ballVelocity = { (server ? 100.0f : -100.0f), 60.0f };

	// Score display
	text.setString(to_string(scoreLeft) + " : " + to_string(scoreRight));
	text.setPosition((gameWidth * 0.5f) - 35, 20);
	//text.setPosition((gameWidth * 0.5f) - (text.getLocalBounds().width * 0.5f), 0);
}

void  Load() {
	// Set  size  and  origin  of  paddles
	for (auto& p : paddles) {
		p.setSize(paddleSize - Vector2f(3, 3));
		p.setOrigin(paddleSize / 2.f);
	}

	// Set  size  and  origin  of ball
	ball.setRadius(ballRadius - 3);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);

	Reset();

	// Font load
	font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");
	text.setFont(font);
	text.setCharacterSize(24);
}

void  Update(RenderWindow& window) {
	//  Reset  clock , recalculate  deltatime
	static  Clock  clock;
	float  dt = clock.restart().asSeconds();

	//  check  and  consume  events
	Event  event;

	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	// Quit  Via  ESC  Key
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

	// Handle  paddle  movement
	float  directionA = 0.0f;
	float  directionB = 0.0f;
	if (Keyboard::isKeyPressed(controls[0]) && (paddles[0].getPosition().y > 0)) {
		directionA--;
	}
	if (Keyboard::isKeyPressed(controls[1]) && (paddles[0].getPosition().y < gameHeight)) {
		directionA++;
	}

	if (Keyboard::isKeyPressed(controls[2]) && (paddles[1].getPosition().y > 0)) {
		directionB--;
	}
	if (Keyboard::isKeyPressed(controls[3]) && (paddles[0].getPosition().y < gameHeight)) {
		directionB++;
	}

	// AI logic
	// if (ball.getPosition().y < paddles[1].getPosition().y)
	// 	directionB--;
	// else if (ball.getPosition().y > paddles[1].getPosition().y)
	// 	directionB++;

	paddles[0].move(0, directionA * paddleSpeed * dt);
	paddles[1].move(0, directionB * paddleSpeed * dt);

	ball.move(ballVelocity * dt);

	//  check  ball  collision
	const  float  bx = ball.getPosition().x;
	const  float  by = ball.getPosition().y;
	if (by > gameHeight) {
		//  bottom  wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(0, -10);
	}
	else if (by < 0) {
		// top  wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(0, 10);
	}
	else if (bx > gameWidth) {
		// right wall
		scoreLeft++;
		Reset();
	}
	else if (bx < 0) {
		// left wall
		scoreRight++;
		Reset();
	}
	else if (bx < paddleSize.x && by > paddles[0].getPosition().y - (paddleSize.y * 0.5) && by < paddles[0].getPosition().y + (paddleSize.y * 0.5)) {
		// left paddle
		ballVelocity.x *= -1;
	}
	else if (bx > gameWidth - paddleSize.x && by > paddles[1].getPosition().y - (paddleSize.y * 0.5) && by < paddles[1].getPosition().y + (paddleSize.y * 0.5)) {
		// right paddle
		ballVelocity.x *= -1;
	}
}

void  Render(RenderWindow& window) {
	// Draw  Everything
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
	window.draw(text);
}

int  main() {
	RenderWindow  window(VideoMode(gameWidth, gameHeight), "PONG");

	Load();

	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}

	return  0;
}
