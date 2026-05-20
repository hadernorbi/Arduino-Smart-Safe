# Okos Széf Projekt (Arduino)

Ez egy biztonsági széf prototípus, amely egy mikrokontroller alapú beágyazott rendszer. A projekt ötvözi a digitális adatbevitelt, a vizuális állapot-visszajelzést és az elektromechanikus zárvezérlést egy integrált hardver-szoftver architektúrában.

---

## 1. Hardver felépítés és komponensek

### Vezérlőegység és tápellátás
A projekt központi eleme egy **Arduino Uno R3** mikrokontroller. Ez az egység felel az összes periféria (billentyűzet, LCD, szervó) összehangolásáért és a logikai műveletek végrehajtásáért. Mivel több modul is igényelt 5V-os tápfeszültséget, a stabil áramellátást egy Breadboard segítségével oldottam meg. Az Arduino 5V és GND kivezetéseit a Breadboard tápsínjeire kötöttem, így párhuzamos kapcsolással minden alkatrész zavartalanul kapta meg a működéséhez szükséges energiát, elkerülve a feszültségeséseket.

### Adatbeviteli és kijelző modulok
A felhasználói interakció két fő komponensen keresztül valósul meg. A bevitelt egy **4x4-es mátrix billentyűzet** biztosítja, amely 8 digitális lábat foglal el az Arduinón; a szoftver folyamatosan pásztázza a sorok és oszlopok kereszteződéseit a gombnyomások érzékeléséhez. Vizuális visszajelzésként egy **16x2-es karakteres LCD kijelző** szolgál. A rendszer hatékonysága érdekében egy **I2C interfész modult** használtam, amely a soros adatátvitelnek köszönhetően mindössze két adatvonalat (SDA, SCL) igényel, jelentősen leegyszerűsítve a kábelezést és szabadon hagyva a digitális pineket.

### Elektromechanikai zár
A fizikai biztonságért egy **SG90 micro szervomotor** felel. Ez az aktuátor PWM (impulzusszélesség-moduláció) jelek segítségével vezérelhető, lehetővé téve a tengely pontos pozicionálását 0° és 180° között. A projektben a szervó tölti be a zár szerepét: a szoftver utasítására elfordulva egy karton papírt mozgat(enged kinyilni és becsukódni). Alacsony áramfelvétele és kompakt mérete ideálissá teszi a prototípus szintű implementációhoz.

### Teljes eszközlista
* **Vezérlés:** Arduino Uno R3 mikrokontroller
* **Bevitel:** 4x4 Mátrix Billentyűzet
* **Megjelenítés:** I2C LCD Kijelző (16x2, kék háttérvilágítással)
* **Aktuátor:** SG90 Szervomotor (9g)
* **Összeköttetés:** Breadboard (hosszanti tápsínekkel) és Apa-Apa / Anya-Apa Jumper kábelek
* **Váz:** Karton doboz (burkolat)

---

## 2. Szoftveres logika és működés

### Programkönyvtárak integrálása
A fejlesztés során több külső programkönyvtárat használtam a hardveres absztrakció érdekében. 
* A `<Keypad.h>` felel a mátrix billentyűzet gombjainak lefordításáért karakterekre a beolvasási mátrix alapján.
* A `<LiquidCrystal_I2C.h>` biztosítja a kijelző inicializálását és a szöveges adatok küldését az I2C buszon keresztül.
* A `<Servo.h>` pedig a motor vezérléséhez szükséges precíz időzítésű impulzusokat generálja a háttérben.

Ezek az előre megírt kódcsomagok lehetővé tették, hogy a fejlesztés során ne a regiszter szintű vezérlésre, hanem a széf logikai működésére koncentrálhassak.

### PIN kód kezelési algoritmus
A biztonság alapja egy string-alapú összehasonlító algoritmus. A program egy `inputCode` nevű változóba gyűjti a billentyűzeten beütött karaktereket. A kód minden gombnyomás után frissíti a kijelzőt, de a biztonság érdekében nem a tényleges számokat, hanem csillagokat (`*`) jelenít meg. A bevitel lezárása és kiértékelése a 'D' (Enter) gombbal történik, ekkor az algoritmus összeveti a beírt sorozatot a flash memóriában előre definiált `masterCode` ("1234") értékkel.

