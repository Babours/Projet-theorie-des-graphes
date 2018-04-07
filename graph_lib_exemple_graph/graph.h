#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED


/**************************************************************
    Ici sont proposées 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Arête ou Arc)
            Graph (=Graphe)

    Les arêtes et les sommets et le graphe qu'ils constituent
    "travaillent" étroitement ensemble : pour cette raison les
    Vertex et Edge se déclarent amis (friend) de Graph pour que
    ce dernier puisse librement accéder aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent êtres complétées. Il est également possible
    de les dériver mais il est malheureusement assez difficile
    de dériver le "triplet" des 3 classes en maintenant des relations
    cohérentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute préférable, si possible,
    de "customiser" ces classes de base directement, sans héritage.

    Le modèle proposé permet de représenter un graphe orienté éventuellement
    pondéré, les arcs portent une ou des informations supplémentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse être assez facilement
    lue et écrite en fichier, et bénéficie d'une bonne lisibilité en cas de bugs...

    Chaque arc possède 2 attributs principaux (en plus de son éventuelle pondération)
        -> m_from (indice du sommet de départ de l'arc )
        -> m_to (indice du sommet d'arrivée de l'arc )

    Chaque sommet possède 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accès aux prédécesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accès aux successeurs)

    Cependant le problème des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entité (un arc et/ou un sommet sont enlevés du graphe) alors :

    - Soit il faut reprendre toute la numérotation pour "boucher le trou"
      (par exemple on a supprimé le sommet n°4, le sommet n°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problèmes de stabilité et de cohérence, et une difficulté à ré-introduire
      le(s) même(s) élément supprimé (au même indice)

    - Soit on admet que la numérotation des sommets et arcs n'est pas contigue, càd qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numérotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet à des indices arbitraires (pas forcément contigus)

    C'est cette 2ème approche qui est proposée ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra être attentif au fait que par rapport à un simple vecteur, le parcours des éléments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours à itérateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'intérêt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associée à chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non représentés à l'écran
    Imaginons par exemple qu'on doive générer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas représenter graphiquement ces 1000 graphes, et les
    interfaces pèsent lourd en ressource, avec cette organisation on est libre de réserver ou
    pas une interface de présentation associée aux datas (découplage données/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <stack>

#include "grman/grman.h"

/***************************************************
                    VERTEX
****************************************************/

class VertexInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // La boite qui contient toute l'interface d'un sommet
        grman::WidgetBox m_top_box;

        // Un slider de visualisation/modification de la valeur du sommet
        grman::WidgetVSlider m_slider_value;

        // Un label de visualisation de la valeur du sommet
        grman::WidgetText m_label_value;

        // Une image de "remplissage"
        grman::WidgetImage m_img;

        // Un label indiquant l'index du sommet
        grman::WidgetText m_label_idx;

        // Une boite pour le label précédent
        grman::WidgetText m_box_label_idx;

        // Un bouton pour ajouter une arete
        grman::WidgetButton m_add_edge;

        // Un bouton pour supprimer un sommet
        grman::WidgetButton m_supp_vertex;

        //Un bouton pour supprimer une arete
        grman::WidgetButton m_supp_edge;



    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

    private :
        /// liste des indices des arcs arrivant au sommet : accès aux prédécesseurs
        std::vector<int> m_in;

        /// liste des indices des arcs partant du sommet : accès aux successeurs
        std::vector<int> m_out;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_value;
        double m_repro;
        double m_k;
        bool m_marque;
        bool m_passe;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<VertexInterface> m_interface = nullptr;

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne précédente est en gros équivalent à la ligne suivante :
        // VertexInterface * m_interface = nullptr;


    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Vertex (double value=0, VertexInterface *interface=nullptr, double repro=0, double k=0, bool m_marque=false, bool m_passe=false) :
            m_value(value), m_interface(interface), m_repro(repro)  {  }

        /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
        double get_value() {return m_value;}
        double get_repro() {return m_repro;}
        double get_k() {return m_k;}
        bool get_mark() {return m_marque;}
        bool get_passe() {return m_passe;}
        void set_value(double val) {m_value=val;}
        void set_k(double k) {m_k=k;}
        void set_mark (bool t) {m_marque=t;}
        void set_passe (bool f) {m_passe=f;}
};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Edge;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge;

        // Une boite pour englober les widgets de réglage associés
        grman::WidgetBox m_box_edge;

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight;

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight;

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        EdgeInterface(Vertex& from, Vertex& to);
};


