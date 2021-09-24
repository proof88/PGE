unit gfxcore;

{
        GfxCore header file -> graphix library DLL
        Készítette: Szabó Ádám aka PR00F
        E-mail & MSN: proof88@freemail.hu
}



interface

uses
  OpenGL;

const
  GL_FLAT                      = GL_FLAT;
  GL_SMOOTH                    = GL_SMOOTH;
  GL_NEAREST                   = GL_NEAREST;
  GL_LINEAR                    = GL_LINEAR;
  GL_NEAREST_MIPMAP_NEAREST    = GL_NEAREST_MIPMAP_NEAREST;
  GL_LINEAR_MIPMAP_NEAREST     = GL_LINEAR_MIPMAP_NEAREST;
  GL_NEAREST_MIPMAP_LINEAR     = GL_NEAREST_MIPMAP_LINEAR;
  GL_LINEAR_MIPMAP_LINEAR      = GL_LINEAR_MIPMAP_LINEAR;
  GL_TEXTURE_MAG_FILTER        = GL_TEXTURE_MAG_FILTER;
  GL_TEXTURE_MIN_FILTER        = GL_TEXTURE_MIN_FILTER;
  GL_MODULATE                  = GL_MODULATE;
  GL_DECAL                     = GL_DECAL;
  GL_TEXTURE_ENV_MODE          = GL_TEXTURE_ENV_MODE;
  GL_TEXTURE_ENV_COLOR         = GL_TEXTURE_ENV_COLOR;
  GL_TEXTURE_ENV               = GL_TEXTURE_ENV;
  GL_REPEAT                    = GL_REPEAT;
  GL_CLAMP                     = GL_CLAMP;
  GL_CLAMP_TO_BORDER           = $812D;
  GL_CLAMP_TO_EDGE             = $812F;
  GL_ZERO                      = GL_ZERO;
  GL_ONE                       = GL_ONE;
  GL_SRC_COLOR                 = GL_SRC_COLOR;
  GL_SRC_ALPHA                 = GL_SRC_ALPHA;
  GL_ONE_MINUS_SRC_COLOR       = GL_ONE_MINUS_SRC_COLOR;
  GL_ONE_MINUS_SRC_ALPHA       = GL_ONE_MINUS_SRC_ALPHA;
  GL_DST_COLOR                 = GL_DST_COLOR;
  GL_DST_ALPHA                 = GL_DST_ALPHA;
  GL_ONE_MINUS_DST_COLOR       = GL_ONE_MINUS_DST_COLOR;
  GL_ONE_MINUS_DST_ALPHA       = GL_ONE_MINUS_DST_ALPHA;
  GL_SRC_ALPHA_SATURATE        = GL_SRC_ALPHA_SATURATE;


type
  HWND      = THandle;
  TSTR40    = string[40];
  TSTR255   = string[255];
  TFileName = TSTR255;
  TGLConst  = 0..32767;
  TXYZ = record
           x,y,z: single;
         end;
  TRGB = record
           red: byte;
           green: byte;
           blue: byte;
         end;
  TRGBA = record
            red: byte;
            green: byte;
            blue: byte;
            alpha: byte;
          end;


