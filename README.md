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
3. Stiahnite a extrahujte súbor `AIS.zip`
4. Otvorte Terminal a napíšte: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<PATH_TO_EXTRACTED_FOLDER>/AIS/lib64/`
5. Nakoniec navigujte do priečinka `<PATH_TO_EXTRACTED_FOLDER>/AIS/bin/`a spustite program pomocou `./AIS`

## Používanie
