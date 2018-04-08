#include "graph.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <memory>
#include <queue>
#include <vector>
#include <stack>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 115);
    m_top_box.set_moveable();
    m_top_box.set_bg_color(CYANCLAIR);

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 1000.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(15,75);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Center);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Les 3 boutons
    m_top_box.add_child(m_add_edge);
    m_add_edge.set_bg_color(VERT);
    m_add_edge.set_dim(10, 10);
    m_add_edge.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Up );
    m_top_box.add_child( m_supp_vertex );
    m_supp_vertex.set_bg_color(ROUGE);
    m_supp_vertex.set_dim(10, 10);
    m_supp_vertex.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up );
    m_top_box.add_child(m_supp_edge);
    m_supp_edge.set_bg_color(GRIS);
    m_supp_edge.set_dim(10,10);
    m_supp_edge.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 10.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    /// afficher bouton bleu
    m_top_box.add_child(m_add_vertex);
    m_add_vertex.set_bg_color(BLEU);
    m_add_vertex.set_dim(10,10);
    m_add_vertex.set_pos(890,308);

    /// afficher bouton rouge
    m_top_box.add_child(m_bouton_rouge);
    m_bouton_rouge.set_bg_color(ROUGE);
    m_bouton_rouge.set_dim(10,10);
    m_bouton_rouge.set_pos(863,308);

    /// afficher bouton abeille
    m_top_box.add_child(m_bouton_abeille);
    m_bouton_abeille.set_dim(40,40);
    m_bouton_abeille.set_pos(863,325);
    m_bouton_abeille.add_child(m_bouton_abeille_image);
    m_bouton_abeille.add_child(m_abeille);
    m_abeille.set_message("Abeil");

    /// afficher bouton aigle_royal
    m_top_box.add_child(m_bouton_aigle_royal);
    m_bouton_aigle_royal.set_dim(40,40);
    m_bouton_aigle_royal.set_pos(863,370);
    m_bouton_aigle_royal.add_child(m_bouton_aigle_royal_image);
    m_bouton_aigle_royal.add_child(m_aigle_royal);
    m_aigle_royal.set_message("Aigle");


    /// afficher bouton boa
    m_top_box.add_child(m_bouton_boa);
    m_bouton_boa.set_dim(40,40);
    m_bouton_boa.set_pos(863,415);
    m_bouton_boa.add_child(m_bouton_boa_image);
    m_bouton_boa.add_child(m_boa);
    m_boa.set_message("Boa");

    /// afficher bouton bonobo
    m_top_box.add_child(m_bouton_bonobo);
    m_bouton_bonobo.set_dim(40,40);
    m_bouton_bonobo.set_pos(863,460);
    m_bouton_bonobo.add_child(m_bouton_bonobo_image);
    m_bouton_bonobo.add_child(m_bonobo);
    m_bonobo.set_message("Bonob");

    /// afficher bouton cerisier
    m_top_box.add_child(m_bouton_cerisier);
    m_bouton_cerisier.set_dim(40,40);
    m_bouton_cerisier.set_pos(863,505);
    m_bouton_cerisier.add_child(m_bouton_cerisier_image);
    m_bouton_cerisier.add_child(m_cerisier);
    m_cerisier.set_message("Ceris");

    /// afficher bouton cocotier
    m_top_box.add_child(m_bouton_cocotier);
    m_bouton_cocotier.set_dim(40,40);
    m_bouton_cocotier.set_pos(863,550);
    m_bouton_cocotier.add_child(m_bouton_cocotier_image);
    m_bouton_cocotier.add_child(m_cocotier);
    m_cocotier.set_message("Coco");

    /// afficher bouton cormoran
    m_top_box.add_child(m_bouton_cormoran);
    m_bouton_cormoran.set_dim(40,40);
    m_bouton_cormoran.set_pos(908,10);
    m_bouton_cormoran.add_child(m_bouton_cormoran_image);
    m_bouton_cormoran.add_child(m_cormoran);
    m_cormoran.set_message("Cormo");

    /// afficher bouton crevette
    m_top_box.add_child(m_bouton_crevette);
    m_bouton_crevette.set_dim(40,40);
    m_bouton_crevette.set_pos(908,55);
    m_bouton_crevette.add_child(m_bouton_crevette_image);
    m_bouton_crevette.add_child(m_crevette);
    m_crevette.set_message("Crevet");

    /// afficher bouton crocodile
    m_top_box.add_child(m_bouton_crocodile);
    m_bouton_crocodile.set_dim(40,40);
    m_bouton_crocodile.set_pos(908,100);
    m_bouton_crocodile.add_child(m_bouton_crocodile_image);
    m_bouton_crocodile.add_child(m_crocodile);
    m_crocodile.set_message("Croco");

    /// afficher bouton dauphin
    m_top_box.add_child(m_bouton_dauphin);
    m_bouton_dauphin.set_dim(40,40);
    m_bouton_dauphin.set_pos(908,145);
    m_bouton_dauphin.add_child(m_bouton_dauphin_image);
    m_bouton_dauphin.add_child(m_dauphin);
    m_dauphin.set_message("Dauph");

    /// afficher bouton dorade
    m_top_box.add_child(m_bouton_dorade);
    m_bouton_dorade.set_dim(40,40);
    m_bouton_dorade.set_pos(908,190);
    m_bouton_dorade.add_child(m_bouton_dorade_image);
    m_bouton_dorade.add_child(m_dorade);
    m_dorade.set_message("Dorad");

    /// afficher bouton fissurelle
    m_top_box.add_child(m_bouton_fissurelle);
    m_bouton_fissurelle.set_dim(40,40);
    m_bouton_fissurelle.set_pos(908,235);
    m_bouton_fissurelle.add_child(m_bouton_fissurelle_image);
    m_bouton_fissurelle.add_child(m_fissurelle);
    m_fissurelle.set_message("Fissu");

    /// afficher bouton flamand_rose
    m_top_box.add_child(m_bouton_flamand_rose);
    m_bouton_flamand_rose.set_dim(40,40);
    m_bouton_flamand_rose.set_pos(908,280);
    m_bouton_flamand_rose.add_child(m_bouton_flamand_rose_image);
    m_bouton_flamand_rose.add_child(m_flamand_rose);
    m_flamand_rose.set_message("FlmdR");

    /// afficher bouton fou_de_bassan
    m_top_box.add_child(m_bouton_fou_de_bassan);
    m_bouton_fou_de_bassan.set_dim(40,40);
    m_bouton_fou_de_bassan.set_pos(908,325);
    m_bouton_fou_de_bassan.add_child(m_bouton_fou_de_bassan_image);
    m_bouton_fou_de_bassan.add_child(m_fou_de_bassan);
    m_fou_de_bassan.set_message("fouDB");

    /// afficher bouton fourmie
    m_top_box.add_child(m_bouton_fourmie);
    m_bouton_fourmie.set_dim(40,40);
    m_bouton_fourmie.set_pos(908,370);
    m_bouton_fourmie.add_child(m_bouton_fourmie_image);
    m_bouton_fourmie.add_child(m_fourmie);
    m_fourmie.set_message("Fourm");

    /// afficher bouton frelon
    m_top_box.add_child(m_bouton_frelon);
    m_bouton_frelon.set_dim(40,40);
    m_bouton_frelon.set_pos(908,415);
    m_bouton_frelon.add_child(m_bouton_frelon_image);
    m_bouton_frelon.add_child(m_frelon);
    m_frelon.set_message("Frelo");

    /// afficher bouton gazelle
    m_top_box.add_child(m_bouton_gazelle);
    m_bouton_gazelle.set_dim(40,40);
    m_bouton_gazelle.set_pos(908,460);
    m_bouton_gazelle.add_child(m_bouton_gazelle_image);
    m_bouton_gazelle.add_child(m_gazelle);
    m_gazelle.set_message("Gazel");

    /// afficher bouton goeland_argente
    m_top_box.add_child(m_bouton_goeland_argente);
    m_bouton_goeland_argente.set_dim(40,40);
    m_bouton_goeland_argente.set_pos(908,505);
    m_bouton_goeland_argente.add_child(m_bouton_goeland_argente_image);
    m_bouton_goeland_argente.add_child(m_goeland_argente);
    m_goeland_argente.set_message("Goela");

    /// afficher bouton hippopotame
    m_top_box.add_child(m_bouton_hippopotame);
    m_bouton_hippopotame.set_dim(40,40);
    m_bouton_hippopotame.set_pos(908,550);
    m_bouton_hippopotame.add_child(m_bouton_hippopotame_image);
    m_bouton_hippopotame.add_child(m_hippopotame);
    m_hippopotame.set_message("Hippo");

    /// afficher bouton lapin
    m_top_box.add_child(m_bouton_lapin);
    m_bouton_lapin.set_dim(40,40);
    m_bouton_lapin.set_pos(953,10);
    m_bouton_lapin.add_child(m_bouton_lapin_image);
    m_bouton_lapin.add_child(m_lapin);
    m_lapin.set_message("Lapin");

    /// afficher bouton leopard
    m_top_box.add_child(m_bouton_leopard);
    m_bouton_leopard.set_dim(40,40);
    m_bouton_leopard.set_pos(953,55);
    m_bouton_leopard.add_child(m_bouton_leopard_image);
    m_bouton_leopard.add_child(m_leopard);
    m_leopard.set_message("Leopa");

    /// afficher bouton lion
    m_top_box.add_child(m_bouton_lion);
    m_bouton_lion.set_dim(40,40);
    m_bouton_lion.set_pos(953,100);
    m_bouton_lion.add_child(m_bouton_lion_image);
    m_bouton_lion.add_child(m_lion);
    m_lion.set_message("Lion");

    /// afficher bouton maquereau
    m_top_box.add_child(m_bouton_maquereau);
    m_bouton_maquereau.set_dim(40,40);
    m_bouton_maquereau.set_pos(953,145);
    m_bouton_maquereau.add_child(m_bouton_maquereau_image);
    m_bouton_maquereau.add_child(m_maquereau);
    m_maquereau.set_message("Maque");

    /// afficher bouton mulot
    m_top_box.add_child(m_bouton_mulot);
    m_bouton_mulot.set_dim(40,40);
    m_bouton_mulot.set_pos(953,190);
    m_bouton_mulot.add_child(m_bouton_mulot_image);
    m_bouton_mulot.add_child((m_mulot));
    m_mulot.set_message("Mulot");

    /// afficher bouton ormeau
    m_top_box.add_child(m_bouton_ormeau);
    m_bouton_ormeau.set_dim(40,40);
    m_bouton_ormeau.set_pos(953,235);
    m_bouton_ormeau.add_child(m_bouton_ormeau_image);
    m_bouton_ormeau.add_child(m_ormeau);
    m_ormeau.set_message("Ormea");

    /// afficher bouton ours
    m_top_box.add_child(m_bouton_ours);
    m_bouton_ours.set_dim(40,40);
    m_bouton_ours.set_pos(953,280);
    m_bouton_ours.add_child(m_bouton_ours_image);
    m_bouton_ours.add_child(m_ours);
    m_ours.set_message("Ours");

    /// afficher bouton pissenlit
    m_top_box.add_child(m_bouton_pissenlit);
    m_bouton_pissenlit.set_dim(40,40);
    m_bouton_pissenlit.set_pos(953,325);
    m_bouton_pissenlit.add_child(m_bouton_pissenlit_image);
    m_bouton_pissenlit.add_child(m_pissenlit);
    m_pissenlit.set_message("pisse");

    /// afficher bouton poule
    m_top_box.add_child(m_bouton_poule);
    m_bouton_poule.set_dim(40,40);
    m_bouton_poule.set_pos(953,370);
    m_bouton_poule.add_child(m_bouton_poule_image);
    m_bouton_poule.add_child(m_poule);
    m_poule.set_message("Poule");

    /// afficher bouton renard
    m_top_box.add_child(m_bouton_renard);
    m_bouton_renard.set_dim(40,40);
    m_bouton_renard.set_pos(953,415);
    m_bouton_renard.add_child(m_bouton_renard_image);
    m_bouton_renard.add_child(m_renard);
    m_renard.set_message("Renar");

    /// afficher bouton saumon
    m_top_box.add_child(m_bouton_saumon);
    m_bouton_saumon.set_dim(40,40);
    m_bouton_saumon.set_pos(953,460);
    m_bouton_saumon.add_child(m_bouton_saumon_image);
    m_bouton_saumon.add_child(m_saumon);
    m_saumon.set_message("Saumo");

    /// afficher bouton tilleul
    m_top_box.add_child(m_bouton_tilleul);
    m_bouton_tilleul.set_dim(40,40);
    m_bouton_tilleul.set_pos(953,505);
    m_bouton_tilleul.add_child(m_bouton_tilleul_image);
    m_bouton_tilleul.add_child(m_tilleul);
    m_tilleul.set_message("Tille");

    /// afficher bouton vipere
    m_top_box.add_child(m_bouton_vipere);
    m_bouton_vipere.set_dim(40,40);
    m_bouton_vipere.set_pos(953,550);
    m_bouton_vipere.add_child(m_bouton_vipere_image);
    m_bouton_vipere.add_child(m_vipere);
    m_vipere.set_message("Viper");
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example(std::string nom)
{
    int nb_sommet, nb_arete, x, y, sommet1, sommet2;
    double poids, val, repro;
    std::string bitmap;
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    std::ifstream fichier ( nom, std::ios::in);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);
    // On lit les 2 premières lignes du fichier qui correspondent à notre nombre de sommets et à notre nombre d'arcs
    fichier >> nb_sommet >> nb_arete;
    /// Les sommets doivent être définis avant les arcs
    // On lit les sommets 1 par 1 et on vient inscrire les informations une à une dans nos variables qui serviront à créer le sommet
    for (int i=0; i<nb_sommet; i++)
    {
        fichier >> val >> x >> y >> bitmap >> repro;
        add_interfaced_vertex(i, val, x, y, repro, bitmap);
    }

    /// Les arcs doivent être définis entre des sommets qui existent !
    // On lit les arcs 1 à 1 et on vient inscrire les informations dans nos variables qui serviront à créer l'arc
    for (int i=0; i<nb_arete; i++)
    {
        fichier >> sommet1 >> sommet2 >> poids;
        add_interfaced_edge(i, sommet1, sommet2, poids);
    }
    fichier.close();

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    double j(0.0);
    std::queue<double> file_j;
    std::string bitmap;
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
    {
        elt.second.post_update();
        /// Si on a cliqué sur le bouton pour ajouter un arc
        if(elt.second.m_interface->m_add_edge.clicked() )
        {
            /// On enregistre la fait que notre bouton soit activé
            elt.second.m_interface->m_add_edge.set_dej_active(true);
            for (auto &id : m_vertices)
            {
                /// Si on a dans notre graphe un bouton qui a cette variable dej_active également activé (et qu'on ne s'agit pas de notre premier sommet)
                if((id.second.m_interface->m_add_edge.get_dej_active()) && (id.first!=elt.first))
                {
                    ///On appelle la fonction pour créer un arc et on réinitialise nos 2 boutons
                    test_add_edge(id.first, elt.first);
                    elt.second.m_interface->m_add_edge.set_dej_active(false);
                    id.second.m_interface->m_add_edge.set_dej_active(false);
                }
            }
        }
    }
    /// On regarde si le bouton bleu est activé
    if(m_interface->m_add_vertex.get_dej_active())
    {
        /// Si il est activé alors pour chaque sommet
        for (auto &id : m_vertices)
        {
            ///On définit le K de ce sommet
            for (unsigned int i=0; i<id.second.m_in.size(); i++)
            {
                id.second.set_k(id.second.get_k()+((m_edges[id.second.m_in[i]].get_weight())*(m_vertices[m_edges[id.second.m_in[i]].get_from()].get_value())));
            }
            /// On fait en sorte que le K ne soit jamais nul
            if(id.second.get_k()<id.second.get_value())
            {
                id.second.set_k(id.second.get_value()*1.001);
            }
        }
        /// Ensuite pour chaque sommet de nouveau
        for (auto &id : m_vertices)
        {
            j=0.0;
            /// On détermine le nombre d'animaux qui vont le manger et en quelle quantité
            for (unsigned int i=0; i<id.second.m_out.size(); i++)
            {
                j=j+((m_edges[id.second.m_out[i]].get_weight())*(m_vertices[m_edges[id.second.m_out[i]].get_to()].get_value()));
            }
            /// On stocke ça dans une file
            file_j.push(j);
        }
        /// Et enfin pour chaque sommet
        for (auto &id : m_vertices)
        {
            /// On rafraichit le nombre de membres par sommet
            id.second.set_value((id.second.get_value())+((id.second.get_repro())*(id.second.get_value())*(1.0-((id.second.get_value())/(id.second.get_k()))))-(0.1*file_j.front()));
            /// On fait en sorte que ce nombre ne soit pas négatif
            if(id.second.get_value()<0)
            {
                id.second.set_value(0);
            }
            if(id.second.get_value()>1000)
            {
                id.second.set_value(1000);
            }
            file_j.pop();
        }
        rest(100);
    }
    for(auto &elt: m_vertices)
    {
        /// Si un bouton pour supprimer est activé
        if(elt.second.m_interface->m_supp_vertex.clicked())
        {
            ///Alonrs on appelle la fonction pour supprimer un sommet
            Supp_Vertex(elt.first);
            break;
        }
    }

    for(auto &elt: m_vertices)
    {
        /// Si le bouton pour supprimer un arc est activé
        if(elt.second.m_interface->m_supp_edge.clicked() )
        {
            /// On enregistre le fait que le bouton aie été activé
            elt.second.m_interface->m_supp_edge.set_dej_active(true);
            for (auto &id : m_vertices)
            {
                /// Si on autre bouton que celui du premier arc a été activé
                if((id.second.m_interface->m_supp_edge.get_dej_active()) && (id.first!=elt.first))
                {
                    /// On appelle la fonction pour supprimer une arête et on réinitialise les valeurs des 2 boutons
                    test_supp_edge(id.first, elt.first);
                    elt.second.m_interface->m_supp_edge.set_dej_active(false);
                    id.second.m_interface->m_supp_edge.set_dej_active(false);
                }
            }
        }
    }


    for (auto &elt : m_edges)
        elt.second.post_update();
    /// Si le bouton bleu a été cliqué alors qu'il n'était pas activé
    if((m_interface->m_add_vertex.clicked())&&(m_interface->m_add_vertex.get_dej_active()==false))
    {
        /// On l'active
        m_interface->m_add_vertex.set_dej_active(true);
    }
    /// Si le bouton bleu a été cliqué alors qu'il était déjà activé
    if((m_interface->m_add_vertex.clicked())&&(m_interface->m_add_vertex.get_dej_active()==true))
    {
        /// On le désactive
        m_interface->m_add_vertex.set_dej_active(false);
    }

    /// Si on appuie sur le bouton rouge
    if ((m_interface->m_bouton_rouge.clicked())&& (m_interface->m_bouton_rouge.get_dej_active()==false))
    {
        /// On appelle la fonction des composantes fortement connexes
        rest(500);
        comp_fort(0);
        m_interface->m_bouton_rouge.set_dej_active(true);
    }


    ///rajouter sommet abeille
    if(m_interface->m_bouton_abeille.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="abeille.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_abeille.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_abeille.clicked())&&(m_interface->m_bouton_abeille.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_abeille.set_dej_active(true);
    }

    ///rajouter sommet aigle royal
    if(m_interface->m_bouton_aigle_royal.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="aigle_royal.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_aigle_royal.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_aigle_royal.clicked())&&(m_interface->m_bouton_aigle_royal.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_aigle_royal.set_dej_active(true);
    }


    ///rajouter sommet boa
    if(m_interface->m_bouton_boa.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="boa.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_boa.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_boa.clicked())&&(m_interface->m_bouton_boa.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_boa.set_dej_active(true);
    }

    ///rajouter sommet bonobo
    if(m_interface->m_bouton_bonobo.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="bonobo.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_bonobo.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_bonobo.clicked())&&(m_interface->m_bouton_bonobo.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_bonobo.set_dej_active(true);
    }

    ///rajouter sommet cerisier
    if(m_interface->m_bouton_cerisier.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="cerisier.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_cerisier.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_cerisier.clicked())&&(m_interface->m_bouton_cerisier.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_cerisier.set_dej_active(true);
    }

    ///rajouter sommet cocotier
    if(m_interface->m_bouton_cocotier.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="cocotier.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_cocotier.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_cocotier.clicked())&&(m_interface->m_bouton_cocotier.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_cocotier.set_dej_active(true);
    }

    ///rajouter sommet cormoran
    if(m_interface->m_bouton_cormoran.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="cormoran.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_cormoran.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_cormoran.clicked())&&(m_interface->m_bouton_cormoran.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_cormoran.set_dej_active(true);
    }

    ///rajouter sommet crevette
    if(m_interface->m_bouton_crevette.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="crevette.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_crevette.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_crevette.clicked())&&(m_interface->m_bouton_crevette.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_crevette.set_dej_active(true);
    }

    ///rajouter sommet crocodile
    if(m_interface->m_bouton_crocodile.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="crocodile.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_crocodile.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_crocodile.clicked())&&(m_interface->m_bouton_crocodile.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_crocodile.set_dej_active(true);
    }

    ///rajouter sommet dauphin
    if(m_interface->m_bouton_dauphin.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="dauphin.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_dauphin.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_dauphin.clicked())&&(m_interface->m_bouton_dauphin.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_dauphin.set_dej_active(true);
    }

    ///rajouter sommet dorade
    if(m_interface->m_bouton_dorade.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="dorade.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_dorade.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_dorade.clicked())&&(m_interface->m_bouton_dorade.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_dorade.set_dej_active(true);
    }

    ///rajouter sommet fissurelle
    if(m_interface->m_bouton_fissurelle.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="fissurelle.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_fissurelle.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_fissurelle.clicked())&&(m_interface->m_bouton_fissurelle.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_fissurelle.set_dej_active(true);
    }

    ///rajouter sommet flamand rose
    if(m_interface->m_bouton_flamand_rose.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="flamand_rose.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_flamand_rose.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_flamand_rose.clicked())&&(m_interface->m_bouton_flamand_rose.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_flamand_rose.set_dej_active(true);
    }

    ///rajouter sommet fou de bassan
    if(m_interface->m_bouton_fou_de_bassan.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="fou_de_bassan.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_fou_de_bassan.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_fou_de_bassan.clicked())&&(m_interface->m_bouton_fou_de_bassan.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_fou_de_bassan.set_dej_active(true);
    }

    ///rajouter sommet fourmie
    if(m_interface->m_bouton_fourmie.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="fourmie.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_fourmie.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_fourmie.clicked())&&(m_interface->m_bouton_fourmie.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_fourmie.set_dej_active(true);
    }


    ///rajouter sommet frelon
    if(m_interface->m_bouton_frelon.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="frelon.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_frelon.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_frelon.clicked())&&(m_interface->m_bouton_frelon.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_frelon.set_dej_active(true);
    }

    ///rajouter sommet gazelle
    if(m_interface->m_bouton_gazelle.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="gazelle.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_gazelle.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_gazelle.clicked())&&(m_interface->m_bouton_gazelle.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_gazelle.set_dej_active(true);
    }

    ///rajouter sommet goeland argente
    if(m_interface->m_bouton_goeland_argente.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="goeland_argente.png";
            test_add_vertex(bitmap);
            m_interface->m_bouton_goeland_argente.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_goeland_argente.clicked())&&(m_interface->m_bouton_goeland_argente.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_goeland_argente.set_dej_active(true);
    }

    ///rajouter sommet hippopotame
    if(m_interface->m_bouton_hippopotame.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="hippopotame.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_hippopotame.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_hippopotame.clicked())&&(m_interface->m_bouton_hippopotame.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_hippopotame.set_dej_active(true);
    }

    ///rajouter sommet lapin
    if(m_interface->m_bouton_lapin.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="lapin.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_lapin.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_lapin.clicked())&&(m_interface->m_bouton_lapin.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_lapin.set_dej_active(true);
    }

    ///rajouter sommet leopard
    if(m_interface->m_bouton_leopard.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="leopard.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_leopard.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_leopard.clicked())&&(m_interface->m_bouton_leopard.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_leopard.set_dej_active(true);
    }

    ///rajouter sommet lion
    if(m_interface->m_bouton_lion.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="lion.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_lion.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_lion.clicked())&&(m_interface->m_bouton_lion.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_lion.set_dej_active(true);
    }

    ///rajouter sommet maquereau
    if(m_interface->m_bouton_maquereau.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="maquereau.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_maquereau.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_maquereau.clicked())&&(m_interface->m_bouton_maquereau.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_maquereau.set_dej_active(true);
    }

    ///rajouter sommet mulot
    if(m_interface->m_bouton_mulot.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="mulot.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_mulot.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_mulot.clicked())&&(m_interface->m_bouton_mulot.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_mulot.set_dej_active(true);
    }

    ///rajouter sommet ours
    if(m_interface->m_bouton_ours.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="ours.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_ours.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_ours.clicked())&&(m_interface->m_bouton_ours.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_ours.set_dej_active(true);
    }

    ///rajouter sommet ormeau
    if(m_interface->m_bouton_ormeau.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="ormeau.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_ormeau.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_ormeau.clicked())&&(m_interface->m_bouton_ormeau.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_ormeau.set_dej_active(true);
    }

    ///rajouter sommet pissenlit
    if(m_interface->m_bouton_pissenlit.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="pissenlit.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_pissenlit.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_pissenlit.clicked())&&(m_interface->m_bouton_pissenlit.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_pissenlit.set_dej_active(true);
    }

    ///rajouter sommet poule
    if(m_interface->m_bouton_poule.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="poule.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_poule.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_poule.clicked())&&(m_interface->m_bouton_poule.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_poule.set_dej_active(true);
    }

    ///rajouter sommet renard
    if(m_interface->m_bouton_renard.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="renard.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_renard.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_renard.clicked())&&(m_interface->m_bouton_renard.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_renard.set_dej_active(true);
    }

    ///rajouter sommet saumon
    if(m_interface->m_bouton_saumon.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="saumon.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_saumon.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_saumon.clicked())&&(m_interface->m_bouton_saumon.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_saumon.set_dej_active(true);
    }

    ///rajouter sommet tilleul
    if(m_interface->m_bouton_tilleul.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="tilleul.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_tilleul.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_tilleul.clicked())&&(m_interface->m_bouton_tilleul.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_tilleul.set_dej_active(true);
    }

    ///rajouter sommet vipere
    if(m_interface->m_bouton_vipere.get_dej_active()==true)
    {
        if(mouse_b&1)
        {
            bitmap="vipere.jpg";
            test_add_vertex(bitmap);
            m_interface->m_bouton_vipere.set_dej_active(false);
        }
    }
    if((m_interface->m_bouton_vipere.clicked())&&(m_interface->m_bouton_vipere.get_dej_active()==false))
    {
        rest(300);
        m_interface->m_bouton_vipere.set_dej_active(true);
    }

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, double repro, std::string pic_name, int pic_idx)
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi, repro);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges.find(idx)->second.set_from(id_vert1);
    m_edges.find(idx)->second.set_to(id_vert2);
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