### Vezérlési folyamatok
A program egy végtelenített ciklusban (`void loop()`) fut, folyamatosan figyelve a felhasználói beavatkozást. Sikeres azonosítás esetén a mikrokontroller kiadja a nyitási parancsot a szervónak (90 fokos elmozdulás), majd egy `delay(5000)` parancs segítségével 5 másodpercig nyitott állapotban tartja a rendszert, lehetőséget adva az ajtó kinyitására. Ezt követően a motor automatikusan visszatér a zárt pozícióba (0 fok). A folyamat tartalmaz egy beépített javítási funkciót is: a '#' gomb megnyomásával a felhasználó bármikor alaphelyzetbe állíthatja a beviteli puffert, ha elrontotta a kódot gépelés közben.

---

## 3. Mechanikai kivitelezés

### Strukturális kialakítás
A prototípus burkolata egy merevített falú kartondoboz, amely elegendő belső teret biztosít az elektronika és a zárómechanika számára. A komponensek elhelyezésekor különös figyelmet fordítottam a stabil rögzítésre: a mikrokontroller és a breadboard a dobozban kapott helyet. A kábelek elvezetése során törekedtem a rendezettségre (amennyire lehetett, bár nagyon pici volt a dobozom), hogy elkerüljem a vezetékek kilazulását vagy a kábelek beakadását a mozgó elektromechanikus alkatrészekbe.

### Zárrendszer implementálása
A zárrendszer egy egyedi tervezésű "sorompó" elven alapul. A szervomotor tengelyére rögzített gyári fehér műanyag kart mozgatom. Amikor a szervó zárt pozícióban van (0°), a pálcika függőlegesen elhelyezkedve elreteszeli az utat. Nyitáskor a pálcika 90 fokban oldalra fordul, így a fül szabadon elmozdulhat. Ez a megoldás hatékonyan demonstrálja a digitális szoftveres parancs fizikai gáttá alakítását.

### Ergonómia és Felhasználói Felület (UI)
A felhasználói felületet a doboz tetején alakítottam ki a letisztult használhatóság érdekében:
* Az **LCD kijelző** is a doboz tetejér került került, hogy a státuszüzenetek (pl. "SZEF READY!", "PIN:") azonnal olvashatóak legyenek.
* A **billentyűzetet** öntapadós felülettel rögzítettem a doboz külsejére, így a kezelése stabil, a szalagkábel pedig egy vékony, vágott résen keresztül, éles törés nélkül jut be a belső vezérlőhöz.
* Az **USB tápcsatlakozó** számára egy külön nyílást vágtam a doboz oldalán, így a széf folyamatos áramellátása biztosított anélkül, hogy a szerkezetet fel kellene nyitni. 
Nyáron még szeretném használni más projektekre is majd, ugyhogy nem szerettem volna ténylegesen leragasztani, nagyon megtetszett a folyamat, ugyhogy még pár dolgot majd meg szeretnék valósítani vele.

---

## 4. A fejlesztés menete lépésről lépésre

Ebben a fejezetben összeszedtem, hogyan jutottam el az asztalon heverő alkatrészhalmaztól a kész, működő széf dobozig. A folyamat nem volt zökkenőmentes, de a végére minden összeállt. (Ez volt az első projektem, szóval a 0-ról kezdtem).

### 1. fázis: A "Gubanc" az asztalon (Az áramkör összerakása)
Az első lépés az volt, hogy az alkatrészeket életre keltsem a breadboardon. Először a billentyűzetet kötöttem be, amihez 8 kábel kellett, így hirtelen elég nagy lett a káosz. Ezután jött az LCD kijelző. Itt óriási segítség volt az I2C modul a hátulján, mert az eredeti 6-8 kábel helyett mindössze 4 vezetékkel (5V, GND, SDA, SCL) le tudtam tudni a teljes kijelzőt. Végül rákötöttem a szervomotort is a 10-es pinre.

### 2. fázis: A kód megírása és a szoftveres "nyomozás"
Miután minden be volt dugva, megírtam a programot az Arduino IDE-ben. Beolvastam a gombokat, és beállítottam, hogy ha a gépelt kód megegyezik a "1234"-gyel, akkor adjon jelet a motornak. Itt jött az első nagyobb elakadás: az LCD kijelző bekapcsolt, de nem írt ki semmit, teljesen üres maradt. 

