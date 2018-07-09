/***********************************************************************//**
* \mainpage Documentation de Transmission Iridium Simplifi�e - Partie Instrument
* Cette documentation pr�sente l'usage de la librairie TIS.
* Ci-dessous vous trouverez un exemple d'utilisation de la librairie pour r�aliser des �changes en SBD. Certaines fonctionnalit�s d�pendent
* de la plateforme utilis�e et non de la librairie, des fonctions factices sont donc utilis�es dans cet exemple.
* \code
* #include "tis.h"
*
* //Cete fonction prend en param�tre un tableau contenant des pointeurs vers le nom des fichiers � envoyer ainsi que le nombre de fichier � renvoyer.
* bool Iridium_send_and_receive_data(char ** files, int files_count) {
*	TIS_properties tis; //D�claration de la structure de la librairie
*	UART_struct serial; //Structure contenant la configuration de la liaison s�rie utilis�e par le modem (d�pend de votre plateforme et non de la librairie)
*   int i = 0;
*
*	//Initialise la liaison s�rie (d�pend de votre plateforme et non de la librairie)
*	UART_init(&serial);
*
*	//Initialise la structure de la librairie
*	if (TIS_init(&tis,
*				 "CMD",				//"CMD" est le chemin du dossier qui contiendra les fichier re�us.
*	 	 		 TIS_MODEM_9602,	//Mod�le du modem
*	 	 		 310036010122330,	//Num�ro IMEI du modem
*	 	 		 TIS_SERVICE_SBD,	//Service SBD
*	 	 		 NULL,				//Inutile avec un modem sans carte SIM
*	 	 		 0,					//Inutile en modem SBD
*	 	 		 NULL,				//Inutile en modem SBD
*	 	 		 files_count,		//Nombre de fichier envoy�s
*	 	 		 &serial,			//Un pointeur vers la structure d�crivant la liaison s�rie
*	 	 		 UART_send_data,	//Fonction utilisant les appelles syst�me de la plateforme pour envoyer des donn�es sur la liaison s�rie
*			 	 UART_receive_data, //Fonction utilisant les appelles syst�me de la plateforme pour recevoir des donn�es sur la liaison s�rie
*			 	 UART_wait_data,	//Fonction utilisant les appelles syst�me de la plateforme pour attendre des donn�es sur la liaison s�rie
*			 	 UART_flush_TX,		//Fonction utilisant les appelles syst�me de la plateforme pour vider le tampon de sortie de la liaison s�rie
*			 	 UART_flush_RX		//Fonction utilisant les appelles syst�me de la plateforme pour vider le tampon d'entr�e de la liaison s�rie
*		) != TIS_ERROR_SUCCESS) {
*		return FALSE;
*	}
*
*	//Ajoute les fichiers � envoyer
* 	for (i = 0; i < files_count; i++) {
*		TIS_add_file_to_send(&tis, files[i]);
*	}
*
*	//Boucle d'envoie et de r�ception, s'arr�te quand tous les fichier sont envoy�s ou re�us
*	do {
*		int32_t result;
*
*		//Met le modem sous tension
*		Iridium_ON(); //(d�pend de votre plateforme et non de la librairie)
*
*		//Lance la transmission
*		result = TIS_transmission(&tis, 10);
*
*		//Met le modem hors tension
*		Iridium_OFF(); //(d�pend de votre plateforme et non de la librairie)
*
*		//Affiche des informations de diagnostiques
*		if (result != TIS_ERROR_SUCCESS) {
*			printf("Iridium : erreur %ld s'est produite pendant la transmission", result);
*		}
*		printf("Iridium : informations de diagnostiques : \n");
*		printf("\tNombre de messages SBD envoy�s avec succ�s : %ld\n", tis.SBD_sent_without_error);
*		printf("\tNombre de messages SBD dont l'envoie a �chou� : %ld\n", tis.SBD_sent_with_error);
*		printf("\tNombre de messages SBD re�ues avec succ�s : %ld\n", tis.SBD_received_without_error);
*		printf("\tNombre de messages SBD dont la r�ception a �chou� : %ld\n", tis.SBD_received_with_error);
*		printf("\tNombre de messages SBD en attente dans la constellation : %ld\n", tis.SBD_waiting_messages);
*		for (i = 0; i < file_count; i++) {
*			printf("Iridium : fichier %s, envoy� � %ld%\n", TIS_get_file_path(&tis, i), TIS_get_file_progress(&tis, i));
*		}
*		
*		//Si la communication n'est pas termin�e, met en veille le syst�me en attendant un nouveau cr�naux de r�ception
*		if ((TIS_remaining_file_to_send(&tis) != 0) || (TIS_waiting_incoming_data() == TRUE)) {
*			sleep(300); //La dur�e d�pend de votre application, la fonction depend de votre plateforme et non de la librairie)
*		}
*
*	//Si il reste des fichier en cours d'envoie ou en attente dans le satellite, reboucle
*	} while ((TIS_remaining_file_to_send(&tis) != 0) || (TIS_waiting_incoming_data() == TRUE));
*	
*	//Lib�re la m�moire occup�e par la structure de la librairie
*	TIS_clean(&tis);
*	return TRUE;
* }
* \endcode
*
* \author Cl�ment Bonnet
* \date 2011-2012
* \copyright Laboratoire de Physique des Oc�ans. Ce code est couvert par la license CeCILL-B.
***********************************************************************/

