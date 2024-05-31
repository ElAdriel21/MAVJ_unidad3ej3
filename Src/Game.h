#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World* phyWorld;
	SFMLRenderer* debugRender;

	// Tiempo de frame
	float frameTime;
	int fps;

	b2MouseJoint* mouseJoint = nullptr; 
	b2Body* groundBody; 

public:
	//Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void CheckCollitions();
	~Game(void);
	void InitPhysics();

	b2Vec2 Game::SFMLToBox2D(sf::Vector2f sfmlVec);

	void StartMouseJoint(b2Vec2 target);
	void EndMouseJoint();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};