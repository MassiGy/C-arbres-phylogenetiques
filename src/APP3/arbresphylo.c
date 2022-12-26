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
      et pour chauqe noeud qui n'est pas une feuille nous allons incrémenter le *nb_carac

      time complexity    ~ O(N) | N = nombre de noeuds
      space complexity   ~ O(1) | aucune duplication ou copies/structures de données annexes necéssaire.
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
    * le chemain qui mène vers elle dans l'arbre, nous devons parcourir l'arbre
    * dans son entrièreté, afin de trouver l'espece.
    *
    * On utilisera un parcours en profondeur, car les especes sont les feuilles de l'arbre.
    *
    * lors du parcour, nous devons construire selon le chemin actuellement suivi la list des caractéristiques (caracs)
    * en ajoutant à chaque fois en tête (la derniere caractéristique)
    *
    * si à la fin d'un parcour,( du coup si on est arrivé vers une feuille)
    *
    * si l'espece est trouvé, on retourne 0, et la liste sera valide !
    * si non, on verfie le fils droit, et si il est pas une feuille, on repars dans un parcours similaire,
    *
    * si non si il est une feuille, et il l'a l'éspece recherché on retourne 0 et la liste sera valide !
    * si non, on supprime la dernière carac de la list ( du coup on supprime un élément en tête), et on sort de
    * l'appel récursif actuel, pour entamer un autre appel récursif sur le fils droit du noeud parant à la racine
    * actuelle.
    */

   if (racine == NULL)
      return 1;

   if (racine->droit == NULL && racine->gauche == NULL && strcmp(racine->valeur, espece) != 0)
      return 1; // feuille qui ne contient pas l'éspece recherchée

   if (racine->droit == NULL && racine->gauche == NULL && strcmp(racine->valeur, espece) == 0)
      return 0; // feuille qui contient l'éspece recherchée

   // si l'éspece n'a pas la caractéristique de racine, alors resGauche sera zgale à 0;
   int resGauche = rechercher_espece(racine->gauche, espece, seq);
   // si l'éspece a la caractéristique de racine, alors resDroite sera égale à 0.
   int resDroite = rechercher_espece(racine->droit, espece, seq);

   int res = resGauche + resDroite;

   if (resDroite == 0)
   {  
      // cela veux dire, que l'espèce a était trouvé dans le sous-arbre droite
      // donc, elle a la caractéristique de la racine actuelle.
      //  on ajoute la carac de la racine actuelle dans la list
      ajouter_tete(seq, racine->valeur);
   }

   if (res == 1)
   {
      // cela veux dire que l'éspèce a était trouvé, car on a eu un resultat du genre 1 = 1 + 0
      // on réajuste pour l'appel recursif parent.
      res = 0;
   }
   else
   {
      // cela veux probablement dire que res = 2, donc on a eu un résultat 2 = 1 + 1
      // on réajuste pour l'appel recursif parent.
      res = 1;
   }

   // retourner le resultat vers l'appel recursif parent.
   return res;
}

/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un
 * message d'erreur.
 */