/// La fonction de sauvegarde
void Graph::sauvegarde(std::string save)
{
    /// On ouvre le fichier texte en mode écriture et on efface tout son contenu précédent
    std::fstream fichier;
    fichier.open( save, std::ios::out | std::ios::trunc);
    /// On prend notre nombre de sommets et d'arcs et on les inscrit dans le fichier
    fichier << m_vertices.size() << std::endl << m_edges.size() << std::endl;
    /// Pour chaque sommet on inscrit ses informations dans le fichier
    for (std::map<int, Vertex>::iterator i=m_vertices.begin(); i!=m_vertices.end(); i++)
    {
        fichier << i->second.get_value() << " " << i->second.m_interface->m_top_box.get_posx() << " " << i->second.m_interface->m_top_box.get_posy() << " " << i->second.m_interface->m_img.get_name() << " " << i->second.get_repro() << std::endl;
    }
    /// Pour chaque arc on inscrit ses informations dans le fichier
    for (std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        fichier << it->second.get_from() << " " << it->second.get_to() << " " << it->second.get_weight() << std::endl;

    }
    /// on ferme le fichier
    fichier.close();
}

/// Fonction pour supprimer un sommet
void Graph::Supp_Vertex(int i)
{
    for (auto &it : m_edges)
    {
        /// On supprime tous les arcs du sommet
        if(it.second.get_from()==i||it.second.get_to()==i)
        {
            test_remove_edge(it.first);
        }
    }
    /// On retire le sommet de l'interface
    m_vertices[i].m_interface->m_top_box.set_pos(1500, 2000);
}


