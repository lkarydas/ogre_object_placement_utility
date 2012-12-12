/*
-----------------------------------------------------------------------------
Filename:    SceneCreatorMain.h
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

#include "SceneCreatorMain.h"
#include <OgreTexture.h>

//-------------------------------------------------------------------------------------

void SceneCreatorMain::init()
{
  // Initialize terrain manager
  terrainManager = new TerrainManager(mSceneMgr, Ogre::Vector3(300,-370,100));
  // Initialize water surface
  waterSurface = new WaterSurface(mSceneMgr);

  /* initialize random seed: */
  srand ( time(NULL) );

}

//-------------------------------------------------------------------------------------
SceneCreatorMain::SceneCreatorMain(void) : objectCounter(0), 
					   mObjectSelected(false),
					   mObjectPlaced(true),
					   mSelectedObject(0)

{
  

}
//-------------------------------------------------------------------------------------
SceneCreatorMain::~SceneCreatorMain(void)
{
}



void SceneCreatorMain::destroyScene(void)
{
  terrainManager->destroyTerrain();
}



//-------------------------------------------------------------------------------------
void SceneCreatorMain::createScene(void)
{
  mSceneMgr->setSkyDome(true, "Skyboxes/CloudySky", 5, 8, 40000, false);

  Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
  lightdir.normalise();
 
  Ogre::Light* light = mSceneMgr->createLight("tstLight");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(lightdir);
  light->setDiffuseColour(Ogre::ColourValue::White);
  light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

  // Create terrain 
  terrainManager->createTerrain(light);
  
  // Setup water surface for the pond
  waterSurface->setup();

  terrainManager->setCamera(mCamera);
}


void SceneCreatorMain::createCamera(void)
{
  // create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");
  mCamera->setPosition(Ogre::Vector3(1066, 625, 1064));
  mCamera->lookAt(Ogre::Vector3(50, 250, 250));
  mCamera->setNearClipDistance(0.1);
  mCamera->setFarClipDistance(50000);
 
  if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    mCamera->setFarClipDistance(0);                    // enable infinite far clip distance if we can
  mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

  // Set the water camera (to get reflection and refraction textures)
  // the main camera is used here
  waterSurface->setCamera(mCamera);
}
 
//-------------------------------------------------------------------------------------
void SceneCreatorMain::createViewports(void)
{
  // Create one viewport, entire window
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
  // Alter the camera aspect ratio to match the viewport
  mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight())); 
  vp->setClearEveryFrame( true );
}


bool SceneCreatorMain::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

  bool ret = BaseApplication::frameRenderingQueued(evt);
  
  if (terrainManager->isDerivedDataUpdateInProgress())
    {
      mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
      mInfoLabel->show();
      if (terrainManager->mTerrainsImported)
	mInfoLabel->setCaption("Building terrain, please wait...");
      else
	mInfoLabel->setCaption("Updating textures, patience...");
    }
  else
    {
      mTrayMgr->removeWidgetFromTray(mInfoLabel);
      mInfoLabel->hide();
      if (terrainManager->mTerrainsImported)
        {
	  terrainManager->save();
	  terrainManager->mTerrainsImported = false;
        }
    }
  
  // Update the camera height based on terrain height (gravity)
  // terrainManager->updateCameraHeight(evt);

  if (mObjectSelected)
    {
      terrainManager->setTrayManager(mTrayMgr);
      Ogre::TerrainGroup::RayResult rayResult = terrainManager->getRayResult(evt);
      if (rayResult.hit)
	{
	  int y_offset = 0;
	  switch (mSelectedObject)
	    {
	    case 1: y_offset = 370;
	      break;
	    case 2: y_offset = 170;
	      break;
	    case 3: y_offset = 70;
	    default:
	      break;
	    }
	  rayResult.position.y += y_offset;
	  char nodeName [11];
	  sprintf (nodeName, "object_%03d", objectCounter);
	  mSceneMgr->getSceneNode(nodeName)->setPosition(rayResult.position);
	}
      
    }
 
  return ret;
}


void SceneCreatorMain::createFrameListener(void)
{
  BaseApplication::createFrameListener();
 
  // Create buttons for selecting different tree models
  tree1_button = mTrayMgr->createButton(OgreBites::TL_LEFT, "Tree1Btn", "Tree 1");
  tree2_button = mTrayMgr->createButton(OgreBites::TL_LEFT, "Tree2Btn", "Tree 2");
  tree3_button = mTrayMgr->createButton(OgreBites::TL_LEFT, "Tree3Btn", "Tree 3");

  // Hide toolbar initially
  tree1_button->hide();
  tree2_button->hide();
  tree3_button->hide();
  mTrayMgr->removeWidgetFromTray(tree1_button);
  mTrayMgr->removeWidgetFromTray(tree2_button);
  mTrayMgr->removeWidgetFromTray(tree3_button);

  mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
  mTopLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TopLabel", "", 350);
  mTopLabel->setCaption("Press SPACE to enter edit mode");
  //topLabelOn = false;
  //mTopLabel->hide();
  //mTrayMgr->removeWidgetFromTray(mTopLabel);
}

