[Public]
;Object=(Name=Engine.Console,Class=Class,MetaClass=Engine.Console)
Object=(Name=Engine.ServerCommandlet,Class=Class,MetaClass=Core.Commandlet)
Object=(Name=Engine.MasterMD5Commandlet,Class=Class,MetaClass=Core.Commandlet)
Preferences=(Caption="Advanced",Parent="Advanced Options")
Preferences=(Caption="Game Engine Settings",Parent="Advanced",Class=Engine.GameEngine,Category=Settings,Immediate=True)
Preferences=(Caption="Key Aliases",Parent="Advanced",Class=Engine.Input,Immediate=True,Category=Aliases)
Preferences=(Caption="Raw Key Bindings",Parent="Advanced",Class=Engine.Input,Immediate=True,Category=RawKeys)
Preferences=(Caption="Drivers",Parent="Advanced Options",Class=Engine.Engine,Immediate=False,Category=Drivers)
Preferences=(Caption="Public Server Information",Parent="Networking",Class=Engine.GameReplicationInfo,Immediate=True)
Preferences=(Caption="Game Settings",Parent="Advanced Options",Class=Engine.GameInfo,Immediate=True)

[Errors]
NetOpen="Erreur d'ouverture de fichier"
NetWrite="Erreur d'�criture de fichier"
NetRefused="Refus d'envoi de '%s' par le serveur"
NetClose="Erreur � la fermeture du fichier"
NetSize="Disparit� de taille de fichier"
NetMove="Erreur de d�placement du fichier"
NetInvalid="Demande de fichier incorrecte re�ue"
NoDownload="L'ensemble '%s' n'est pas t�l�chargeable"
DownloadFailed="Echec du t�l�chargement de l'ensemble '%s' : %s"
RequestDenied="Demande de fichier du niveau en cours par serveur : rejet"
ConnectionFailed="Echec de la connexion"
ChAllocate="Allocation de canal impossible"
NetAlready="D�j� en r�seau"
NetListen="Echec de l'�coute : aucun contexte de liaison disponible"
LoadEntry="Impossible de charger l'entr�e : %s"
InvalidUrl="URL incorrect : %s"
InvalidLink="Lien incorrect : %s"
FailedBrowse="Impossible d'entrer %s : %s"
Listen="Echec de l'�coute : %s"
AbortToEntry="Echec ; retour � l'entr�e"
ServerOpen="Ouverture des URL r�seau par serveurs impossible"
ServerListen="Ecoute impossible par serveur d�di� : %s"
Pending="Echec de la connexion en cours � '%s' ; %s"
LoadPlayerClass="Echec du chargement de classe de joueur"
ServerOutdated="Version de serveur p�rim�e"
ClientOutdated="Vous devez appliquer les patches les plus r�cents"
InvalidCDKey="Votre cl� de CD semble incorrecte. Vous allez devoir r�installer le jeu et saisir votre cl� de CD pour r�soudre ce probl�me."
ConnectLost="Connexion perdue"

[UpgradeDrivers]
OutdatedDrivers="Vous utilisez des pilotes de carte graphique obol�tes, ils ne pourraient pas supporter le jeu."
OursOrWeb="Choisir OUI pour installer les pilotes depuis notre CD et NON pour visiter le site du distributeur."
InsertCD="Veuillez ins�rer le CD 3 d'Unreal Tournament"
NvidiaURL="http://www.nvidia.com/content/drivers/drivers.asp"
AtiURL="http://www.ati.com/support/driver.html"
CDButton="Mettre � jour depuis CD"
WebButton="Mettre � jour depuis site Web"
cancelButton="Ne pas mettre � jour"

