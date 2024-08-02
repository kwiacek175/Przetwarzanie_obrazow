#ifndef MODUL_H
#define MODUL_H

typedef struct {
  FILE *plik_we, *plik_wy;
  char *nazwa_pliku;
  int negatyw,progowanie,konturowanie,wyswietlenie,korekcja_gamma,roz_histogramu;
  int progowanie_czerni,progowanie_bieli,zmiana_poziomow, rozmywanie_poziome, rozmywanie_pionowe;
  int przetwarzaj_kolor, konwersja_do_szarosci;
  int w_progu;
  float w_gamma;
  int w_progu_bieli;
  int w_progu_czerni;
  int biel,czern;
  char kolor;
  } t_opcje;

typedef struct {
  int format;
  int wym_x, wym_y, odcieni;
  void *piksele;
} t_obraz;

void wyzeruj_opcje(t_opcje *);
void wyswietlanie(const char *);

int przetwarzaj_opcje(int ,char **, t_opcje *);
int wykonaj_opcje(const t_opcje *, t_obraz *,int ,char**);

int wczytaj(FILE *, t_obraz *);
int zapisz(FILE *, t_obraz *);

int negatyw(const t_obraz *);
int konturowanie(const t_obraz *);
int roz_histogramu(const t_obraz *);
int rozmywanie_poziome(const t_obraz *);
int rozmywanie_pionowe(const t_obraz *);
int konwersja_do_szarosci(t_obraz *);
int zmiana_poziomow(const t_obraz *, int , int );
int progowanie_czerni(const t_obraz *, int );
int progowanie_bieli(const t_obraz *, int );
int progowanie(const t_obraz *, int );
int korekcja_gamma(const t_obraz *, float );
int przetwarzaj_kolor(const t_opcje *, const t_obraz *, char, int, char**);

#endif
