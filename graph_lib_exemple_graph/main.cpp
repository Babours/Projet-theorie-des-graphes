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
    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g1;

/// Un exemple de graphe2
    Graph g2;

        while ( !key[KEY_Q] )
    {
     num_graph = menu();

    if (num_graph == 1)
    {
    graphe="graphe1.txt";
    g1.make_example(graphe);
        while ( !key[KEY_ESC] )
        {
            /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
            g1.update();

            /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
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
            /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
            g2.update();

            /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
            grman::mettre_a_jour();
        }
        g2.sauvegarde(graphe);
    }


    //grman::fermer_allegro();

    }

    return 0;
}
END_OF_MAIN();
// yctvbuino,l; tvybunio,;p cvybuni,o


