/***********************************************************************//**
 * \file  TIS_errors.h
 * \brief D�claration des diff�rents codes d'erreurs.
 *
 * Certaines de ces erreurs sont utilis�es en interne par la librairie et ne serons retourn�s par aucune fonction publique.
 *
 * \author Cl�ment Bonnet
 * \date 2011-2012
 * \copyright Laboratoire de Physique des Oc�ans. Ce code est couvert par la license CeCILL-B.
 ***********************************************************************/

#ifndef TIS_ERRORS_H
#define	TIS_ERRORS_H

/***********************************************************************//**
 * \def TIS_ERROR_SUCCESS
 * \brief La fonction s'est termin� avec succ�s.
 ***********************************************************************/
#define TIS_ERROR_SUCCESS						 0

/***********************************************************************//**
 * \def TIS_ERROR_UNKNOWN
 * \brief Une erreur s'est produite et sa cause est inconnue.
 ***********************************************************************/
#define TIS_ERROR_UNKNOWN						 1

/***********************************************************************//**
 * \def TIS_ERROR_NOT_AVAILABLE
 * \brief La fonction ne peut s'ex�cuter car le modem ne supporte pas la fonctionnalit�e associ�e.
 ***********************************************************************/
#define TIS_ERROR_NOT_AVAILABLE 				 2

/***********************************************************************//**
 * \def TIS_ERROR_SERIAL_ERROR
 * \brief Une erreur de lecture ou d'�criture s'est produite sur la liaison s�rie.
 *
 * La lecture de donn�es attendues mais pas pr�sente non consid�r� comme une erreur.
 * A la r�ception de cette erreur, il est conseill� de red�marrer le modem.
 ***********************************************************************/
#define TIS_ERROR_SERIAL_ERROR  				 3

/***********************************************************************//**
 * \def TIS_ERROR_FILE_ACCESS
 * \brief Une erreur lors de l'�criture ou de la lecture d'un fichier s'est produite.
 ***********************************************************************/
#define TIS_ERROR_FILE_ACCESS  					 4

/***********************************************************************//**
 * \def TIS_ERROR_UNDEFINED_PARAMETER
 * \brief Au moins un des param�tres donn�s � la fonction n'est pas d�fini.
 ***********************************************************************/
#define TIS_ERROR_UNDEFINED_PARAMETER	  		 5

/***********************************************************************//**
 * \def TIS_ERROR_CHECK_SUM
 * \brief Une erreur de transfert a �t� d�tect� entre l'instrument et le modem Iridium.
 *
 * A la r�ception de cette erreur, il est conseill� de red�marrer le modem.
 ***********************************************************************/
#define TIS_ERROR_CHECK_SUM				  		 6

/***********************************************************************//**
 * \def TIS_ERROR_TIMEOUT
 * \brief Un timeout a �t� d�pass� dans la communication avec le modem.
 *
 * A la r�ception de cette erreur, il est conseill� de red�marrer le modem.
 ***********************************************************************/
#define TIS_ERROR_TIMEOUT				  		 7

/***********************************************************************//**
 * \def TIS_ERROR_OVERFLOW
 * \brief Un tampon a �t� d�pass�.
 ***********************************************************************/
#define TIS_ERROR_OVERFLOW				  		 8

/***********************************************************************//**
 * \def TIS_ERROR_DIALUP
 * \brief Une erreur s'est produite lors d'un appel RUDICS, soit le num�ro est incorrect, soit la r�ception satellite n'est pas suffisante.
 ***********************************************************************/
#define TIS_ERROR_DIALUP						 9

/***********************************************************************//**
 * \def TIS_ERROR_NO_SPACE_AVAILABLE
 * \brief Il n'y a plus assez de place sur le disque ou la carte m�moire.
 ***********************************************************************/
#define TIS_ERROR_NO_SPACE_AVAILABLE			10

/***********************************************************************//**
 * \def TIS_ERROR_RUDICS_IDENTIFICATION_FAILED
 * \brief Une erreur s'est produite lors de l'identification de l'instrument au d�but d'une session RUDICS.
 ***********************************************************************/
#define TIS_ERROR_RUDICS_IDENTIFICATION_FAILED 	11

/***********************************************************************//**
 * \def TIS_ERROR_TOO_MANY_CONSECUTIVE_ERRORS
 * \brief La communication s'est termin� car trop d'erreurs cons�cutives se sont produites. Une partie des donn�es a cependant �t� transf�r�.
 ***********************************************************************/
#define TIS_ERROR_TOO_MANY_CONSECUTIVE_ERRORS	12

/***********************************************************************//**
 * \def TIS_ERROR_NOT_REGISTERED
 * \brief Le modem n'a pas r�ussi � s'enregistrer avec la constellation.
 *
 * A la r�ception de cette erreur, il est conseill� de red�marrer le modem.
 ***********************************************************************/
#define TIS_ERROR_NOT_REGISTERED				13

/***********************************************************************//**
 * \def TIS_ERROR_FILE_TOO_LARGE
 * \brief Le ficher envoy� est trop gros.
 ***********************************************************************/
#define TIS_ERROR_FILE_TOO_LARGE				14

/***********************************************************************//**
 * \def TIS_ERROR_TOO_MANY_FILES_PARTS
 * \brief Cette erreur se produit lorsque la liste des parties de fichiers en cours de r�ception est pleine.
 *
 * Il vous fait alors augmenter la valeur de TIS_SBD_INCOMMING_FILE_QUEUE_SIZE.
 *
 * \sa TIS_SBD_INCOMMING_FILE_QUEUE_SIZE
 ***********************************************************************/
#define TIS_ERROR_TOO_MANY_FILES_PARTS			15

/***********************************************************************//**
 * \def TIS_ERROR_INVALID_FILE_NAME
 * \brief Le nom d'un fichier envoy� est incorrect.
 ***********************************************************************/
#define TIS_ERROR_INVALID_FILE_NAME				16

/***********************************************************************//**
 * \def TIS_ERROR_NOT_ENOUGH_MEMORY
 * \brief Le syst�me n'a pas assez de m�moire, cette erreur se produit lorsque la librairie alloue de la m�moire dynamiquement et que la m�moire est trop faible.
 ***********************************************************************/
#define TIS_ERROR_NOT_ENOUGH_MEMORY				17

/***********************************************************************//**
 * \def TIS_ERROR_LIST_FULL
 * \brief Une liste est pleine.
 ***********************************************************************/
#define TIS_ERROR_LIST_FULL						18

#endif //TIS_ERRORS_H