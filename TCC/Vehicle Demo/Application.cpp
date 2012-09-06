#include "Application.h"
#include "World.h"


#include "OgreResourceGroupManager.h"

using namespace Ogre;

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    using namespace OIS;
#endif //OGRE_VERSION not Eihort

using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

// -------------------------------------------------------------------------
Application::Application(std::vector <World *> *bulletListeners) :
    ExampleApplication(),
    FrameListener(),
    mInputSystem(0),
    mInput(0),
    mWorld(0)
{
    mWorlds = bulletListeners;
    assert (!mWorlds->empty());
}
// -------------------------------------------------------------------------
Application::~Application()
{
    if (mInputSystem || mInput)
    {
        #if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))

            delete mInputSystem;
        #else

            mInputSystem->destroyInputObject(mInput);
            mInputSystem->destroyInputObject(mMouse);
            InputManager::destroyInputSystem(mInputSystem);
        #endif //OGRE_VERSION not Eihort
    }
}
// -------------------------------------------------------------------------
bool Application::switchListener(World *newListener)
{

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))

    if (mWorld)
    {
        mInputSystem->removeMouseMotionListener(mWorld->getInputListener());
        mInputSystem->removeMouseListener(mWorld->getInputListener());
        mInputSystem->removeKeyListener(mWorld->getInputListener());
        mWorld->shutdown ();
    }

    newListener->init (mRoot, mWindow);

    mInputSystem->addMouseMotionListener(newListener->getInputListener());
    mInputSystem->addMouseListener(newListener->getInputListener());
    mInputSystem->addKeyListener(newListener->getInputListener());

#else

    if (mWorld)
    {
        mInput->setEventCallback (0);
        mMouse->setEventCallback (0);
        mWorld->shutdown ();
    }

    newListener->init (mRoot, mWindow, this);
    mInput->setEventCallback (newListener->getInputListener());
    mMouse->setEventCallback (newListener->getInputListener());

#endif //OGRE_VERSION not Eihort

    mWorld = newListener;

    return true;
}
// -----------------------IMPORTANTE--------------------------------------------------
bool Application::frameStarted(const FrameEvent& evt)
{

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
        mMouse->capture();
        mInput->capture();
#else
    mInput->capture();
#endif

    std::vector <World *>::iterator it =  mWorlds->begin();
    while (it != mWorlds->end())
    {
        if ((*(*it)->getBoolActivator()) == true ||
            mInput->isKeyDown ((*it)->getNextKey ()))
        {
            //if ((*it) !=  mWorld)
            {
                switchListener(*it);
            }
            break;
        }
        ++it;
    }

    assert (mWorld);

    if (!mWorld->frameStarted(evt.timeSinceLastFrame))
    {
        mWorld->shutdown ();
        return false;
    }
    return true;
}

// -------------------------------------------------------------------------
bool Application::frameEnded(const FrameEvent& evt)
{
    assert (mWorld);
    // we're running a scene, tell it that a frame's started
    ;

    if (!mWorld->frameEnded(evt.timeSinceLastFrame))
    {
        mWorld->shutdown ();
        return false;
    }
    return true;
}

// -------------------------------------------------------------------------
void Application::createFrameListener(void)
{

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))

    mInput = PlatformManager::getSingleton().createInputReader();

    mInputSystem = new EventProcessor();
    mInputSystem->initialise (mWindow);
    mInputSystem->startProcessingEvents();
    mInput = mInputSystem->getInputReader();

#else

    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
	OIS::ParamList pl;

    #if defined OIS_WIN32_PLATFORM
        mWindow->getCustomAttribute("WINDOW", &windowHnd);
    #elif defined OIS_LINUX_PLATFORM
		mWindow->getCustomAttribute( "WINDOW", &windowHnd );
    #endif

    // Fill parameter list
    windowHndStr << (unsigned int) windowHnd;
    pl.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

    mInputSystem  = InputManager::createInputSystem( pl );

    mInput = static_cast<Keyboard*>(mInputSystem->createInputObject( OISKeyboard, true ));
    mMouse = static_cast<Mouse*>(mInputSystem->createInputObject( OISMouse, true ));

    unsigned int width, height, depth;
    int left, top;
    mWindow->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;

#endif //OGRE_VERSION not Eihort

    switchListener (*(mWorlds->begin()));
    mRoot->addFrameListener(this);

}
// ------------------CARREGANDO OS ARQUIVOS DE RESOURCES -------------------------------------------------------
void Application::setupResources(void)
{
	ExampleApplication::setupResources();
	ResourceGroupManager *rsm = ResourceGroupManager::getSingletonPtr();
	StringVector groups = rsm->getResourceGroups();
	FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
		"Bootstrap", "axes.mesh");
	const bool isSDK =  (!finfo->empty()) &&
		StringUtil::startsWith (finfo->begin()->archive->getName(), "../../media/packs/ogrecore.zip", true);

	const String resName ("OgreBullet");
	{
		if (std::find(groups.begin(), groups.end(), resName) == groups.end())
		{
			rsm->createResourceGroup(resName);
			String baseName;
			if (isSDK)
			{
				baseName = "../../../ogrebullet/";
			}
			else
			{
				baseName = "../../../../../ogreaddons/ogrebullet/";
			}

			rsm->addResourceLocation (baseName + "demos/Media","FileSystem", resName);
			rsm->addResourceLocation (baseName + "demos/Media/textures", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "demos/Media/overlays", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "demos/Media/materials", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "demos/Media/models", "FileSystem", resName);

			//rsm->addResourceLocation (baseName + "demos/Media/gui", "FileSystem", resName);
		}
	}
}
// ------------------------------------------------------------------------
void Application::loadResources(void)
{
	ResourceGroupManager *rsm = ResourceGroupManager::getSingletonPtr();
	StringVector groups = rsm->getResourceGroups();
	for (StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		rsm->initialiseResourceGroup((*it));
	}
	}

// -------------------------------------------------------------------------
