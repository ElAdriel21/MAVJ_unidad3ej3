#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física
}

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point) : m_point(point), m_fixture(nullptr) {}

	bool ReportFixture(b2Fixture* fixture) override
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;
				return false; // Termina la búsqueda
			}
		}
		return true; // Continua buscando
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};

void Game::Loop()
{
	while (wnd->isOpen()) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
}


void Game::DrawGame()
{
	// Función para dibujar los elementos del juego (a implementar)
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt)) // Procesa todos los eventos acumulados
	{
		switch (evt.type)
		{
		case Event::Closed: // Si se solicita cerrar la ventana
			wnd->close(); // Cierra la ventana
			break;
		case Event::MouseButtonPressed:
			if (evt.mouseButton.button == Mouse::Left)
			{
				b2Vec2 worldPos = SFMLToBox2D(wnd->mapPixelToCoords(Mouse::getPosition(*wnd)));
				StartMouseJoint(worldPos);
			}
			break;
		}
	}
}

b2Vec2 Game::SFMLToBox2D(sf::Vector2f sfmlVec)
{
	return b2Vec2(sfmlVec.x, sfmlVec.y);
}

void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::InitPhysics()
{
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea un cuerpo estático para el suelo
	groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	// Crea cuerpos estáticos para las paredes
	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Crea un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	b2Body* circuloA = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloA->SetTransform(b2Vec2(50.0f, 40.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, circuloA, circuloA->GetWorldCenter(),
		topWallBody, topWallBody->GetWorldCenter(), 10.0f, 0.1f, 1.0f);
}

void Game::StartMouseJoint(b2Vec2 target)
{
	if (mouseJoint != nullptr) return;

	b2AABB aabb;
	b2Vec2 d(0.001f, 0.001f);
	aabb.lowerBound = target - d;
	aabb.upperBound = target + d;

	QueryCallback callback(target);
	phyWorld->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef mjd;
		mjd.bodyA = groundBody; 
		mjd.bodyB = body;
		mjd.target = target;
		mjd.maxForce = 50000.0f * body->GetMass();

		mouseJoint = (b2MouseJoint*)phyWorld->CreateJoint(&mjd);
		body->SetAwake(true);
	}
}

void Game::EndMouseJoint()
{
	if (mouseJoint)
	{
		phyWorld->DestroyJoint(mouseJoint);
		mouseJoint = nullptr;
	}
}

Game::~Game(void)
{
	// Destructor de la clase Game (a implementar si es necesario)
}