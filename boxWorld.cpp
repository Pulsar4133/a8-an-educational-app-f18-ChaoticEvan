#include "boxWorld.h"
#include "Box2D/Box2D.h"

boxWorld::boxWorld()
{
    b2Vec2 gravity(0, -9.8f);
    b2World lemonadeWorld(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -9.8f);

    b2Body* groundBody = lemonadeWorld.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 5.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define lemon body definition
    b2BodyDef lemonBodyDef;
    lemonBodyDef.type = b2_dynamicBody;
    lemonBodyDef.position.Set(20.0f, 5.0f);
    lemonBodyDef.active = true;
    b2Body* lemon = lemonadeWorld.CreateBody(&lemonBodyDef);

    // Define polygon shape of lemon
    b2PolygonShape lemonDBox;
    lemonDBox.SetAsBox(1.0f, 1.0f);

    // Define fixture for lemon
    b2FixtureDef lemonFixDef;
    lemonFixDef.shape = &lemonDBox;

    // Set the box density to be non-zero, so it will be dynamic.
    lemonFixDef.density = 1.0f;

    // Override the default friction.
    lemonFixDef.friction = 0.3f;
    lemonFixDef.restitution = 0.9f;

    // Add the shape to the body.
    lemon->CreateFixture(&lemonFixDef);



    //Not sure if any below this is necessary




    b2BodyDef sugarBodyDef;
    sugarBodyDef.type = b2_dynamicBody;
    sugarBodyDef.position.Set(30.0f, 5.0f);
    sugarBodyDef.active = true;
    b2Body* sugar = lemonadeWorld.CreateBody(&sugarBodyDef);

    b2BodyDef iceBodyDef;
    iceBodyDef.type = b2_dynamicBody;
    iceBodyDef.position.Set(40.0f, 5.0f);
    iceBodyDef.active = true;
    b2Body* ice = lemonadeWorld.CreateBody(&iceBodyDef);

    b2DynamicTree pitcherRegion;
    const b2Vec2 pitcherLowerBound(0.0f, 5.0f);
    const b2Vec2 pitcherUpperBound(10.0f, 15.0f);
    b2BodyDef pitcherBodyDef;
    pitcherBodyDef.type = b2_staticBody;
    b2Body* pitcher = lemonadeWorld.CreateBody(&pitcherBodyDef);
    const b2AABB pitcherAABB;
//    pitcherAABB.lowerBound = pitcherLowerBound;
//    pitcherAABB.upperBound = pitcherUpperBound;
    pitcherRegion.CreateProxy(pitcherAABB, pitcher);
}
