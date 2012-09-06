#ifndef _Application_H_
#define _Application_H_

// Include the OgreBullet interface which includes Ogre itself
#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"

#include "World.h"

#include "ExampleApplication.h"

#include <vector>

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    using namespace OIS;
#endif //OGRE_VERSION eihort


class Application;


/*
The test application, based on the Ogre example application for consistency
*/
class Application: public ExampleApplication,  public FrameListener
{
public:
	// Standard constructor/destructor
    Application(std::vector <World *> *worlds);
    ~Application();

    std::vector <World *> *getScenesList(){return mWorlds;};

protected:
	// Override stuff from the base class
    void createScene(void){};
    void chooseSceneManager(void){};

    void createFrameListener(void);
	// Add the standard resources, plus our own pack
    void setupResources(void);
	void loadResources(void);

    bool frameStarted(const FrameEvent& evt);
    bool frameEnded(const FrameEvent& evt);

    bool switchListener(World *newListener);

protected:
    World *mWorld;
    std::vector <World *> *mWorlds;

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    Ogre::InputReader       *mInput;
    Ogre::EventProcessor    *mInputSystem;

#else
    OIS::Keyboard       *mInput;
    OIS::Mouse          *mMouse;
    OIS::InputManager   *mInputSystem;

#endif

};

#endif //_OgreBulletApplication_H_

