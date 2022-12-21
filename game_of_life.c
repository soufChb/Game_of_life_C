#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "mer_twister.h"

/* --------------------------------------------------------------------------------------*/
/* init_grid_randomly       initialse aléatoirement un tableau                           */
/*                          en utilisant la fonction genrand_int32() du générateur       */
/*                          Mersenne Twister et qui génére des entiers 32 bit non signé. */
/*                                                                                       */
/*En entrée :   grid : un tableau d'entiers                                              */
/*              size : sa taille.                                                        */
/*                                                                                       */
/*En sortie :   void                                                                     */
/*---------------------------------------------------------------------------------------*/


void init_grid_randomly(int **grid, unsigned short int size_t) 
{
    unsigned short int i, j;   //indices du tableau(i indiquant la ligne, et j la colonne).
    
    for(i = 0; i < size_t; i++) //parcourt les lignes.
    {
        for(j = 0; j < size_t; j++) //parcourt les colonnes corespondantes à la ligne en i.
        {
            /*remplissage de chaque case du tableau en appelant la fonction genrand_int32() avec un modulo 2
              pour avoir que les valeurs 0 ou 1(si la valeur générée est pair alors le reste de la division 
              par 2 vaut 0, sinon le reste vaut 1).*/ 

            grid[i][j] = genrand_int32() % 2;

        }
    }
}

/*-------------------------------------------------------------------------------------------------------------*/
/* init_grid_manually       initialise les cases choisies par l'utilisateur à la valeur 1, */
/*                                                les autres cases ont la valeur 0.                              */
/*                                                                                                                                  */
/* En entrée :   grid : un tableau d'entiers(toutes les cases sont inialisées à 0).   */
/*               size : sa taille.                                                                                           */
/*                                                                                                                                 */
/* En sortie :   void                                                                                                     */
/*-----------------------------------------------------------------------------------------------------------*/

void init_grid_manually(int **grid, unsigned int size_t) 
{

    unsigned short int row_t, column_t;    //indices de la case que l'utilisateur veut initialiser.  
    
    unsigned short int continu_t = 0;    /*variable pour arreter la boucle lorsque l'utilisateur le décide.
                                         la valeur 0 pour continuer, 1 ou autre valeur pour sortir.*/
    
    while (!(continu_t))        //tant que continu_t est vrai.
    {
        //demande à l'utilisateur d'entrer la ligne et la colonne de la case qu'il veut initialiser à la valeur 1. 
        
        printf("please entre row and column of the cell you want to initialise(bring to life) : \n");
        scanf("%hd", &row_t);
        scanf("%hd", &column_t);
        
        /*vérifier si la case est bien dans le tableau(le comportement(roll over) des entiers non signés lorsque ils ont
          une valeur signé, est souhaitable ici, permet de ne pas évaluer ligne/colonne<0)*/

        if(row_t > size_t - 1 || column_t > size_t - 1) 
        {
            printf("cell out of current grid\n");   //case n'appartient pas au tableau.
        }
        
        else 
        {        
            grid[row_t][column_t] = 1;           //la case choisie appartient au tableau prend la valeur 1.
            
            printf("cell was succefully initialised !\n");
        }

        printf("press 0 to initialise another cell or 1 to quit : " ); //initialiser une autre case ou quitter.
        scanf("%hd", &continu_t);
    }

}

/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*print_2d_grid      affiche un tableau de deux dimensions remplie des valeurs 0 pour une cellule  */
/*                              morte(affiche un point . ) et 1 pour une cellule vivante(affiche un croix X ).        */
/*                                                                                                                                                                */
/*En entrée :   grid : tableau d'entiers.                                                                                                     */  
/*                      size : sa taille.                                                                                                                   */
/*                                                                                                                                                                */
/*En sortie :   void                                                                                                                                     */
/*-------------------------------------------------------------------------------------------------------------------------------------*/

