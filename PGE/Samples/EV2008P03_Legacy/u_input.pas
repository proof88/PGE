unit u_input;

interface

uses
  sysutils,
  classes,
  controls,
  windows;

type
  TMouseButton = (MBTN_LEFT,MBTN_MIDDLE,MBTN_RIGHT);

procedure inputInitialize;
function  inputIsKeyPressed(vcode: integer): boolean;
procedure inputSetKeyPressed(wp: wparam);
procedure inputSetKeyReleased(wp: wparam);
function  inputIsMouseButtonPressed(btn: TMouseButton): boolean;
procedure inputSetMouseButtonPressed(btn: TMouseButton);
procedure inputSetMouseButtonReleased(btn: TMouseButton);
procedure inputMouseWheelRotation(value: smallint);
function inputGetMouseWheelScrollCount(): integer;


implementation

type
  TKeyboard = record
                keys: array[0..255] of boolean;
              end;
  TMouse = record
             leftdown: boolean;
             middledown: boolean;
             rightdown: boolean;
             zdelta: integer;
           end;

var
  keyboard: TKeyBoard;
  mouse: TMouse;

procedure inputInitialize;
var
  i: byte;
begin
  for i := 0 to 255 do
    keyboard.keys[i] := FALSE;
  mouse.leftdown := FALSE;
  mouse.middledown := FALSE;
  mouse.rightdown := FALSE;
end;

function inputIsKeyPressed(vcode: integer): boolean;
begin
  result := keyboard.keys[vcode];
end;

procedure inputSetKeyPressed(wp: wparam);
begin
  keyboard.keys[wp] := TRUE;
end;

procedure inputSetKeyReleased(wp: wparam);
begin
  keyboard.keys[wp] := FALSE;
end;

function inputIsMouseButtonPressed(btn: TMouseButton): boolean;
begin
  case btn of
    MBTN_LEFT   : result := mouse.leftdown;
    MBTN_MIDDLE : result := mouse.middledown;
    MBTN_RIGHT  : result := mouse.rightdown;
    else result := FALSE;
  end;
end;

procedure inputSetMouseButtonPressed(btn: TMouseButton);
begin
  case btn of
    MBTN_LEFT   : mouse.leftdown := TRUE;
    MBTN_MIDDLE : mouse.middledown := TRUE;
    MBTN_RIGHT  : mouse.rightdown := TRUE;
  end;
end;

procedure inputSetMouseButtonReleased(btn: TMouseButton);
begin
  case btn of
    MBTN_LEFT   : mouse.leftdown := FALSE;
    MBTN_MIDDLE : mouse.middledown := FALSE;
    MBTN_RIGHT  : mouse.rightdown := FALSE;
  end;
end;

procedure inputMouseWheelRotation(value: smallint);
begin
  mouse.zdelta := mouse.zdelta + value;
end;

function inputGetMouseWheelScrollCount(): integer;
begin
  if ( (mouse.zdelta mod WHEEL_DELTA) = 0 ) then
    begin
      result := mouse.zdelta div WHEEL_DELTA;
      mouse.zdelta := 0;
    end;
end;

end.
