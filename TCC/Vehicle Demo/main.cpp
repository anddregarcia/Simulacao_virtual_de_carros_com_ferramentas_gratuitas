#include "Application.h"

#include "Environment.h"

// Windows stuff
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

/*
Windows entry point
*/
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT nCmdShow)
#else
int main(int argc,char* argv[])
#endif

{


    std::vector<World *> ogrebulletscenes;
    //criando vector que guarda a lista de menus

    std::vector <World *>::iterator it;

    ogrebulletscenes.push_back(new Environment());


    ogrebulletscenes[0]->setNextKey(KC_F1);

    // Create the application and try to run it
    Application app(&ogrebulletscenes);

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    SET_TERM_HANDLER;
#endif

    try
    {
        app.go();
    }
    catch(Ogre::Exception& e)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( 0, e.getFullDescription().c_str(), "OgreBullet OgreBulletListener: Exception", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
    }

    it =  ogrebulletscenes.begin();
    while (it != ogrebulletscenes.end())
    {
        delete *it;
        ++it;
    }

    return 0;
}