void SceneCreatorMain::buttonHit(OgreBites::Button *button)
{
  // Create a Scene Node for the new object

  char name [11];
  Ogre::SceneNode* node;
  
  // If previous object was not placed
  if (!mObjectPlaced)
    {
      sprintf (name, "object_%03d", objectCounter);
      mSceneMgr->destroySceneNode(name);
    }

  objectCounter++;
  sprintf (name, "object_%03d", objectCounter);
  node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
  



  // Attach the tree  entity
  // Initialize our tree entity
  sprintf (name, "entity_%03d", objectCounter);

  if ( button->getName().compare("Tree1Btn") == 0 )
    {
      treeEntity = mSceneMgr->createEntity(name, "tree.09.mesh");
      node->scale( 1.5, 1.5, 1.5 );
      mSelectedObject = 1;
    }
  else if ( button->getName().compare("Tree2Btn") == 0 )
    {
      treeEntity = mSceneMgr->createEntity(name, "tree.05.mesh");
      node->scale( 3, 3, 3 );
      mSelectedObject = 2;
    }
  else if ( button->getName().compare("Tree3Btn") == 0 )
    {
      treeEntity = mSceneMgr->createEntity(name, "tree.07.mesh");
      node->scale( 1.4, 1.4, 1.4 );
      mSelectedObject = 3;
    }
  // Set random orientation for the new tree
  Ogre::Real randomRads = ((float)(rand() % 1000) / 1000)*6.28;
  node->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(randomRads));
  node->attachObject(treeEntity);

  mObjectSelected = true;
  mObjectPlaced = false;
  
}

bool SceneCreatorMain::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  if (mTrayMgr->injectMouseDown(arg, id)) return true;

  bool ret = BaseApplication::mousePressed(arg,id);
  switch (id)
    {
    case OIS::MB_Left:
      //mCameraActive = true;
      //mTrayMgr->hideCursor();
      mObjectSelected = false;
      mObjectPlaced = true;
      break;
    default:
      break;
    }

  /*
  
    tree1_button->hide();
    tree2_button->hide();
    tree3_button->hide();
    mTrayMgr->removeWidgetFromTray(tree1_button);
    mTrayMgr->removeWidgetFromTray(tree2_button);
    mTrayMgr->removeWidgetFromTray(tree3_button);
  */

  return true;
}
bool SceneCreatorMain::keyPressed (const OIS::KeyEvent &e)
{
  bool ret = BaseApplication::keyPressed(e);
  switch (e.key)
    {
    case OIS::KC_J:
      if (topLabelOn)
	{
	  topLabelOn = false;
	  mTopLabel->hide();
	  mTrayMgr->removeWidgetFromTray(mTopLabel);
	}
      else
	{
	  topLabelOn = true;
	  mTrayMgr->moveWidgetToTray(mTopLabel, OgreBites::TL_TOP, 0);

	  mTopLabel->show();
	}
      break;
    case OIS::KC_SPACE:
      if (mCameraActive)
	{
	  mTrayMgr->showCursor();
	  mCameraActive = false;
	  tree1_button->show();
	  tree2_button->show();
	  tree3_button->show();
	  mTrayMgr->moveWidgetToTray(tree3_button, OgreBites::TL_LEFT, 0);
	  mTrayMgr->moveWidgetToTray(tree2_button, OgreBites::TL_LEFT, 0);
	  mTrayMgr->moveWidgetToTray(tree1_button, OgreBites::TL_LEFT, 0);
	}
      else
	{
	  mTrayMgr->hideCursor();
	  mCameraActive = true;
	  tree1_button->hide();
	  tree2_button->hide();
	  tree3_button->hide();
	  mTrayMgr->removeWidgetFromTray(tree1_button);
	  mTrayMgr->removeWidgetFromTray(tree2_button);
	  mTrayMgr->removeWidgetFromTray(tree3_button);
	}
      break;
      
    case OIS::KC_S:
      // CTRL-S to save
      if (mKeyboard->isKeyDown(OIS::KC_LCONTROL) || mKeyboard->isKeyDown(OIS::KC_RCONTROL))
	{
	  // Pressing CTRL-S
	}
      break;
    default:

      return ret;
    }
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
  {
    // Create application object
    SceneCreatorMain app;

    try {
      app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " <<
	e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
  }

#ifdef __cplusplus
}
#endif
