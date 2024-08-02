#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "modul.h"

#define DL_LINII 1024
#define W_OK 0
#define B_NIEPOPRAWNAOPCJA (-1)
#define B_BRAKNAZWY   (-2)
#define B_BRAKWARTOSCI  (-3)
#define B_BRAKPLIKU   (-4)
#define B_BLEDNE_DANE (-5)
#define B_NIEPOPRAWNY_PLIK (-6)
#define B_BRAK_PAMIECI (-7)

/****************************************************************************************
 * Funkcja wyzeruj_opcje zeruje opcje na poczatku działania programu i ustawia          *
 * uchwyty od plików na wartość NULL                                                    *
 *\param[in] wybor wskaznik na strukture opcje zawierajaca wszystkie wczytane parametry *
oraz argumenty w momencie wywolania programau                                           *
*****************************************************************************************/

void wyzeruj_opcje(t_opcje * wybor) {
    wybor->plik_we=nullptr;
    wybor->plik_wy=nullptr;
    wybor->negatyw=0;
    wybor->konturowanie=0;
    wybor->progowanie=0;
    wybor->wyswietlenie=0;
    wybor->korekcja_gamma=0;
    wybor->roz_histogramu=0;
    wybor->progowanie_bieli=0;
    wybor->progowanie_czerni=0;
    wybor->zmiana_poziomow=0;
    wybor->konwersja_do_szarosci=0;
    wybor->przetwarzaj_kolor=0;
    wybor->rozmywanie_poziome=0;
    wybor->rozmywanie_pionowe=0;
}

