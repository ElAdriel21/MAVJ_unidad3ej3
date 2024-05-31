#include "Game.h"
#include <tchar.h>

using namespace sf;

class Pelota
{
private:
    // Sprite y su imagen para representación gráfica en el juego
    Sprite* _sprite;
    Texture* _image;
    RenderWindow* wnd;

    // Body para representación física en la simulación Box2D
    b2Body* _body;

public:
    // Constructor que inicializa la pelota con un cuerpo físico y una representación gráfica
    Pelota(b2World* _world, RenderWindow* _wnd)
    {
        // Guardamos puntero a ventana para poder dibujar la pelota más tarde
        wnd = _wnd;
        _image = new Texture();
        // Cargamos la imagen de la pelota desde un archivo
        _image->loadFromFile("Pelota.png");
        // Cargamos el sprite con la imagen cargada
        _sprite = new Sprite(*_image);

        // Definimos las propiedades del cuerpo físico y lo creamos en el mundo Box2D
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody; // El cuerpo es dinámico, puede moverse y ser afectado por fuerzas
        bodyDef.position.Set(100.0f, 0.0f); // Posición inicial del cuerpo
        _body = _world->CreateBody(&bodyDef);

        // Definimos la forma de colisión del cuerpo, en este caso una caja cuadrada
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(20.0f, 20.0f);

        // Creamos el fixture (propiedades físicas) del cuerpo
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox; // Forma de la colisión
        fixtureDef.density = 10.0f; // Densidad
        fixtureDef.friction = 0.3f; // Fricción
        fixtureDef.restitution = 1.0f; // Restitución (rebote)
        _body->CreateFixture(&fixtureDef); // Asignamos el fixture al cuerpo
    }

    // Método para actualizar la posición del sprite basada en la posición del cuerpo físico
    void ActualizarPosiciones()
    {
        b2Vec2 pos = _body->GetPosition();
        _sprite->setPosition(pos.x, pos.y);
    }

    // Método para dibujar el sprite en la ventana de renderizado
    void Dibujar()
    {
        wnd->draw(*_sprite);
    }

    // Otros métodos relacionados al personaje podrían ser implementados aquí
};

int _tmain(int argc, _TCHAR* argv[])
{
    // Creación del juego y bucle principal
    Game* Juego;
    Juego = new Game(800, 600, "Resortes - MAVII");
    Juego->Loop(); // Inicia el bucle principal del juego

    return 0;
}
