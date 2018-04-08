#include "grman/grman.h"
#include <iostream>
#include <string>

#include "graph.h"
#include "menu.h"

int main()
{
    int num_graph = 0;
    std::string graphe;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();
    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// On charge le graphe 1
    Graph g1;
    graphe="graphe1.txt";
    g1.make_example(graphe);

    /// On charge le graphe 2
    Graph g2;
    graphe="graphe2.txt";
    g2.make_example(graphe);

    /// On charge le graphe 3
    Graph g3;
    graphe="graphe3.txt";
    g3.make_example(graphe);

    ///On rentre dans la boucle du menu
    while ( !key[KEY_Q] )
    {
        /// On prend la valeur correspondant à l'endroit sélectionné par l'utilisateur
        num_graph = menu();

        /// Pour le premier graphe
        if (num_graph == 1)
        {
            graphe="graphe1.txt";
            /// On rentre dans la boucle de jeu
            while ( !key[KEY_ESC] )
            {
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                g1.update();

                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
                /// Le joueur peut sauvegarder en appuyant sur la touche espace
                if(key[KEY_SPACE])
                {
                    g1.sauvegarde(graphe);
                    std::cout<< "saved" << std::endl;
                }
            }
        }
        /// Pour le graphe 2
        if (num_graph == 2)
        {
            graphe="graphe2.txt";
            /// On rentre dans la boucle de jeu
            while ( !key[KEY_ESC] )
            {
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                g2.update();

                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
                /// Le joueur peut sauvegarder en appuyant sur la touche espace
                if(key[KEY_SPACE])
                {
                    g2.sauvegarde(graphe);
                    std::cout << "saved" << std::endl;
                }
            }
        }
        /// Pour le graphe 3
        if (num_graph == 3)
        {
            graphe="graphe3.txt";
            /// On rentre dans la boucle de jeu
            while ( !key[KEY_ESC] )
            {
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                g3.update();

                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
                /// Le joueur peut sauvegarder en appuyant sur espace
                if(key[KEY_SPACE])
                {
                    g3.sauvegarde(graphe);
                    std::cout << "saved" << std::endl;
                }
            }
        }

    }
    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();
// yctvbuino,l; tvybunio,;p cvybuni,o