/***************************************************************************
 * Funkcja przetwarzaj_opcje rozpoznaje opcje wywolania programu zapisane  *
 * w tablicy argv                                                          *
 * i zapisuje je w strukturze opcje                                        *
 * Skladnia opcji wywolania programu                                       *
 *         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-r] [-d] }     *
 * Argumenty funkcji:                                                      *
 *         argc  -  liczba argumentow wywolania wraz z nazwa programu      *
 *         argv  -  tablica argumentow wywolania                           *
 *         wybor -  struktura z informacjami o wywolanych opcjach          *
 * PRE:                                                                    *
 *      brak                                                               *
 * POST:                                                                   *
 *      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich          *
 *      w strukturze wybór, do tego ustawia wartość pola równą 1           *
 *   dla opcji, ktore poprawnie wystapily w linii wywolania programu,      *
 *	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;            *
 *	zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne                   *
 *	lub kod bledu zdefiniowany stalymi B_* (<0)                            *
 * UWAGA:                                                                  *
 *      funkcja nie sprawdza istnienia i praw dostepu do plikow            *
 *      w takich przypadkach zwracane uchwyty maja wartosc NULL.           *
/***************************************************************************/

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor) {
    int prog, prog_cz, prog_b, bialy, czarny;
    float gamma;
    char kolorek;

    wyzeruj_opcje(wybor);
    wybor->plik_wy=stdout;

    for (int i = 1; i<argc; i++) {
      if (argv[i][0] != '-'){
          fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");
          return B_NIEPOPRAWNAOPCJA;}
    switch (argv[i][1]) {
    case 'i': {
        if (++i<argc) {
	          char *nazwa_pliku_we = argv[i];
        if (strcmp(nazwa_pliku_we,"-")==0)
            wybor->plik_we=stdin;
    else
	      wybor->plik_we=fopen(nazwa_pliku_we,"r");
    } else {
        fprintf(stderr,"\nBlad brak nazwy pliku\n\n");
	      return B_BRAKNAZWY;}
        break;
    }
    case 'o': {
      if (++i<argc) {
	char *nazwa_pliku_wy = argv[i];
	if (strcmp(nazwa_pliku_wy,"-")==0)
	    wybor->plik_wy=stdout;
  else  {
      if(argv[i][0] != '-'){
	  wybor->plik_wy=fopen(nazwa_pliku_wy,"w");
      wybor->nazwa_pliku=nazwa_pliku_wy;}
    else{
      fprintf(stderr,"\nBlad, Brak nazwy pliku\n\n");
      return B_BRAKNAZWY;}}
    }else{
  fprintf(stderr,"\nBlad, Brak nazwy pliku\n\n");
	return B_BRAKNAZWY;}
      break;
    }

    case 'n': {
      wybor->negatyw=1;
      break;
    }
    case 'k': {
      wybor->konturowanie=1;
      break;
    }
    case 'd': {
      wybor->wyswietlenie=1;
      break;
    }
    case 'h' : {
      wybor->roz_histogramu=1;
      break;
    }
    case 'g': {
      if((++i<argc)&&(argv[i][0]!='-')) {
  if(sscanf(argv[i],"%f",&gamma)==1) {
    wybor->korekcja_gamma=1;
    wybor->w_gamma=gamma;
  } else {
    fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc wspolczynnika gamma\n\n");
    return B_BRAKWARTOSCI;}
      } else{
  fprintf(stderr,"\nBlad, Nie podano wartosci wspolczynnika gamma\n\n");
  return B_BRAKWARTOSCI;}
      break;
    }
    case 'z' : {
      if((++i<argc)&&(argv[i][0]!='-')) {
  if(sscanf(argv[i],"%d", &czarny)==1) {
    wybor->czern=czarny;
  } else {
    fprintf(stderr,"\nBlad, Podano nieprawidlową wartośc czerni\n\n");
    return B_BRAKWARTOSCI;}
     } else {
  fprintf(stderr,"\nBlad, Nie Podano argumentu czerni oraz bieli\n\n");
  return B_BRAKWARTOSCI;}
     if((++i<argc)&&(argv[i][0]!='-')) {
  if(sscanf(argv[i],"%d", &bialy)==1) {
    wybor->biel=bialy;
    wybor->zmiana_poziomow=1;
  } else {
    fprintf(stderr,"\nBlad, Podano nieprawidlową wartość bieli\n\n");
    return B_BRAKWARTOSCI;}
  } else {
  fprintf(stderr,"\nBlad, Nie Podano drugiego argumentu bieli\n\n");
  return B_BRAKWARTOSCI;}
      break;
  }
    case 'm' : {
      if((++i<argc)&&(argv[i][0]!='-')){
  if(sscanf(argv[i],"%c", &kolorek)==1) {
        if((kolorek=='r') || (kolorek=='g') || (kolorek=='b')) {
            wybor->kolor=kolorek;
            wybor->przetwarzaj_kolor=1;
        }
        else if(kolorek=='s') wybor->konwersja_do_szarosci=1;
        else{
         fprintf(stderr,"\nBlad! Niepoprawny argument do opcji -m!\n\n");
         return B_NIEPOPRAWNAOPCJA;}
  } else {
    fprintf(stderr,"\nBlad, Podano nieprawidlowy parametr\n\n");
    return B_BRAKWARTOSCI;}
  } else {
  fprintf(stderr,"\nBlad,Nie podano koloru do przetworzenia badź parametru 's'!\n\n");
  return B_BRAKWARTOSCI;}
      break;
  }
    case 'p': {
      switch(argv[i][2]) {
        case 'c' : {
            if((++i<argc)&&(argv[i][0]!='-')) {
    if(sscanf(argv[i],"%d", &prog_cz)==1) {
      wybor->progowanie_czerni=1;
      wybor->w_progu_czerni=prog_cz;
    } else { /*blad bledny argument*/
      fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc progu czerni\n\n");
      return B_BRAKWARTOSCI;}
      } else { /*blad brak argumentu*/
      fprintf(stderr,"\nBlad,Nie podano wartosci progu czerni\n\n");
    return B_BRAKWARTOSCI;}
      break;
      }
      case 'b' : {
          if((++i<argc)&&(argv[i][0]!='-')) {
    if(sscanf(argv[i],"%d", &prog_b)==1) {
      wybor->progowanie_bieli=1;
      wybor->w_progu_bieli=prog_b;
    } else {
    fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc progu bieli\n\n");
    return B_BRAKWARTOSCI;}
      } else {
    fprintf(stderr,"\nBlad,Nie podano wartosci progu bieli\n\n");
    return B_BRAKWARTOSCI;}
      break;
      }
      case '\0' : {
          if ((++i<argc)&&(argv[i][0]!='-')) {
	  if (sscanf(argv[i],"%d",&prog)==1) {
	    wybor->progowanie=1;
      wybor->w_progu=prog;
	  } else {
    fprintf(stderr,"\nBlad, Podano nieprawidlowa wartosc progu\n\n");
	  return B_BRAKWARTOSCI;}
     } else {
    fprintf(stderr,"\nBlad,Nie podano wartosci progu\n\n");
	  return B_BRAKWARTOSCI;}
      break;
      }
      default: {
        fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");
        return B_NIEPOPRAWNAOPCJA;}
    }
    break;
  }
    case 'r' : {
      switch(argv[i][2]) {
        case 'x' : {
            wybor->rozmywanie_poziome=1;
            break;
        }
        case 'y' : {
            wybor->rozmywanie_pionowe=1;
            break;
        }
        default: {
          fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");
          return B_NIEPOPRAWNAOPCJA;}
    }
    break;
  }
    default: {
      fprintf(stderr,"\nBlad! Niepoprawna opcja!\n\n");
      return B_NIEPOPRAWNAOPCJA;}
    }
  }

  if (wybor->plik_we!=nullptr)
      return W_OK;
  else {
    fprintf(stderr,"\nBlad, Nie otwarto pliku wejsciowego\n\n");
    return B_BRAKPLIKU;}
}