/// Fonction pour ajouter un sommet
void Graph::test_add_edge(int idx1, int idx2)
{
    /// On stocke tous les indices d'arc dans un vecteur
    if(m_edges.size()>1)
    {
        std::vector<int> select;
        for (std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
        {
            select.push_back(it->first);
        }
        ///On tire pour avoir le plus grand sommet
        for (unsigned int i=0; i<select.size(); i++)
        {
            for(unsigned int j=0; j<select.size(); j++)
            {
                if(select[i]<select[j])
                {
                    select[i]=select[j];
                }
                else
                {
                    select[j]=select[i];
                }
            }
        }
        ///On appelle la création d'arc avec les données recues
        add_interfaced_edge(select[0]+1, idx1, idx2, 0.0);
    }
    else
    {
        add_interfaced_edge(m_edges.size()+500, idx1, idx2, 0.0);
    }
}

/// eidx index of edge to remove

void Graph::test_remove_edge(int eidx)

{

    /// référence vers le Edge à enlever

    Edge &remed=m_edges.at(eidx);



    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;



    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;



    /// test : on a bien des éléments interfacés

    if (m_interface && remed.m_interface)

    {

        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :

        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */

        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */

        /* m_edges[idx] = Edge(weight, ei);*/

        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr

        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe

        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !

    /// References sur les listes de edges des sommets from et to

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );



    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface

    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !

    m_edges.erase( eidx );





    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;




}

/// Fonction pour supprimer un arc
void Graph::test_supp_edge (int i, int j)
{
    /// On cherche parmis tous les arcs le bon
    for (std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        if((it->second.get_from()==i)&&(it->second.get_to()==j))
        {
            /// on le supprime
            test_remove_edge(it->first);
        }
    }
}

/// Fonction pour ajouter un sommet
void Graph::test_add_vertex(std::string bitmap)
{
    /// On entre tous les indices de sommets dans un vector
    std::vector<int> select;
    for (std::map<int, Vertex>::iterator it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        select.push_back(it->first);
    }
    /// On rie pour obtenir le plus grand
    for (unsigned int i=0; i<select.size(); i++)
    {
        for(unsigned int j=0; j<select.size(); j++)
        {
            if(select[i]<select[j])
            {
                select[i]=select[j];
            }
            else
            {
                select[j]=select[i];
            }
        }
    }
    /// On appelle la fonction pour ajouter un sommet avec les données qu'on a reçu
    add_interfaced_vertex(select[0]+1, 0.0, mouse_x, mouse_y, 1, bitmap);
}


/// Fonction pour afficher les composantes fortement connexes
void Graph::comp_fort(int i)
{
    if(m_vertices.count(i)>0)
    {
        /// On marque le sommet et on applique la fonction chemin() dessus
        m_vertices[i].set_mark(true);
        m_sommets.push(i);
        chemin(m_sommets.top());
    }
}

/// Fonction chemin()
void Graph::chemin(int idx)
{
    int k=0;
    int c;
    unsigned int i=0;
    /// Si le sommet a des arcs qui partent de lui
    if (m_vertices[idx].m_out.size()>0)
    {
        do
        {
            i++;
        }while ((m_edges[m_vertices[idx].m_out[i]].get_mark()==true) && (i<m_vertices[idx].m_out.size()));
        i=0;
            /// Si l'arc n'est pas marqué
            if(m_edges[m_vertices[idx].m_out[i]].get_mark()==false)
            {
                /// Si le sommet d'arrivée est notre sommet de départ
                if(m_edges[m_vertices[idx].m_out[i]].get_to()==m_sommets.top())
                {
                    /// Alors on met l'arc dans notre vecteur et on appelle la fonction afficher
                    m_chemin.push_back(m_edges[m_vertices[idx].m_out[i]]);
                    afficher();
                }
                /// Si le sommet d'arrivée possède des arcs qui partent de lui et qu'on est pas déjà passé dessus
                if((m_vertices[m_edges[m_vertices[idx].m_out[i]].get_to()].m_out.size()!=0)&&(m_vertices[m_edges[m_vertices[idx].m_out[i]].get_to()].get_passe()==false))
                {
                    /// Alors on parcours tous les arcs qui sortent de ce sommet
                    for(unsigned int j=0; j<m_vertices[m_edges[m_vertices[idx].m_out[i]].get_to()].m_out.size(); j++)
                    {
                        /// Si au moins l'un d'entre eux n'est pas marqué
                        if (m_edges[m_vertices[m_edges[m_vertices[idx].m_out[i]].get_to()].m_out[j]].get_mark()==false)
                        {
                            k=1;
                        }
                    }
                    if(k==1)
                    {
                        /// Alors on marque l'arc
                        /// On indique qu'on est passé sur le sommet
                        /// On rentre l'arc dans le vecteur du chemin
                        /// On appelle la fonction chemin pour le sommet suivant
                        m_edges[m_vertices[idx].m_out[i]].set_mark(true);
                        m_vertices[m_edges[m_vertices[idx].m_out[i]].get_to()].set_passe(true);
                        m_chemin.push_back(m_edges[m_vertices[idx].m_out[i]]);
                        chemin(m_edges[m_vertices[idx].m_out[i]].get_to());
                    }
                    k=0;
                }
                else
                {
                    /// Sinon on marque l'arc et on précise qu'on est passé sur le sommet suivant
                    m_edges[m_vertices[idx].m_out[i]].set_mark(true);
                    m_vertices[m_edges[m_vertices[idx].m_out[i]].get_to()].set_passe(true);
                }
            }
        ///On retire le dernier arc du vecteur
        k=m_chemin[m_chemin.size()-1].get_from();
        m_chemin.pop_back();
        /// si le vecteur est vide
        if(m_chemin.size()==0)
        {
            c=0;
            /// on parcours tous les arcs du sommet initial
            for(unsigned int w=0; w<m_vertices[k].m_out.size(); w++)
            {
                if (m_edges[m_vertices[k].m_out[w]].get_mark()==false)
                    c=1;
            }
            /// Si tous les sommets sont marqués
            if(c==0)
            {
                /// On appelle la fonction composantes connexes du sommet suivant
                k=0;
                comp_fort(idx+1);
            }
        }
        /// Sinon on appelle la fonction chemin denouveau sur le sommet initial
        k=0;
        chemin(k);
    }
    /// Si le sommet n'a pas d'arc qui partent de lui alors on appelle les composantes fortement connexes du sommet suivant
    else
    {
        comp_fort(idx+1);
    }
}

/// Fonction afficher
void Graph::afficher()
{
    int j=0;
    /// On parcours tous les arcs du vecteur
    for(unsigned int i=0; i<m_chemin.size(); i++)
    {
        /// On met tous les sommets des arcs en orange et on affiche les arcs
        m_vertices[m_chemin[i].get_from()].m_interface->m_top_box.set_bg_color(ORANGE);
        std::cout << "(" << m_chemin[i].get_from() << ";" << m_chemin[i].get_to() << ")";
    }
    std::cout << std::endl;
    /// On appelle ensuite la fonction reboot()
    reboot();
    /// On parcours tous les sommets
    for (unsigned int i=0; i<m_vertices.size(); i++)
    {
        if(m_vertices[i].get_mark()==false)
            j=1;
    }
    /// Si au moins 1 sommet n'est pas marqué
    if(j==1)
    {
        /// Alors on appelle la fonction composantes fortement connexes pour le sommet suivant
        j=0;
        comp_fort(m_sommets.top()+1);
    }
}

/// Fonction reboot
void Graph::reboot()
{
    /// On met toutes les arêtes non marquées et tout les sommets non passés et on vide notre vecteur chemin
    for(unsigned int i=0; i<m_edges.size(); i++)
    {
        m_edges[i].set_mark(false);
    }
    for(unsigned int i=0; i<m_vertices.size(); i++)
    {
        m_vertices[i].set_passe(false);
    }
    for(unsigned int i=0; i<m_chemin.size(); i++)
    {
        m_chemin.pop_back();
    }
    m_chemin.pop_back();
}