Először azt hittem, rossz a kódom, ezért átírtam az I2C címet `0x27`-ről `0x3F`-re, de ez sem segített. Végül rájöttem, hogy a kijelző hátulján lévő pici kék csavaron kell rengeteget tekerni, mert a kontraszt miatt a betűk egyszerűen "láthatatlanok" voltak. Amint megtekertem, azonnal megjelent a "SZEF READY" felirat.

### 3. fázis: Az áramellátás rejtélye
A következő probléma az volt, hogy a szervó bekötése után az LCD elkezdett vibrálni vagy teljesen kikapcsolni. Kis nyomozás után észrevettem, hogy a breadboardomon a piros és kék tápvonal középen fizikailag meg van szakadva. Emiatt az áram nem jutott el a tábla egyik feléből a másikba, és a motor megette az összes energiát a kijelző elől. Fogtam egy-egy jumper kábelt, és onnantól kezdve stabilan működött a teljes rendszer.

### 4. fázis: Barkácsolás és a zár összeszerelése
A hardver után jött a doboz átalakítása. Kivágtam a helyet az LCD-nek és a billentyüzet kábeleinek is. Majd a szervo motor kábelének is.

---

## 5. Tesztelés és eredmények

### Funkcionális ellenőrzés
A tesztelési fázisban az alábbi három fő esetet ellenőriztem szisztematikusan:
1.  **Helyes PIN kód tesztje:** A billentyűzeten az "1234" beírása után a 'D' megnyomására az LCD-n megjelent a "KOD JO! NYITAS..." felirat, a szervó sikeresen elfordult, majd 5 másodperc után visszazárt.
2.  **Helytelen PIN kód tesztje:** Rossz számsorozat megadása után a 'D' gombra a kijelzőn a "ROSSZ KOD!" figyelmeztetés jelent meg, a motor pedig teljesen mozdulatlan maradt, igazolva a védelmi funkciót.
3.  **Puffer törlés tesztje:** Hibás gépelés közben a '#' gomb megnyomására a kijelző sikeresen törlődött, és a puffer visszaállt a kezdőállapotba.

### Hibaelhárítás (Debugging)
A fejlesztés nem volt zökkenőmentes, az alábbi hardveres és szoftveres hibákat kellett elhárítani:
* **LCD Kontraszt és Címzés:** A kijelző kezdetben világított, de nem mutatott karaktereket. Ezt a hátoldali kék potenciométer csavarjának finomhangolásával és a kódban az I2C cím `0x27`-re való pontosításával oldottam meg.
* **Breadboard Tápellátási Hiba:** A szervó bekötése után a rendszer instabillá vált, mert a használt breadboard tápsínje középen meg volt szakadva. Ezt egy fizikai áthidaló Jumper kábel behelyezésével hárítottam el, így az áram eljutott az LCD-hez is.
* **Mechanikai igazítás:** A szervó pálcikája kezdetben szorult az ajtó fülében. A rögzítési magasság minimális korrekciójával értem el a teljesen sima, akadásmentes nyitást és zárást.

### Összegzés és fejlesztési lehetőségek
A projekt sikeresen teljesítette a kitűzött technikai és funkcionális célokat. Egy teljesen működőképes, interaktív és fizikai akadályt képező széfprototípus jött létre. Hozzáteszem, hogyha leragasztom sokkal jobb és szebb lenne, de még szeretném a jövőben használni az eszközöket.

A rendszer jövőbeli továbbfejlesztési irányai:
1.  **Nem-felejtő memória (EEPROM):** Olyan kódmódosító funkció beépítése (pl. a 'C' gombbal), amely áramszünet vagy az USB kihúzása után is megőrzi a felhasználó által újonnan beállított PIN kódot.
2.  **Riasztási modul:** Egy piezó zümmer (buzzer) vagy piros LED beépítése, ami 3 hibás kódbevitel után hang- és fényjelzéssel riaszt, valamint időzárat aktivál a rendszeren.

---

## Mellékletek

### Képek a folyamatról
Csatoltam külön IMG fileokban

### Videó a működésről
A projekt működését bemutató videót is feltöltöttem.

### Arduino IDE-ben lévő kód
A széf működéséhez szükséges kódot is csatoltam az okos_safe.ino nevű fájlban