/***********************************************************************//**
 * \file tis.h
 * \brief El�ments publiques de la librairie. Dans un programme, vous pouvez utiliser toutes ses fonctions et structures.
 *
 * Vous trouverez des exemples dans la page principale de cette documentation.
 *
 * \author Cl�ment Bonnet
 * \date 2011-2012
 * \copyright Laboratoire de Physique des Oc�ans. Ce code est couvert par la license CeCILL-B.
 ***********************************************************************/
 
#ifndef TIS_H
#define	TIS_H

/***********************************************************************//**
 * \def DISABLE_RUDICS
 * \brief Permet de d�sactiver le mode RUDICS.
 *
 * Pour r�duire la taille de code lorsque le mode RUDICS n'est pas utilis�, on peut ne pas fournir la librairie Zmodem.
 * Dans ce cas, il faut mettre DISABLE_RUDICS � la valeur TRUE afin de supprimer les appels � cette librairie.
 ***********************************************************************/
#define DISABLE_RUDICS TRUE

#include "TIS_platforms.h"
#include "TIS_modems.h"
#include "TIS_errors.h"

/***********************************************************************//**
 * \def TIS_SERVICE_SBD
 * \brief Service Iridium SBD.
 ***********************************************************************/
#define TIS_SERVICE_SBD 0

/***********************************************************************//**
 * \def TIS_SERVICE_RUDICS
 * \brief Service Iridium RUDICS.
 ***********************************************************************/
#define TIS_SERVICE_RUDICS 1

/***********************************************************************//**
 * \def TIS_SBD_INCOMMING_FILE_EMPTY
 * \brief Indique que le fichier n'existe pas (valeur par d�faut).
 ***********************************************************************/
#define TIS_SBD_INCOMMING_FILE_EMPTY		0

/***********************************************************************//**
 * \def TIS_SBD_INCOMMING_FILE_BEGINNING
 * \brief Indique que le fichier contient le d�but d'un fichier en cours de transfert.
 ***********************************************************************/
#define TIS_SBD_INCOMMING_FILE_BEGINNING	1

/***********************************************************************//**
 * \def TIS_SBD_INCOMMING_FILE_PART
 * \brief Indique que le fichier contient une partie n'�tant ni le d�but, ni la fin d'un fichier en cours de transfert.
 ***********************************************************************/
#define TIS_SBD_INCOMMING_FILE_PART			2

/***********************************************************************//**
 * \def TIS_SBD_INCOMMING_FILE_END
 * \brief Indique que le fichier contient la fin d'un fichier en cours de transfert.
 ***********************************************************************/
#define TIS_SBD_INCOMMING_FILE_END			3

/***********************************************************************//**
 * \def TIS_SBD_INCOMMING_FILE_COMPLET
 * \brief Indique que le fichier contient un fichier complet en cours de transfert.
 ***********************************************************************/
#define TIS_SBD_INCOMMING_FILE_COMPLET		4

/***********************************************************************//**
 * \def TIS_SBD_INCOMMING_FILE_QUEUE_SIZE
 * \brief Longueur de la liste des fichier contenant des parties des fichiers en cours de r�ceptions.
 ***********************************************************************/
#define TIS_SBD_INCOMMING_FILE_QUEUE_SIZE 10

/***********************************************************************//**
 * \struct TIS_properties
 * \brief Contient toutes les donn�es n�cessaires au fonctionnement de la librairie.
 ***********************************************************************/
