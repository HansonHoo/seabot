/***********************************************************************//**
 * \file TIS_rudics.h
 * \brief El�ments permettant de r�aliser une communication via RUDICS.
 *
 * La communication RUDICS utilise le protocole Zmodem, il faut donc que la librairie Zmodem soit pr�sente pour que cette partie soit op�rationnelle.
 * Si vous n'utilisez pas le mode RUDICS et que vous voulez diminuer la taille du code, vous pouvez utiliser l'option DISABLE_RUDICS (voir tis.h).
 *
 * Ces fonctions communiquent avec le modem par l'interm�diaire des commandes AT d�finis dans TIS_at.h.
 * 
 * \author Cl�ment Bonnet
 * \date 2011-2012
 * \copyright Laboratoire de Physique des Oc�ans. Ce code est couvert par la license CeCILL-B.
 ***********************************************************************/

#ifndef TIS_RUDICS_H
#define	TIS_RUDICS_H

#include "TIS_platforms.h"
#include "tis.h"

/***********************************************************************//**
 * \brief Effectue une session d'envoie et de r�ception RUDICS.
 *
 * Les fichiers reconstitu�s sont stock�s dans le dossier "receive_folder".
 *
 * Si le nombre d'erreurs de connexions (et non de d�connexions) cons�cutives "maximum_consecutive_errors" est atteint, la fonction se termine et renvoie l'erreur TIS_ERROR_TOO_MANY_CONSECUTIVE_ERRORS.
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 * \param[in] maximum_consecutive_errors Nombre d'erreurs cons�cutives maximum avant de quitter.
 *
 * \return Un code d'erreur.
 * \sa TIS_errors.h
 *
 * \todo D�placer les fichiers re�ues compl�tement du dossier temporaire au dossier de r�ception.
 ***********************************************************************/
 int32_t TIS_RUDICS_transmission(TIS_properties * properties, int32_t maximum_consecutive_errors);
					  	    
/***********************************************************************//**
 * \brief Callback fournie � la librairie Zmodem pour r�cup�rer l'avancement du fichier courant.
 *
 * \param[in] not_used Pour compatibilit� de prototype uniquement, ne pas utiliser.
 * \param[in] progress Position dans le fichier courant.
 ***********************************************************************/
void TIS_progress(void * not_used, int64_t progress);

/***********************************************************************//**
 * \brief R�cup�re la valeur de l'avancement du fichier courant fourni par Zmodem.
 *
 * \return not_used Position dans le fichier courant.
 ***********************************************************************/
int64_t TIS_RUDICS_get_current_file_progress();

/***********************************************************************//**
 * \brief Callback fournie � la librairie Zmodem pour lui permettre d'envoyer un octet sur la liaison s�rie.
 *
 * \param[in] properties Pointeur vers les propri�t�s de la liaison s�rie.
 * \param[in] data La donn�e � envoyer.
 * \param[in] timeout La dur�e du timeout.
 *
 * \return 0  en cas de succ�s.
 * \return -1 en cas d'erreur ou de timeout.
 ***********************************************************************/
int32_t TIS_send_byte(void * properties, uint8_t data, uint32_t timeout);

/***********************************************************************//**
 * \brief Callback fournie � la librairie Zmodem pour lui permettre de recevoir un octet sur la liaison s�rie.
 *
 * \param[in] properties Pointeur vers les propri�t�s de la liaison s�rie.
 * \param[in] timeout La dur�e du timeout.
 *
 * \return La donn�e en cas de succ�s.
 * \return -1 en cas d'erreur ou de timeout.
 ***********************************************************************/
int32_t TIS_receive_byte(void * properties, uint32_t timeout);

/***********************************************************************//**
 * \brief Callback fournie � la librairie Zmodem pour lui permettre de vider le tampon d'�mission de la liaison s�rie.
 *
 * \param[in] properties Pointeur vers les propri�t�s de la liaison s�rie.
 ***********************************************************************/
void TIS_flush_TX(void * properties);

/***********************************************************************//**
 * \brief Callback fournie � la librairie Zmodem pour lui permettre d'attendre des donn�es sur la liaison s�rie.
 *
 * \param[in] properties Pointeur vers les propri�t�s de la liaison s�rie.
 * \param[in] timeout La dur�e du timeout.
 *
 * \return TRUE  d�s que des donn�es sont disponibles.
 * \return FALSE si le timeout a �t� atteint et qu'aucune donn�e n'est disponible.
 ***********************************************************************/
bool TIS_wait_data(void * properties, uint32_t timeout);

/***********************************************************************//**
 * \brief Callback fournie � la librairie Zmodem pour lui permettre de signaler la r�ception d'un fichier.
 * 
 * Cette fonction d�place le fichier re�ue du dossier temporaire au dossier de r�ception.
 ***********************************************************************/
void TIS_file_received(void * properties, int8_t* name);					  	 

#endif //TIS_RUDICS_H