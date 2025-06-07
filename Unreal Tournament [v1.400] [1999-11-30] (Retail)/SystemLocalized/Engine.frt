[Public]
Object=(Name=Engine.Console,Class=Class,MetaClass=Engine.Console)
Preferences=(Caption="Avanc�es",Parent="Options avanc�es")
Preferences=(Caption="Param�tres du moteur du jeu",Parent="Avanc�es",Class=Engine.GameEngine,Category=Settings,Immediate=True)
Preferences=(Caption="Alias cl�s",Parent="Avanc�es",Class=Engine.Input,Immediate=True,Category=Aliases)
Preferences=(Caption="Liaisons cl�s",Parent="Avanc�es",Class=Engine.Input,Immediate=True,Category=RawKeys)
Preferences=(Caption="Pilotes",Parent="Options avanc�es",Class=Engine.Engine,Immediate=False,Category=Drivers)
Preferences=(Caption="Informations serveur public",Parent="R�seau",Class=Engine.GameReplicationInfo,Immediate=True)
Preferences=(Caption="R�glages du jeu",Parent="Options avanc�es",Class=Engine.GameInfo,Immediate=True)

[Errors]
NetOpen=Erreur d'ouverture de fichier
NetWrite=Erreur d'�criture dans un fichier 
NetRefused=Le serveur a refus� d'envoyer '%s'
NetClose=Erreur de fermeture de fichier
NetSize=Diff�rence de taille de fichier
NetMove=Erreur de d�placement de fichier 
NetInvalid=R�ception d'une demande de fichier non valide
NoDownload=Le package '%s' n'est pas t�l�chargeable
DownloadFailed=Echec du t�l�chargement du package '%s' : %s
RequestDenied=Le serveur a demand� un fichier en �tat d'attente : refus�
ConnectionFailed=Echec de connexion
ChAllocate=Impossible d'allouer une cha�ne
NetAlready=D�j� en r�seau
NetListen=Echec d'�coute : Aucun contexte de liaison disponible 
LoadEntry=Impossible de charger l'entr�e : %s 
InvalidUrl=URL non valide : %s 
InvalidLink=Lien non valide : %s
FailedBrowse=Impossible d'entrer %s: %s
Listen=Echec d'�coute : %s
AbortToEntry=Echec; retour � Entr�e 
ServerOpen=Les serveurs ne r�ussissent pas � ouvrir les URLs r�seau
ServerListen=Le serveur d�di� n'arrive pas � �couter : %s
Pending=Echec de connexion � '%s' ; %s 
LoadPlayerClass=Echec de chargement de la classe du joueur
ServerOutdated=La version du serveur est ancienne

[Progress]
RunningNet=%s: %s (%i joueur)
NetReceiving=R�ception de '%s': %i/%i
NetReceiveOk=R�ception r�ussie de '%s'
NetSend=Envoi '%s'
NetSending=Envoi '%s': %i/%i
Connecting=Connexion...
Listening=Ecoute des clients...
Loading=Chargement
Saving=Enregistrement
Paused=Interrompu par %s
ReceiveFile=R�ception de '%s' (F10 pour annuler)
ReceiveSize=Taille %iK, Total %3.1f%%
ConnectingText=Connexion (F10 pour annuler):
ConnectingURL=unreal://%s/%s

[Console]
ClassCaption=Console Unreal standard
LoadingMessage=CHARGEMENT
SavingMessage=ENREGISTREMENT
ConnectingMessage=CONNEXION
PausedMessage=INTERROMPU
PrecachingMessage=MISE EN MEMOIRE CACHE PROVISOIRE
ChatChannel=(MESSAGE) 
TeamChannel=(EQUIPE) 

[General]
Upgrade=Pour acc�der � ce serveur, vous devez vous procurer la derni�re mise � jour gratuite de Unreal sur le site Web de Epic :
UpgradeURL=http://www.unreal.com/upgrade
UpgradeQuestion=Souhaitez-vous d�marrer maintenant votre explorateur Web et afficher la page des mises � jour ?
Version=Version %i

[Menu]
HelpMessage=
MenuList=
LeftString=Gauche
RightString=Droite
CenterString=Centre
EnabledString=Activ�
DisabledString=D�sactiv� 
HelpMessage[1]="Ce menu n'a pas encore �t� introduit."
YesString=oui
NoString=non

[Inventory]
PickupMessage=R�cup�r� un �l�ment
M_Activated=" activ�"
M_Selected=" s�lectionn�"
M_Deactivated=" d�sactiv�"
ItemArticle=a

[WarpZoneInfo]
OtherSideURL=

[GameInfo]
SwitchLevelMessage=Changement de niveaux
DefaultPlayerName=Joueur
LeftMessage=" a quitt� la partie."
FailedSpawnMessage=N'a pas r�ussi � g�n�rer un acteur joueur
FailedPlaceMessage=N'a pas trouv� de point de d�marrage (le niveau n�cessite peut-�tre un acteur 'PlayerStart')
NameChangedMessage=Nom chang� en... 
EnteredMessage=" a rejoint la partie."
GameName=Partie
MaxedOutMessage=Le serveur a d�j� atteint sa capacit� maximale.
WrongPassword=Le mot de passe que vous avez entr� n'est pas valide.
NeedPassword="Vous devez entrer un mot de passe pour rejoindre cette partie."
FailedTeamMessage=Impossible de trouver l'�quipe du joueur 

[LevelInfo]
Title=Sans titre

[Weapon]
MessageNoAmmo=" est sans munitions."
PickupMessage=Vous avez une arme 
DeathMessage=%o a �t� tu� par %w de %k.
ItemName=Arme
DeathMessage[0]=%o a �t� tu� par %w de %k.
DeathMessage[1]=%o a �t� tu� par %w de %k.
DeathMessage[2]=%o a �t� tu� par %w de %k.
DeathMessage[3]=%o a �t� tu� par %w de %k.

[Counter]
CountMessage=Plus que %i...
CompleteMessage=Termin� !

[Ammo]
PickupMessage=Vous avez r�cup�r� des munitions.

[Pickup]
ExpireMessage=

[SpecialEvent]
DamageString=

[DamageType]
Name=tu�
AltName=tu� 

[PlayerPawn]
QuickSaveString=Enregistrement rapide
NoPauseMessage=Impossible de mettre la partie en pause
ViewingFrom=Affichage en cours
OwnCamera=cam�ra personnelle
FailedView=Echec de changement d'affichage.
CantChangeNameMsg=Vous ne pouvez pas changer de nom lors d'une partie en r�seau.

[Pawn]
NameArticle=" a "

[Spectator]
MenuName=Spectateur