/*
-----------------------------------------------------------------------------
Filename:    SceneCreatorMain.h
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/
#ifndef __SceneCreatorMain_h_
#define __SceneCreatorMain_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "TerrainManager.h"
#include "WaterSurface.h"
#include "BaseApplication.h"

class SceneCreatorMain : public BaseApplication
{

 private:

  bool mObjectSelected;
  bool mObjectPlaced;
  int mSelectedObject;


  // Toolbar buttons
  OgreBites::Button* tree1_button;
  OgreBites::Button* tree2_button;
  OgreBites::Button* tree3_button;

  // This is an entity for an example object to be placed
  Ogre::Entity* treeEntity;
  int objectCounter;

  OgreBites::Label* mInfoLabel;
  OgreBites::Label* mTopLabel;
  bool topLabelOn;
  TerrainManager* terrainManager;
  WaterSurface* waterSurface;

 public:
  SceneCreatorMain(void);
  virtual ~SceneCreatorMain(void);

 protected:
  virtual void createScene(void);
  virtual void init(void);
  virtual void createCamera(void);
  virtual void createViewports(void);
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  virtual void createFrameListener(void);
  virtual void destroyScene(void);

  // OIS::KeyListener
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  //virtual bool keyReleased( const OIS::KeyEvent &arg );
  // OIS::MouseListener
  //virtual bool mouseMoved( const OIS::MouseEvent &arg );
  virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  //virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

  // SDK Listener
  virtual void buttonHit(OgreBites::Button *button);

};

#endif // #ifndef __SceneCreatorMain_h_
