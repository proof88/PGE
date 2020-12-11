# Engine Usage

Main class of the engine is PR00FsReducedRenderingEngine. Initialization, shutdown, and access to other engine classes is done through this main class.

This page gives you a brief hint on the usage, but you can always check the UnitTests as well for different cases.  

## Initialization

PR00FsReducedRenderingEngine::initialize() is responsible for initialization.  
The user can select which renderer should be initialized with the engine.  
The detailed documentation of this function can be found at the initialize() member function of these renderers. The reason for this is that some renderer may not use a parameter or may use it in a different way than another renderer.  
All renderers must implement the PRREIRenderer interface.

Currently the following renderers are available:  
 - PRRERendererSWincremental
 - PRRERendererHWfixedPipe

PR00FsReducedRenderingEngine initializes one of the above renderers at initialization.  

Example code snippet for initializing the engine, with PRRERendererHWfixedPipe renderer, with a 800x600 pixels window and 24 bpp Z-buffer:

```{.cpp}
//

  PR00FsReducedRenderingEngine& engine = PR00FsReducedRenderingEngine::createAndGet();
  if ( 0 == engine.initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0) ) {
      // success
  } else {
      // error
  }

//
```

Related PURE API: PR00FsReducedRenderingEngine::initialize(), PRREIRenderer::initialize().  

## Loading Resources

Resources such as textures, 3D models, etc. can be created or loaded by Managers.  
Any class descending from PRREManager class is a manager.

Following resource-specific Managers are available:  
 - PRREImageManager : parsing image files and creating PRREImage objects of them;
 - PRRETextureManager : creating PRRETexture objects from existing PRREImage objects or by parsing image files;
 - PRREMaterialManager : creating PRREMaterial objects programatically or by parsing material files;
 - PRREMesh3DManager : parsing 3D mesh files and creating PRREMesh3D objects of them;
 - PRREObject3DManager : extending PRREMesh3DManager with the ability of creating PRREObject3D objects;
 - PRREMesh3D : is also a Manager, managing its own submeshes of the same PRREMesh3D class;
 - PRREVertexTransfer : extending PRREMesh3D by vertex transfer mode (geometry pipeline) configuration ability;
 - PRREObject3D : extending PRREVertexTransfer by rendering code, to be utilized by the current renderer implementing PRREIRenderer.

Note that Managers are only available when the engine is initialized.  
Managers can be accessed by the relevant get...() member functions of main class PR00FsReducedRenderingEngine.

Example code snippet for creating a texture from an image file:

```{.cpp}
//
  
  PRRETexture* myTexture = engine.getTextureManager().createFromFile("texture.bmp");
  if ( PGENULL != myTexture ) {
      // success
  } else {
      // error
  }
  
//
```

Example code snippet for creating a renderable object from a 3D mesh file:

```{.cpp}
//
  
  PRREObject3D* myObject = engine.getObject3DManager().createFromFile("mesh.obj");
  if ( PGENULL != myObject ) {
      // success
  } else {
      // error
  }

//  
```

Related PURE API: TODO  

## Rendering

As mentioned at the Initialization section above, there are multiple renderers available. 
The initialized renderer object can be accessed with PR00FsReducedRenderingEngine::getRenderer().

Rendering can be executed with the PRREIRenderer::RenderScene() implementation of the initialized renderer object.

Example code snippet for rendering (since default position of PRRECamera object equals to default position of any created PRREObject3D instance, changing there position is likely needed to become actually visible):

```{.cpp}
//
  
  engine.getRenderer().RenderScene();

//  
```

Related PURE API: PR00FsReducedRenderingEngine::getRenderer(), PRREIRenderer::RenderScene().  

## Shutdown

Graceful termination of the application can be a reason for shutting down the graphics engine.  
Another reason could be the need for changing some basic renderer setting that can be specified only in PR00FsReducedRenderingEngine::initialize().  
In any case, PR00FsReducedRenderingEngine::shutdown() should be called that invokes the PRREIRenderer::shutdown() implementation of the current renderer.

Note that an uninitialized engine cannot be shut down. First you need to initialize the engine to shut it down.

Example code snippet for shutdown:

```{.cpp}
//
  
  if ( engine.shutdown() ) {
      // success
  } else {
      // error
  }

//  
```

Related PURE API: PR00FsReducedRenderingEngine::shutdown(), PRREIRenderer::shutdown().


## Samples

TODO