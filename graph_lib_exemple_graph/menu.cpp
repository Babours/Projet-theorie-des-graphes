#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "menu.h"

void lancerToutAllegro(int largeur, int hauteur)
{
    // Initialisation globale d'allegro
    allegro_init();

    // Initialisation clavier
    install_keyboard();

    // Initialisation souris
    if(install_mouse() == -1)
    {
        allegro_message("Erreur d'installation de la souris ! %s", allegro_error);
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    // OUVERTURE MODE GRAPHIQUE (ouverture fenêtre allegro)
    set_color_depth(desktop_color_depth());

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,largeur,hauteur,0,0)!=0)
    {
        allegro_message("probleme mode graphique : %s", allegro_error);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    show_mouse(screen); // Affiche pointeur de souris en mode allegro
}

int menu()
{
    bool graphe1 = false, graphe2 = false, graphe3 = false;
    BITMAP*fond_d_ecran; // il faut coller l'image sur une autre page pour eviter les erreurs de compilation
    BITMAP*menu; // Pour coller l'image par dessu
    //BITMAP*menu1;// menu 1
    grman::init(); // on réinitialise

    fond_d_ecran = create_bitmap(SCREEN_W, SCREEN_H); // Pour que l'image de fond prenne la totalite de l'écran
    clear_bitmap(fond_d_ecran);

    menu=load_bitmap("menu.bmp", NULL);
    if (!menu)
    {
        allegro_message("erreur");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    while (graphe1 == false && graphe2 == false && graphe3 == false)
//    while (!key[KEY_SPACE])
{
    blit(menu, fond_d_ecran, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

   textprintf_ex(menu, font, 255,30, makecol(255,255,255),-1,"________________________________________");
   textprintf_ex(menu, font, 265,50, makecol(255,255,255),-1," C H A I N E   A L I M E N T A I R E ");
   textprintf_ex(menu, font, 255,60, makecol(255,255,255),-1,"________________________________________");
    textprintf_ex(menu, font, 255,31, makecol(255,255,255),-1,"________________________________________");
   textprintf_ex(menu, font, 265,51, makecol(255,255,255),-1," C H A I N E   A L I M E N T A I R E ");
   textprintf_ex(menu, font, 255,61, makecol(255,255,255),-1,"________________________________________");

   textprintf_ex(menu, font, 252,60, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,57, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,54, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,52, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,50, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,48, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,46, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,44, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,42, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,40, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 252,38, makecol(255,255,255),-1,"|");

   textprintf_ex(menu, font, 570,60, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,57, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,54, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,52, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,50, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,48, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,46, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,44, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,42, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,40, makecol(255,255,255),-1,"|");
   textprintf_ex(menu, font, 570,38, makecol(255,255,255),-1,"|");

   textprintf_ex(menu, font, 0,110, makecol(255,0,0),-1,"______________________________________________________________________________________________________");
   textprintf_ex(menu, font, 0,111, makecol(255,255,255),-1,"______________________________________________________________________________________________________");
   textprintf_ex(menu, font, 0,112, makecol(255,0,0),-1,"______________________________________________________________________________________________________");
   textprintf_ex(menu, font, 0,130, makecol(255,255,255),-1,"-------------------------------------------------M E N U----------------------------------------------");
   textprintf_ex(menu, font, 0,131, makecol(255,0,0),-1,"-------------------------------------------------M E N U----------------------------------------------");
   textprintf_ex(menu, font, 1,131, makecol(255,255,255),-1,"-------------------------------------------------M E N U----------------------------------------------");
   textprintf_ex(menu, font, 0,138, makecol(255,0,0),-1,"______________________________________________________________________________________________________");
   textprintf_ex(menu, font, 0,139, makecol(255,255,255),-1,"______________________________________________________________________________________________________");
   textprintf_ex(menu, font, 0,140, makecol(255,0,0),-1,"______________________________________________________________________________________________________");

   textprintf_ex(menu, font, 220,220, makecol(255,255,255),-1," [ 1 ]    A C C E D E R   A U  G R A P H E  E U R O P E");
   textprintf_ex(menu, font, 220,221, makecol(255,255,255),-1," [ 1 ]    A C C E D E R   A U  G R A P H E  E U R O P E");
   textprintf_ex(menu, font, 220,270, makecol(255,255,255),-1," [ 2 ]    A C C E D E R   A U   G R A P H E  A F R I Q U E ");
   textprintf_ex(menu, font, 220,271, makecol(255,255,255),-1," [ 2 ]    A C C E D E R   A U   G R A P H E  A F R I Q U E ");
   textprintf_ex(menu, font, 220,320, makecol(255,255,255),-1," [ 3 ]    A C C E D E R   A U   G R A P H E  M A R R I N");
   textprintf_ex(menu, font, 220,321, makecol(255,255,255),-1," [ 3 ]    A C C E D E R   A U   G R A P H E  M A R R I N");


    blit(fond_d_ecran, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    if(mouse_b&1 && mouse_x > 215 && mouse_x < 590 && mouse_y > 210 && mouse_y < 240 )
{
    std::cout<<"GRAPHE 1"<<std::endl;
    graphe1 = true;
    return 1;
}

if(mouse_b&1 && mouse_x > 215 && mouse_x < 590 && mouse_y > 250 && mouse_y < 290 )
{
    std::cout<<"GRAPHE AFRIQUE"<<std::endl;
    graphe2 = true;
    return 2;
}

if(mouse_b&1 && mouse_x > 215 && mouse_x < 590 && mouse_y > 305 && mouse_y < 340 )
{
    std::cout<<"GRAPHE MARRIN"<<std::endl;
    graphe3 = true;
    return 3;
}
}
    return 0;
}

