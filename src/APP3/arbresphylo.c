#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "arbres.h"
#include "arbresphylo.h"
#include "listes.h"

void analyse_arbre(arbre racine, int *nb_esp, int *nb_carac)
{

   /**
      nous allons parcourir l'arbre binaire avec un depth first search,
      pour chaque feuille, nous allons incrémenter le *nb_esp,
      et pour chauqe noeud qui n'est pas une feuille nous allons incrémenter le *,b_carac

      complexity en temps ~ O(N) | N = nombres de noeuds
      complexity en espace ~ O(1) | aucune duplication ou copie necéssaire.
   */

   if (racine == NULL)
      return;

   if (racine->droit == NULL && racine->gauche == NULL)
   {
      (*nb_esp) += 1;
      return;
   }

   // à ce point, nous somme sur que ce n'est pas une feuille
   (*nb_carac) += 1;

   // nous pouvons entamner les appel récursifs
   analyse_arbre(racine->gauche, nb_esp, nb_carac);
   analyse_arbre(racine->droit, nb_esp, nb_carac);

   // pour sortir de puis le premier appel de la fonction
   return;
}

/* ACTE II */
/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */
int rechercher_espece(arbre racine, char *espece, liste_t *seq)
{
   /**
    * Étant donnée que le fait de savoir l'espace nous permet pas d'avoir
    * le chemain qui mene vers elle dans l'arbre, nous devons parcourir l'arbre
    * dans son entrièreté, afin de trouver l'espece.
    *
    * On utilisera un parcours en profondeur, car les especes sont les feuilles de l'arbre
    *
    * lors du parcour, nous devons construire selon le chemin actuellement suivi la list des caracs
    * en ajoutant à chaque fois en tête (la derniere caracs)
    * si à la fin d'un parcour, du coup si on est arrivé vers une feuille,
    * si alors l'espece est trouvé, on retourne 0, et la liste sera valide !
    * si non, on verfie le fils droit, et si il est pas une feuille, on repars dans un parcours similair,
    * si non si il est une feuille, si il l'a l'éspece recherché on retourne 0 et la list sera valid !
    * si non, on supprime la dernière caracs de la list ( du coup on supprime un elmenet en tête), et on pars sur
    * le fils droit du noeud parant de la racine, ( recursivement )
    */
   if (racine == NULL)
      return 1;

   if (racine->droit == NULL && racine->gauche == NULL && strcmp(racine->valeur, espece) != 0)
      return 1;

   if (racine->droit == NULL && racine->gauche == NULL && strcmp(racine->valeur, espece) == 0)
      return 0;

   // l'espece n'a pas la caracteristique de racine, alors resGauche sera egale à 0;
   int resGauche = rechercher_espece(racine->gauche, espece, seq);
   // si l'espece a la caracteristique de racine, alors resDroite sera egale à 0.
   int resDroite = rechercher_espece(racine->droit, espece, seq);

   int res = resGauche + resDroite;

   if (resDroite == 0)
   {
      //  on ajoute la carac de la racine actuel dans la list
      ajouter_tete(seq, racine->valeur);
   }

   if (res == 1)
   {
      res = 0;
   }
   else
   {
      res = 1;
   }

   return res;
}

/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un
 * message d'erreur.
 */
int ajouter_espece(arbre *a, char *espece, cellule_t *seq)
{

   return 1;
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */
void afficher_par_niveau(arbre racine, FILE *fout)
{
   printf("<<<<< À faire: fonction afficher_par_niveau fichier " __FILE__ "\n >>>>>");
}

// Acte 4

int ajouter_carac(arbre *a, char *carac, cellule_t *seq)
{
   printf("<<<<< À faire: fonction ajouter_carac fichier " __FILE__ "\n >>>>>");
   return 0;
}