typedef struct {
	//Propri�t�s du modem
	TIS_modem	modem;			/**< Caract�ristiques du modem utilis�. */
	uint64_t	IMEI_number;	/**< Num�ro IMEI du modem utilis�. */
	uint8_t		pin[5];			/**< Code PIN de la carte SIM utilis�e. */
	uint32_t	service;		/**< Service a utiliser lors des transferts. */
	
	//Gestion de la liaison s�rie
	void *		serial_struct;															/**< Pointeur vers la stucture utilis� par les fonctions de communications. */
	int32_t		(*send_data)(void * serial_struct, uint8_t * data, int32_t count);		/**< Pointeur vers la fonction permettant l'envoie des donn�es. */
	int32_t 	(*receive_data)(void * serial_struct, uint8_t * data, int32_t count);	/**< Pointeur vers la fonction permettant la r�ception des donn�es. */
	int32_t 	(*wait_data)(void * serial_struct, uint32_t timeout);					/**< Pointeur vers la fonction permettant l'attente de donn�es. */
	int32_t		(*flush_TX)(void * serial_struct);										/**< Pointeur vers la fonction permettant de vider de tampon d'�mission. */
	int32_t		(*flush_RX)(void * serial_struct);										/**< Pointeur vers la fonction permettant de vider de tampon de reception. */
		
	//Fichiers en cours d'envoie
	uint8_t * sent_files_path;			/**< Pointeur vers le tableau contenant les chemins des fichiers � envoyer. */
	int64_t * sent_files_progress;		/**< Pointeur vers le tableau contenant le nombre d'octets envoy�s pour chaque fichier. */
	uint16_t * sent_files_part_number;	/**< Pointeur vers le tableau contenant le num�ro de la partie en cours d'envoie lorsque le fichier est envoy� via SBD. */
	uint8_t sent_files_count;			/**< Nombre de fichier actuellement stock�s dans la m�moire. */
	uint8_t sent_files_allocated;		/**< Nombre de fichier pouvant �tre stock�s compte tenu de la m�moire allou�e. */
	
	//Dossier de r�ception
	uint8_t	receive_folder[MAXPATHLEN];	/**<  chemin du dossier o� doivent �tre stock�s les fichiers entrants. */
		
	//Propri�t�s sp�cifiques au mode SBD
	FILE * 		SBD_files[TIS_SBD_INCOMMING_FILE_QUEUE_SIZE];			/**< Tableau des descripteurs de fichiers entrant. */
	int8_t		SBD_file_name[TIS_SBD_INCOMMING_FILE_QUEUE_SIZE][13];	/**< Nom du fichier � reconstituer. */
	uint16_t	SBD_file_number[TIS_SBD_INCOMMING_FILE_QUEUE_SIZE]; 	/**< Contient le num�ro du dernier message �crit dans le fichier entrant associ�. */
	uint16_t	SBD_file_status[TIS_SBD_INCOMMING_FILE_QUEUE_SIZE];		/**< Statu du fichier associ�. */
	
	//Propri�t�s sp�cifiques au mode RUDICS
	uint64_t	RUDICS_dial_number;						/**< Num�ro appel� par le modem pour initier les communications RUDICS. */
	uint8_t		rudics_temporary_folder[MAXPATHLEN];	/**< Dossier dans lequel serons stock�s les fichiers en cours de r�ception lors des �changes RUDICS. */
		
	//Donn�es de diagnostique, elle sont remise � z�ro � chaque transmission
	uint32_t	SBD_sent_without_error;		/**< Nombre de SBD envoy�s sans erreurs durant la derni�re session SBD. */
	uint32_t	SBD_sent_with_error;		/**< Nombre de SBD envoy�s avec erreurs durant la derni�re session SBD. */
	uint32_t	SBD_received_without_error;	/**< Nombre de SBD re�ues sans erreurs durant la derni�re session SBD. */
	uint32_t	SBD_received_with_error;	/**< Nombre de SBD re�ues avec erreurs durant la derni�re session SBD. */
	uint32_t	SBD_waiting_messages;		/**< Nombre de SBD en attente dans la constellation. */
	uint32_t	RUDICS_disconnections;		/**< Nombre de d�connexions durant la derni�re session RUDICS. */
	uint32_t	RUDICS_incomplete_files;	/**< Nombre de fichier en cours de r�ception via RUDICS. Ces fichiers sont stock�s dans le dossier temporaire et ne sont pas complets. */

} TIS_properties;

