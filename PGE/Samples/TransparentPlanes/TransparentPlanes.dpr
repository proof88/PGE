program TransparentPlanes;

uses
  Classes,
  Windows,
  Controls,
  Sysutils,
  Forms,
  Dialogs,
  gfxcore,
  U_input in 'u_input.pas';

const
  PRODUCTNAME = 'TransparentPlanes';           // A progi neve
  PRODUCTVERSION = '1.0';                      // verziója
  APP_TITLE = PRODUCTNAME+' '+PRODUCTVERSION;  // Alkalmazás címe
 { ************************************************************************************************************* }

  WM_SYSCOMMAND       = 274;                   // Window Message konstansok (WndProc()-on belül üzenetkezeléshez)
  WM_ACTIVATE         = 6;
  WM_CLOSE            = 16;
  WM_KEYDOWN          = 256;
  WM_KEYUP            = 257;
  WM_QUIT             = 18;
  WM_WINDOWPOSCHANGED = 71;
  WM_MOUSEMOVE        = 512;
  WM_MOUSEWHEEL       = 522;
  WM_LBUTTONDOWN      = 513;
  WM_LBUTTONUP        = 514;
  WM_MBUTTONDOWN      = 519;
  WM_MBUTTONUP        = 520;
  WM_RBUTTONDOWN      = 516;
  WM_RBUTTONUP        = 517;
  WM_PAINT            = 15;

  { ************************************************************************************************************* }

  GAMEWINDOW_CLASSNAME = 'gamewindow';        // Fõ játékablak osztályának neve

  { ************************************************************************************************************* }

  ERRORDLG_BASE_TITLE   = 'Hiba';                                               // Hiba esetén megjelenített dialógus ablak címe
  ERROR_TMCSINITGRAPHIX = 'Nem sikerült inicializálni a grafikus rendszert.';   // tmcsInitGraphix() nem volt ok
  ERROR_CREATEWINDOWEX  = 'Nem sikerült létrehozni az ablakot.';                // fõ játékablak createWindowEx()-je nem volt ok
  ERROR_REGISTERCLASS   = 'Nem sikerült regisztrálni az osztályt.';             // fõ játékablak osztályának regje nem volt ok
  ERROR_FMOD_INIT       = 'Nem sikerült inicializálni a hangkezelõ rendszert!'+#10+
                          'Bõvebb információt a dokumentációban talál!';        // FMOD nem lett inicializálva
  ERROR_NOSP            = 'A pálya nem tartalmaz kezdõpontot!';                 // no spawnpoint
  ERROR_NOCFG           = 'Nem sikerült betölteni a pályához tartozó beállítás fájlt!'; // nincs beálítás fájl
  ERROR_LOADMAPFAILED   = 'Nem sikerült betölteni a kiválasztott pályát!';      // magáért beszél

  { ************************************************************************************************************* }

  GAME_FPS_INTERVAL = 200;                                       // az fps-statisztikák frissítési intervalluma ms-ben
  GAME_MAXFPS = 100;                                             // max. fps (-1, ha a lehetõ legnagyobb fps-t akarjuk elérni)
  GAME_INACTIVE_SLEEP = 30;                                      // hány ms-et várjon ciklusonként a progi, ha inaktív az ablak
  GAME_ASPECTRATIO = 4/3;                                        // renderelt kép szélességének és magasságának aránya
  GAME_FULLSCREEN = false;
  GAME_CDEPTH = 32;
  GAME_WIDTH = 1024;
  GAME_HEIGHT = 768;
  GAME_ZDEPTH = 24;

  GAME_MOVESPEED_HOR = 100;
  GAME_MOVESPEED_VER = 3;

  GAME_PATH_ITEMS = '..\_res\bigstuff\items\';
  GAME_ITEMS_SCALE = 45;
  GAME_SNAIL_SCALE = 50;
  GAME_MAP_SCALE = 30;




