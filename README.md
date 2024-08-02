# Projekt Przetwarzania Obrazów

## Opis

Projekt ten służy do przetwarzania obrazów w trybie tekstowym. Program pozwala na wykonanie różnych operacji na obrazach, takich jak negatyw, konturowanie, progowanie, korekcja gamma i inne. Umożliwia także zapis wyników do pliku wyjściowego lub wyświetlanie ich na ekranie.

## Pliki

- `main.cpp`: Główny plik źródłowy programu.
- `modul.h`: Nagłówek zawierający definicje struktur oraz deklaracje funkcji używanych w projekcie.
- `modul.cpp`: Plik źródłowy implementujący funkcje zadeklarowane w `modul.h`.

## Kompilacja

Aby skompilować projekt, użyj poniższego polecenia:

`g++ -o program main.cpp modul.cpp`


## Użycie

Program przyjmuje opcje wiersza poleceń, które pozwalają na określenie różnych operacji do wykonania na obrazie. Składnia wywołania programu wygląda następująco:

`./program [-i nazwa_pliku_we] [-o nazwa_pliku_wy] [-p liczba] [-n] [-r] [-d] [-h] [-g gamma] [-z czern biel] [-m kolor|s] [-p c|b|prog liczba] [-r x|y]`


## Opcje

- `-i nazwa_pliku_we`  
  Wskazuje plik wejściowy. Jeśli `-` zostanie podany jako nazwa pliku, dane będą odczytywane ze standardowego wejścia.

- `-o nazwa_pliku_wy`  
  Wskazuje plik wyjściowy. Jeśli `-` zostanie podany jako nazwa pliku, dane będą zapisywane do standardowego wyjścia.

- `-n`  
  Wykonuje operację negatywu.

- `-k`  
  Wykonuje operację konturowania.

- `-d`  
  Wyświetla obraz.

- `-h`  
  Oblicza histogram obrazu.

- `-g gamma`  
  Wykonuje korekcję gamma z wartością `gamma`.

- `-z czern biel`  
  Ustawia progi czerni i bieli dla operacji zmiany poziomów.

- `-m kolor|s`  
  Ustawia kolor do przetworzenia (`r`, `g`, `b` dla kolorów lub `s` dla konwersji do odcieni szarości).

- `-p c|b|prog liczba`  
  Ustawia progowanie czerni (`c`), bieli (`b`), lub ogólne progowanie (`prog`) z wartością `liczba`.

- `-r x|y`  
  Wykonuje rozmywanie poziome (`x`) lub pionowe (`y`).