/********************************************************************************************************
 * Funkcja wykonaj_opcje wykonuje opcje na podstawie parametrow wywolania programu                      *
 * \param[in] opcje wskaznik na strukture opcje zawierajaca wszystkie wybrane opcje i parametry         *
 * \param[in] obraz wskaznik na strukture obraz zawierajaca dane o wczytanym obrazie                    *
 * \param[in] argc liczba argumentów wywołania wraz z nazwą programu                                    *
 * \param[in] argv tablica argumentów wywołania programu                                                *
 * \return W_OK -> zwraca 0 ->brak bledow                                                               *
 * W przypadku wystąpienia błędów kończymy natychmiastowo działanie programu (exit(-1))                 *
 * Funkcja wczytaj wykonuje się zawsze pierwsza, operacje przetarzaj_kolor oraz konwersja_do szarosci   *
 * gdy są wywołane, wykonują się na początku.                                                           *
 * FUnkcje przetwarzania obrazu wykonywują się zgodnie z kolejnością wywołania programu, możliwe jest   *
 * wielokrotne wywołanie jednej i tej samej opcji                                                       *
 * Funkcja zapisz wykonuje się zawsze na końcu, wyświetlanie również o ile zostanie wywołane na początku*
*********************************************************************************************************/

int wykonaj_opcje(const t_opcje *opcje, t_obraz *obraz, const int argc, char **argv) {
  if(wczytaj(opcje->plik_we,obraz)!=0) exit(-1);
  if(opcje->przetwarzaj_kolor==1) if(przetwarzaj_kolor(opcje,obraz,opcje->kolor,argc,argv)!=0) exit(-1);
  if(opcje->konwersja_do_szarosci==1) if(konwersja_do_szarosci(obraz)!=0) exit(-1);
  if(opcje->przetwarzaj_kolor!=1){
  for(int i=0; i<argc; i++){
      switch(argv[i][1]){
          case 'n':{
               if(negatyw(obraz)!=0) exit(-1);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      if(progowanie(obraz,opcje->w_progu)!=0) exit(-1);
                      break;}
                  case 'c':{
                      if(progowanie_bieli(obraz,opcje->w_progu_czerni)!=0) exit(-1);
                      break;}
                  case 'b':{
                      if(progowanie_czerni(obraz,opcje->w_progu_bieli)!=0) exit(-1);
                      break;}
                  default: ;
              }
              break;}
          case 'g':{
              if(korekcja_gamma(obraz,opcje->w_gamma)!=0) exit(-1);
              break;}
          case 'z':{
              if(zmiana_poziomow(obraz,opcje->czern,opcje->biel)!=0) exit(-1);
              break;}
          case 'k':{
              if(konturowanie(obraz)!=0) exit(-1);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      if(rozmywanie_poziome(obraz)!=0) exit(-1);
                      break;}
                  case 'y':{
                      if(rozmywanie_pionowe(obraz)!=0) exit(-1);
                      break;}
                  default: ;
              }
              break; }
          case 'h':{
              if(roz_histogramu(obraz)!=0) exit(-1);
              break;}
          default: break;}}}
  zapisz(opcje->plik_wy, obraz);
  if(opcje->wyswietlenie==1){
      if(opcje->plik_wy==stdout) printf("\n\nNie mozna wyswietlic obrazu ze standardowego wyjscia!!\n\n");
      else wyswietlanie(opcje->nazwa_pliku);}
  return W_OK;
}

void wyswietlanie(const char *n_pliku){
    char polecenie[DL_LINII];

    strcpy(polecenie,"display ");
    strcat(polecenie,n_pliku);
    strcat(polecenie," &");
    printf("%s\n",polecenie);
    system(polecenie);
}

/************************************************************************************
 * Funkcja wczytaj wczytuje PGM lub PPM z pliku do struktury typu t_obraz obraz     *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM lub PPM	            *
 * \param[out] obraz wskaznik na strukture obraz do której zostanie zapisany obraz  *
 * \return W_OK -> zwraca 0 ->brak bledow                                           *
************************************************************************************/

