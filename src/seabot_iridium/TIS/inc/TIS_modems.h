/***********************************************************************//**
 * \file  TIS_modems.h
 * \brief D�finitions des diff�rents modems.
 *
 * Les modems sont d�finies par une constante permettant de les indexer dans la structure TIS_modems.
 * Les modems poss�dant les m�mes caract�ristiques pointes sur la m�me entr�e.
 *
 * Pour rajouter un modem, si celui-ci est similaire � un modem d�j� pr�sent,
 * rajoutez une constante ayant comme valeur le modem de r�f�rence.
 * Si le modem poss�de des caract�ristiques diff�rentes des modems d�j� pr�sents,
 * rajoutez une entr�e dans la structure TIS_modems puis une constante correspondant � ce modem.
 *
 * \author Cl�ment Bonnet
 * \date 2011-2012
 * \copyright Laboratoire de Physique des Oc�ans. Ce code est couvert par la license CeCILL-B.
 ***********************************************************************/


#ifndef TIS_GPS_H
#define	TIS_GPS_H

#include "TIS_platforms.h"

/***********************************************************************//**
 * \struct TIS_modem
 * \brief Repr�sentation des caract�ristiques d'un modem Iridium.
 ***********************************************************************/
typedef struct {
	bool sbd;				/**< Indique si le modem g�re le mode SBD.*/
	bool rudics;			/**< Indique si le modem g�re le mode RUDICS.*/
	bool sim_card;			/**< Indique si le modem poss�de une carte SIM.*/
	bool gps;				/**< Indique si le modem poss�de une puce GPS.*/
	uint32_t sbd_size_max;	/**< Taille maximale d'un SBD en �mission.*/
} TIS_modem;


/***********************************************************************//**
 * \var TIS_modems
 * \brief Variable globale contenant l'ensembles de propri�t�s des modems support�s par la librairie.
 ***********************************************************************/
//								  SBD	RUDICS SIM	  GPS	 taille SBD
static TIS_modem TIS_modems[] = {{TRUE, FALSE, FALSE, FALSE, 340}, 		//modem 9601 ou similaire
				 				 {TRUE, TRUE,  TRUE,  FALSE, 1960},		//modem A3LA ou similaire
				 				 {TRUE, TRUE,  TRUE,  TRUE,  1960}};	//modem A3LA avec GPS ou similaire

/***********************************************************************//**
 * \def TIS_MODEM_9601
 * \brief Index des caract�ristiques du modem 9601 dans TIS_modems.
 ***********************************************************************/
#define TIS_MODEM_9601			0

/***********************************************************************//**
 * \def TIS_MODEM_A3LA_X
 * \brief Index des caract�ristiques du modem A3LA X de NAL dans TIS_modems.
 ***********************************************************************/
#define TIS_MODEM_A3LA_X		1

/***********************************************************************//**
 * \def TIS_MODEM_A3LA_XG
 * \brief Index des caract�ristiques du modem A3LA XG de NAL dans TIS_modems.
 ***********************************************************************/
#define TIS_MODEM_A3LA_XG		2

/***********************************************************************//**
 * \def TIS_MODEM_9602
 * \brief Index des caract�ristiques du modem 9602 dans TIS_modems.
 *
 * Ce modem poss�de les m�mes caract�ristiques que le 9601.
 *
 * \sa TIS_MODEM_9601
 ***********************************************************************/
#define TIS_MODEM_9602			TIS_MODEM_9601

/***********************************************************************//**
 * \def TIS_MODEM_A3LA_XM
 * \brief Index des caract�ristiques du modem A3LA XM de NAL dans TIS_modems.
 *
 * Ce modem poss�de les m�mes caract�ristiques que le A3LA X.
 *
 * \sa TIS_MODEM_A3LA_X
 ***********************************************************************/
#define TIS_MODEM_A3LA_XM		TIS_MODEM_A3LA_X

/***********************************************************************//**
 * \def TIS_MODEM_A3LA_XP
 * \brief Index des caract�ristiques du modem A3LA XP de NAL dans TIS_modems.
 *
 * Ce modem poss�de les m�mes caract�ristiques que le A3LA X.
 *
 * \sa TIS_MODEM_A3LA_X
 ***********************************************************************/
#define TIS_MODEM_A3LA_XP		TIS_MODEM_A3LA_X

/***********************************************************************//**
 * \def TIS_MODEM_A3LA_XL
 * \brief Index des caract�ristiques du modem A3LA XL de NAL dans TIS_modems.
 *
 * Ce modem poss�de les m�mes caract�ristiques que le A3LA X.
 *
 * \sa TIS_MODEM_A3LA_X
 ***********************************************************************/
 #define TIS_MODEM_A3LA_XL		TIS_MODEM_A3LA_X

/***********************************************************************//**
 * \def TIS_MODEM_A3LA_XGP
 * \brief Index des caract�ristiques du modem A3LA XGP de NAL dans TIS_modems.
 *
 * Ce modem poss�de les m�mes caract�ristiques que le A3LA XG.
 *
 * \sa TIS_MODEM_A3LA_XG
 ***********************************************************************/
 #define TIS_MODEM_A3LA_XGP		TIS_MODEM_A3LA_XG

#endif //TIS_GPS
