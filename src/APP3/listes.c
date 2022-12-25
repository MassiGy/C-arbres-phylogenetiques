#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listes.h"

/* fichier à compléter au besoin */

void init_liste_vide(liste_t *L)
{
    //    printf ("<<<<< À faire: fonction init_liste_vide fichier " __FILE__ "\n >>>>>");
    //     /* a completer */
    L->tete = NULL;
}
void afficher_list(liste_t *L)
{
    if (L == NULL || L->tete == NULL)
        return;

    printf("[ ");
    cellule_t *temp = L->tete;

    while (temp->suivant != NULL)
    {
        printf("%s, ", temp->val);
        temp = temp->suivant;
    }
    // print the last one without the comma
    printf("%s ", temp->val);

    printf("] \n");
}

void liberer_liste(liste_t *L)
{

    if (L->tete == NULL)
    {
        return;
    }
    if (L->tete->suivant == NULL)
    {
        free(L->tete);
        return;
    }

    // supprimer à partir de la tete, une cellule à la fois
    cellule_t *temp = L->tete->suivant;

    while (L->tete->suivant != NULL)
    {
        L->tete->suivant = NULL;
        free(L->tete);
        L->tete = temp;
        temp = temp->suivant;
    }

    // supprimer la derniere cellule
    free(L->tete);

    return;
}

int ajouter_tete(liste_t *L, string c)
{ /* retourne 0 si OK, 1 sinon  */

    if (strcmp(c, "") == 0 || c == NULL || strcmp(c, "\0") == 0)
        return 1;

    if (L == NULL)
        return 1; // on ne peut pas ajouté vers une list non crée !, car l'adresse de L est passé par copie!

    if (L->tete == NULL)
    { // par contre on peu ajouter vers une list vide

        L->tete = malloc(sizeof(cellule_t));
        L->tete->val = malloc((strlen(c) + 1) * sizeof(char));
        strcpy(L->tete->val, c);
        L->tete->suivant = NULL;
        return 0;
    }

    // si la list n'est pas vide;

    // ajouter une liste au debut
    cellule_t *temp = malloc(sizeof(cellule_t));
    if (temp == NULL)
        return 1;

    temp->suivant = L->tete;
    temp->val = malloc((strlen(c) + 1) * sizeof(char));
    strcpy(temp->val, c);

    // repositionner la tête
    L->tete = temp;
    return 0;
}

int supprimer_tete(liste_t *L)
{ /* retourne 0 si OK, 1 sinon  */
    if (L == NULL || L->tete == NULL)
    {
        return 1;
    }

    cellule_t *temp = L->tete->suivant;

    free(L->tete);
    L->tete = temp;
    return 0;
}