int wczytaj(FILE *plik_we, t_obraz *obraz) {
  char buf[DL_LINII];
  int znak;
  int koniec=0;


  if (fgets(buf,DL_LINII,plik_we)==nullptr)
        koniec=1;


  if ( (buf[0]!='P') || koniec) {
    fprintf(stderr,"\n\t\tBlad: To nie jest plik PGM ani PPM!\n\n");
    return B_NIEPOPRAWNY_PLIK;
  }

  if (buf[1]!='2') {
      if (buf[1]!='3') {
             fprintf(stderr,"\n\t\tBlad: To nie jest plik PGM ani PPM!\n\n");
             return B_NIEPOPRAWNY_PLIK;
      }
  }

  if(buf[1]=='2'){

      do {
          if ((znak=fgetc(plik_we))=='#') {
              if (fgets(buf,DL_LINII,plik_we)==nullptr)
              koniec=1;
          }
          else{
              ungetc(znak,plik_we);
          }
      }while (znak=='#' && !koniec);

      if(fscanf(plik_we,"%d %d %d", &(obraz->wym_y), &(obraz->wym_x), &(obraz->odcieni))!=3){
      fprintf(stderr,"\n\t\tBlad: Brak wymiarow obrazu lub liczby stopni szarosci\n\n");
      return B_NIEPOPRAWNY_PLIK;
      }

  obraz->piksele = malloc(obraz->wym_x*obraz->wym_y*sizeof(int));
  if(obraz->piksele==nullptr){
    free(obraz->piksele);
    fprintf(stderr,"\nBlad przydzialu pamieci\n\n");
    return B_BRAK_PAMIECI;
  }
      int (*piksele)[obraz->wym_x] = static_cast<int(*)[obraz->wym_x]>(obraz->piksele);


  for(int i=0; i<obraz->wym_y; i++) {
    for(int j=0; j<obraz->wym_x; j++) {
        if(fscanf(plik_we,"%d", &(piksele[i][j]))!=1){
            fprintf(stderr,"\n\t\tBlad: Niewlasciwe wymiary obrazu\n\n");
	          return B_NIEPOPRAWNY_PLIK;
        }
    }
  }
  obraz->format=2;
}

else {
    do {
        if ((znak=fgetc(plik_we))=='#') {
            if (fgets(buf,DL_LINII,plik_we)==nullptr)
            koniec=1;
        }
        else{
            ungetc(znak,plik_we);
        }
    }while (znak=='#' && !koniec);

    if(fscanf(plik_we,"%d %d %d", &(obraz->wym_y), &(obraz->wym_x), &(obraz->odcieni))!=3){
    fprintf(stderr,"\n\t\tBlad: Brak wymiarow obrazu lub maksymalnej liczby składowych kolorowych obrazu\n\n");
    return B_NIEPOPRAWNY_PLIK;
    }

    obraz->piksele = malloc(obraz->wym_x*3*obraz->wym_y*sizeof(int));
    if(obraz->piksele==nullptr){
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;
    }
    int (*piksele)[obraz->wym_x * 3] = static_cast<int(*)[obraz->wym_x * 3]>(obraz->piksele);


    for(int i=0; i<obraz->wym_y; i++) {
        for(int j=0; j<obraz->wym_x*3; j++) {
            if(fscanf(plik_we,"%d", &(piksele[i][j]))!=1){
                fprintf(stderr,"\n\t\tBlad: Niewlasciwe wymiary obrazu\n\n");
	              return B_NIEPOPRAWNY_PLIK;
            }
        }
    }

    obraz->format=3;
}
    return W_OK;
}

/*********************************************************************************************
 * * Funkcja zapisz zapisuje obraz PGM lub PPM do pliku     	    						 *
 * \param[in] plik_wy uchwyt do pliku z obrazem w formacie PGM lub PPM			             *
 * \param[in] obraz wskaznik na strukture obraz zawierajaca dane o obrazie ktory mamy zapisac*
 * \return W_OK -> zwraca 0 ->brak bledow                                                    *
 *********************************************************************************************/

int zapisz(FILE *plik_wy, t_obraz *obraz){
    if(obraz->format==2){
        fprintf(plik_wy,"P2\n");
        fprintf(plik_wy,"%d %d\n%d\n",obraz->wym_y,obraz->wym_x,obraz->odcieni);

        int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
        for(int i=0; i<obraz->wym_y; i++){
            for(int j=0; j<obraz->wym_x; j++){
                fprintf(plik_wy,"%d ",piksele[i][j]);
            }
        }
    }
    if(obraz->format==3){
        fprintf(plik_wy,"P3\n");
        fprintf(plik_wy,"%d %d\n%d\n",obraz->wym_y,obraz->wym_x,obraz->odcieni);

        int (*piksele)[obraz->wym_x*3]  =static_cast<int(*)[obraz->wym_x * 3]>(obraz->piksele);
        for(int i=0; i<obraz->wym_y; i++){
            for(int j=0; j<obraz->wym_x*3; j++){
                fprintf(plik_wy,"%d ",piksele[i][j]);
            }
        }
    }
        free(obraz->piksele);
        obraz->piksele=nullptr;
    return W_OK;
}

/*********************************************************************************************
 * Funkcja negatyw przetwarza obraz PGM lub wczesniej skonwetowany obraz PPM na obraz PGM    *
 * 		 badz przetwarza tylko jeden kolor                                                   *
 * \param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie *
 * \return W_OK -> brak bledow ->wartosc 0                                                   *
 ********************************************************************************************/

int negatyw(const t_obraz *obraz) {
     if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
    int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
    for(int i=0; i<obraz->wym_y; i++){
        for(int j=0; j<obraz->wym_x; j++){
            piksele[i][j]=obraz->odcieni-piksele[i][j];
        }
    }
    return W_OK;
}

/**********************************************************************************************
 * Funkcja progowanie przetwarza obraz PGM  lub wczesniej skonwertowany obraz PPM na obraz PGM*
 * 	badz przetwarza tylko jeden kolor                                                         *
 * \param[in] w_progu wartosc podanego progu w momencie wywolania programu                    *
 * \param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie  *
 * \return W_OK -> brak bledow zwroc 0                                                        *
 **********************************************************************************************/

