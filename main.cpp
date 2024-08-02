#include <cstdio>
#include <cstdlib>
#include "modul.h"

int main(int argc, char ** argv) {
    t_opcje opcje;
    t_obraz obraz;

    if(przetwarzaj_opcje(argc,argv,&opcje)!=0) exit(-1);
    wykonaj_opcje(&opcje,&obraz,argc,argv);
    return 0;
}