type
  TGameWindow = record                   // játék ablak tulajdonságai
                  wc: WNDCLASS;            // window class
                  dwStyle: DWORD;          // createwindowex()-hez
                  dwStyleEx: DWORD;        // createwindowex()-hez
                  hwindow: HWND;           // window handle
                  hdevctx: HDC;            // device context
                  hrenctx: HGLRC;          // rendering context
                  wndmsg: MSG;             // window message
                  active: boolean;         // aktív-e a window
                  cdepth: integer;         // colordepth
                  clientsize: TPoint;      // kliens mérete
                  wndrect: trect;          // ablak rectje
                end;



var
  gamewindow: TGameWindow;                 // a játék ablak (WinAPI)
  mx,my,mmx,mmy: integer;                  // egérkurzor új pozíciója, elmozdulása
  ocur: TPoint;
  tmcsstatus: byte;                        // tmcsInitGraphix() visszatérési értéke
  tex_transp: integer;



// egérkurzor pozícionálása
procedure mouseToWndCenter;
begin
  mx := gamewindow.wndrect.Left + ((gamewindow.wndrect.Right-gamewindow.wndrect.Left) div 2);
  my := gamewindow.wndrect.top + ((gamewindow.wndrect.bottom-gamewindow.wndrect.top) div 2);
  setcursorpos(mx,my);
end;

// a játék ablak üzenetkezelése
function WndProc(hwindow: HWND; wndmsg: uint; wp: wparam; lp: lparam): lresult; stdcall;
begin
  if ( wndmsg = WM_SYSCOMMAND ) then
    if ( wp = SC_SCREENSAVE ) then
      begin
        result := 0;
        exit;
      end
     else if ( wp = SC_MONITORPOWER ) then
       begin
         result := 0;
         exit;
       end;
  case wndmsg of
    WM_ACTIVATE: begin
                   gamewindow.active := loword(wp) in [WA_ACTIVE,WA_CLICKACTIVE];
                   if ( gamewindow.active ) then
                     begin
                       //tmcsrestoredisplaymode();
                       //mouseToWndCenter;
                     end;
                   result := 0;
                 end;
    WM_CLOSE: begin
                PostQuitMessage(0);
                result := 0;
              end;
    WM_KEYDOWN: begin
                  inputSetKeyPressed(wp);
                  result := 0;
                end;
    WM_KEYUP: begin
                inputSetKeyReleased(wp);
                result := 0;
              end;
    WM_WINDOWPOSCHANGED: begin
                           getwindowrect(gamewindow.hwindow,gamewindow.wndrect);
                           result := 0;
                         end;
   else result := DefWindowProc(hwindow, wndmsg, wp, lp);
  end;
end;

function WinMain(hInstance: HINST; hPrevInstance: HINST; lpCmdLine: PChar; nCmdShow: integer): integer; stdcall;
var
  windmsg: TMsg;                                        // lekérdezett window message
  done: boolean;                                        // a játék fõciklusának elhagyása, ha TRUE
  fps_ms,fps_ms2,fps_ms3,fps_ms_old: cardinal;          // fps méréséhez kell
  fps,fps_old: integer;                                 // detto
  texture: integer;
  obj: integer;
  i,j: integer;
  texwidth, texheight: integer;
  dbx, dby: integer;
  arena1, arena2, arena3: integer;
  rooms1, rooms2, rooms3: integer;
  maps: array[1..6] of integer;
  mapI: integer;
  buffer: string;
  obj_healthitem,obj_shielditem,
  obj_teleportitem,obj_quaddmgitem,
  obj_pistolammoitem,obj_mchgunammoitem,
  obj_rocketammoitem : integer;
  tmpobj, tmpobj2: integer;
  snail: integer;
  isItemNow: boolean;

  key_t_stop: boolean;
  key_g_stop: boolean;
  key_o_stop: boolean;
  key_b_stop: boolean;
  key_p_stop: boolean;
  key_c_stop: boolean;
  key_x_stop: boolean;
  key_return_stop: boolean;
  
  prre_rh_iRenderPath: word;
  prre_rh_iOCmethod: word;
  prre_rh_bOCdrawBoundingBoxes: boolean;
  prre_rh_bOCdrawIfQueryPending: boolean;
  prre_rh_bOrderByDistance: boolean;