{ Inicializálás, renderelés, debuggolás, finalizálás }
function tmcsInitialized(): boolean; stdcall; external 'gfxcore.dll';
function tmcsInitGraphix(wnd: HWND; fs: boolean; freq: integer; cdepth,zdepth: integer; vsyncstate: boolean; shading: integer): byte; stdcall; external 'gfxcore.dll';
procedure tmcsRender(); stdcall; external 'gfxcore.dll';
procedure tmcsRestoreOriginalDisplayMode; stdcall; external 'gfxcore.dll';
procedure tmcsRestoreDisplayMode; stdcall; external 'gfxcore.dll';
function tmcsShutdownGraphix(): byte; stdcall; external 'gfxcore.dll';
procedure tmcsEnableDebugging(); stdcall; external 'gfxcore.dll';
procedure tmcsDisableDebugging(); stdcall; external 'gfxcore.dll';
procedure tmcsSetGamma(r,g,b: integer); stdcall; external 'gfxcore.dll';
function tmcsGetRenderedVertices(): integer; stdcall; external 'gfxcore.dll';
function tmcsGetRenderedFaces(): integer; stdcall; external 'gfxcore.dll';
function tmcsGetNumFuncCalls(): integer; stdcall; external 'gfxcore.dll';
{ Motion Blur }
procedure tmcsEnableMotionBlur(width, height: word); stdcall; external 'gfxcore.dll';
procedure tmcsDisableMotionBlur(); stdcall; external 'gfxcore.dll';
procedure tmcsFreeMotionBlurResources(); stdcall; external 'gfxcore.dll';
procedure tmcsSetMotionBlurUpdateRate(rate: byte); stdcall; external 'gfxcore.dll';
function tmcsGetMotionBlurColor(): TRGBA; stdcall; external 'gfxcore.dll';
procedure tmcsSetMotionBlurColor(red, green, blue, alpha: byte); stdcall; external 'gfxcore.dll';
{ Kamera }
procedure tmcsSetCameraTarget(index: integer); stdcall; external 'gfxcore.dll';
procedure tmcsSetBgColor(r,g,b,a: byte); stdcall; external 'gfxcore.dll';
procedure tmcsInitCamera(x,y,z,ax,ay,az: single; fov,aspect,znear,zfar: double); stdcall; external 'gfxcore.dll';
procedure tmcsSetviewport(x,y,w,h: integer); stdcall; external 'gfxcore.dll';
function tmcsGetCameraX(): single; stdcall; external 'gfxcore.dll';
function tmcsGetCameraY(): single; stdcall; external 'gfxcore.dll';
function tmcsGetCameraZ(): single; stdcall; external 'gfxcore.dll';
function tmcsGetCameraAngleX(): single; stdcall; external 'gfxcore.dll';
function tmcsGetCameraAngleY(): single; stdcall; external 'gfxcore.dll';
function tmcsGetCameraAngleZ(): single; stdcall; external 'gfxcore.dll';
function tmcsGetCameraFov(): double; stdcall; external 'gfxcore.dll';
function tmcsGetCameraAspect(): double; stdcall; external 'gfxcore.dll';
function tmcsGetCameraNearPlane(): double; stdcall; external 'gfxcore.dll';
function tmcsGetCameraFarPlane(): double; stdcall; external 'gfxcore.dll';
function tmcsWrapAngle(f: single): single; stdcall; external 'gfxcore.dll';
function tmcsGetNewX(x: single; angle: single; factor: single): single; stdcall; external 'gfxcore.dll';
function tmcsGetNewZ(z: single; angle: single; factor: single): single; stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraX(posx: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraY(posy: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraZ(posz: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraPos(posx,posy,posz: single); stdcall; external 'gfxcore.dll';
procedure tmcsMoveCamera(factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraAngleX(anglex: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraAngleY(angley: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraAngleZ(anglez: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraAngle(anglex,angley,anglez: single); stdcall; external 'gfxcore.dll';
procedure tmcsXRotateCamera(angle: single); stdcall; external 'gfxcore.dll';
procedure tmcsYRotateCamera(angle: single); stdcall; external 'gfxcore.dll';
procedure tmcsZRotateCamera(angle: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraFov(fov: double); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraAspect(aspect: double); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraNearPlane(znear: double); stdcall; external 'gfxcore.dll';
procedure tmcsSetCameraFarPlane(zfar: double); stdcall; external 'gfxcore.dll';
{ Billentyûzet }
procedure tmcsHandleKeyboard(num: byte; pressed: boolean); stdcall; external 'gfxcore.dll';
function tmcsIsKeyPressed(num: byte): boolean; stdcall; external 'gfxcore.dll';
function tmcsKeyPressed(): boolean; stdcall; external 'gfxcore.dll';
function tmcsGetKeyPressed(): byte; stdcall; external 'gfxcore.dll';
{ Objektumok }
function tmcsGetTotalObjects(): word; stdcall; external 'gfxcore.dll';
function tmcsGetNumSubObjects(num: word): integer; stdcall; external 'gfxcore.dll';
function tmcsCreatePlane(w,h: single): integer; stdcall; external 'gfxcore.dll';
function tmcsCreateCube(a: single): integer; stdcall; external 'gfxcore.dll';
function tmcsCreateBox(a,b,c: single): integer; stdcall; external 'gfxcore.dll';
function tmcsCreateObjectFromFile(fname: TSTR40; compiled: boolean): integer; stdcall; external 'gfxcore.dll';
function tmcsCreateClonedObject(refersto: integer): integer; stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectMultiTextured(index: word); stdcall; external 'gfxcore.dll';
procedure tmcsMultiTexAssignObject(index1, index2: word); stdcall; external 'gfxcore.dll';
procedure tmcsDeleteObject(index: word); stdcall; external 'gfxcore.dll';
procedure tmcsDeleteObjects(); stdcall; external 'gfxcore.dll';
procedure tmcsSetExtObjectsTextureMode(mipmapping: boolean; filtering,envmode: TGLConst; border,compressed: boolean; wrap_s,wrap_t: TGLConst); stdcall;  external 'gfxcore.dll';
procedure tmcsCompileObject(index: word); stdcall; external 'gfxcore.dll';
procedure tmcsShowObject(num: word); stdcall; external 'gfxcore.dll';
procedure tmcsHideObject(num: word); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectWireframe(num: word; wf: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectDoublesided(num: word; ds: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectStickedState(num: word; state: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectZBuffered(num: word; state: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetWiredCulling(state: boolean); stdcall; external 'gfxcore.dll';
function tmcsGetXPos(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetYPos(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetZPos(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSizeX(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSizeY(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSizeZ(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetAngleX(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetAngleY(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetAngleZ(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsIsVisible(num: word): boolean; stdcall; external 'gfxcore.dll';
function tmcsGetScaling(num: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetName(num: word): TSTR40; stdcall; external 'gfxcore.dll';
procedure tmcsSetXPos(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetYPos(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetZPos(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsScaleObject(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsScaleObjectAbsolute(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsXRotateObject(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsYRotateObject(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsZRotateObject(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetAngleX(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetAngleY(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetAngleZ(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectRotationXZY(num: word); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectRotationYXZ(num: word); stdcall; external 'gfxcore.dll';
procedure tmcsSetName(num: word; name: TSTR40); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectLit(num: word; state: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectColor(num: word; r,g,b: byte); stdcall; external 'gfxcore.dll';
function tmcsGetObjectColorKey(num: word): TRGBA; stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectColorKey(num: word; r,g,b,a: byte); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectAlpha(num: word; a: byte); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectBlendMode(num: word; sfactor,dfactor: TGLConst); stdcall; external 'gfxcore.dll';
procedure tmcsSetObjectBlending(num: word; state: boolean); stdcall; external 'gfxcore.dll';
{ Alobjektumok }
function tmcsGetSubXPos(num1, num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSubYPos(num1, num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSubZPos(num1, num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsSubIsVisible(num1, num2: word): boolean; stdcall; external 'gfxcore.dll';
function tmcsGetSubSizeX(num1, num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSubSizeY(num1, num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSubSizeZ(num1, num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetSubName(num1, num2: word): TSTR40; stdcall; external 'gfxcore.dll';
procedure tmcsSetSubXPos(num1, num2: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetSubYPos(num1, num2: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsSetSubZPos(num1, num2: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsShowSubObject(num1, num2: word); stdcall; external 'gfxcore.dll';
procedure tmcsHideSubObject(num1, num2: word); stdcall; external 'gfxcore.dll';
procedure tmcsSetSubobjectColor(num1, num2: word; r,g,b: byte); stdcall; external 'gfxcore.dll';
procedure tmcsSetSubName(num1, num2: word; name: TSTR40); stdcall; external 'gfxcore.dll';

{ Fények }
procedure tmcsEnableLights(); stdcall; external 'gfxcore.dll';
procedure tmcsDisableLights(); stdcall; external 'gfxcore.dll';
procedure tmcsEnableAmbientLight(); stdcall; external 'gfxcore.dll';
procedure tmcsDisableAmbientLight(); stdcall; external 'gfxcore.dll';
procedure tmcsSetAmbientLight(r,g,b: single); stdcall; external 'gfxcore.dll';
{ Textúrák }
function tmcsCreateTextureFromFile(fname: TFileName; mipmapped,border,compressed: boolean; filtering,envmode,wrap_s,wrap_t: TGLConst): integer; stdcall; external 'gfxcore.dll';
procedure tmcsFrameBufferToTexture(texnum: integer); stdcall; external 'gfxcore.dll';
function tmcsCreateBlankTexture(width, height: integer; filtering,envmode,wrap_s,wrap_t: TGLConst): integer; stdcall; external 'gfxcore.dll';
procedure tmcsDeleteTexture(num: word); stdcall; external 'gfxcore.dll';
procedure tmcsDeleteTextures(); stdcall; external 'gfxcore.dll';
procedure tmcsTextureObject(num: word; num2: word); stdcall; external 'gfxcore.dll';
function tmcsGetTextureWidth(num: word): integer; stdcall; external 'gfxcore.dll';
function tmcsGetTextureHeight(num: word): integer; stdcall; external 'gfxcore.dll';
function tmcsGetTextureInternalnum(num: word): cardinal; stdcall; external 'gfxcore.dll';
function tmcsGetObjectTexture(num: word): integer; stdcall; external 'gfxcore.dll';
function tmcsGetSubObjectTexture(num1: word; num2: word): integer; stdcall; external 'gfxcore.dll';
procedure tmcsMultiplyUVCoords(num: word; factorw: single; factorh: single); stdcall; external 'gfxcore.dll';
procedure tmcsAdjustUVCoords(num: word; factor: single); stdcall; external 'gfxcore.dll';
procedure tmcsAdjustPlaneCoordsToViewport(num: word; num2: word); stdcall; external 'gfxcore.dll';

{ Vertexszintû manipulálás }
function tmcsGetTotalVertices(num: word): integer; stdcall; external 'gfxcore.dll';
function tmcsGetVertexX(num: word; num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetVertexY(num: word; num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetVertexZ(num: word; num2: word): single; stdcall; external 'gfxcore.dll';
procedure tmcsSetVertex(num: word; num2: word; x,y,z: single); stdcall; external 'gfxcore.dll';
function tmcsGetNormalX(num: word; num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetNormalY(num: word; num2: word): single; stdcall; external 'gfxcore.dll';
function tmcsGetNormalZ(num: word; num2: word): single; stdcall; external 'gfxcore.dll';
procedure tmcsSetNormal(num: word; num2: word; nx,ny,nz: single); stdcall; external 'gfxcore.dll';

{ Szövegek kiírásának kezelése }
procedure tmcsLoadFontInfo(path,name: tstr255); stdcall; external 'gfxcore.dll';
procedure tmcsText(text: tstr255; x,y: word; fontheight: word; scaling: word); stdcall; external 'gfxcore.dll';
procedure tmcsSetTextColor(r,g,b,a: byte); stdcall; external 'gfxcore.dll';
procedure tmcsSetTextBlendingState(state: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetTextBlendMode(sfactor, dfactor: TGLConst); stdcall; external 'gfxcore.dll';
function tmcsGetTextWidth(text: tstr255; fontheight: word; scaling: word): integer; stdcall; external 'gfxcore.dll';

procedure tmcsSetRenderPath(renderPath: word); stdcall; external 'gfxcore.dll';
procedure tmcsSetOcclusionCullingMethod(ocMethod: word); stdcall; external 'gfxcore.dll';
procedure tmcsSetOcclusionCullingBoundingBoxes(state: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetOcclusionCullingDrawIfPending(state: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsSetOrderingByDistance(state: boolean); stdcall; external 'gfxcore.dll';
procedure tmcsResetStatistics(); stdcall; external 'gfxcore.dll';
procedure tmcsEngineDump(); stdcall; external 'gfxcore.dll';

implementation

end.
