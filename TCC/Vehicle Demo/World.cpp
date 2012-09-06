
#include "Application.h"
#include "World.h"
#include "InputListener.h"

//#include "OgreBulletCollisionsShape.h"

//REFERENTE A COLISÃO DO CARRO
#include "Shapes/OgreBulletCollisionsBoxShape.h"

//REFERENTE A COLISÃO DO PLANO (CHÃO)
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"


#include "OgreBulletDynamicsRigidBody.h"

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
using namespace OIS;
#endif //OGRE_VERSION not Eihort

size_t World::mNumEntitiesInstanced = 0;

// -------------------------------------------------------------------------
Ogre::ShadowTechnique convertToShadowTechnique(int i)
{
    switch(i)
    {

    case 1: return SHADOWTYPE_TEXTURE_ADDITIVE;

    default: return SHADOWTYPE_NONE;
    }
}
// ------------------------------------------------------------------------
int convertShadowTechniqueToInt(Ogre::ShadowTechnique i)
{
	switch(i)
	{

	case  SHADOWTYPE_TEXTURE_ADDITIVE: return 1; //ADICIONA A SOMBRA NO CARRO

	default: return 0;
	}
}
// -------------------------------------------------------------------------
const Ogre::ColourValue g_minLightColour(0.2, 0.1, 0.0);
const Ogre::ColourValue g_maxLightColour(0.5, 0.3, 0.1);
// -------------------------------------------------------------------------

const float      gStaticBodyRestitution  = 0.1f;
const float      gStaticBodyFriction     = 0.8f;

const float      gDynamicBodyRestitution = 0.6f;
const float      gDynamicBodyFriction    = 0.6f;
const float      gDynamicBodyMass        = 1.0f;

const Ogre::Vector3    gCubeBodyBounds      = Ogre::Vector3 (1, 1, 1);
const Ogre::Vector3    gCylinderBodyBounds  = Ogre::Vector3 (0.5, 1, 0.5);
const Ogre::Vector3    gConeBodyBounds      = Ogre::Vector3 (1, 1, 1);
const Real       gSphereBodyBounds    = 1.0f;
// -------------------------------------------------------------------------


World::World() :
mCameraMove (0.3f),//velocidade da camera
mCameraTrans (Ogre::Vector3::ZERO),
mCamera(0),
mRoot(0),
mSceneMgr(0),
mWindow(0),
mWorld(0),
mStatsOn (true),
mPaused (false),
mShootSpeed (7.f),
mImpulseForce (10.f),
mDebugRayLine(0),
mRayQuery(0),
mInputListener(0),
//mPickConstraint(0),
mCollisionClosestRayResultCallback(0)
{


}
// -------------------------------------------------------------------------
void World::init(Ogre::Root *root, Ogre::RenderWindow *win, Application *application)
{
    mRoot = root;
    mWindow = win;
    mApplication = application;
    mCameraTrans = Ogre::Vector3::ZERO;

    mInputListener = new InputListener(this, win);

    /******************* CREATE SHADOWS ***************************/
#ifndef _DEBUG
    //REFERENTE A SOMBRA DO CARRO
    mCurrentShadowTechnique = convertShadowTechniqueToInt(SHADOWTYPE_TEXTURE_ADDITIVE);

	mSceneMgr->setShadowColour(ColourValue(10,10,10));


	mSceneMgr->setShadowTechnique(convertToShadowTechnique(mCurrentShadowTechnique));
	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE))
	{
		// In D3D, use a 1024x1024 shadow texture
		mSceneMgr->setShadowTextureSettings(2048, 2);
	}
	else
	{
		// Use 512x512 texture in GL since we can't go higher than the window res
		mSceneMgr->setShadowTextureSettings(512, 2);
	}


