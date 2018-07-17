/***********************************************************************//**
 * \file  TIS_platforms.h
 * \brief Ensemble des �l�ments sp�cifiques � une plateforme utilis�s par TIS.
 *
 * Les �l�ments sp�cifiques � une plateforme sont principalement les types variables, la librairie utilise
 * les types POSIX, il faut donc les red�finir sur les plateforme qui ne sont pas POSIX.
 *
 * Les impl�mentations des fonctions sont pr�sente dans /platforms/src.
 *
 * \author Cl�ment Bonnet
 * \date 2011-2012
 * \copyright Laboratoire de Physique des Oc�ans. Ce code est couvert par la license CeCILL-B.
 ***********************************************************************/

#ifndef TIS_PLATFORMS_H
#define	TIS_PLATFORMS_H


//Plateforme compatible POSIX

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#ifndef PATH_SEPARATOR
/***********************************************************************//**
        * \def PATH_SEPARATOR
        * \brief Caract�re de s�paration de dossier dans les chemins.
        ***********************************************************************/
#define PATH_SEPARATOR "/"
#endif

/***********************************************************************//**
* \def FALSE
* \brief Valeur bool�enne correspondant � FAUX, compatible avec le type bool.
***********************************************************************/
#ifndef FALSE
#define FALSE 0
#endif // FALSE

/***********************************************************************//**
* \def TRUE
* \brief Valeur bool�enne correspondant � VRAI, compatible avec le type bool.
***********************************************************************/
#ifndef TRUE
#define TRUE 1
#endif // TRUE

/***********************************************************************//**
 * \brief Cr�e un fichier temporaire.
 *
 * \return Un pointeur vers le descripteur de fichier du fichier temporaire cr�e.
 ***********************************************************************/
FILE * TIS_create_temporary_file();

/***********************************************************************//**
 * \brief Ferme un fichier temporaire, puis le supprimer
 *
 * \param[in] tfp Pointeur vers le descripteur de fichier du fichier temporaire.
 *
 * \return 0  en cas de succ�s.
 * \return -1 en cas d'erreur.
 ***********************************************************************/
int32_t TIS_delete_temporary_file(FILE * tfp);

/***********************************************************************//**
 * \brief Retourne la taille d'un fichier.
 *
 * \param[in] path Un pointeur vers le chemin du fichier.
 *
 * \return La taille du fichier, s'il existe.
 * \return 0, si le fichier n'existe pas.
 ***********************************************************************/
int64_t TIS_get_file_size(uint8_t * path);

/***********************************************************************//**
 * \brief Permet de savoir si un dossier est vide
 *
 * Un dossier qui n'existe pas est toujours vide.
 *
 * \param[in] path Un pointeur vers le chemin du dossier.
 *
 * \return TRUE s'il est vide.
 * \return FALSE s'il contient au moins un fichier ou un dossier.
 ***********************************************************************/
bool TIS_folder_empty(uint8_t * path);

#endif //TIS_PLATFORMS_H
