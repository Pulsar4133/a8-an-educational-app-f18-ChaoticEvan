#include "Box2D/Box2D.h"
#include "boxWorld.h"

boxWorld::boxWorld()
    : world(b2Vec2 (0.0f, 50.0f))
{
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(375.0f, -10.0f);
    groundBodyDef.type = b2_staticBody;

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(375.0f, 5.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 1.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(375.0f, 10.0f);
    body = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.9f;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    lemonImage = new QLabel();
    lemonImage->setFixedSize(100, 100);
    QPixmap lemonPix("C:/Users/spenc/CS3505/lemon image.jpg");
    int w = lemonImage->width();
    int h = lemonImage->height();
    layout = new QHBoxLayout();
    layout->addWidget(lemonImage);
    qw->setLayout(layout);
    lemonImage->setPixmap(lemonPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    body->SetUserData(lemonImage);
}