[KeyNames]
Up="HAUT"
Right="DROITE"
Left="GAUCHE"
Down="BAS"
LeftMouse="SOURIS G"
RightMouse="SOURIS D"
MiddleMouse="SOURIS C"
MouseX="SOURIS X"
MouseY="SOURIS Y"
MouseZ="SOURIS Z"
MouseW="SOURIS W"
JoyX="JOYX"
JoyY="JOYY"
JoyZ="JOYZ"
JoyU="JOYU"
JoyV="JOYV"
JoySlider1="JOYSLIDER1"
JoySlider2="JOYSLIDER2"
MouseWheelUp="MOLETTE HAUT"
MouseWheelDown="MOLETTE BAS"
Joy1="JOY1"
Joy2="JOY2"
Joy3="JOY3"
Joy4="JOY4"
Joy5="JOY5"
Joy6="JOY6"
Joy7="JOY7"
Joy8="JOY8"
Joy9="JOY9"
Joy10="JOY10"
Joy11="JOY11"
Joy12="JOY12"
Joy13="JOY13"
Joy14="JOY14"
Joy15="JOY15"
Joy16="JOY16"
Space="ESPACE"
PageUp="PAGE HAUT"
PageDown="PAGE BAS"
End="FIN"
Home="DEBUT"
Select="SELECT."
Print="IMPR."
Execute="EXECUTER"
PrintScrn="IMPR ECRAN"
Insert="INSER"
Delete="SUPPR"
Help="AIDE"
NumPad0="0 PAVNUM"
NumPad1="1 PAVNUM"
NumPad2="2 PAVNUM"
NumPad3="3 PAVNUM"
NumPad4="4 PAVNUM"
NumPad5="5 PAVNUM"
NumPad6="6 PAVNUM"
NumPad7="7 PAVNUM"
NumPad8="8 PAVNUM"
NumPad9="9 PAVNUM"
GreyStar="* PAVNUM"
GreyPlus="+ PAVNUM"
Separator="SEPARATEUR"
GreyMinus="- PAVNUM"
NumPadPeriod=". PAVNUM"
GreySlash="/ PAVNUM"
Pause="PAUSE"
CapsLock="VERR MAJ"
Tab="TAB"
Enter="ENTREE"
Shift="MAJ"
NumLock="VERR NUM"
Escape="ECHAP"

[Progress]
CancelledConnect="Tentative de connexion annul�e"
RunningNet="%s : %s (%i joueurs)"
NetReceiving="R�ception de '%s' : %i/%i"
NetReceiveOk="'%s' bien re�u"
NetSend="Envoi de '%s'"
NetSending="Envoi de '%s' : %i/%i"
Connecting="Connexion..."
Listening="Ecoute des clients..."
Loading="Chargement"
Saving="Sauvegarde"
Paused="Mis en pause par %s"
ReceiveFile="R�ception de '%s' (F10 pour annuler)"
ReceiveOptionalFile="R�ception du fichier optionnel '%s' (F10 pour passer)"
ReceiveSize="Taille %i Ko, effectu�e � %3.1f%%"
ConnectingText=""
ConnectingURL="unreal://%s/%s"
CorruptConnect="Connexion corrompue d�tect�e"

[ServerCommandlet]
HelpCmd=server
HelpOneLiner="Serveur de partie en r�seau"
HelpUsage=server map.unr[?game=gametype] [-option...] [parm=value]...
HelpWebLink=http://unreal.epicgames.com/servertips.htm
HelpParm[0]=Log
HelpDesc[0]="Indiquer le fichier de log � g�n�rer"
HelpParm[1]=AllAdmin
HelpDesc[1]="Donner les privil�ges administrateur � tous les joueurs"

[MasterMD5Commandlet]
HelpCmd=mastermd5 [*.ext {*.ext ...} ]
HelpOneLiner="G�n�rer le tableau principal MD5"
HelpUsage=mastermd5
HelpWebLink=http://unreal.epicgames.com/servertips.htm

[General]
Upgrade="Pour entrer sur ce serveur, il vous faut la derni�re mise � jour gratuite d'Unreal disponible sur le site web d'Epic :"
UpgradeURL="http://www.unreal.com/upgrade"
UpgradeQuestion="Voulez-vous vous lancer votre navigateur et rejoindre la page de mise � jour ?"
Version="Version %i"