#endif // _DEBUG


}
// #####################################################################
//############### CRIANDO LUZ DO AMBIENTE ###########################
//###################################################################3
void World::setBasicLight()
{
    // Set ambient light
    mSceneMgr->setAmbientLight(ColourValue(0.8, 0.8, 0.8));

	// Fixed light, dim
 	mSunLight = mSceneMgr->createLight("Sun");
 	mSunLight->setPosition(0.0, 60, 0.0);
 	mSunLight->setCastShadows(true);
 	mSunLight->setType(Light::LT_POINT);


	// Point light, movable, reddish
	mLight = mSceneMgr->createLight("Spot");
	mLight->setType(Light::LT_SPOTLIGHT);
	mLight->setPosition(100.0, 80.5,-10.0);
	mLight->setSpotlightRange(Degree(30), Degree(50));
	Ogre::Vector3 dir;
	dir = -mLight->getPosition();
	dir.normalise();
	mLight->setDirection(dir);
//	mLight->setDiffuseColour(0.70, 0.70, 0.72);
//	mLight->setSpecularColour(0.1, 0.1, 0.3);
//
//
//	mLight2 = mSceneMgr->createLight("Spot2");
//	mLight2->setType(Light::LT_SPOTLIGHT);
//	mLight2->setPosition(-100.0, 80.5, 10.0);
//	mLight2->setSpotlightRange(Degree(30), Degree(50));
//	dir = -mLight2->getPosition();
//	dir.normalise();
//	mLight2->setDirection(dir);
//	mLight2->setDiffuseColour(0.72, 0.70, 0.70);
//	mLight2->setSpecularColour(0.3, 0.1, 0.1);

}

// -------------------------------------------------------------------------
void World::shutdown ()
{
    delete mInputListener;

    // OgreBullet physic delete
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
    while (mBodies.end() != itBody)
    {
        delete *itBody;
        ++itBody;
    }

    delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    delete mWorld;

    // Ogre delete
    mSceneMgr->destroyQuery (mRayQuery);
    std::deque<Ogre::Entity *>::iterator itEntity = mEntities.begin();
    while (mEntities.end() != itEntity)
    {
        SceneNode *node = static_cast<SceneNode*>((*itEntity)->getParentNode());
        node->detachObject(*itEntity);
        mSceneMgr->destroyEntity(*itEntity);
        assert (node->getParent());
        static_cast <SceneNode *> (node->getParent())->removeAndDestroyChild(node->getName());

        ++itEntity;
    }
    mEntities.clear();
    mSceneMgr->destroyCamera(mCamera->getName ());
    mWindow->removeViewport(0);
    mRoot->destroySceneManager (mSceneMgr);
    delete mDebugRayLine;

    mInputListener = 0;
    mRayQuery = 0;
    mWorld = 0;
    mCamera = 0;
    mSceneMgr = 0;
    mDebugRayLine = 0;

    mBodies.clear();
    mEntities.clear();
    mShapes.clear();

}

//// -----------------PARA MOVIMENTAR A CAMERA NA TELA PARA CIMA OU PARA BAIXO--------------------------------------------------------
void World::mouseMoved()
{

    if (mInputListener->getButton2Pressed())
    {
        mCameraRotX = Degree(-mInputListener->getRelMouseX () * 0.13);
        mCameraRotY = Degree(-mInputListener->getRelMouseY () * 0.13);
    }

}
// -------------------------------------------------------------------------
void World::keyPressed(BULLET_KEY_CODE key)
{
    static int count = 0;
    // Scene Debug Options

    switch(key)
    {
        // Application Utils
    case KC_ESCAPE:
        mQuit = true;
        break;


        // Scene Debug Options

    case KC_T:
        mWireFrame = true;
        break;

        // pause
    case KC_P:
        mPaused = !mPaused;
        break;

        //Camera
    case KC_W:
        mCameraTrans.z -= mCameraMove;
        break;

    case KC_S:
        mCameraTrans.z += mCameraMove;
        break;

    case KC_A:
        mCameraTrans.x -= mCameraMove;
        break;

    case KC_D:
        mCameraTrans.x += mCameraMove;
        break;

    default:
        break;
    }
}
// -------------------------------------------------------------------------
void World::keyReleased(BULLET_KEY_CODE key)
{
    switch(key)
    {
        //Camera
    case KC_W:
        mCameraTrans.z += mCameraMove;
        break;
    case KC_S:
        mCameraTrans.z -= mCameraMove;
        break;
    case KC_A:
        mCameraTrans.x += mCameraMove;
        break;
    case KC_D:
        mCameraTrans.x -= mCameraMove;
        break;

    default:
        break;
    }
}

