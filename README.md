# AIS
Jednoduchý akademický informačný systém, ktorý zhromažďuje informácie
o študentoch, predmetoch a výsledkoch.

Program bol vyvinutý na platforme `Fedora Linux 39`, pomocou frameworku `QT 6.6.1`. Pripája sa na server (localhost), kde má `relačnú databázu`, ktorá uchováva všetky informácie.
## Spustenie

1. Nainštalujte nasledovné prerekvizity (testované Ubuntu 22.04):
   - `sudo apt install libxcb-cursor0`
   - `sudo apt install mysql-server`
   - `sudo apt install mysql-client`
   - `sudo apt install libmysqlclient-dev`
2. Vytvorte nového "admin" užívateľa pre mysql databázu:
   ```
   $ sudo mysql -u root -p (2x pyta heslo, 1. krat pre sudo, 2. krat pre root-a do mysql databazy, co je po instalacii to iste, ako pre sudo)
   (alebo bolo potrebne sa predtym prepnut do roota cez sudo su a potom stacilo # mysql) - to nas tiez prihlasi ako root
   mysql> CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin';
   mysql> GRANT ALL PRIVILEGES ON *.* TO 'admin'@'localhost' WITH GRANT OPTION;
   mysql> FLUSH PRIVILEGES;
   mysql> quit;
   ```
3. Vytvorte novú databázu:
   ```
   $ mysql -u admin -p
   mysql> create database Faculty;
   mysql> show databases;
   mysql> quit; 
   ```
4. Nainštalujte `phpMyAdmin`: `sudo apt install phpmyadmin`
5. V prehliadači otvorte `http://localhost/phpmyadmin/` a zadajte používateľské meno `admin` a heslo `admin`
6. Navigujte do novovytvorenej databázy Faculty a importujte súbor `Faculty.sql`, ktorý načíta existujúcu štruktúru a záznamy v databáze. (netestované)
7. Takisto importujte nasledovné do sekcie `TRIGGERS`:
   ```
   CREATE TRIGGER `setName` BEFORE INSERT ON `Students`
   FOR EACH ROW SET NEW.name := CONCAT(NEW.first_name, CONCAT(" ", NEW.last_name))
   
   CREATE TRIGGER `setNameU` BEFORE UPDATE ON `Students`
   FOR EACH ROW SET NEW.name := CONCAT(NEW.first_name, CONCAT(" ", NEW.last_name))
   ```
9. Stiahnite a extrahujte súbor `AIS.tar.xz`
10. Otvorte Terminal a napíšte: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<PATH_TO_EXTRACTED_FOLDER>/AIS/lib64/`
11. Nakoniec navigujte do priečinka `<PATH_TO_EXTRACTED_FOLDER>/AIS/bin/`a spustite program pomocou `./AIS`

## Používanie
### Prehľadový režim

![image](https://github.com/kristiangreif/AIS/assets/62846329/fe425f0d-d6fc-4943-ac09-8e26b52ca37b)

Po spustení sa zobrazí okno s prehľadmi študentov, predmetov a známok - tento režim je prednastavený a voliteľný tlačidlom hore vpravo `Overview Mode`.

V tomto režime program ponúka tri záložky, zodpovedajúce jednotlivým prehľadom. Záznamy v prehľadoch nemožno pridávať, odoberať, ani upravovať. Užívateľ však môže využiť dva filtre, ktoré platia súčasne (napr. študenti, ktorí študujú daný predmet a majú danú známku). Takisto je možné zoradiť údaje podľa zvoleného stĺpca - kliknutím na jeho hlavičku. V tomto režime sa zobrazujú vybrané údaje z viacerých databázových tabuliek.

### Režim úprav
Hore vľavo sa nachádza tlačidlo `Edit Mode`, ktoré užívateľa prepne do režimu úprav.

![image](https://github.com/kristiangreif/AIS/assets/62846329/11302e5a-2bea-4a60-b387-ee59371941b4)

V tomto režime má užívateľ k dispozícii filter jednej kategórie, možnosť usporiadania podľa zvoleného stĺpca a možnosť pridávať (`ADD`), odstraňovať (`DELETE`) a upravovať (klik na políčka tabuľky) záznamy. Tlačidlo `REVERT` vráti doposiaľ zaznamenané zmeny na tabuľke, kým nie je stlačené tlačidlo `SUBMIT`, ktoré definitívne vykoná zaznamenané zmeny na databáze.

Taktiež sú tu tri záložky, každá s jednou databázovou tabuľkou (`Students`, `Courses`, `Evaluation`), pričom záložka `Evaluation` sprístupňuje relačnú tabuľku, kde je vyjadrený vzťah medzi študentmi, predmetmi a známkami.

### Štatistika
V dolnej časti pod tabuľkou sa v každej karte zobrazuje počet vyfiltrovaných výsledkov - je tak možné zistiť celkový počet študentov a predmetov, prípadne pomocou filtrov zistiť počty požadovaných študentov, predmetov alebo známok. V prípade karty `Student Overview` sa zobrazí vždy počet jedinečných výsledkov, teda ak chceme zisitiť počet študentov s danou známkou, no jeden študent získal z rôznych predmetov požadovanú známku, zaráta sa iba raz!

## Problémy
- Neošetrené vstupy pri úprave pridávaní záznamov do databázy
- Neošetrená možná duplicita záznamov (študent môže dostať z rovnakého predmetu dve rôzne známky)
- Možno filtrovať na základe skrytých stĺpcov
- Veľká duplicita vytvorených tried - možné využitie dedenia alebo spoločnej triedy pre základ UI
- Veľmi základné používateľské rozhranie

## Problémy pri tvorbe
- Problém s načítaním vhodných knižníc pre `MYSQL DRIVER` (bola potrebná špecifická verzia)
- Limitovaná funkcionalita dostupných tried pre prácu s modelom tabuľky/relačnej tabuľky/query
- Problémy pri deployovaní - chýbajúce knižnice, cesty...
