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
    // bla bla bla
    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g1;

/// Un exemple de graphe2
    Graph g2;

    Graph g3;

        while ( !key[KEY_Q] )
    {
     num_graph = menu();

    if (num_graph == 1)
    {
    graphe="graphe1.txt";
    g1.make_example(graphe);
        while ( !key[KEY_ESC] )
        {
            /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
            g1.update();

            /// Mise à jour générale (clavier/souris/buffer etc...)
            grman::mettre_a_jour();
        }
        g1.sauvegarde(graphe);
    }
    if (num_graph == 2)
    {
    graphe="graphe2.txt";
    g2.make_example(graphe);
while ( !key[KEY_ESC] )
        {
            /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
            g2.update();

            /// Mise à jour générale (clavier/souris/buffer etc...)
            grman::mettre_a_jour();
        }
        g2.sauvegarde(graphe);
    }

    if (num_graph == 3)
    {
    graphe="graphe3.txt";
    g3.make_example(graphe);
while ( !key[KEY_ESC] )
        {
            /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
            g3.update();

            /// Mise à jour générale (clavier/souris/buffer etc...)
            grman::mettre_a_jour();
        }
        g3.sauvegarde(graphe);
    }

    }
    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();
// yctvbuino,l; tvybunio,;p cvybuni,o