function messageProcessing: boolean;
var
  l: boolean;
begin
  l := PeekMessage(windmsg, 0, 0, 0, PM_REMOVE);
  if ( l ) then
    begin
      if ( windmsg.message = WM_QUIT ) then
        begin
          done := TRUE;
        end
       else
        begin
          TranslateMessage(windmsg);
          DispatchMessage(windmsg);
        end;
    end;
  result := l;
end;

procedure KeyBoard;

begin
  if ( inputIsKeypressed(VK_ESCAPE) ) then done := TRUE;
  if ( inputIsKeypressed(VK_UP) or inputiskeypressed(vkkeyscan('w')) ) then
    begin
      tmcsSetCameraX( tmcsGetNewX(tmcsGetCameraX(),tmcsGetCameraAngleY(), GAME_MOVESPEED_HOR) );
      tmcsSetCameraZ( tmcsGetNewZ(tmcsGetCameraZ(),tmcsGetCameraAngleY(), GAME_MOVESPEED_HOR) );
    end;
  if ( inputIsKeypressed(VK_DOWN) or inputiskeypressed(vkkeyscan('s')) ) then
    begin
      tmcsSetCameraX( tmcsGetNewX(tmcsGetCameraX(),tmcsGetCameraAngleY(), -GAME_MOVESPEED_HOR) );
      tmcsSetCameraZ( tmcsGetNewZ(tmcsGetCameraZ(),tmcsGetCameraAngleY(), -GAME_MOVESPEED_HOR) );
    end;
  if ( inputIsKeypressed(VK_LEFT) or inputiskeypressed(vkkeyscan('a')) ) then
    begin
      tmcsSetCameraX( tmcsGetNewX(tmcsGetCameraX(),tmcsGetCameraAngleY()-90, GAME_MOVESPEED_HOR) );
      tmcsSetCameraZ( tmcsGetNewZ(tmcsGetCameraZ(),tmcsGetCameraAngleY()-90, GAME_MOVESPEED_HOR) );
    end;
  if ( inputIsKeypressed(VK_RIGHT) or inputiskeypressed(vkkeyscan('d')) ) then
    begin
      tmcsSetCameraX( tmcsGetNewX(tmcsGetCameraX(),tmcsGetCameraAngleY()+90, GAME_MOVESPEED_HOR) );
      tmcsSetCameraZ( tmcsGetNewZ(tmcsGetCameraZ(),tmcsGetCameraAngleY()+90, GAME_MOVESPEED_HOR) );
    end;
  if ( inputiskeypressed(vkkeyscan('i')) ) then
    begin
      tmcsSetCameraY( tmcsGetCameraY()+GAME_MOVESPEED_VER );
    end;
  if ( inputiskeypressed(vkkeyscan('k')) ) then
    begin
      tmcsSetCameraY( tmcsGetCameraY()-GAME_MOVESPEED_VER );
    end;

  // next render path
  if ( inputiskeypressed(vkkeyscan('t')) ) then
    begin
      if ( not(key_t_stop) ) then
        begin
          key_t_stop := TRUE;
          prre_rh_iRenderPath := prre_rh_iRenderPath+1;
          if ( prre_rh_iRenderPath > 2 ) then
            prre_rh_iRenderPath := 0;

          tmcsSetRenderPath(prre_rh_iRenderPath);
        end;
    end
   else key_t_stop := FALSE;

  // prev render path
  if ( inputiskeypressed(vkkeyscan('g')) ) then
    begin
      if ( not(key_g_stop) ) then
        begin
          key_g_stop := TRUE;
          if ( prre_rh_iRenderPath > 0 ) then
            prre_rh_iRenderPath := prre_rh_iRenderPath-1
           else
            prre_rh_iRenderPath := 2;

          tmcsSetRenderPath(prre_rh_iRenderPath);
        end;
    end
   else key_g_stop := FALSE;

  // next occlusion culling method
  if ( inputiskeypressed(vkkeyscan('o')) ) then
    begin
      if ( not(key_o_stop) ) then
        begin
          key_o_stop := TRUE;
          if ( prre_rh_iOCmethod < 1 ) then
            prre_rh_iOCmethod := prre_rh_iOCmethod+1
           else
            prre_rh_iOCmethod := 0;

          tmcsSetOcclusionCullingMethod(prre_rh_iOCmethod);
        end;
    end
   else key_o_stop := FALSE;

  // draw occlusion culling bounding boxes of occludees
  if ( inputiskeypressed(vkkeyscan('b')) ) then
    begin
      if ( not(key_b_stop) ) then
        begin
          key_b_stop := TRUE;
          prre_rh_bOCdrawBoundingBoxes := not(prre_rh_bOCdrawBoundingBoxes);
          tmcsSetOcclusionCullingBoundingBoxes(prre_rh_bOCdrawBoundingBoxes);
        end;
    end
   else key_b_stop := FALSE;

  // draw occludees if their query is pending
  if ( inputiskeypressed(vkkeyscan('p')) ) then
    begin
      if ( not(key_p_stop) ) then
        begin
          key_p_stop := TRUE;
          prre_rh_bOCdrawIfQueryPending := not(prre_rh_bOCdrawIfQueryPending);
          tmcsSetOcclusionCullingDrawIfPending(prre_rh_bOCdrawIfQueryPending);
        end;
    end
   else key_p_stop := FALSE;

  // order objects by distance to camera
  if ( inputiskeypressed(vkkeyscan('c')) ) then
    begin
      if ( not(key_c_stop) ) then
        begin
          key_c_stop := TRUE;
          prre_rh_bOrderByDistance := not(prre_rh_bOrderByDistance);
          tmcsSetOrderingByDistance(prre_rh_bOrderByDistance);
        end;
    end
   else key_c_stop := FALSE;

  // reset engine statistics
  if ( inputiskeypressed(vkkeyscan('x')) ) then
    begin
      if ( not(key_x_stop) ) then
        begin
          key_x_stop := TRUE;
          tmcsResetStatistics();
        end;
    end
   else key_x_stop := FALSE;

  // engine dump to console
  if ( inputiskeypressed(VK_RETURN) ) then
    begin
      if ( not(key_return_stop) ) then
        begin
          key_return_stop := TRUE;
          tmcsEngineDump();
        end;
    end
   else key_return_stop := FALSE;