/***********************************************************************//**
 * \brief Initialise la structure de la librairie.
 * \public
 *
 * Les fonctions fournies � la librairie au travers de cette fonction ne doivent pas �tre bloquante.
 * Elles doivent poss�der en interne un timeout afin de ne pas bloquer le syst�me.
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 * \param[in] receive_folder Pointeur vers la chaine de caract�res contenant le chemin du dossier o� doivent �tre stock�s les fichiers entrants.
 * \param[in] modem Nom du modem, voir TIS_modems.h pour plus de d�tails.
 * \param[in] IMEI_number Num�ro IMEI du modem utilis� par le syst�me.
 * \param[in] service Service utilis� lors des transferts (RUDICS ou SBD).
 * \param[in] pin Code PIN de votre carte SIM, si vous n'avez pas de carte SIM utiliser la valeur NULL.
 * \param[in] rudics_dial_number Num�ro appel� lors de la communication RUDICS.
 * \param[in] rudics_temporary_folder Dossier utiliser pour stocker les fichiers temporaires lors des transmissions RUDICS. Vous ne devez en aucun cas utiliser ce dossier pour un autre usage !
 * \param[in] sent_files_count Nombre de fichiers qui seront envoy�s au cours de la m�me session.
 * \param[in] serial_struct Pointeur vers la structure d�finissant la liaison s�rie.
 * \param[in] send_data Pointeur vers la fonction permettant l'envoie de donn�es.
 * \param[in] receive_data Pointeur vers la fonction permettant la r�ception de donn�es.
 * \param[in] wait_data Pointeur vers la fonction permettant l'attente de donn�es.
 * \param[in] flush_TX Pointeur vers la fonction permettant de vider le tampon d'�mission.
 * \param[in] flush_RX Pointeur vers la fonction permettant de vider le tampon de r�ception.
 *
 * \return Retourne un code d'erreur de la librairie.
 *
 * \sa TIS_modems.h TIS_errors.h
 ***********************************************************************/
int32_t TIS_init(TIS_properties *	properties,
				 uint8_t *			receive_folder,
				 int32_t			modem,
				 uint64_t			IMEI_number,
				 uint32_t			service,
				 uint8_t *			pin,
				 uint64_t			rudics_dial_number,
				 uint8_t * 			rudics_temporary_folder,
				 uint8_t			sent_files_count,
				 void *				serial_struct,
				 int32_t			(*send_data)(void * serial_struct, uint8_t * data, int32_t count),
				 int32_t 			(*receive_data)(void * serial_struct, uint8_t * data, int32_t count),
				 int32_t			(*wait_data)(void * serial_struct, uint32_t timeout),
				 int32_t			(*flush_TX)(void * serial_struct),
				 int32_t			(*flush_RX)(void * serial_struct));


/***********************************************************************//**
 * \brief Lib�re la m�moire occup�e par la structure de la librairie
 * \public
 *
 * Cette fonction lib�re la m�moire allou� dynamiquement par la libraire et supprime les fichiers temporaires.
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 ***********************************************************************/
void TIS_clean(TIS_properties * properties);

/***********************************************************************//**
 * \brief Cette fonction vous permet d'ajouter un fichier dans la liste des fichiers � envoyer
 * \public
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 * \param[in] path Pointeur vers la chaine de caract�re contenant le chemin du fichier, cette cha�ne est copi�e par la fonction.
 *
 * \return En cas de succ�s, retourne l'index du fichier dans la liste des fichier � envoyer. En cas d'erreur, un code d'erreur est retourn� sous la forme de l'oppos� d'un code d'erreur.
 * \sa TIS_error.h
 ***********************************************************************/
int32_t TIS_add_file_to_send(TIS_properties * properties, uint8_t * path);


/***********************************************************************//**
 * \brief Cette fonction vous permet de vider la liste des fichier � envoyer.
 * \public
 *
 * Il n'est pas possible de supprimer un seul fichier de la liste. Il est � noter que les fichiers d�j� envoy�s ne sont jamais renvoy�s.
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 ***********************************************************************/
void TIS_remove_files_to_send(TIS_properties * properties);


/***********************************************************************//**
 * \brief Cette fonction vous permet de r�cup�rer le chemin d'un fichier dans la liste des fichiers � envoyer gr�ce � son index.
 * \public
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 * \param[in] index Index du fichier dans la liste des fichiers � envoyer.
 *
 * \return Un pointeur vers le chemin du fichier. Un pointeur vers une chaine vide est renvoy� si le fichier n'existe pas.
 ***********************************************************************/
uint8_t * TIS_get_file_path(TIS_properties * properties, uint8_t index);