int progowanie(const t_obraz *obraz, int w_progu) {
   if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
    int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
    int prog=(w_progu*obraz->odcieni)/100;
    for(int i=0; i<obraz->wym_y; i++){
        for(int j=0; j<obraz->wym_x; j++){
            if(piksele[i][j]<=prog){
                piksele[i][j]=0;
            }
            else piksele[i][j]=obraz->odcieni;
        }
    }
    return W_OK;
}

/**************************************************************************************************************
 * Funkcja progowanie_bieli przetwarza obraz PGM  lub wczesniej skonwertowany obraz PPM na obraz PGM          *
 *  badz przetwarza tylko jeden kolor                                                                         *
 * \param[in] w_progu_bieli wartosc podanego progu dla funckji progowanie bieli  w momencie wywolania programu*
 * \param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                  *
 * \return W_OK -> brak bledow zwroc 0                                                                        *
 **************************************************************************************************************/

int progowanie_bieli(const t_obraz *obraz, const int w_progu_bieli) {
   if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
   int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
   int prog=(w_progu_bieli*obraz->odcieni)/100;
   for(int i=0; i<obraz->wym_y; i++){
        for(int j=0; j<obraz->wym_x; j++){
            if(piksele[i][j]>prog)
                piksele[i][j]=obraz->odcieni;
        }
    }
    return W_OK;
}

/****************************************************************************************************************
 * Funkcja progowanie_czerni przetwarza obraz PGM  lub wczesniej skonwertowany obraz PPM na obraz PGM           *
 *  badz przetwarza tylko jeden kolor                                                                           *
 * \param[in] w_progu_czerni wartosc podanego progu dla funckji progowanie czerni  w momencie wywolania programu*
 *\param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                     *
 *\return W_OK -> brak bledow zwroc 0                                                                           *
 ****************************************************************************************************************/

int progowanie_czerni(const t_obraz *obraz, const int w_progu_czerni) {
    if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
   int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
   int prog=(w_progu_czerni*obraz->odcieni)/100;
   for(int i=0; i<obraz->wym_y; i++){
        for(int j=0; j<obraz->wym_x; j++){
            if(piksele[i][j]<=prog)
                piksele[i][j]=0;
        }
    }
    return W_OK;
}

/*************************************************************************************************
 * Funkcja korekcja_gamma przetwarza obraz PGM lub wczesniej skonwertowany obraz PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                            *
 *\param[in] w_gamma	wspolczynnik gamma podany w momencie wywolania programu                  *
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie     *
 * \return W_OK ->brak bledow zwroc 0                                                            *
 *************************************************************************************************/

int korekcja_gamma(const t_obraz *obraz, const float w_gamma){
     if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
   }

    int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
    if(w_gamma==0){
        fprintf(stderr,"\nBlad! WSpolczynnik gamma nie moze sie zerowac! Operacja sie nie powiedzie!\n\n");
        return B_BLEDNE_DANE;
    }
    float w=1/ w_gamma;
    float w2=(1-w_gamma)/w_gamma;
    for(int i=0; i<obraz->wym_y; i++){
        for(int j=0; j<obraz->wym_x; j++){
            piksele[i][j]=(pow(piksele[i][j],w))/(pow(obraz->odcieni,w2));
        }
    }
    return W_OK;
}

/****************************************************************************************************
 * Funkcja zmiana poziomow przetwarza obraz PGM lub wczesniej skonwertowany obraz PPM na obraz PGM  *
 *  badz przetwarza tylko jeden kolor                                                               *
 *\param[in] bialy	wartosc bieli podany w momencie wywolania programu                              *
 *\param[in] czarny wartosc czerni podany w momencie wywolania programu                             *
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie        *
 * \return W_OK ->brak bledow zwroc 0                                                               *
 ****************************************************************************************************/

int zmiana_poziomow(const t_obraz *obraz, const int czarny, const int bialy){
   if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
  int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
  int biel=(bialy*obraz->odcieni)/100;
  int czern=(czarny*obraz->odcieni)/100;
  if(biel==czern){
      fprintf(stderr,"\nBlad!! czern nie moze byc rowna bieli!, Przetwarzanie się nie wykona.\n\n");
      return B_BLEDNE_DANE;
  }
  for(int i=0; i<obraz->wym_y; i++){
        for(int j=0; j<obraz->wym_x; j++){
          if(piksele[i][j]<=czern)
              piksele[i][j]=0;
          else if(piksele[i][j]>=biel)
              piksele[i][j]=obraz->odcieni;
          else piksele[i][j]=(piksele[i][j]-czern)*obraz->odcieni/(biel-czern);
        }
    }
  return W_OK;
}

/**********************************************************************************************
 * Funkcja konturowanie przetwarza obraz PGM lub wczesniej skonwertowany plik PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                         *
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie  *
 * \return W_OK brak bledow zworc 0                                                           *
 **********************************************************************************************/