class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class EdgeInterface;

    private :
        /// indice du sommet de départ de l'arc
        int m_from;

        /// indice du sommet d'arrivée de l'arc
        int m_to;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_weight;
        bool m_marque;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<EdgeInterface> m_interface = nullptr;


    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Edge (double weight=0, EdgeInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }

        /// Edge étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
        int get_from() {return m_from;}
        int get_to() {return m_to;}
        double get_weight() {return m_weight;}
        bool get_mark() {return m_marque;}
        void set_from(int i) {m_from=i;}
        void set_to(int i) {m_to=i;}
        void set_mark(bool t) {m_marque=t;}

};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        /// La boite qui contient toute l'interface d'un graphe
        grman::WidgetBox m_top_box;

        /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
        grman::WidgetBox m_main_box;

        /// Dans cette boite seront ajoutés des boutons de contrôle etc...
        grman::WidgetBox m_tool_box;

        /// deux boutons pour activer le mode dynamique et les composantes fortement connexes
        grman::WidgetButton m_add_vertex;
        grman::WidgetButton m_bouton_rouge;

        /// bouton abeille
        grman::WidgetButton m_bouton_abeille;
        grman::WidgetImage m_bouton_abeille_image;
        grman::WidgetText m_abeille;

        /// bouton aigle_royal
        grman::WidgetButton m_bouton_aigle_royal;
        grman::WidgetImage m_bouton_aigle_royal_image;
        grman::WidgetText m_aigle_royal;

        /// bouton boa
        grman::WidgetButton m_bouton_boa;
        grman::WidgetImage m_bouton_boa_image;
        grman::WidgetText m_boa;

        /// bouton bonobo
        grman::WidgetButton m_bouton_bonobo;
        grman::WidgetImage m_bouton_bonobo_image;
        grman::WidgetText m_bonobo;

        /// bouton cerisier
        grman::WidgetButton m_bouton_cerisier;
        grman::WidgetImage m_bouton_cerisier_image;
        grman::WidgetText m_cerisier;

        /// bouton cocotier
        grman::WidgetButton m_bouton_cocotier;
        grman::WidgetImage m_bouton_cocotier_image;
        grman::WidgetText m_cocotier;

        /// bouton cormoran
        grman::WidgetButton m_bouton_cormoran;
        grman::WidgetImage m_bouton_cormoran_image;
        grman::WidgetText m_cormoran;

        /// bouton crevette
        grman::WidgetButton m_bouton_crevette;
        grman::WidgetImage m_bouton_crevette_image;
        grman::WidgetText m_crevette;

        /// bouton crocodile
        grman::WidgetButton m_bouton_crocodile;
        grman::WidgetImage m_bouton_crocodile_image;
        grman::WidgetText m_crocodile;

        /// bouton dauphin
        grman::WidgetButton m_bouton_dauphin;
        grman::WidgetImage m_bouton_dauphin_image;
        grman::WidgetText m_dauphin;

        /// bouton dorade
        grman::WidgetButton m_bouton_dorade;
        grman::WidgetImage m_bouton_dorade_image;
        grman::WidgetText m_dorade;

        /// bouton fissurelle
        grman::WidgetButton m_bouton_fissurelle;
        grman::WidgetImage m_bouton_fissurelle_image;
        grman::WidgetText m_fissurelle;

        /// bouton flamand_rose
        grman::WidgetButton m_bouton_flamand_rose;
        grman::WidgetImage m_bouton_flamand_rose_image;
        grman::WidgetText m_flamand_rose;

        /// bouton fou_de_bassan
        grman::WidgetButton m_bouton_fou_de_bassan;
        grman::WidgetImage m_bouton_fou_de_bassan_image;
        grman::WidgetText m_fou_de_bassan;

        /// bouton fourmie
        grman::WidgetButton m_bouton_fourmie;
        grman::WidgetImage m_bouton_fourmie_image;
        grman::WidgetText m_fourmie;

        /// bouton frelon
        grman::WidgetButton m_bouton_frelon;
        grman::WidgetImage m_bouton_frelon_image;
        grman::WidgetText m_frelon;

        /// bouton gazelle
        grman::WidgetButton m_bouton_gazelle;
        grman::WidgetImage m_bouton_gazelle_image;
        grman::WidgetText m_gazelle;

        /// bouton goeland_argente
        grman::WidgetButton m_bouton_goeland_argente;
        grman::WidgetImage m_bouton_goeland_argente_image;
        grman::WidgetText m_goeland_argente;

        /// bouton hippopotame
        grman::WidgetButton m_bouton_hippopotame;
        grman::WidgetImage m_bouton_hippopotame_image;
        grman::WidgetText m_hippopotame;

        /// bouton lapin
        grman::WidgetButton m_bouton_lapin;
        grman::WidgetImage m_bouton_lapin_image;
        grman::WidgetText m_lapin;

        /// bouton leopard
        grman::WidgetButton m_bouton_leopard;
        grman::WidgetImage m_bouton_leopard_image;
        grman::WidgetText m_leopard;

        /// bouton lion
        grman::WidgetButton m_bouton_lion;
        grman::WidgetImage m_bouton_lion_image;
        grman::WidgetText m_lion;

        /// bouton maquereau
        grman::WidgetButton m_bouton_maquereau;
        grman::WidgetImage m_bouton_maquereau_image;
        grman::WidgetText m_maquereau;

        /// bouton mulot
        grman::WidgetButton m_bouton_mulot;
        grman::WidgetImage m_bouton_mulot_image;
        grman::WidgetText m_mulot;

        /// bouton ormeau
        grman::WidgetButton m_bouton_ormeau;
        grman::WidgetImage m_bouton_ormeau_image;
        grman::WidgetText m_ormeau;

        /// bouton ours
        grman::WidgetButton m_bouton_ours;
        grman::WidgetImage m_bouton_ours_image;
        grman::WidgetText m_ours;

        /// bouton pissenlit
        grman::WidgetButton m_bouton_pissenlit;
        grman::WidgetImage m_bouton_pissenlit_image;
        grman::WidgetText m_pissenlit;

        /// bouton poule
        grman::WidgetButton m_bouton_poule;
        grman::WidgetImage m_bouton_poule_image;
        grman::WidgetText m_poule;

        /// bouton renard
        grman::WidgetButton m_bouton_renard;
        grman::WidgetImage m_bouton_renard_image;
        grman::WidgetText m_renard;

        /// bouton saumon
        grman::WidgetButton m_bouton_saumon;
        grman::WidgetImage m_bouton_saumon_image;
        grman::WidgetText m_saumon;

        /// bouton tilleul
        grman::WidgetButton m_bouton_tilleul;
        grman::WidgetImage m_bouton_tilleul_image;
        grman::WidgetText m_tilleul;

        /// bouton vipere
        grman::WidgetButton m_bouton_vipere;
        grman::WidgetImage m_bouton_vipere_image;
        grman::WidgetText m_vipere;

        // A compléter éventuellement par des widgets de décoration ou
        // d'édition (boutons ajouter/enlever ...)

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        GraphInterface(int x, int y, int w, int h);
};


class Graph
{
    private :

        /// La "liste" des arêtes
        std::map<int, Edge> m_edges;

        /// La liste des sommets
        std::map<int, Vertex> m_vertices;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;

        std::vector<Edge> m_chemin;
        std::stack<int> m_sommets;
        bool m_t;



    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        void add_interfaced_vertex(int idx, double value, int x, int y, double repro, std::string pic_name="", int pic_idx=0);
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);
        void test_add_vertex(std::string image);

        /// Méthode spéciale qui construit un graphe arbitraire (démo)
        /// Voir implémentation dans le .cpp
        /// Cette méthode est à enlever et remplacer par un système
        /// de chargement de fichiers par exemple.
        void make_example(std::string graphe);


        /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
        void update();
        void Supp_Vertex(int i);
        void Supp_Edge(int i);
        void sauvegarde(std::string save);
        void test_remove_edge(int i);
        void test_add_edge(int i, int j);
        void test_supp_edge(int i, int j);
        void comp_fort(int i);
        void chemin (int idx);
        void afficher();
        void reboot();
        void set_t(bool i) {m_t=i;}
        bool get_t() {return m_t;}

};
#endif // GRAPH_H_INCLUDED