end;


procedure Mouse;
begin
  getCursorPos(ocur);
  mmx := ocur.x-mx;
  mmy := ocur.y-my;
  mouseToWndCenter;
  tmcsXRotateCamera(mmy*0.4);
  if ( tmcsGetCameraAngleX() > 90 ) then tmcsSetCameraAngleX(90);
  if ( tmcsGetCameraAngleX() < -90 ) then tmcsSetCameraAngleX(-90);
  tmcsYRotateCamera(mmx*0.3);
end;


begin
  prre_rh_iRenderPath := 2; // PRRE_RH_RP_OCCLUSION_CULLED
  prre_rh_iOCmethod := 1;   // async
  prre_rh_bOCdrawBoundingBoxes := false;
  prre_rh_bOCdrawIfQueryPending := true;
  prre_rh_bOrderByDistance := true;

  done := FALSE;
  showwindow(gamewindow.hwindow,SW_SHOW);
  setforegroundwindow(gamewindow.hwindow);
  setfocus(gamewindow.hwindow);
  while ( showcursor(FALSE) > -1 ) do ;
  mouseToWndCenter;
  tmcsEnableAmbientLight();
  tmcsSetAmbientLight(255,255,255);
  tmcsInitCamera(0,0,0, 0,0,0, 70, GAME_WIDTH/GAME_HEIGHT, 10, 10000);
  tmcsSetBgColor(220,220,255,255);
  //tmcsenabledebugging();
  tmcsSetExtObjectsTextureMode(true, GL_LINEAR_MIPMAP_LINEAR, GL_DECAL, false, false, GL_REPEAT, GL_REPEAT);

  tex_transp := tmcscreatetexturefromfile('..\_res\smoke.bmp',FALSE,FALSE,TRUE,GL_LINEAR,GL_DECAL,
                                             GL_CLAMP,GL_CLAMP);

      arena1 := tmcsCreateObjectFromFile('..\_res\bigstuff\arena1\arena1.obj', true);
      if ( arena1 > -1 ) then
        begin
          tmcsSetObjectLit(arena1, FALSE);
          tmcsSetYPos(arena1, -300);
        end
       else
        begin
          MessageBox(0,'arena1 = -1',ERRORDLG_BASE_TITLE,MB_OK or MB_ICONSTOP or MB_APPLMODAL);
        end;

      maps[1] := arena1;

      for mapI := 1 to 1 do
      begin
        for i := 0 to tmcsGetNumSubObjects(maps[mapI])-1 do
        begin
          buffer := tmcsgetsubname(maps[mapI],i);
          isItemNow := false;
          if ( pos('sp_',buffer) = 1 ) then
            begin
              isItemNow := true;
            end
           else if ( pos('teleport',buffer) = 1 ) then
            begin
              isItemNow := true;
            end
           else if ( pos('health',buffer) = 1 ) then
            begin
              isItemNow := true;
            end
           else if ( pos('shield',buffer) = 1 ) then
            begin
              isItemNow := true;
            end
           else if ( pos('quaddamage',buffer) = 1 ) then
            begin
              isItemNow := true;
            end
           else if ( pos('pistolammo',buffer) = 1 ) then
            begin
              isItemNow := true;
            end
           else if ( pos('mchgunammo',buffer) = 1 ) then
            begin
              isItemNow := true;
            end
           else if ( pos('rocketammo',buffer) = 1 ) then
            begin
              isItemNow := true;
            end;
          if ( isItemNow ) then
            begin
              tmpobj := tmcsCreatePlane(50, 50);
              tmcsSetYPos(tmpobj, tmcsGetSubYPos(maps[mapI],i)+tmcsGetYPos(maps[mapI]) + 20);
              if ( tmcsGetAngleY( maps[mapI] ) = 180 ) then
                begin
                  tmcsSetXPos(tmpobj, -tmcsGetSubXPos(maps[mapI],i)+tmcsGetXPos(maps[mapI]));
                  tmcsSetZPos(tmpobj, tmcsGetSubZPos(maps[mapI],i)+tmcsGetZPos(maps[mapI]));
                end
               else
                begin
                  tmcsSetXPos(tmpobj, tmcsGetSubXPos(maps[mapI],i)+tmcsGetXPos(maps[mapI]));
                  tmcsSetZPos(tmpobj, -tmcsGetSubZPos(maps[mapI],i)+tmcsGetZPos(maps[mapI]));
                end;
              tmcstextureobject(tmpobj, tex_transp);
              tmcssetobjectblending(tmpobj, TRUE);
              tmcssetobjectblendmode(tmpobj, gl_src_color,gl_one_minus_src_color);
              tmcssetobjectdoublesided(tmpobj, true);
              if ( i mod 2 = 0 ) then
                tmcssetobjectcolorkey(tmpobj,255,0,0,255)
              else
                tmcssetobjectcolorkey(tmpobj,0,255,255,255);
              tmcscompileobject(tmpobj);

              tmpobj2 := tmcsCreatePlane(50, 50);
              tmcsSetXPos(tmpobj2, tmcsGetXPos(tmpobj));
              tmcsSetYPos(tmpobj2, tmcsGetYPos(tmpobj));
              tmcsSetZPos(tmpobj2, tmcsGetZPos(tmpobj));
              tmcstextureobject(tmpobj2, tex_transp);
              tmcssetobjectblending(tmpobj2, TRUE);
              tmcssetobjectblendmode(tmpobj2, gl_src_color,gl_one_minus_src_color);
              tmcssetobjectdoublesided(tmpobj2, true);
              tmcsYRotateObject(tmpobj2, 90);
              if ( i mod 2 = 0 ) then
                tmcssetobjectcolorkey(tmpobj2,255,0,0,255)
              else
                tmcssetobjectcolorkey(tmpobj2,0,255,255,255);
              tmcscompileobject(tmpobj2);
              tmcsShowObject(tmpobj);
              tmcsShowObject(tmpobj2);

              tmcsHideSubObject(maps[mapI], i);
            end;
        end;
      end;
       
      fps_ms := 0;
      if ( GAME_MAXFPS > -1 ) then fps := GAME_MAXFPS
        else fps := 85;
      fps_old := 0;
      fps_ms_old := gettickcount();

      while ( not(done) ) do
      begin
        if ( not(messageprocessing) ) then
          begin
            if ( gamewindow.active ) then
              begin
                fps_ms := GetTickCount();
                tmcsRender();
                Mouse;
                Keyboard;
                fps_old := fps_old + 1;
                fps_ms2 := gettickcount() - fps_ms;
                fps_ms3 := gettickcount() - fps_ms;
              end  // gamewindow.active
             else
              begin
                Sleep(GAME_INACTIVE_SLEEP);
                tmcsRender();
                UpdateWindow(gamewindow.hwindow);
              end;
            if ( (fps_ms - fps_ms_old) >= GAME_FPS_INTERVAL ) then
              begin
                fps := round(fps_old*(1000/GAME_FPS_INTERVAL));
                fps_old := 0;
                fps_ms_old := fps_ms;
                SetWindowText(gamewindow.hwindow,pchar( inttostr(fps) + '  CAM: '+floattostr(tmcsGetCameraX())+'  '+floattostr(tmcsGetCameraY())+'  '+floattostr(tmcsGetCameraZ()) ) );
              end;
          end;  // not(messageprocessing)
      end;  // while ( not(done) ) do

  result := windmsg.wParam;
