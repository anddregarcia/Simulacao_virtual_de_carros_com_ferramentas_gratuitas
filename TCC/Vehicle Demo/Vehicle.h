#ifndef _OGREBULLET_Vehicle_H
#define _OGREBULLET_Vehicle_H
class Environment;
#include "OgreBulletDynamics.h"
#include "World.h"
#include "Environment.h"

class InputListener;
#include "InputListener.h"


// -------------------------------------------------------------------------
class Vehicle : public World//, public Environment
{
public:

    Vehicle(){};

    virtual ~Vehicle(){};

    void init(Environment* env);


    void keyPressed(BULLET_KEY_CODE key);
    void keyReleased(BULLET_KEY_CODE key);

    bool frameStarted(Ogre::Real elapsedTime);


private:

    OgreBulletDynamics::WheeledRigidBody        *mCarChassis;
    OgreBulletDynamics::VehicleTuning	        *mTuning;
    OgreBulletDynamics::VehicleRayCaster	    *mVehicleRayCaster;
    OgreBulletDynamics::RaycastVehicle	        *mVehicle;

    Ogre::Entity    *mChassis;
    Ogre::Entity    *mWheels[4];
    Ogre::SceneNode *mWheelNodes[4];


    int mWheelsEngine[4];
    int mWheelsEngineCount;
    int mWheelsSteerable[4];
    int mWheelsSteerableCount;

    float mEngineForce;
    float mSteering;

    int mWheelEngineStyle;
    int mWheelSteeringStyle;


    bool mSteeringLeft;
    bool mSteeringRight;

    Environment *env;

};


#endif //_OGREBULLET_Vehicles_Demo__H
