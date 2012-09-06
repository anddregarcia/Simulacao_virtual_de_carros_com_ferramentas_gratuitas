
#include "World.h"
#include "InputListener.h"

using namespace Ogre;


#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
using namespace OIS;
#endif //OGRE_VERSION not Eihort


// -------------------------------------------------------------------------
InputListener::InputListener(World * world,
                                                 Ogre::RenderWindow *win) :

//    mButton0Pressed (false),
//    mButton1Pressed (false),
    mButton2Pressed  (false),
    mWindow (win),
    mListener(world)
{
    mMouseCursorX = 0.5;
    mMouseCursorY = 0.5;

}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::mouseClicked(BULLET_MOUSE_EVENT e)
{
    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::mouseEntered(BULLET_MOUSE_EVENT e)
{
    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::mouseExited(BULLET_MOUSE_EVENT e)
{
    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::mousePressed(BULLET_MOUSE_EVENT e
#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
                                                      , OIS::MouseButtonID buttonid
#endif //OGRE_VERSION is Eihort
                                                                       )
{
    if (BULLET_BUTTON2_DOWN)
    {
        mButton2Pressed = true;
//        mListener->button2Pressed();
    }


    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::mouseReleased(BULLET_MOUSE_EVENT e
#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
                                                                             , OIS::MouseButtonID buttonid
#endif //OGRE_VERSION is Eihort
                                                                             )
{
    if (BULLET_BUTTON2_UP)
    {
        mButton2Pressed = false;
//        mListener->button2Released ();
    }


    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::mouseDragged(BULLET_MOUSE_EVENT e)
{
//     This populates the cursor moves or camera rotation variables
    mRelX = BULLET_GETRELX;
    mRelY = BULLET_GETRELY;

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    mMouseCursorX = Real(BULLET_GETX) / mWindow->getWidth ();
    mMouseCursorY = Real(BULLET_GETY) / mWindow->getHeight ();
#else
    mMouseCursorX = BULLET_GETX;
    mMouseCursorY = BULLET_GETY;
#endif

    mListener->mouseMoved ();


    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -----------------------IMPLEMENTAÇÃO DE IO COM O VEICULO--------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::mouseMoved(BULLET_MOUSE_EVENT e)
{
    // This populates the cursor moves or camera rotation variables
    mRelX = BULLET_GETRELX;
    mRelY = BULLET_GETRELY;

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    mMouseCursorX = Real(BULLET_GETX) / mWindow->getWidth ();
    mMouseCursorY = Real(BULLET_GETY) / mWindow->getHeight ();
#else
    mMouseCursorX = BULLET_GETX;
    mMouseCursorY = BULLET_GETY;
#endif


    mListener->mouseMoved ();

    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::keyClicked(BULLET_KEY_EVENT e)
{
    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::keyPressed(BULLET_KEY_EVENT e)
{
    mListener->keyPressed(BULLET_GETKEY);
    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
// -------------------------------------------------------------------------
BULLET_LISTENER_IMPLEMENTATION_RETURN InputListener::keyReleased(BULLET_KEY_EVENT e)
{
    mListener->keyReleased(BULLET_GETKEY);


    BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE
}