int ajouter_espece(arbre *p_arbre, char *espece, cellule_t *seq)
{
   /**
    * Notre algorithm va consister à parcourir l'arbre à partir de la racine vers le bas,
    * En faisant cela, nous allons à chaque étape voir si l'espece à insérer a la carac
    * du noeud actuel de l'arbre.
    *
    * Si oui, nous allons continuer vers la droite, 
    * et nous allons en même temp supprimer un element de la tête de la list.
    * 
    * Si non, nous allons continuer vers la gauche, 
    * et nous allons en même temp supprimer un element de la tête de la list.
    *
    * Si au bout d'une certaine progression dans le parcours de l'arbre, 
    * on découvre que l'espece à une carac qui n'est pas dans l'arbre.
    * 
    * Nous devons alors insérer les caracs que cette nouvelle éspèce possède et qui
    * ne sont pas dans l'arbre.
    * 
    * Pour cela, si nous nous retrouvons dans un cas ou:
    * 
    * - l'insertion de l'espece, doit se faire dans un sous arbre gauche, qui contient
    * qu'une feuille actuellement (une autre espèce dèjà dans l'arbre):
    * 
    * Si on a plus de carac dans la liste de caracs, alors cela c'est une erreur, car
    * les deux espèce ne peuvent partager le même emplacement
    * 
    * Si non, nous allons créer un sous arbre gauche et droit pour cette feuille,
    * nous allons stocker l'éspèce déjà dans l'arbre dans le fils gauche de cette feuille.
    * et puis nous allons insérer toutes les caracs que possède l'éspèce passée en paramètres en plus,
    * dans des sous-arbres droit uniquement, avant d'insérer l'éspèce elle - même à la fin.
    * 
    * Note: la raison, pour laquelle, nous avons stocker l'espèce déjà dans l'arbre dans le sous-arbre gauche
    * crée, est car cette éspèce n'a aucune carac parmis les caracs en plus qu'a l'éspèce à insérer (passée en 
    * paramètres ), c'est logique, si non, les caracs en communs seront déjà dans l'arbre, étant donnée que l'espèce est 
    * déjà dans l'arbre [Hypothèse de l'acte 2]
    * 
    * 
    * - l'insertion de l'éspèce, doit se faire dans un sous arbre droit, qui contient déjà une feuille (éspèce)
    * 
    * 
    * Si on a plus de carac dans la liste de caracs, alors cela c'est une erreur, car
    * les deux espèce ne peuvent partager le même emplacement
    * 
    * Si non, nous allons faire une idée similaire à celle énoncée en haut. Nous allons rendre la feuille un sous-arbre
    * en lui ajoutant des fils gauche et droit. Le fils gauche, sera une feuille et stockera l'éspèce déjà en place
    * dans l'arbre. Le fils droit va être une succession de sous arbre droit (une branche vers la droite) 
    * qui contiendra toute les carac en plus dans la liste, et puis l'espèce à insérer elle même comme la feuille
    * à la fin.
    */

   if (p_arbre == NULL)
   {
      printf("\033[0;31mERREUR\033[0m : l'adresse de l'arbre passée n'est pas valide\n");
      return 1;
   }

   if ((*p_arbre) == NULL)
   {
      // ça veux dire qu'il faut construire l'arbre / le sous-arbre
      if (seq == NULL)
      {
         // ça veux dire que c'est le dernier appel récursif.
         // ajouter l'espece
         *p_arbre = malloc(sizeof(arbre));
         (*p_arbre)->valeur = malloc(sizeof(char) * (strlen(espece) + 1));
         strcpy((*p_arbre)->valeur, espece);
         (*p_arbre)->droit = NULL;
         (*p_arbre)->gauche = NULL;

         return 0;
      }
      else
      {
         // ça veux dire que ce n'est pas le dernier appel récursif, ou bein que l'arbre est vide

         // ajouter dans la racine actuelle la carac en tête de la seq
         *p_arbre = malloc(sizeof(arbre));
         (*p_arbre)->valeur = malloc(sizeof(char) * (strlen(seq->val) + 1));
         strcpy((*p_arbre)->valeur, seq->val);
         (*p_arbre)->gauche = NULL;
         (*p_arbre)->droit = NULL;

         // faire un appel recursif sur le fil droit pour insérer l'espece
         return ajouter_espece(&((*p_arbre)->droit), espece, seq->suivant);
      }
   }

   if ((*p_arbre)->droit == NULL && (*p_arbre)->gauche == NULL)
   {
      // ça veux dire qu'on est arrivé à une espèce.
      if (seq == NULL)
      {
         // si l'espece dans l'arbre et celle à insérer ont exactement les mêmes caracs,
         // on revois un message d'erreurs, car on ne peut l' insérer.
         printf("\033[0;31mERREUR\033[0m : l'espèce %s a les mêmes caracs que l'espèce %s.\n", espece, (*p_arbre)->valeur);
         return 1;
      }
      else
      {
         // si non, on insere les carac de l'espece en paramètres,
         // puis reinsérer l'espece présente dans l'arbre dans le fils gauche,
         // car forcement elle a pas les caracs en plus, que l'espece en paramètres a.
         // puis enfin, insérer l'espece en paramètres, dans sous fil droit,
         // car l'espece en param a toutes les carac de la seq.

         // sauvegarder la valeur de l'espece actuellement dans l'arbre. ( de la racine actuelle )
         char *temp = malloc(sizeof(char) * (strlen((*p_arbre)->valeur) + 1));
         strcpy(temp, (*p_arbre)->valeur);

         // copier la carac en tête de seq vers la racince actuelle
         strcpy((*p_arbre)->valeur, seq->val);

         // copier l'espece stocker dans temp vers le fils gauche de la racine actuelle
         // note: ce fil gauche n'existe pas encore.
         (*p_arbre)->gauche = malloc(sizeof(arbre));
         (*p_arbre)->gauche->valeur = malloc(sizeof(char) * (strlen(temp) + 1));
         strcpy((*p_arbre)->gauche->valeur, temp);
         (*p_arbre)->gauche->droit = NULL;
         (*p_arbre)->gauche->gauche = NULL;

         // insérer l'espece en paramètre dans le fils droit.
         // note: ce fils droit n'existe pas encore.
         return ajouter_espece(&((*p_arbre)->droit), espece, seq->suivant);
      }
   }

   if (strcmp((*p_arbre)->valeur, seq->val) == 0)
   {
      // ça veut dire que l'espèce a la carac et qu'on doit l'insérer à la droite
      // si j'ai la carac dans l'arbre, passer à la suivante dans la liste
      return ajouter_espece(&((*p_arbre)->droit), espece, seq->suivant);
   }
   else
   {
      // ça veut dire que l'espèce n'a pas la carac et qu'on doit l'insérer à la gauche
      // si je n'ai pas la carac dans l'arbre, passer la liste entière pour l'insérer
      return ajouter_espece(&((*p_arbre)->gauche), espece, seq);
   }

   return 1;
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */
void afficher_par_niveau(arbre racine, FILE *fout)
{
   /**
    * L'algorithm va être un parcour en largeur, breadth first search
    *
    * Pour cela, on peut utiliser un algorithm itératif. On utilisera aussi
    * deux listes comme des files
    *
    * une file result, elle stockera les valeurs des carac et un "\n" après
    * chaque niveau parcouru, pour un correct affichage.
    *
    * une file visited, elle stockera les références des noeuds contenant une carac visités.
    *
    * On va appeler la fonction à partir de la racine, et on va mettre la valeur de la racine
    * dans la file result, suivi du "\n", et on mettera la référence de la racine dans la file
    * visited.
    *
    * Puis à l'aide d'une boucle tant que, muni d'une condition d'arrêt qui teste le fait que
    * la longueure de la file visited > 0
    *
    * Nous allons, retirer le premier elements de la file visited
    * puis le déréférencé et ajouter les références de son fils gauche d'abord dans la file
    * visited, et aussi sa valeur suivi d'un "\n" dans la file result,
    * 
    * puis faire la même chose pour le fils droit.
    *
    * Passer à l'itération suivante de la boucle tant que.
    *
    * à la fin on aurra notre affichage dans la liste result,
    * chaque deux étage sont sparré par un "\n"
    *
    *
    * Or, même si cela est la plus facile, optimisé et intuitive approche, cela ne marchera pas dans notre
    * contexte, car notre liste n'a pas de champ pour stocker une référence. À moins de l'ajouter.
    *
    *
    *
    * Du coup, nous allons utiliser la version récursive, qui est un peu plus compliqué à assimiler.
    *
    * l'idée est simple: nous allons utiliser un depth first search pour afficher, mais nous allons indiquer
    * la profondeur à laquelle l'affichage doit être éffectuer.
    *
    * Nous allons aussi, déclarer une fonction utilitaire (helper) pour arriver à ce but.
    * cette fonction nous permettera d'afficher un étage de l'arbre à une certaine profondeur passée en paramètres
    *
    * Premièrement, dans la fonction principale, nous allons itérer sur tous les étage de l'arbre et appeler
    * la fonction utilitaire.
    *
    * Dans la fonction utilitaire, qui va recevoir la racine de l'arbre et un étage L, nous allons faire deux appels
    * recursif sur le fils gauche puis sur le fils droit, en décrimentant l'etage de 1 à chaque fois.
    *
    * notre condition d'arret est quand L == 0, dans ce cas on va afficher la valeur.
    *
    *
    * Une autre façon de voir cette idée, c'est de faire un depth first search, avec un compteur, et tant que
    * ce compteur n'a pas attient une valeur, aucune valeur ne doit être affichée.
    *
    * Nous pouvons aussi faire le contraire, ou on passe un compteur qui commence à 1, et qu'on va incrémenter
    * jusqu'a la valeur L. mais la valeur de L, devra être ajouter à l'aide d'un troisième paramètre.
    */
   int height = hauteur_arbre(racine);
   for (int level = 1; level <= height; ++level)
   {
      // afficher l'étage à la profondeur i
      printCurrentLevel(racine, fout, 1, level);
      fprintf(fout, "%s", "\n");
   }
   return;
}

void printCurrentLevel(arbre racine, FILE *fout, int accumulator, int level)
{

   if (racine == NULL)
      return;

   if (accumulator == level && !(racine->droit == NULL && racine->gauche == NULL))
   {
      fprintf(fout, "%s ", racine->valeur);
   }
   else
   {
      // the accumulator goes from 1 (first level | top most level ) to the given level. 
      printCurrentLevel(racine->gauche, fout, accumulator + 1, level);
      printCurrentLevel(racine->droit, fout, accumulator + 1, level);
   }
}

int max(int a, int b)
{
   if (a >= b)
      return a;
   return b;
}

int hauteur_arbre(arbre racine)
{
   if (racine == NULL)
      return 0;
   if (racine->droit == NULL && racine->gauche == NULL)
      return 1;

   return (1 + max(hauteur_arbre(racine->gauche), hauteur_arbre(racine->droit)));
}

// Acte 4

int ajouter_carac(arbre *a, char *carac, cellule_t *seq)
{
   printf("<<<<< À faire: fonction ajouter_carac fichier " __FILE__ "\n >>>>>");
   return 0;
}
