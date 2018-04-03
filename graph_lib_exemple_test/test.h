#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "grman/grman.h"
#include <iostream>
#include <stack>

/***************************************************
                    THING (test)

  Cette classe correspond au cadre en bas à gauche
  avec différents bidules interactifs assemblés dessus
  VOIR LE CONSTRUCTEUR ET LA METHODE UPDATE EN DETAIL
  ( dans test.cpp ) pour décrypter l'utilisation
  des widgets proposés ( vous pouvez en ajouter d'autres
  en complétant widget.h et widget.cpp, répertoire
  de projet grman )
****************************************************/
class Thing {

    private :
        /// Utiliser une top_box de type WidgetBox pour encapsuler
        /// tous les éléments de l'interface associés à votre objet
        grman::WidgetBox m_top_box;

        /// Les Widgets qui constitueront l'interface de l'objet
        grman::WidgetImage m_img;            // Sera l'arbre à droite
        grman::WidgetImage m_img_anime;      // Sera le clown marcheur au milieu
        grman::WidgetCheckBox m_marche;      // Sera la boite à cocher en bas à gauche (faire marcher le clown)
        grman::WidgetText m_legende;         // Sera le texte "CLICK ME" en bas

        grman::WidgetBox m_boite_regles;     // Sera la boite à sliders en haut à gauche
        grman::WidgetVSlider m_regle_reel;   // Sera le slider de gauche (ordonnée du clow)
        grman::WidgetVSlider m_regle_entier; // Sera le slider de droite ( vitesse marche clown)

        grman::WidgetBox m_boite_boutons;    // Sera la boite à boutons en haut à droite
        grman::WidgetButton m_bouton1;       // Sera le bouton avec le texte NEW
        grman::WidgetText m_bouton1_label;   // Le texte NEW
        grman::WidgetButton m_bouton2;       // Sera le bouton avec l'image de clown méchant
        grman::WidgetImage m_bouton2_image;  // L'image de clown méchant
        grman::WidgetText m_dragme;          // Le message "DRAG ME" à droite des 2 boutons

        grman::WidgetEdge m_lien1;           // Sera un lien non orienté entre l'arbre et le clown
        grman::WidgetEdge m_lien2;           // Sera un lien orienté entre la boite à boutons et le clown
        grman::WidgetText m_lien2_label;     // Sera un label sur le lien précédent
        grman::WidgetEdge m_lien3;           // Sera un lien orienté entre la check box et le clown

        std::stack<grman::WidgetImage *> m_dynaclowns;    // Expérimental : les clowns ajoutés/enlevés avec les boutons

    public :

        /// Le constructeur de la classe (pas forcément par défaut !)
        /// initialise les données des widgets, place la hiérarchie des sous-cadres etc...
        Thing();

        /// Une méthode "update" de la classe doit être appelée dans la boucle de jeu
        /// et cette méthode doit propager l'appel à update sur les widgets contenus...
        /// Cette méthode fait le lien entre l'interface, les événements, et les conséquences
        //  ( Cette méthode pourrait s'appeler autrement, avoir des paramètres... )
        void update();

        /// On a des allocations dynamiques dans m_dynaclowns => à nettoyer dans le destructeur
        ~Thing();

};


#endif // TEST_H_INCLUDED