[AccessControl]
IPBanned="Votre adresse IP a �t� bannie de ce serveur."
WrongPassword="Le mot de passe saisi est incorrect."
NeedPassword="Il vous faut un mot de passe pour rejoindre cette partie."
SessionBanned="Votre adresse IP a �t� bannie de la s�ance de jeu en cours."
KickedMsg="Vous avez �t� vir� du jeu de force."

[Ammo]
PickupMessage="Vous avez ramass� des munitions."

[Crushed]
DeathString="%o a �t� �cras�(e) par %k."
FemaleSuicide="%o a �t� �cras�(e)."
MaleSuicide="%o a �t� �cras�(e)."

[DamTypeTelefragged]
DeathString="%o a �t� lit�ralement dispers�(e) par %k."
FemaleSuicide="%o a tent� d'aller l� o� aucune femme n'�tait all�e."
MaleSuicide="%o a tent� d'aller l� o� aucun homme n'�tait all�."

[DamageType]
DeathString="%o a �t� tu�(e) par %k."
FemaleSuicide="%o s'est tu�(e)."
MaleSuicide="%o s'est tu�(e)."

[FailedConnect]
FailMessage[0]="CONNECTION INTERROMPUE.  MOT DE PASSE REQUIS."
FailMessage[1]="CONNECTION INTERROMPUE.  MOT DE PASSE INVALIDE."
FailMessage[2]="CONNECTION INTERROMPUE.  PARTIE EN COURS."
FailMessage[3]="CONNECTION INTERROMPUE."

[FellLava]
DeathString="%o est tomb�(e) et parti(e) en cendres"
FemaleSuicide="%o est tomb�e et partie en cendres"
MaleSuicide="%o est tomb� et parti en cendres"

[GameInfo]
bAlternateMode=False
DefaultPlayerName="Joueur"
GameName="Game"
VoteTypeStrings[0]="Relancer la partie"
VoteTypeStrings[1]="Changer de map"
VoteTypeStrings[2]="Lancer l'�preuve"
VoteTypeStrings[3]="Ejecter le joueur"
VoteTypeStrings[4]="Faire taire le joueur"
GIPropsDisplayText[0]="Comp�tence des robots"
GIPropsDisplayText[1]="Maintien des armes"
GIPropsDisplayText[2]="Employer rotation des maps"
GIPropsDisplayText[3]="Pas de robots"
GIPropsDisplayText[4]="Degr� de gore"
GIPropsDisplayText[5]="Vitesse de jeu"
GIPropsDisplayText[6]="Max. spectateurs"
GIPropsDisplayText[7]="Max. joueurs"
GIPropsDisplayText[8]="Score objectif"
GIPropsDisplayText[9]="Max. vies"
GIPropsDisplayText[10]="Limite de temps"
GIPropsDisplayText[11]="Rounds par map"
GIPropsDisplayText[12]="Journal des stats"
GIPropsExtras[0]="0;Novice;1;Moyen;2;Exp�riment�;3;Habile;4;Initi�;5;Ma�tre de guerre;6;Inhumain;7;Divin"
GIPropsExtras[1]="0;Total gore;1;Mini-gore"
MustHaveStats="Les stats doivent �tre activ�es pour joindre la partie."
NewPlayerMessage="Un nouveau joueur s'est connect�."

