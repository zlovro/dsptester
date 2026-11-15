# Sustav za prikaz teksta
Koristi se 256-znakovni font gdje je svaki znak crno-bijela slika veličine 8x8 piksela. 
Znakovi su spremljeni u niz ST7920_Font. Jedan znak zauzima 9 bajtova. 
- Struktura znaka:
  - prvi bajt:
    - bitovni raspored xxEEESSS
    - EEE = zadnji stupac znaka koji sadrži popunjene piksele
    - SSS = prvi stupac znaka koji sadrži popunjene piksele

Prvih 128 znakova se tretiraju kao tekst, to jest, razmak između znakova nije fiksan (monospace).
Drugih 128 znakova se tretiraju kao ikone te je razmak između 2 ikone fiksan pri njihovu prikazivanju na ekranu.

Također je bitno napomenuti kako se prva 32 znaka koriste kao znakovi posebne namjene, na primjer:
- Znakovi 16 do 32 su znamenke od 0 do 9, ali maksimalne veličine 4x4 piksela