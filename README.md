# AIS
Jednoduchý akademický informačný systém, ktorý zhromažďuje informácie
o študentoch, predmetoch a výsledkoch.

Program bol vyvinutý na platforme Fedora Linux 39, pomocou frameworku QT 6.6.1. Pripája sa na server (localhost), kde má relačnú databázu, ktorá uchováva všetky informácie.
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
6. Navigujte do novovytvorenej databázy Faculty a importujte súbor `Faculty.sql`, ktorý načíta existujúcu štruktúru a záznamy v databáze.
7. Stiahnite a extrahujte súbor `AIS.zip`
8. Otvorte Terminal a napíšte: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<PATH_TO_EXTRACTED_FOLDER>/AIS/lib64/`
9. Nakoniec navigujte do priečinka `<PATH_TO_EXTRACTED_FOLDER>/AIS/bin/`a spustite program pomocou `./AIS`

## Používanie
