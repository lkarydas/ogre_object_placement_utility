#include "WaterSurface.h"

WaterSurface::WaterSurface(Ogre::SceneManager* sceneManager)
{
 
  mSceneMgr = sceneManager;
}

void WaterSurface::setCamera(Ogre::Camera* camera)
{
  mCamera = camera;
}

void WaterSurface::setup()
{
  for (unsigned int i = 0; i < 2; i++)
    {
      Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().
	createManual(i == 0 ? "refraction" : "reflection",
		     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		     Ogre::TEX_TYPE_2D, 1024, 1024, 0, 
		     Ogre::PF_R8G8B8, 
		     Ogre::TU_RENDERTARGET);

      Ogre::RenderTarget* rtt = tex->getBuffer()->getRenderTarget();
      rtt->addViewport(mCamera)->setOverlaysEnabled(false);
      rtt->addListener(this);
      
      if (i == 0) mRefractionTarget = rtt;
      else mReflectionTarget = rtt;
    }
  
  // create our water plane mesh
  mWaterPlane = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().
    createPlane("water", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		mWaterPlane, 1500, 1500, 10, 10, true, 1, 3, 5, Ogre::Vector3::UNIT_Z);
  
  // create a water entity using our mesh, give it the shader material, and attach it to the origin
  mWater = mSceneMgr->createEntity("Water", "water");
  mWater->setCastShadows(false);
  mWater->setMaterialName("Examples/FresnelReflectionRefraction");
  Ogre::SceneNode* waterPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WaterPlaneNode");
  waterPlaneNode->attachObject(mWater);
  waterPlaneNode->translate( Ogre::Vector3( 0, -8, 0 ) );
}


void WaterSurface::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
  mWater->setVisible(false);
  if (evt.source == mReflectionTarget)  
    {
      mCamera->enableReflection(mWaterPlane);
      mCamera->enableCustomNearClipPlane(mWaterPlane);
    }
}
  



void WaterSurface::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
  mWater->setVisible(true);
  if (evt.source == mReflectionTarget)  
    {
      mCamera->disableReflection();
      mCamera->disableCustomNearClipPlane();
    }
}