end;


{$R *.res}

begin

  { Játék ablak }
  with gamewindow do
    begin
      wc.style := CS_OWNDC;
      wc.lpfnWndProc := @WndProc;
      wc.cbClsExtra := 0;
      wc.cbWndExtra := 0;
      wc.hInstance := getmodulehandle(nil);
      wc.hIcon := Application.Icon.Handle;
      wc.hCursor := LoadCursor(0,IDC_ARROW);
      wc.hbrBackground := 0;
      wc.lpszMenuName := nil;
      wc.lpszClassName := GAMEWINDOW_CLASSNAME;
      if ( GAME_FULLSCREEN ) then
        begin
          dwStyle := WS_POPUP or WS_CLIPCHILDREN or WS_CLIPSIBLINGS;
          dwStyleEx := WS_EX_APPWINDOW;
        end
       else
        begin
          dwStyle := WS_POPUPWINDOW or WS_CAPTION or WS_MINIMIZEBOX;
          dwStyleEx := WS_EX_APPWINDOW or WS_EX_WINDOWEDGE;
        end;
    end;  // with gamewindow do
  if ( RegisterClass(gamewindow.wc) <> 0 ) then
    begin
      gamewindow.clientsize.X := GAME_WIDTH;
      gamewindow.clientsize.Y := GAME_HEIGHT;
      if ( GAME_FULLSCREEN ) then
        begin
          gamewindow.wndrect.Left := 0;
          gamewindow.wndrect.Top := 0;
        end
       else
        begin
          gamewindow.wndrect.Left := GetSystemMetrics(SM_CXSCREEN) div 2 - gamewindow.clientsize.X div 2;
          gamewindow.wndrect.Top := GetSystemMetrics(SM_CYSCREEN) div 2 - gamewindow.clientsize.Y div 2;
        end;
      gamewindow.wndrect.Right := gamewindow.wndrect.Left+gamewindow.clientsize.X;
      gamewindow.wndrect.Bottom := gamewindow.wndrect.Top+gamewindow.clientsize.Y;
      gamewindow.wndrect.TopLeft.X := gamewindow.wndrect.Left;
      gamewindow.wndrect.TopLeft.Y := gamewindow.wndrect.Top;
      gamewindow.wndrect.BottomRight.X := gamewindow.wndrect.Right;
      gamewindow.wndrect.BottomRight.Y := gamewindow.wndrect.Bottom;
      gamewindow.cdepth := GAME_CDEPTH;
      if ( GAME_FULLSCREEN ) then gamewindow.hwindow := CreateWindowEx(gamewindow.dwStyleEx,GAMEWINDOW_CLASSNAME,PRODUCTNAME+' '+PRODUCTVERSION,gamewindow.dwStyle,0,0,gamewindow.wndrect.Right,gamewindow.wndrect.Bottom,0,0,gamewindow.wc.hInstance,nil)
        else
       begin
         adjustwindowrectex(gamewindow.wndrect,gamewindow.dwStyle,FALSE,gamewindow.dwStyleEx);
         gamewindow.hwindow := CreateWindowEx(gamewindow.dwStyleEx,GAMEWINDOW_CLASSNAME,PRODUCTNAME+' '+PRODUCTVERSION,gamewindow.dwStyle,gamewindow.wndrect.Left,gamewindow.wndrect.Top,gamewindow.wndrect.Right-gamewindow.wndrect.Left,gamewindow.wndrect.Bottom-gamewindow.wndrect.Top,0,0,hInstance,nil);
       end;
      if ( gamewindow.hwindow <> 0 ) then
        begin
          if ( MessageBox(gamewindow.hwindow, 'Logging ?', 'Logging ?', MB_YESNO) = IDYES ) then
            tmcsEnableDebugging();
            
          if ( MessageBox(gamewindow.hwindow, 'HW render ?', 'HW render ?', MB_YESNO) = IDYES ) then
            tmcsstatus := tmcsInitGraphix(gamewindow.hwindow,GAME_FULLSCREEN,255,GAME_CDEPTH,GAME_ZDEPTH,TRUE,GL_SMOOTH)
          else
            tmcsstatus := tmcsInitGraphix(gamewindow.hwindow,GAME_FULLSCREEN,255,GAME_CDEPTH,GAME_ZDEPTH,TRUE,GL_FLAT);
            
          if ( tmcsstatus = 0 ) then
            begin
              WinMain(hInstance,hPrevInst,CmdLine,CmdShow);
              showcursor(TRUE);
              tmcsRestoreOriginalDisplaymode();
              tmcsShutDownGraphix();
              DestroyWindow(gamewindow.hwindow);
              UnregisterClass(GAMEWINDOW_CLASSNAME,gamewindow.wc.hInstance);
            end  // tmcsstatus = 0
           else
            begin
              MessageBox(gamewindow.hwindow,ERROR_TMCSINITGRAPHIX,ERRORDLG_BASE_TITLE,MB_OK or MB_ICONSTOP or MB_APPLMODAL);
              DestroyWindow(gamewindow.hwindow);
              UnregisterClass(GAMEWINDOW_CLASSNAME,gamewindow.wc.hInstance);
            end;
        end  // gamewindow.hwindow <> 0
       else
        begin
          MessageBox(0,ERROR_CREATEWINDOWEX,ERRORDLG_BASE_TITLE,MB_OK or MB_ICONSTOP or MB_APPLMODAL);
          UnregisterClass(GAMEWINDOW_CLASSNAME,gamewindow.wc.hInstance);
        end;
    end  // RegisterClass(gamewindow.wc) <> 0
   else
    begin
      MessageBox(0,ERROR_REGISTERCLASS,ERRORDLG_BASE_TITLE,MB_OK or MB_ICONSTOP or MB_APPLMODAL);
    end;
end.