void print_2d_grid(int **grid, unsigned short int size_t) 
{
    unsigned int i,j;
    unsigned int state;  //variable contient le statut des cellules.
    
    for(i = 0; i < size_t; i++)  //parcourt les lignes.
    {
        for(j = 0; j < size_t; j++)   //parcourt les colonnes corespondantes à la ligne i.
        {
            state = grid[i][j];   //on stocke le statut du cellule dans state.
            
            if(state == 0)  //si cellule morte 
            {
                printf(" . ");    //affiche un point
            }
            else if(state == 1) //si cellule vivante
            {
                printf(" X ");   //affiche un croix
            }
        }
        printf("\n");     //retour à la ligne pour améliorer l'affichage
    }
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------*/
/* count_nghbrs_simple_grid     compte les voisins vivants d'une cellule(case) spécifique dans un tableau  */  
/*                                                   sans prendre en compte le voisinage de torus.                                              */
/*                                                                                                                                                                               */
/* En entrée :  grid : un tableau d'entiers.                                                                                                               */
/*              x   : la ligne de la cellule que l'on veut compter les voisins.                                                                 */
/*              y   : la colonne de la cellule que l'on veut compter les voisins.                                                            */
/*             size : la taille du tableau.                                                                                                                           */
/*                                                                                                                                                                                */
/* En sortie :   retourne le nombre des voisins.                                                                                                      */                                              
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

unsigned short int count_nghbrs_simple_grid(int **grid, unsigned short int x, unsigned short int y, unsigned short int size_t) 
{
    signed short int i, j;
    unsigned short int count = 0;      //compteur des voisins.
    unsigned short int row = 0, column =0;   //ligne et colonne de la case voisine.

    /*les voisins d'une cellules sont les cellules qui sont contigues avec cette cellule donc qui sont dans 
      la ligne/colonne juste au dessus ou, juste en dessous ou sur la ligne de la cellule en question.*/

    for(i = -1; i < 2; i++)  //parcourt trois lignes, celle ou se trouve la case celle de dessus et celle dessous.
    {
        for(j = -1; j < 2; j++)  //parcourt trois colonne, celle ou se trouve la case et celles des deux cotés.
        {
            row  = x + i;        //la ligne de la case voisine de notre case.
            column = y + j;      //la colonne de la case voisine de notre case.
            
            /*on verifie que la ligne et la colonne de la case voisine sont bien dans les dimensions du tableau
              et qu'elles sont differentes de la case à laquelle on souhaite calculer les voisins.*/ 

            if(row > size_t - 1 || column > size_t - 1 || (row == x && column == y)) 
            {    
                continue;   //la case voisine n'appartient pas au tableau, en passe à la case suivante.
            }

            if(grid[row][column] == 1) 
            {
                count++;   /*si la case appartient au tableau et qu'elle est vivante(vaut 1)
                              on incremente notre compteur de voisins.*/
            }
        }
    }
    return count;      //retourne le nombre des voisins.
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* count_nghbrs_torus_univers    compte le nombre de voisins d'une case passée en argument dans l'univers */
/*                                                     de torus(c-à-d chaque case possède 8 voisins en considérant que                 */
/*                                                      notre tableau est circulaire).                                                                                 */
/*                                                                                                                                                                                     */
/* En entrée :  grid : un tableau d'entiers.                                                                                                                     */
/*              x   : la ligne de la cellule que l'on veut compter les voisins.                                                                       */
/*              y   : la colonne de la cellule que l'on veut compter les voisins.                                                                 */
/*             size : la taille du tableau.                                                                                                                               */
/*                                                                                                                                                                                     */
/* En sortie :   retourne le nombre des voisins.                                                                                                           */
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

int count_nghbrs_torus_univers(int **grid, unsigned int x, unsigned int y, unsigned int size_t) 
{
    signed short int i,j;
    unsigned short int count = 0;   //compteur des voisins
    unsigned int row = 0, column = 0;   //ligne et colonne de la case voisine.

    /*pour avoir un tableau circulaire on utilisé row/column % size, et pour eviter d'avoir des valeurs de row
        et column notament pour les cases des coins du tableau, on rajoute + size, comme si notre tableau commence
        de la case [size][size] vers la case [taille + size - 1][taille + size - 1]*/

    for(i = -1 ;i < 2; i++) 
    {
        for(j = -1; j < 2; j++) 
        {
            row = (x + i + size_t) % size_t;      //calcule de la ligne de la case voisine. 
            column = (y + j + size_t) % size_t;   //calcule de la colonne de la case voisine.

            //verifie si la case est vivante(vaut 1) et on est different de la case à laquelle on calcule les voisins.  

            if(grid[row][column] == 1 && (row != x || column != y)) 
            {
                count++;     //condition verifiee, on incremente le nombre des voisins
            }

        }
    }
    return count;   //retourne le nombre des voisins.
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
/*game_of_life     remplie un nouveau tableau en se basant sur un tableau de référence et en appliquant       */
/*                                            les règles du jeu de la vie sur les cases du tableau de référence.                         */
/*                                                                                                                                                                                  */
/*En entrée : ref_grid : tableau(d'entiers) de référence à l'instant t.                                                                       */
/*            new_grid : nouveau tableau(d'entiers) qui va etre rempli à l'instant t+1.                                                  */
/*            size : la taille des tableaux.                                                                                                                         */
/*            choice : entier qui prend la valeur 1 ou 2 pour préciser la nature du tableau(tableau avec/sans          */
/*                      torus univers).                                                                                                                                    */
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void game_of_life(int **ref_grid, int **new_grid, unsigned int size_t, unsigned short int choice_t) {
    
    unsigned short int neighbours = 0;   //stocke le nombre de voisins d'une cellule
    unsigned short int i,j;
    unsigned short int state = 0;       //l'état de la cellule (vivante ou morte)

    if(choice_t == 1)       //l'utilisateur a choisi un tableau avec torus univers.
    {
        
        for(i = 0; i < size_t; i++)    //parcout les lignes du tableau de référence
        {
            for(j = 0; j < size_t; j++)   //parcourt les colonnes du tableau de référence
            {     
                state = ref_grid[i][j];   //on stocke l'état de la cellule
                
                neighbours = count_nghbrs_torus_univers(ref_grid, i, j, size_t);  //on compte le nombre de ses voisins

                //si la cellule est morte et le nombre de ses voisins est trois, alors elle revit.

                if(state == 0 && neighbours == 3) 
                {
                    new_grid[i][j] = 1;  /*on stocke son nouvel état dans un nouveau tableau(dans la meme ligne/colonne)
                                         et qui servira de référence pour la prochaine génération.*/ 
                }                        

                /*si la cellule est vivante et le nombre de ses voisins est strictement inf. à 2
                  ou strictement sup. à 3, alors la cellule devient morte.*/

                else if(state == 1 && (neighbours < 2 || neighbours > 3)) 
                {
                    new_grid[i][j] = 0;  //on stocke son nouvel état.
                }

                else      //si le nombre des voisins est dans l'intervalle [2,3], alors la cellule garde son état actuel.
                {
                    new_grid[i][j] = state;   //on stocke son état dans le nouveau tableau.
                }
            }
        }
    }

    if(choice_t == 2)  //l'utilisateur a choisit un tableau non circulaire(sans le voisinage de torus).
    {
        for(i = 0; i < size_t; i++) 
        {
            for(j = 0; j < size_t; j++) 
            {
                state = ref_grid[i][j];
                neighbours = count_nghbrs_simple_grid(ref_grid, i, j, size_t);
            
                if(state == 0 && neighbours == 3) 
                {
                    new_grid[i][j] = 1;
                }

                else if(state == 1 && (neighbours < 2 || neighbours > 3)) 
                {
                    new_grid[i][j] = 0;
                }
                else 
                {
                    new_grid[i][j] = state;
                }
            }
        }
    }
}




int main() 
{

    int **ref_grid;
    int **new_grid;
    int **swap;
    unsigned short int i,j;
    unsigned short int steps_t;
    unsigned short int univers_choice_t;
    unsigned short int init_grid_t;
    unsigned short int size_t;

    printf("please entre size of your grid : "); //demande à l'utilisateur d'entrer la taille du tableau
    scanf("%hd", &size_t);

    //déclaration/initialisation des tableaux à deux dimensions

    ref_grid = (int**) calloc(size_t, sizeof(int*));
    for(i = 0; i < size_t; i++) 
    {
        ref_grid[i] = (int*) calloc(size_t, sizeof(int));
    }

    new_grid = (int**) malloc(size_t * sizeof(int*));
    for(j = 0; j < size_t; j++) 
    {
        new_grid[j] = (int*) malloc(size_t * sizeof(int));
    }

    while (init_grid_t != 1 && init_grid_t != 2)
    {
        //demande à l'utilisateur s'il souhaite remplir le tableau manuellement ou aléatoirement
        printf("press 1 to initialise the grid manually or 2 to initialise it randomly : ");
        scanf("%hd", &init_grid_t);
        
        if(init_grid_t == 1)   //remplissage manuel
        {
            init_grid_manually(ref_grid, size_t);
        }
        
        else if(init_grid_t == 2) 
        {
            
            init_grid_randomly(ref_grid, size_t); //remplissage aléatoire
        }
        
        else 
        {
            printf("choice not valid !\n");
        }
    }
    
    printf("your current grid : \n");  //affichage de tableau remplit par l'utilisateur 
    print_2d_grid(ref_grid, size_t);  

    //demande à l'ulitilisateur le nombre de génération 
    printf("please entre how many steps you want to execute : ");
    scanf("%hd", &steps_t);

    while(univers_choice_t != 1 && univers_choice_t != 2)  //demande du choix de l'univers
    {
        
        printf("press 1 for a game of life in torus univers or 2 for a game of life without torus neighbourhood : ");
        scanf("%hd", &univers_choice_t);
        
        if(univers_choice_t != 1 && univers_choice_t !=2) 
        {
            printf("choice not valid !\n");
        }
    }

    //éxecution du générateur des générations
    for(i = 0; i < steps_t; i++) 
    {   
        system("clear");  //amélioration de l'affichage

        game_of_life(ref_grid, new_grid, size_t, univers_choice_t);
        print_2d_grid(new_grid, size_t);

        sleep(1);   //amélioration de l'affichage
        
        //le nouveau tableau devient référence de la génération suivante et l'ancien contient cette génération
        swap = ref_grid;
        ref_grid = new_grid;
        new_grid = swap;
    }
    return 0;
}
