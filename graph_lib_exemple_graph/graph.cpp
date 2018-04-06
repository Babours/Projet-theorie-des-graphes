#include "graph.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <memory>
#include <queue>

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

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 10000.0); // Valeurs arbitraires, à adapter...
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
        m_img.set_gravity_xy(grman::GravityX::Right , grman::GravityY::Down);
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
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
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
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    int nb_sommet, nb_arete, x, y, sommet1, sommet2, repro;
    double poids, val;
    std::string bitmap, nom;
    std::cout << "Rentrer le nom du graphe: ";
    std::cin >> nom;
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    std::ifstream fichier ( nom , std::ios::in);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);
    fichier >> nb_sommet >> nb_arete;
    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    for (int i=0; i<nb_sommet; i++)
    {
        fichier >> val >> x >> y >> bitmap >> repro;
        add_interfaced_vertex(i, val, x, y, repro, bitmap);
    }

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
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
        if(elt.second.m_interface->m_supp_vertex.clicked())
        {
            Supp_Vertex(elt.first);
        }
        if(elt.second.m_interface->m_add_edge.clicked() )
        {
            elt.second.m_interface->m_add_edge.set_dej_active(true);
            for (auto &id : m_vertices)
            {
                if((id.second.m_interface->m_add_edge.get_dej_active()) && (id.first!=elt.first))
                {
                    test_add_edge(id.first, elt.first);
                    elt.second.m_interface->m_add_edge.set_dej_active(false);
                    id.second.m_interface->m_add_edge.set_dej_active(false);
                }
            }
        }
        if(elt.second.m_interface->m_supp_edge.clicked() )
        {
            elt.second.m_interface->m_supp_edge.set_dej_active(true);
            for (auto &id : m_vertices)
            {
                if((id.second.m_interface->m_supp_edge.get_dej_active()) && (id.first!=elt.first))
                {
                    test_supp_edge(id.first, elt.first);
                    elt.second.m_interface->m_supp_edge.set_dej_active(false);
                    id.second.m_interface->m_supp_edge.set_dej_active(false);
                }
            }
        }
        ///if(bouton_bleu)
        {
            for (auto &id : m_vertices)
            {
                for (int i=0; i<id.second.m_in.size(); i++)
                {
                    id.second.set_k(id.second.get_k()+((m_edges[id.second.m_in[i]].get_weight())*(m_vertices[m_edges[id.second.m_in[i]].get_from()].get_value())));
                }
            }
            for (auto &id : m_vertices)
            {
                j=0.0;
                for (int i=0; i<id.second.m_out.size(); i++)
                {
                    j=j+((m_edges[id.second.m_out[i]].get_weight())*(m_vertices[m_edges[id.second.m_out[i]].get_to()].get_value()));
                }
                file_j.push(j);
            }
            for (auto &id : m_vertices)
            {
                id.second.set_value((id.second.get_value())+((id.second.get_repro())*(id.second.get_value())*(1.0-((id.second.get_value())/(id.second.get_k()))))-(file_j.front()));
                if(id.second.get_value()<0)
                {
                    id.second.set_value(0);
                }
                file_j.pop();
            }

        }
    }

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, int repro, std::string pic_name, int pic_idx)
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

void Graph::sauvegarde()
{
    std::fstream fichier;
    fichier.open( "test.txt" , std::ios::out | std::ios::trunc);
    fichier << m_vertices.size() << std::endl << m_edges.size() << std::endl;
    for (std::map<int, Vertex>::iterator i=m_vertices.begin(); i!=m_vertices.end(); i++)
    {
        fichier << i->second.get_value() << " " << i->second.m_interface->m_top_box.get_posx() << " " << i->second.m_interface->m_top_box.get_posy() << " " << i->second.m_interface->m_img.get_name() << " " << i->second.get_repro() << std::endl;
    }
    for (std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        fichier << it->second.get_from() << " " << it->second.get_to() << " " << it->second.get_weight() << std::endl;

    }
    fichier.close();
}

void Graph::Supp_Vertex(int i)
{
    for (auto &it : m_edges)
    {
        if(it.second.get_from()==i||it.second.get_to()==i)
        {
            test_remove_edge(it.first);
        }
    }
            //m_vertices.erase(i);
}



void Graph::test_add_edge(int idx1, int idx2)
{
    std::vector<int> select;
    for (std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        select.push_back(it->first);
    }

    for (unsigned int i=0; i<select.size(); i++)
    {
        for(unsigned int j=0; j<select.size(); j++)
        {
            if(select[i]<select[j])
            {
                select[i]=select[j];
            }
            else {select[j]=select[i];}
        }
    }
    add_interfaced_edge(select[0]+1, idx1, idx2, 0.0);
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

void Graph::test_supp_edge (int i, int j)
{
    for (std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        if((it->second.get_from()==i)&&(it->second.get_to()==j))
        {
            test_remove_edge(it->first);
        }
    }
}
