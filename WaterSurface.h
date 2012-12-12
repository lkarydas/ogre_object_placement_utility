#ifndef __WaterSurface_h__
#define __WaterSurface_h__


#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreRenderTargetListener.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTarget.h>
#include <OgreMeshManager.h>
#include <OgreEntity.h>

class WaterSurface : public Ogre::RenderTargetListener
{
 private:
  Ogre::Camera* mCamera;
  Ogre::SceneManager* mSceneMgr;
  
  Ogre::RenderTarget* mRefractionTarget;
  Ogre::RenderTarget* mReflectionTarget;
  Ogre::Plane mWaterPlane;
  Ogre::Entity* mWater;


 public:
  WaterSurface(Ogre::SceneManager* sceneManager);
  void setCamera(Ogre::Camera* camera);
  void setup();

 protected:
  virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
  virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

};

#endif