/***********************************************************************//**
 * \brief Cette fonction vous permet de r�cup�rer l'avancement d'un fichier dans la liste des fichiers � envoyer gr�ce � son index.
 * \public
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 * \param[in] index Index du fichier dans la liste des fichiers � envoyer.
 *
 * \return La valeur en pourcentage d'avancement du fichier. Un fichier qui n'existe pas � toujours un avancement de 0.
 ***********************************************************************/
int32_t TIS_get_file_progress(TIS_properties * properties, uint8_t index);

/***********************************************************************//**
 * \brief Cette fonction vous permet de connaitre le nombre de fichier de la liste des fichiers � envoyer qui n'ont pas encore �t� envoy�s.
 * \public
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 *
 * \return Le nombre de fichier restant � envoyer.
 ***********************************************************************/
uint8_t TIS_remaining_file_to_send(TIS_properties * properties);

/***********************************************************************//**
 * \brief Cette fonction vous permet de savoir si des fichiers sont en attente dans la constellation (SBD) ou sur le serveur (RUDICS).
 * \public
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 *
 * \return TRUE si des donn�es doivent encore �tre re�ues
 * \return FALSE si toutes les donn�es ont �t� re�ues
 ***********************************************************************/
bool TIS_waiting_incoming_data(TIS_properties * properties);

/***********************************************************************//**
 * \brief Effectue une session d'envoie et de r�ception de fichier via Iridium.
 *
 * Les fichiers reconstitu�s sont stock�s dans le dossier "receive_folder".
 *
 * Il est � noter que la structure contenant les propri�t�s de la librairie ne doit pas �tre perdue entre deux appelles de "TIS_transmission" si tous les fichiers non sont pas re�ues ou envoy�s.
 * En effet, ceci aurais pour cons�quence la perte des fichiers en cours de r�ception et le renvoie int�grale des fichiers en cours d'envoie.
 *
 * Si le nombre d'erreurs cons�cutives "maximum_consecutive_errors" est atteint, la fonction se termine et renvoie l'erreur TIS_ERROR_TOO_MANY_CONSECUTIVE_ERRORS.
 * Lors d'une communication RUDICS, les d�connexions ne sont pas consid�r�es comme des erreurs.
 *
 * \param[in,out] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 * \param[in] maximum_consecutive_errors Nombre d'erreurs cons�cutives maximum avant de quitter.
 *
 * \return Un code d'erreur.
 * \sa TIS_errors.h
 ***********************************************************************/
int32_t TIS_transmission(TIS_properties * properties, int32_t maximum_consecutive_errors);

/***********************************************************************//**
 * \brief Donne une id�e de la puissance du signal satellite re�u.
 * \public
 *
 * Cette fonction repose sur des informations fournies par le modem qui ne sont pas fiables.
 * Cette fonction est fournie principalement pour �tre utilis� lors des phase de mise au point et de tests.
 * Cette fonction n�cessite l'initialisation de la librairie et que le modem soit sous tension.
 *
 * \param[in] properties Pointeur vers la structure qui contient les propri�t�s de la librairie.
 *
 * \return Une valeur correspondant � la puissance du signal ou une valeur n�gative en cas d'�chec.
 *
 * \sa TIS_SIGNAL_STRENGTH_0 TIS_SIGNAL_STRENGTH_1 TIS_SIGNAL_STRENGTH_2 TIS_SIGNAL_STRENGTH_3 TIS_SIGNAL_STRENGTH_4 TIS_SIGNAL_STRENGTH_5
 ***********************************************************************/
int32_t TIS_signal_strenght(TIS_properties * properties);

/***********************************************************************//**
 * \brief Retourne la position GPS du modem.
 * \public
 * \todo Fonction non impl�ment�e
 ***********************************************************************/
int32_t TIS_get_gps_position();

/***********************************************************************//**
 * \brief Demande au modem de mettre hors tension sa partie Iridium
 * \public
 * \todo Fonction non impl�ment�e
 ***********************************************************************/
int32_t TIS_power_off_iridium();

/***********************************************************************//**
 * \brief Demande au modem de mettre sous tension sa partie Iridium
 * \public
 * \todo Fonction non impl�ment�e
 ***********************************************************************/
int32_t TIS_power_on_iridium();

/***********************************************************************//**
 * \brief Demande au modem de mettre hors tension sa partie GPS
 * \todo Fonction non impl�ment�e
 ***********************************************************************/
int32_t TIS_power_off_gps();

/***********************************************************************//**
 * \brief Demande au modem de mettre sous tension sa partie GPS
 * \public
 * \todo Fonction non impl�ment�e
 ***********************************************************************/
int32_t TIS_power_on_gps();

#endif //TIS_H