bool World::frameStarted(Real elapsedTime)
{
    if (mQuit)
        return false;

    if (mInputListener->getButton2Pressed())
    {
        mCamera->yaw(mCameraRotX);
        mCamera->pitch(mCameraRotY);

        mCameraRotX = 0;
        mCameraRotY = 0;
    }

   mCamera->moveRelative(mCameraTrans);

    // update physics
    if (!mPaused || mDoOnestep)
        mWorld->stepSimulation(elapsedTime);

    mDoOnestep = false;

    return true;
}
// -----------------CRIA OS RISCOS VERDES NA TELA DEMOSNTRANDO OS PONTO DE COLISÃO--------------------------------------------------------
bool World::frameEnded(Real elapsedTime)
{
    if (mQuit)
        return false;

    DebugDrawer *debugDrawer = mWorld->getDebugDrawer ();

    // Scene Debug Options
    if (mWireFrame)
    {
        const bool wasWireframeShapes = debugDrawer->doesDrawWireframe();
        debugDrawer->setDrawWireframe(!wasWireframeShapes);
        mWorld->setShowDebugShapes(!wasWireframeShapes);
        mWireFrame = false;
    }


    return true;
}

// -------------------------------------------------------------------------
void World::initWorld(const Ogre::Vector3 &gravityVector,
                                   const Ogre::AxisAlignedBox &bounds)
{
    // Start Bullet
    mWorld = new DynamicsWorld (mSceneMgr, bounds, gravityVector, true, true, 10000);

    // add Debug info display tool
    DebugDrawer *debugDrawer = new DebugDrawer();

    mWorld->setDebugDrawer(debugDrawer);

    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    node->attachObject (static_cast <SimpleRenderable *> (debugDrawer));

}
// -------------------------------------------------------------------------
void World::addGround()
{
    addStaticPlane(
        gStaticBodyRestitution,
        gStaticBodyFriction);
}
// -------------------------------------------------------------------------
RigidBody *World::addCube(const Ogre::String instanceName,
                                       const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
                                       const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction,
                                       const Ogre::Real bodyMass)
{
    Entity *entity = mSceneMgr->createEntity(
        instanceName + StringConverter::toString(mNumEntitiesInstanced),
         "Crate.mesh");



//    entity->setQueryFlags (GEOMETRY_QUERY_MASK);
#if (OGRE_VERSION < ((1 << 16) | (5 << 8) | 0)) // only applicable before shoggoth (1.5.0)
    entity->setNormaliseNormals(true);
#endif
	entity->setCastShadows(true);

    entity->setMaterialName("Bullet/box");

    BoxCollisionShape *sceneCubeShape = new BoxCollisionShape(size);

    RigidBody *defaultBody = new RigidBody(
        "defaultCubeRigid" + StringConverter::toString(mNumEntitiesInstanced),
        mWorld);

    SceneNode *node = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();
    node->attachObject (entity);

    defaultBody->setShape (node,  sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q);

    mEntities.push_back(entity);
    mShapes.push_back(sceneCubeShape);
    mBodies.push_back(defaultBody);
    mNumEntitiesInstanced++;

    return defaultBody;
}

// ------------------TIRAR ESTA FUNÇÃO RETIRA O PLANO E FAZ O CARRO CAIR EM ABISMO-------------------------------------------------------
RigidBody *World::addStaticPlane( const Ogre::Real bodyRestitution,
                                              const Ogre::Real bodyFriction)
{
    // Use a load of meshes to represent the floor
    int i = 0;
    StaticGeometry* s;
    s = mSceneMgr->createStaticGeometry("StaticFloor");
    s->setRegionDimensions(Ogre::Vector3(1600.0, 1000.0, 1600.0));
    // Set the region origin so the center is at 0 world
    s->setOrigin(Ogre::Vector3::ZERO);
    for (Real z = -80.0;z <= 80.0;z += 20.0)
    {
        for (Real x = -80.0;x <= 80.0;x += 20.0)
        {
            String name = String("Plane_") + StringConverter::toString(i++);

            Entity* entity = mSceneMgr->createEntity(name, "plane.mesh");
			entity->setMaterialName("Examples/DarkMaterial");
//            entity->setQueryFlags (STATIC_GEOMETRY_QUERY_MASK);
            entity->setCastShadows(false);
            s->addEntity(entity, Ogre::Vector3(x,0,z));
        }
    }
    s->build();

    CollisionShape *Shape = new StaticPlaneCollisionShape (Ogre::Vector3(0,1,0), 0);

    RigidBody *defaultPlaneBody = new RigidBody(
        "Plane" + StringConverter::toString(mNumEntitiesInstanced),
        mWorld);


    defaultPlaneBody->setStaticShape (Shape, bodyRestitution, bodyFriction);

    mBodies.push_back(defaultPlaneBody);
    mShapes.push_back(Shape);

    mNumEntitiesInstanced++;

    return defaultPlaneBody;
}

// -------------------------------------------------------------------------