[GameMessage]
SwitchLevelMessage="Changement de niveau"
LeftMessage=" a quitt� la partie."
FailedTeamMessage="Impossible de trouver l'�quipe du joueur"
FailedPlaceMessage="Impossible de trouver le point de d�part"
FailedSpawnMessage="Impossible de r�g�n�rer le joueur"
EnteredMessage=" est entr�(e) dans la partie."
MaxedOutMessage="Serveur d�j� � pleine capacit�."
OvertimeMessage="Ex aequo en fin d'�preuve. Prolongation pour mort subite !"
GlobalNameChange="a pour nouveau nom"
NewTeamMessage="est maintenant sur"
NoNameChange="Ce nom est d�j� employ�."
VoteStarted=" a lanc� un vote."
VotePassed="Vote effectu�."
MustHaveStats="Les stats doivent �tre activ�es pour rejoindre cette partie."
NewPlayerMessage="Un nouveau joueur est entr� dans la partie."

[GameReplicationInfo]
GRIPropsDisplayText[0]="Nom de serveur"
GRIPropsDisplayText[1]="Nom serveur abr�g�"
GRIPropsDisplayText[2]="Nom admin"
GRIPropsDisplayText[3]="Email admin"
GRIPropsDisplayText[4]="Message"
GRIPropsDisplayText[5]="du"
GRIPropsDisplayText[6]=""
GRIPropsDisplayText[7]="jour"

[GameProfile]
LeagueNames[0]="Ligue Deathmatch"
LeagueNames[1]="Ligue Capture de flag"
LeagueNames[2]="Ligue Double domination"
LeagueNames[3]="Ligue Pose de bombe"
DivisionNames[0]="Division imp�riale"
DivisionNames[1]="Division galactique"
PositionName[0]="POSITION AUTO."
PositionName[1]="DEFENSE"
PositionName[2]="ATTAQUE"
PositionName[3]="LIBRE"
PositionName[4]="SOUTIEN"

[Gibbed]
DeathString="%o a explos� en petits morceaux"
FemaleSuicide="%o a explos� en petits morceaux"
MaleSuicide="%o a explos� en petits morceaux"

[LevelInfo]
Title="Sans titre"

[MatSubAction]
Desc="s.o."

[Mutator]
FriendlyName="Variante"
Description="Description"

[Pickup]
PickupMessage="a ramass� un objet."

[PlayerController]
QuickSaveString="Sauvegarde rapide"
NoPauseMessage="Partie sans pause"
ViewingFrom="Affichage depuis"
OwnCamera="Affichage depuis la propre cam�ra"

[PlayerProfile]
StyleString[0]="Psycho"
StyleString[1]="Agressif"
StyleString[2]="R�fl�chi"
StyleString[3]="Insaisissable"
StyleString[4]="Furtif"

[SubActionCameraEffect]
Desc="Effet de cam�ra"

[SubActionCameraShake]
Desc="Secousse"

[SubActionFOV]
Desc="Champ"

[SubActionFade]
Desc="Fondu"

[SubActionGameSpeed]
Desc="Vitesse de jeu"

[SubActionOrientation]
Desc="Orientation"

[SubActionSceneSpeed]
Desc="Vitesse de sc�ne"

[SubActionTrigger]
Desc="D�tente"

[Suicided]
DeathString="%o a eu une crise cardiaque."
FemaleSuicide="%o a claqu� une durit."
MaleSuicide="%o a claqu� une durit."

[TeamInfo]
ColorNames[0]="Rouge"
ColorNames[1]="Bleue"
ColorNames[2]="Verte"
ColorNames[3]="Or"
TeamName="Equipe"

[Volume]
LocationName="non pr�cis�"

[Weapon]
MessageNoAmmo=" n'a pas de munitions"

[WeaponPickup]
PickupMessage="Vous avez trouv� une arme"

[XBoxPlayerInput]
LookPresets[0].PresetName="Lin�aire"
LookPresets[1].PresetName="Exponentiel"
LookPresets[2].PresetName="Hybride"
LookPresets[3].PresetName="Personnalis�"

[fell]
DeathString="%o a laiss� un petit crat�re"
FemaleSuicide="%o a laiss� un petit crat�re"
MaleSuicide="%o a laiss� un petit crat�re"

[PlayerReplicationInfo]
StringSpectating="Spectateur"
StringUnknown="Inconnu"
