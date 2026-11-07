# Laboratorijska vežba 2 – Generički kaktus

## Opis zadatka
Cilj vežbe je iscrtavanje generičkog kaktusa dimenzija 500×500 piksela korišćenjem GDI funkcija, metafajlova i svetskih transformacija.

## Implementacija

- **Translate()** – translacija objekta po X i Y osi.
- **Scale()** – skaliranje objekta po X i Y osi.
- **Rotate()** – rotacija objekta za zadati ugao.
- **DrawFigure()** – iscrtavanje kompletne figure korišćenjem transformacija i pomoćnih funkcija.
- **Metafajlovi** – korišćenje EMF resursa za iscrtavanje bodljikavih delova kaktusa.
- **Promenljiva širina** – prilagođavanje širine delova kaktusa u zavisnosti od broja grana.

## Dodatne funkcionalnosti

- Uključivanje i isključivanje mreže (grid) pomoću tastature.
- Rotiranje svetlijih delova kaktusa ulevo i udesno pomoću tastature.

## Tehnička realizacija

Sve dodatne funkcije implementirane su kao metode View klase. Transformacije podržavaju izbor redosleda množenja transformacione matrice (`rightMultiply`).