int konturowanie(const t_obraz *obraz) {
     if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
    int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);

   for(int i=0; i<obraz->wym_y; i++){
       for(int j=0; j<obraz->wym_x; j++){
            if((i==obraz->wym_y-1)&&(j==obraz->wym_x-1)) piksele[i][j]=piksele[i][j];
            else if(i==obraz->wym_y-1) piksele[i][j]=abs(piksele[i][j+1]-piksele[i][j]);
            else if(j==obraz->wym_x-1) piksele[i][j]=abs(piksele[i+1][j]-piksele[i][j]);
            else piksele[i][j]=abs(piksele[i+1][j]-piksele[i][j])+abs(piksele[i][j+1]-piksele[i][j]);
        }

    }
    return W_OK;
}

/*****************************************************************************************************************
 * Funkcja rozmywanie_poziome o promienu 1 przetwarza obraz PGM lub wczesniej skonwertowany plik PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                                            *
 *\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie                     *
 * \return W_OK brak bledow zworc 0                                                                              *
 ****************************************************************************************************************/

int rozmywanie_poziome(const t_obraz *obraz){

   if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }

  int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
  int (*pomocna_tablica)[obraz->wym_x]=static_cast<int (*)[obraz->wym_x]>(malloc(obraz->wym_x * obraz->wym_y * sizeof(int)));
  if(pomocna_tablica==nullptr){
      free(pomocna_tablica);
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;
  }
  for(int i=0; i<obraz->wym_y; i++){
      for(int j=0; j<obraz->wym_x; j++){
          pomocna_tablica[i][j]=piksele[i][j];
      }
  }

  for(int i=0; i<obraz->wym_y; i++){
      for(int j=0; j<obraz->wym_x; j++){
          if(j==0) piksele[i][j]=(piksele[i][j]+piksele[i][j+1])/2;
          else if(j==obraz->wym_x-1) piksele[i][j]=(piksele[i][j]+pomocna_tablica[i][j-1])/2;
          else piksele[i][j]=(pomocna_tablica[i][j-1]+piksele[i][j]+piksele[i][j+1])/3;
      }
  }
  free(pomocna_tablica);
  pomocna_tablica=nullptr;
  return W_OK;
}

/*****************************************************************************************************************
 * Funkcja rozmywanie_pionowe o promienu 1 przetwarza obraz PGM lub wczesniej skonwertowany plik PPM na obraz PGM*
 *  badz przetwarza tylko jeden kolor                                                                            *
 *\param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                      *
 * \return W_OK brak bledow zworc 0                                                                              *
 ****************************************************************************************************************/

int rozmywanie_pionowe(const t_obraz *obraz){
   if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }

  int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
  int (*pomocna_tablica)[obraz->wym_x]=static_cast<int (*)[obraz->wym_x]>(malloc(obraz->wym_x * obraz->wym_y * sizeof(int)));
  if(pomocna_tablica==nullptr){
      free(pomocna_tablica);
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;
  }
  for(int i=0; i<obraz->wym_y; i++){
      for(int j=0; j<obraz->wym_x; j++){
          pomocna_tablica[i][j]=piksele[i][j];
      }
  }
  for(int i=0; i<obraz->wym_y; i++){
      for(int j=0; j<obraz->wym_x; j++){
          if(i==0) piksele[i][j]=(piksele[i][j]+piksele[i+1][j])/2;
          else if (i==obraz->wym_y-1) piksele[i][j]=(piksele[i][j]+pomocna_tablica[i-1][j])/2;
          else piksele[i][j]=(piksele[i+1][j]+piksele[i][j]+pomocna_tablica[i-1][j])/3;
      }
  }
  free(pomocna_tablica);
  pomocna_tablica=nullptr;
  return W_OK;
}

/*******************************************************************************************
 * Funkcja roz_histogramu przetwarza obraz PGM lub wczesniej skonwertowany obraz PPM na PGM*
 * badz przetwarza tylko jeden kolor                                                       *
*\param[out] obraz wskaznik na strukture obraz  do ktorej zostana zapisane zmiany w obrazie*
 * \return W_OK brak bledow zwroc 0                                                        *
*******************************************************************************************/

int roz_histogramu(const t_obraz *obraz){
    if(obraz->format!=2){
      fprintf(stderr,"\nBlad Nie mozna przetworzyc calego obrazu PPM! Nalezy wybrac jeden kolor do przetworzenia badz skonwertowac obraz do szarosci!!\n\n");
      return B_BLEDNE_DANE;
  }
    int (*piksele)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(obraz->piksele);
    int min=obraz->odcieni;
    int max=0;
    for(int i=0; i<obraz->wym_y; i++){
        for(int j=0; j<obraz->wym_x; j++){
            if(piksele[i][j]>max)
                max=piksele[i][j];
            if(piksele[i][j]<min)
                min=piksele[i][j];
        }
    }
    if((max==obraz->odcieni)&&(min==0)) printf("\nPiksele pokrywaja caly zakres jasnosci obrazu!,Obraz sie nie zmienil!\n\n");
    if(min!=max){
        for(int i=0; i<obraz->wym_y; i++){
            for(int j=0; j<obraz->wym_x; j++){
                piksele[i][j]=((piksele[i][j]-min)*obraz->odcieni)/(max-min);
            }
        }
    }
    else{
        printf("\nBlad: Najmniejsza jasnosc = maksymalnej jasnosci! Operacja sie nie powiedzie!\n\n");
        return B_BLEDNE_DANE;
    }
    return W_OK;
}

