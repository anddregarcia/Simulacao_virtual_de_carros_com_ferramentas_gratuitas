#ifndef _OGREBULLET_Environment__H
#define _OGREBULLET_Environment__H
class Vehicle;
#include "Vehicle.h"
#include "World.h"

#include "InputListener.h"

// -------------------------------------------------------------------------
class Environment : public World
{
public:

Environment(): World(), car(NULL)
    {
        mName = "Vehicle Test Scene";
    };
    virtual ~Environment();

    Ogre::SceneManager * getSceneMgr(){

    return mSceneMgr;

    };

    OgreBulletDynamics::DynamicsWorld * getmWorld(){

    return mWorld;
    };


    void init(Ogre::Root *root, Ogre::RenderWindow *win, Application *application);


    void keyPressed(BULLET_KEY_CODE key);
    void keyReleased(BULLET_KEY_CODE key);
    bool frameStarted(Ogre::Real elapsedTime);


  private:

  Vehicle* car;

};


#endif
