
#ifndef _World_H_
#define _World_H_

#include "OgreBulletDynamics.h"

#include "InputListener.h"


class Application;

/*
The base Test class, is also able to listen for collisions and thus change the contact properties
*/

class World
{
public:

	// Constructor/destructor
    World();
    virtual ~World(){};
    virtual void init(Ogre::Root *root, Ogre::RenderWindow *win, Application *application);

    virtual void shutdown();

    void setBasicLight();

    const BULLET_KEY_CODE getNextKey() const {return mActivationKeyCode;};
    void setNextKey(BULLET_KEY_CODE code){mActivationKeyCode = code;};


    const Ogre::String getName() {return mName;}

    virtual bool frameStarted(Ogre::Real elapsedTime);
    virtual bool frameEnded(Ogre::Real elapsedTime);

    InputListener *getInputListener(){return mInputListener;}

    virtual void mouseMoved();

    virtual void keyPressed(BULLET_KEY_CODE key);
    virtual void keyReleased(BULLET_KEY_CODE key);

    bool *getBoolActivator(){return &mActivationBool;}


protected:
    //Declarando o valor da gravidade
//    void initWorld (const Ogre::Vector3 &gravityVector = Ogre::Vector3 (0,-9.81,0),
//                    const Ogre::AxisAlignedBox &bounds = Ogre::AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),
//                                                                               Ogre::Vector3 (10000,  10000,  10000)));

    void initWorld (const Ogre::Vector3 &gravityVector = Ogre::Vector3 (0,-9.81,0),
                    const Ogre::AxisAlignedBox &bounds = Ogre::AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),
                                                                               Ogre::Vector3 (10000,  10000,  10000)));

    void addGround();
    //função de chamada dos objetos
    OgreBulletDynamics::RigidBody *addCube(const Ogre::String instanceName,
        const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
        const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction,
        const Ogre::Real bodyMass);


    OgreBulletDynamics::RigidBody *addStaticPlane( const Ogre::Real bodyRestitution,
                                                   const Ogre::Real bodyFriction);

protected:
    static size_t mNumEntitiesInstanced;

    Ogre::RenderWindow      *mWindow;
    Ogre::Root              *mRoot;
    Ogre::SceneManager      *mSceneMgr;

    int mCurrentShadowTechnique;
    Ogre::Light *mSunLight;
	Ogre::Light *mLight;

    Ogre::Camera            *mCamera;
    Ogre::Radian             mCameraRotX;
    Ogre::Radian             mCameraRotY;
    float                    mCameraMove;
    Ogre::Vector3            mCameraTrans;

    OgreBulletDynamics::DynamicsWorld *mWorld;
    Application *mApplication;


    std::deque<Ogre::Entity *>                          mEntities;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

    bool                    mStatsOn;
    bool                    mQuit;
    bool                    mDoOnestep;

    float                    mShootSpeed;
    float                    mImpulseForce;
    bool                     mPaused;


    bool                    mWireFrame;


   OgreBulletCollisions::CollisionClosestRayResultCallback  *mCollisionClosestRayResultCallback;

   OgreBulletCollisions::DebugLines    *mDebugRayLine;
   Ogre::RaySceneQuery                 *mRayQuery;

   BULLET_KEY_CODE mActivationKeyCode;
   bool mActivationBool;

   InputListener *mInputListener;


   Ogre::String mName;

};

#endif