/*******************************************************************************************
* Funkcja konwersja_do_szarosci zamienia obraz PPM NA PGM                                  *
* \param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie*
* \return W_OK brak bledow zwroc 0                                                         *
*******************************************************************************************/

int konwersja_do_szarosci(t_obraz *obraz){

  if(obraz->format!=3){
      fprintf(stderr,"\nBlad Nie wczytano kolorowego obrazu, Nie mozna wykonac operacji konwersji do szarosci!\n\n");
      return B_BLEDNE_DANE;
  }

  int (*piksele)[obraz->wym_x*3]=static_cast<int(*)[obraz->wym_x * 3]>(obraz->piksele);
  int k=0;
  int w=0;
  for(int i=0; i<obraz->wym_y; i++){
      for(int j=0; j<obraz->wym_x*3; j+=3){
          piksele[k][w]=(piksele[i][j]+piksele[i][j+1]+piksele[i][j+2])/3;
          w++;
          if(w==obraz->wym_x*3){
            k++;
            w=0;
          }
      }
  }
  obraz->format=2;
  obraz->piksele=realloc(obraz->piksele,obraz->wym_x*obraz->wym_y*sizeof(int));
  if(obraz->piksele==nullptr){
    free(obraz->piksele);
    fprintf(stderr,"\nBlad przydzialu pamieci\n\n");
    return B_BRAK_PAMIECI;
  }
  return W_OK;
}

/*************************************************************************************************************************
* Funkcja przetwarzaj_opcje wykonuje operacje przetwarzania na wybranym kolorze z 3 mozliwych                            *
* czerwony, zielony oraz niebieski                                                                                       *
* Należy podać dodatkowo przynajmniej jedną operację przetwarzającą kolor w momencie wywołania programu                  *
*\param[out] obraz wskaznik na strukture obraz do ktorej zostana zapisane zmiany w obrazie                               *
*\param[in] opcje wskaznik na strukture opcje, ktora przechowuje informacje o opcjach wywolania programu                 *
*oraz argumentay do tych opcji                                                                                           *
*\param[in] kolor zmienna przechowuje znak 'r' lub 'g' lub 'b' dzieki ktoremu wiadomo jaki kolor ma zostac przetworzony  *
*\param[in] argc przechowuje liczbę argumentów wywołania programu                                                        *
*\param[in] argv tablica z opcjami wywołania funkcji 				                                                     *
 *\return W_OK brak bledow zwroc 0                                                                                       *
**************************************************************************************************************************/

