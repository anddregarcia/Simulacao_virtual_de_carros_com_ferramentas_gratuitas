
#include "Vehicle.h"

#include <iostream>

#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsCompoundShape.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "Constraints/OgreBulletDynamicsRaycastVehicle.h"

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
using namespace OIS;
#endif

using namespace std;

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

static float	gMaxEngineForce = 3000.f;

static float	gSteeringIncrement = 0.04f;
static float	gSteeringClamp = 0.8f;

static float	gWheelRadius = 0.5f;
static float	gWheelWidth = 0.4f;

static float	gWheelFriction = 1e30f;//1000;//1e30f;
static float	gSuspensionStiffness = 20.f;
static float	gSuspensionDamping = 2.3f;
static float	gSuspensionCompression = 4.4f;

static float	gRollInfluence = 0.1f;//1.0f;
static float   gSuspensionRestLength = 0.6;
static float   gMaxSuspensionTravelCm = 500.0;
static float   gFrictionSlip = 10.5;

static const Ogre::Vector3    CameraStart = Ogre::Vector3(0, 25, 0);
// -------------------------------------------------------------------------

static const Ogre::Vector3   CarPosition = Ogre::Vector3(15, 3, -15);

#define CUBE_HALF_EXTENTS 1


// -------------------------------------------------------------------------
void Vehicle::init(Environment* env)
{

    this->env = env;
    mSceneMgr = env->getSceneMgr();
    mWorld = env->getmWorld();


    mWheelsEngineCount = 2;
    mWheelsEngine[0] = 0;
    mWheelsEngine[1] = 1;
    mWheelsEngine[2] = 2;
    mWheelsEngine[3] = 3;

    mWheelsSteerableCount = 2;
    mWheelsSteerable[0] = 0;
    mWheelsSteerable[1] = 1;


    mWheelEngineStyle = 0;
    mWheelSteeringStyle = 0;

    mSteeringLeft = false;
    mSteeringRight = false;

    mEngineForce = 0;
    mSteering = 0;



    /// create vehicle
	{
		const Ogre::Vector3 chassisShift(0, 1.0, 0);
		float connectionHeight = 0.7f;

        mChassis = mSceneMgr->createEntity(
                "chassis" + StringConverter::toString(mNumEntitiesInstanced++),
                "chassis.mesh");

        SceneNode *node = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();

        SceneNode *chassisnode = node->createChildSceneNode ();
        chassisnode->attachObject (mChassis);
        chassisnode->setPosition (chassisShift);


//        mChassis->setQueryFlags (GEOMETRY_QUERY_MASK);
#if (OGRE_VERSION < ((1 << 16) | (5 << 8) | 0)) // only applicable before shoggoth (1.5.0)
        mChassis->setNormaliseNormals(true);
#endif
		mChassis->setCastShadows(true);

        BoxCollisionShape* chassisShape = new BoxCollisionShape(Ogre::Vector3(1.f,0.75f,2.1f));
        CompoundCollisionShape* compound = new CompoundCollisionShape();
        compound->addChildShape(chassisShape, chassisShift);

        mCarChassis = new WheeledRigidBody("carChassis", mWorld);

        //valor de 800 faz referencia a massa do chassis (OgrebulletDynamicsRigidBody.h) 03/09/2011
        mCarChassis->setShape (node, compound, 0.6, 0.6, 800, CarPosition, Quaternion::IDENTITY);
        mCarChassis->setDamping(0.2, 0.2);

        mCarChassis->disableDeactivation ();
        mTuning = new VehicleTuning(
            gSuspensionStiffness,
            gSuspensionCompression,
            gSuspensionDamping,
            gMaxSuspensionTravelCm,
            gFrictionSlip);

        mVehicleRayCaster = new VehicleRayCaster(mWorld);
        mVehicle = new RaycastVehicle(mCarChassis, mTuning, mVehicleRayCaster);

        {
            int rightIndex = 0;
            int upIndex = 1;
            int forwardIndex = 2;

            mVehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

            Ogre::Vector3 wheelDirectionCS0(0,-1,0);
            Ogre::Vector3 wheelAxleCS(-1,0,0);

            for (size_t i = 0; i < 4; i++)
            {
                mWheels[i] = mSceneMgr->createEntity(
                    "wheel" + StringConverter::toString(mNumEntitiesInstanced++),
                    "wheel.mesh");

//                mWheels[i]->setQueryFlags (GEOMETRY_QUERY_MASK);
#if (OGRE_VERSION < ((1 << 16) | (5 << 8) | 0)) // only applicable before shoggoth (1.5.0)
                mWheels[i]->setNormaliseNormals(true);
#endif
				mWheels[i]->setCastShadows(true);

                mWheelNodes[i] = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();
                mWheelNodes[i]->attachObject (mWheels[i]);

            }

            {


                bool isFrontWheel = true;

                Ogre::Vector3 connectionPointCS0 (
                    CUBE_HALF_EXTENTS-(0.3*gWheelWidth),
                    connectionHeight,
                    2*CUBE_HALF_EXTENTS-gWheelRadius);


                mVehicle->addWheel(
                    mWheelNodes[0],
                    connectionPointCS0,
                    wheelDirectionCS0,
                    wheelAxleCS,
                    gSuspensionRestLength,
                    gWheelRadius,
                    isFrontWheel, gWheelFriction, gRollInfluence);

                connectionPointCS0 = Ogre::Vector3(
                    -CUBE_HALF_EXTENTS+(0.3*gWheelWidth),
                    connectionHeight,
                    2*CUBE_HALF_EXTENTS-gWheelRadius);


                mVehicle->addWheel(
                    mWheelNodes[1],
                    connectionPointCS0,
                    wheelDirectionCS0,
                    wheelAxleCS,
                    gSuspensionRestLength,
                    gWheelRadius,
                    isFrontWheel, gWheelFriction, gRollInfluence);


                connectionPointCS0 = Ogre::Vector3(
                    -CUBE_HALF_EXTENTS+(0.3*gWheelWidth),
                    connectionHeight,
                    -2*CUBE_HALF_EXTENTS+gWheelRadius);

                isFrontWheel = false;
                mVehicle->addWheel(
                    mWheelNodes[2],
                    connectionPointCS0,
                    wheelDirectionCS0,
                    wheelAxleCS,
                    gSuspensionRestLength,
                    gWheelRadius,
                    isFrontWheel, gWheelFriction, gRollInfluence);

                connectionPointCS0 = Ogre::Vector3(
                    CUBE_HALF_EXTENTS-(0.3*gWheelWidth),
                    connectionHeight,
                    -2*CUBE_HALF_EXTENTS+gWheelRadius);

                mVehicle->addWheel(
                    mWheelNodes[3],
                    connectionPointCS0,
                    wheelDirectionCS0,
                    wheelAxleCS,
                    gSuspensionRestLength,
                    gWheelRadius,
                    isFrontWheel, gWheelFriction, gRollInfluence);


            }

        }
    }



}
// -------------------------------------------------------------------------
void Vehicle::keyPressed(BULLET_KEY_CODE key)
{

    switch(key)
    {


    case KC_LEFT:
        mSteeringLeft = true;
        break;
    case KC_RIGHT:
        mSteeringRight = true;
        break;
    case KC_DOWN:
        mEngineForce = -gMaxEngineForce;
        break;
    case KC_UP:
        mEngineForce = gMaxEngineForce;
        break;

    default:
        break;

    }


    return env->World::keyPressed (key);
}
// -------------------------------------------------------------------------
void Vehicle::keyReleased(BULLET_KEY_CODE key)
{
    switch(key)
    {

    case KC_LEFT:
        mSteering = 0;
        mSteeringLeft = false;
        break;
    case KC_RIGHT:
        mSteering = 0;
        mSteeringRight = false;
        break;
    case KC_DOWN:
        mEngineForce = 0;
        break;
    case KC_UP:
        mEngineForce = 0;
        break;
    default:
        break;

    }
    return env->World::keyReleased (key);
}
// -------------------------------------------------------------------------
bool Vehicle::frameStarted(Real elapsedTime)
{

    // apply engine Force on relevant wheels
    for (int i = mWheelsEngine[0]; i < mWheelsEngineCount; i++)
    {
        mVehicle->applyEngineForce (mEngineForce, mWheelsEngine[i]);
    }

    if (mSteeringLeft)
    {
        mSteering += gSteeringIncrement;
        if (mSteering > gSteeringClamp)
            mSteering = gSteeringClamp;
    }
    else if (mSteeringRight)
    {
        mSteering -= gSteeringIncrement;
        if (mSteering < -gSteeringClamp)
            mSteering = -gSteeringClamp;
    }

    // apply Steering on relevant wheels
    for (int i = mWheelsSteerable[0]; i < mWheelsSteerableCount; i++)
    {
        if (i < 2)
            mVehicle->setSteeringValue (mSteering, mWheelsSteerable[i]);
        else
            mVehicle->setSteeringValue (-mSteering, mWheelsSteerable[i]);
    }
    return env->World::frameStarted(elapsedTime);
}
// -------------------------------------------------------------------------
