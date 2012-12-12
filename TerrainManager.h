
#ifndef __TerrainManager_h_
#define __TerrainManager_h_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>


#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>

#define TERRAIN_WORLD_SIZE 12000.0f


class TerrainManager
{

 private:
  Ogre::Real mBrushSizeTerrainSpace;
  Ogre::Camera* mCamera;
  bool mCameraSet;
  bool mTrayManagerSet;
  Ogre::Real mFallVelocity;
  Ogre::SceneManager* mSceneMgr;
  Ogre::TerrainGlobalOptions* mTerrainGlobals;
  Ogre::TerrainGroup* mTerrainGroup;
  Ogre::Vector3 mTerrainPos;
  OgreBites::SdkTrayManager* mTrayMgr;
 
  void defineTerrain(long x, long y);
  void initBlendMaps(Ogre::Terrain* terrain);
  void configureTerrainDefaults(Ogre::Light* light);
  void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);


 public:
  TerrainManager(Ogre::SceneManager* sceneManager, Ogre::Vector3 TerrainPosition);
  bool mTerrainsImported;
  void createTerrain(Ogre::Light* light);
  void destroyTerrain();
  bool isDerivedDataUpdateInProgress();
  void save();
  void updateCameraHeight(const Ogre::FrameEvent& evt);
  Ogre::TerrainGroup::RayResult getRayResult(const Ogre::FrameEvent& evt);
  void setCamera(Ogre::Camera* currentCamera);
  void setTrayManager(OgreBites::SdkTrayManager* trayManager);

};



#endif