int przetwarzaj_kolor(const t_opcje *opcje, const t_obraz *obraz, const char kolor, const int argc, char **argv){
  if(obraz->format!=3){
      fprintf(stderr,"\nBlad Nie wczytano kolorowego obrazu, Nie mozna wykonac operacji przetwarznia koloru!\n\n");
      return B_BLEDNE_DANE;}

  if((opcje->negatyw==0)&&(opcje->progowanie==0)&&(opcje->progowanie_czerni==0)&&(opcje->progowanie_bieli==0)&&(opcje->korekcja_gamma==0)&&(opcje->zmiana_poziomow==0)
  &&(opcje->konturowanie==0)&&(opcje->rozmywanie_poziome==0)&&(opcje->rozmywanie_pionowe==0)&&(opcje->roz_histogramu==0)){
  fprintf(stderr,"\nBlad! Nie podano operacji do przetwarzania wybranego koloru w obrazie PPM!\n\n");
  return B_BLEDNE_DANE; }

  t_obraz pojedynczy_kolor;
  pojedynczy_kolor.wym_x=obraz->wym_x;
  pojedynczy_kolor.wym_y=obraz->wym_y;
  pojedynczy_kolor.odcieni=obraz->odcieni;
  pojedynczy_kolor.format=2;
  pojedynczy_kolor.piksele=malloc(obraz->wym_x*obraz->wym_y*sizeof(int));
  if(pojedynczy_kolor.piksele==nullptr){
      free(pojedynczy_kolor.piksele);
      fprintf(stderr,"\nBlad, Brak przydzialu pamieci\n\n");
      return B_BRAK_PAMIECI;}

  int (*piksele)[obraz->wym_x*3]=static_cast<int(*)[obraz->wym_x * 3]>(obraz->piksele);

  int (*pomocna_tablica)[obraz->wym_x]=static_cast<int(*)[obraz->wym_x]>(pojedynczy_kolor.piksele);
  int k=0,w=0;

  if(kolor=='r'){
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=0; j<obraz->wym_x*3; j+=3){
              pomocna_tablica[k][w]=piksele[i][j];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}
     for(int i=0; i<argc; i++){
      switch(argv[i][1]){
          case 'n':{
               negatyw(&pojedynczy_kolor);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      progowanie(&pojedynczy_kolor,opcje->w_progu);
                      break;}
                  case 'c':{
                      progowanie_bieli(&pojedynczy_kolor,opcje->w_progu_czerni);
                      break;}
                  case 'b':{
                      progowanie_czerni(&pojedynczy_kolor,opcje->w_progu_bieli);
                      break;}
                  default: ;
              }
              break;}
          case 'g':{
              korekcja_gamma(&pojedynczy_kolor,opcje->w_gamma);
              break;}
          case 'z':{
              zmiana_poziomow(&pojedynczy_kolor,opcje->czern,opcje->biel);
              break;}
          case 'k':{
              konturowanie(&pojedynczy_kolor);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      rozmywanie_poziome(&pojedynczy_kolor);
                      break;}
                  case 'y':{
                      rozmywanie_pionowe(&pojedynczy_kolor);
                      break;}
                  default: ;
              }
              break; }
          case 'h':{
              roz_histogramu(&pojedynczy_kolor);
              break;}
          default: break;}}
      w=0; k=0;
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=0; j<obraz->wym_x*3; j+=3){
              piksele[i][j]=pomocna_tablica[k][w];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}}
  else if(kolor=='g'){
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=1; j<obraz->wym_x*3; j+=3){
              pomocna_tablica[k][w]=piksele[i][j];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}

      for(int i=0; i<argc; i++){
          switch(argv[i][1]){
          case 'n':{
               negatyw(&pojedynczy_kolor);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      progowanie(&pojedynczy_kolor,opcje->w_progu);
                      break;}
                  case 'c':{
                      progowanie_bieli(&pojedynczy_kolor,opcje->w_progu_czerni);
                      break;}
                  case 'b':{
                      progowanie_czerni(&pojedynczy_kolor,opcje->w_progu_bieli);
                      break;}
                  default: ;
              }
              break;}
          case 'g':{
              korekcja_gamma(&pojedynczy_kolor,opcje->w_gamma);
              break;}
          case 'z':{
              zmiana_poziomow(&pojedynczy_kolor,opcje->czern,opcje->biel);
              break;}
          case 'k':{
              konturowanie(&pojedynczy_kolor);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      rozmywanie_poziome(&pojedynczy_kolor);
                      break;}
                  case 'y':{
                      rozmywanie_pionowe(&pojedynczy_kolor);
                      break;}
                  default: ;
              }
              break; }
          case 'h':{
              roz_histogramu(&pojedynczy_kolor);
              break;}
          default: break;}}
      w=0; k=0;
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=1; j<obraz->wym_x*3; j+=3){
              piksele[i][j]=pomocna_tablica[k][w];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}}
  else if(kolor=='b'){
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=2; j<obraz->wym_x*3; j+=3){
              pomocna_tablica[k][w]=piksele[i][j];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}

     for(int i=0; i<argc; i++){
      switch(argv[i][1]){
          case 'n':{
               negatyw(&pojedynczy_kolor);
               break;}
          case 'p':{
              switch(argv[i][2]){
                  case '\0':{
                      progowanie(&pojedynczy_kolor,opcje->w_progu);
                      break;}
                  case 'c':{
                      progowanie_bieli(&pojedynczy_kolor,opcje->w_progu_czerni);
                      break;}
                  case 'b':{
                      progowanie_czerni(&pojedynczy_kolor,opcje->w_progu_bieli);
                      break;}
                  default: ;
              }
              break;}
          case 'g':{
              korekcja_gamma(&pojedynczy_kolor,opcje->w_gamma);
              break;}
          case 'z':{
              zmiana_poziomow(&pojedynczy_kolor,opcje->czern,opcje->biel);
              break;}
          case 'k':{
              konturowanie(&pojedynczy_kolor);
              break;}
          case 'r':{
              switch(argv[i][2]){
                  case 'x':{
                      rozmywanie_poziome(&pojedynczy_kolor);
                      break;}
                  case 'y':{
                      rozmywanie_pionowe(&pojedynczy_kolor);
                      break;}
                  default: ;
              }
              break; }
          case 'h':{
              roz_histogramu(&pojedynczy_kolor);
              break;}
          default: break;}}
      w=0; k=0;
      for(int i=0; i<obraz->wym_y; i++){
          for(int j=2; j<obraz->wym_x*3; j+=3){
              piksele[i][j]=pomocna_tablica[k][w];
              w++;
              if(w==obraz->wym_x){
                k++;
                w=0;}}}}
  free(pojedynczy_kolor.piksele);
  pojedynczy_kolor.piksele=nullptr;
  return W_OK;
}