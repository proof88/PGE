
A v0.1.2-ben a szerver még a mp-enkénti képkockaszámnak (framerate) megfelelő gyakorisággal
számolta a fizikát és küldte ki a klienseknek a frissítéseket. A framerate 60 FPS-nél volt/van maximalizálva (egyelőre).

Bár bevezettem a framerate-től független fizika számolást, ez kezdetben a naiv implementáció volt, ahogy az eredeti
PR00FPS is csinálta: a fizikát változó időeltérések (variable delta time) alapján számolta, azaz: minden mozgó objektumot
a legutóbbi mozgatás óta eltelt idővel arányosan mozgatunk el, ami több szempontból is rossz: egyrészt
ha túl nagy a delta time (alacsony FPS esetén), akkor a mozgó objektumok ütközésdetektálásánál lehet olyan, hogy
a mozgó objektum "átugrik" egy objektumot, pl. lövedék egy falat. Másrészt pedig a lebegőpontos számolás sajátossága
miatt szemmel ugyan nem feltűnő de alapvetően hibás, hogy egy játékos pontosan ugyanolyan input sorozat után kicsit
más mozgássorozatot fog látni fix illetve nagyon ingadozó framerate esetén (bár a lebegőpontos ábrázolásból adódó
hibák nagyon kicsik, és ezért egyébként szemmel kb láthatatlan különbségekről van szó, azért el tudjuk képzelni, hogy
ha egy replay fájlba csak a játékosok inputját mentenénk, és a játék a mentett inputok alapján csinálja a visszajátszást, akkor
egy hosszabb replay visszajátszása nem pont ugyanazt az eredményt fogja adni, mint ami a valódi játékmenet volt!).

A v0.1.3-ban aztán már fix időeltérések (fix delta time) alapján számolja a szerver a fizikát, azaz mindegy mennyi idő telt
el a legutóbbi frissítés óta, ha kell akkor több kisebb lépésben számol alacsony FPS esetén, így a fenti problémák megoldódnak.

Szintén ebben a verzióban bevezetett tickrate egy fontos fejlesztés volt, mert így a framerate-től függetlenül
lehet állítani szerver oldalon a fizikai számolások és a klienseknek küldött frissítések frekvenciáját.
Itt gyakorlatilag azt is mondjuk, hogy a fix delta time az = 1000 millisec / tickrate.

Hosszú távon 60 Hz helyett 20 Hz-et szeretnék tickrate-nek, mert az a véleményem, hogy elég mp-enként 60 helyett 20x
számolni a fizikát és értesíteni az eredményekről a klienseket.

A 60 helyett 20 Hz gyakorlati negatív hatásait láthatták a játékosok is a legutóbbi LAN partin:
 - láthatóan szaggatott mozgás: elég egyértelmű hogy ha mp-enként csak 20x frissül kliens oldalon a játékosok pozíciója, akkor
az nem tekinthető folyamatos mozgásnak, viszont ezt meg fogja oldani a kliens oldali lineáris interpoláció, ezért ezt nem tekintem problémának;
 - bizonyos dobozokra nem lehetett felugrani: ez nagyobb gond, és pont az okozza, hogy a fix delta time 20 Hz esetén túl nagy.

Én egyébként kipróbáltam 30 Hz-es tickrate-en, az pl megoldotta ezt a dobozra ugrás problémát, de cserébe 2 közeli doboz közé
nem lehetett futás közben beesni. :D

Ezért egész egyszerűen a v0.1.5-ben bevezettem a cl_updaterate-et, ami a szerver->kliens frissítések frekvenciáját állítja, azaz
meghagyhatom 60 Hz-en a tickrate-et, azaz a fizikai számolások mp-enként 60x lesznek számolva, de a cl_updaterate lehet 20 Hz, azaz
csak minden 3. tickben lesz hálózati forgalom a kliensek felé.

Így a kecske is jól fog lakni, és a káposzta is megmarad.

Viszont egy tick során nem csak fizikai számolások történnek, hanem egyéb dolgok is, amikre továbbra is soknak érzem a 60-at.
Ezért, hogy még jobban finomhangolható legyen a rendszer, bevezettem a physics_rate_min-t, ami megmondja, hogy a játéknak mi az elvárása
a fizikai számolások MINIMUM frekvenciáját illetően: ha pl 40-et adunk meg, tickrate-nek viszont csak 20-at, akkor tick-enként
2 lépésben lesz számolva a fizika, tehát 20 Hz-es tickrate-tel is 40 Hz-en lesz számolva a fizika. A cl_updaterate ezektől
független marad a továbbiakban is.

Szóval végeredményben már van 3 állítható rate paraméter, amik framerate-től függetlenek.



volt:
tickrate: játékosokra és lövedékekre ható fizika, valamint szerver->kliens frissítések frekvenciája
tickrate 60 Hz : default
tickrate 20 Hz : harmadannyi fizika és szerver->kliens traffic

regression tesztek:
1.) tickrate 60
2.) tickrate 20

tickrate csökkentés előnyei:
 - szerver->kliens hálózati forgalom csökkentése, szerver oldali CPU használat csökkentése

tickrate csökkentés hátrányai:
 - fizika pontossága csökken


lett:
tickrate         -> játékosokra és lövedékekre ható fizika
cl_updaterate    -> szerver->kliens frissítések

A tickrate csökkentés előnyei a szerver oldali CPU használat csökkentése, a hátrány ugyanaz.
Viszont már nem kell csökkenteni, hogy csökkenjen a szerver->kliens adatforgalom.
Meg lehet tartani a pontos fizikai eredményeket adó 60 Hz-et, míg a szerver->kliens frissítéseket külön lehet csökkenteni 20 Hz-re.

regression tesztek:
1.)
tickrate 60
cl_updaterate 60

2.)
tickrate 60
cl_updaterate 20

3.)
tickrate 20
cl_updaterate 20



lett:
tickrate         -> legalább hányszor adunk lehetőséget a szervernek elvégezni a játékállapot frissítést
physics_rate_min -> játékosokra és lövedékekre ható fizika minimum frekvencia
cl_updaterate    -> szerver->kliens frissítések


regression tesztek:
1.)
tickrate 60
physics_rate_min 60
cl_updaterate 60

2.)
tickrate 60
physics_rate_min 60
cl_updaterate 20

3.)
tickrate 20
physics_rate_min 60
cl_updaterate 20
