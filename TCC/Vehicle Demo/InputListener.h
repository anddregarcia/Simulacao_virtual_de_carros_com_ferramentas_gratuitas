#ifndef _InputListener_H_
#define _InputListener_H_

#include "Ogre.h"

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))

    #include "OgreInput.h"
    #include "OgreKeyEvent.h"
    #include "OgreEventListeners.h"


    #define BULLET_KEY_CODE                             int
    #define BULLET_KEY_EVENT                            Ogre::KeyEvent*
    #define BULLET_MOUSE_EVENT                          Ogre::MouseEvent*
    #define BULLET_KC                                   Ogre::KC
    #define BULLET_LISTENER_IMPLEMENTATION_RETURN       void
    #define BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE


    #define BULLET_GETKEY       e->getKey()


    #define BULLET_BUTTON2_DOWN e->getButtonID() == e->BUTTON2_MASK


    #define BULLET_BUTTON2_UP e->getButtonID() == e->BUTTON2_MASK

    #define BULLET_GETRELX      e->getRelX()
    #define BULLET_GETRELY      e->getRelY()

    #define BULLET_GETX         e->getX()
    #define BULLET_GETY         e->getY()

#else
// NOTICE: You may need to change this line to <OIS.h> if you build OIS from source.
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #include "OIS.h"
#else
    #include "OIS.h"
#endif
    namespace OIS
    {
        class Keyboard;
        class Mouse;
    };

    #define BULLET_KEY_CODE                             OIS::KeyCode
    #define BULLET_KEY_EVENT                            const OIS::KeyEvent&
    #define BULLET_MOUSE_EVENT                          const OIS::MouseEvent&
    #define BULLET_KC                                   OIS::KC
    #define BULLET_LISTENER_IMPLEMENTATION_RETURN       bool
    #define BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE  return true;

    #define BULLET_GETKEY       e.key


    #define BULLET_BUTTON2_DOWN buttonid == OIS::MB_Right


    #define BULLET_BUTTON2_UP buttonid == OIS::MB_Right


    #define BULLET_GETRELX      e.state.X.rel
    #define BULLET_GETRELY      e.state.Y.rel

    #define BULLET_GETX         e.state.X.abs
    #define BULLET_GETY         e.state.Y.abs

#endif //OGRE_VERSION not Eihort

//#include "OgreBulletGuiListener.h"
class World;

/*
The base Test class, is also able to listen for collisions and thus change the contact properties
*/
class InputListener :
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    public Ogre::MouseMotionListener,
    public Ogre::MouseListener,
    public Ogre::KeyListener
#else
    public OIS::MouseListener,
    public OIS::KeyListener
#endif
{
public:
    static const Ogre::Real KEY_DELAY;

	// Constructor/destructor
    InputListener(World * world,
                            Ogre::RenderWindow *win);
    virtual ~InputListener(){};

    // MouseMotionListener Callbacks
    BULLET_LISTENER_IMPLEMENTATION_RETURN mouseDragged (BULLET_MOUSE_EVENT e);
    BULLET_LISTENER_IMPLEMENTATION_RETURN mouseMoved   (BULLET_MOUSE_EVENT e);

    // MouseListener Callbacks
    BULLET_LISTENER_IMPLEMENTATION_RETURN mouseClicked (BULLET_MOUSE_EVENT e);
    BULLET_LISTENER_IMPLEMENTATION_RETURN mouseEntered (BULLET_MOUSE_EVENT e);
    BULLET_LISTENER_IMPLEMENTATION_RETURN mouseExited  (BULLET_MOUSE_EVENT e);

    BULLET_LISTENER_IMPLEMENTATION_RETURN mousePressed (BULLET_MOUSE_EVENT e
#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
        , OIS::MouseButtonID buttonid
#endif //OGRE_VERSION is Eihort
        );
    BULLET_LISTENER_IMPLEMENTATION_RETURN mouseReleased(BULLET_MOUSE_EVENT e
#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
        , OIS::MouseButtonID buttonid
#endif //OGRE_VERSION is Eihort
        );

    // KeyListener Callbacks
    BULLET_LISTENER_IMPLEMENTATION_RETURN keyClicked(BULLET_KEY_EVENT e);
    BULLET_LISTENER_IMPLEMENTATION_RETURN keyPressed(BULLET_KEY_EVENT e);
    BULLET_LISTENER_IMPLEMENTATION_RETURN keyReleased(BULLET_KEY_EVENT e);

    Ogre::Real getRelMouseX() const {return mRelX;}
    Ogre::Real getRelMouseY() const {return mRelY;}

    Ogre::Real getAbsMouseX() const {return mMouseCursorX;}
    Ogre::Real getAbsMouseY() const {return mMouseCursorY;}



    bool getButton2Pressed() const {return mButton2Pressed;}


protected:


   Ogre::Real              mRelX;
   Ogre::Real              mRelY;
   Ogre::Real              mMouseCursorX;
   Ogre::Real              mMouseCursorY;


   bool                     mButton2Pressed;


    Ogre::RenderWindow      *mWindow;
    World      *mListener;
};

#endif

