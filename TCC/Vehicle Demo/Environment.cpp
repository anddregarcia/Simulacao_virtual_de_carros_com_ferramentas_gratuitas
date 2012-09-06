
#include "Environment.h"
//#include "Vehicle.h"

#include "InputListener.h"

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
using namespace OIS;
#endif

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

static const Ogre::Vector3    CameraStart = Ogre::Vector3(0, 25, 0);
// -------------------------------------------------------------------------

static const Ogre::Vector3   CarPosition = Ogre::Vector3(15, 3, -15);

#define CUBE_HALF_EXTENTS 1

// -------------------------------------------------------------------------
void Environment::init(Ogre::Root *root, Ogre::RenderWindow *win, Application *application)
{
    // Start OgreScene
    mSceneMgr = root->createSceneManager(ST_GENERIC);

    mCamera = mSceneMgr->createCamera("Cam");
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(10000);
    Viewport *vp = win->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(1,1,1));

    mCamera->setPosition(CameraStart);
    mCamera->lookAt(CarPosition);


    World::init(root, win, application);

    // ------------------------
    // add lights
    setBasicLight();

    // ------------------------
    // Start Bullet
    initWorld();

    // ------------------------
    // Add the ground
    addStaticPlane(0.3, 2.8);


    // create obstacle in front of car
    addCube("obstacle", Ogre::Vector3(13,  -0.25, -5),  Quaternion(Radian(Degree(22.5)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(15,  -0.25, -5),  Quaternion(Radian(Degree(22.5)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(17,  -0.25, -5),  Quaternion(Radian(Degree(22.5)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);

    addCube("obstacle", Ogre::Vector3(13,  -0.25, -10), Quaternion(Radian(Degree(-22.5)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(15,  -0.25, -10), Quaternion(Radian(Degree(-22.5)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(17,  -0.25, -10), Quaternion(Radian(Degree(-22.5)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);

    //pilha
    addCube("obstacle", Ogre::Vector3(13,  1, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(15,  1, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(17,  1, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);

    addCube("obstacle", Ogre::Vector3(13,  3, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(15,  3, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(17,  3, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);

    addCube("obstacle", Ogre::Vector3(13,  5, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(15,  5, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(17,  5, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);

    addCube("obstacle", Ogre::Vector3(13,  7, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(15,  7, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(17,  7, 0),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);


    // create obstacle a bit aside
    addCube("obstacle", Ogre::Vector3(-2,  0, -5),  Quaternion(Radian(Degree(45.0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(0,  0, -5),   Quaternion(Radian(Degree(45.0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(2,  0, -5),   Quaternion(Radian(Degree(45.0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);

    addCube("obstacle", Ogre::Vector3(-2,  0, -10), Quaternion(Radian(Degree(-45.0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(0,  0, -10),  Quaternion(Radian(Degree(-45.0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(2,  0, -10),  Quaternion(Radian(Degree(-45.0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);

    // create obstacle just for fun
    addCube("obstacle", Ogre::Vector3(25, 0, -25), Quaternion(Radian(Degree(45.0)), Ogre::Vector3::UNIT_Z), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(25, 0, -27), Quaternion(Radian(Degree(45.0)), Ogre::Vector3::UNIT_Z), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);
    addCube("obstacle", Ogre::Vector3(25, 0, -29), Quaternion(Radian(Degree(45.0)), Ogre::Vector3::UNIT_Z), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 0);


    //pilha
    addCube("obstacle", Ogre::Vector3(12,  1, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(14,  1, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(16,  1, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);

    addCube("obstacle", Ogre::Vector3(12,  3, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(14,  3, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(16,  3, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);

    addCube("obstacle", Ogre::Vector3(12,  5, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(14,  5, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(16,  5, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);

    addCube("obstacle", Ogre::Vector3(12,  7, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(14,  7, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);
    addCube("obstacle", Ogre::Vector3(16,  7, -30),  Quaternion(Radian(Degree(0)), Ogre::Vector3::UNIT_X), Ogre::Vector3(1, 1, 1), 0.3, 0.8, 30);

//    PRECISO CONSTRUIR O CARRO AQUI.....

    car = new Vehicle();
    car->init(this);


    }

Environment::~Environment(){

    if (car!=NULL)
       delete car;
    };


void Environment::keyPressed(BULLET_KEY_CODE key) {
    car->keyPressed(key);
}

void Environment::keyReleased(BULLET_KEY_CODE key){
    car->keyReleased(key);
}

bool Environment::frameStarted(Ogre::Real elapsedTime){
    return car->frameStarted(elapsedTime);
}




