[Public]
Object=(Name=UTMenu.UTConsole,Class=Class,MetaClass=Engine.Console)

[OnlineServices]
MPlayer=Parties Internet sur mplayer.com,S�lectionnez cette option pour jouer sur Internet en utilisant le service en ligne mplayer.com.,CMDQUIT,mplayer
Heat=Parties Internet sur HEAT,S�lectionnez cette option pour  jouer sur Internet en utilisant le service en ligne HEAT.,CMDQUIT,heat
WON=Parties Internet sur WON,S�lectionnez cette option pour  jouer sur Internet en utilisant le service en ligne WON.,URL,http://www.won.net/channels/action/ut/

[PhysicalChildWindow]
PhysicalTaunts[0]="Moquerie de base"
PhysicalTaunts[1]=D�hanchement
PhysicalTaunts[2]=Dance De Victoire
PhysicalTaunts[3]=Signe de la main

[UTConsole]
ClassCaption=Console de Tournoi Unreal

[UTLadder]
MapText=Carte :
AuthorText=Auteur :
FragText=Nombre de frags :
TeamScoreText=Score limite de l'�quipe :

[MatchButton]
UnknownText=? Inconnu ?

[ManagerWindow]
DMText=Match � mort
DOMText=Domination
CTFText="Capturer le drapeau "
ASText=Assaut
ChallengeText=Challenge
ChallengeString=DERNIER CHALLENGE DU TOURNOI
ChalPosString=Grade Challenge :
TrophyText=Troph�es
RankString[0]=Grade Match � mort :
RankString[1]=Grade Domination :
RankString[2]=Grade CLD :
RankString[3]=Grade Assaut :
MatchesString=Matches effectu�s :

[SpeechWindow]
Options[0]=Accus� de r�ception
Options[1]=Tir ami
Options[2]=Ordres
Options[3]=Insultes
Options[4]=Autres/Divers
Options[5]=Geste
Options[6]=Commander ce bot
WindowTitle=Ordres

[SlotWindow]
EmptyText=INUTILISE
AvgRankStr=Grade moyen :
CompletedStr=Matches gagn�s :

[NewCharacterWindow]
MaleText=Homme
FemaleText=Femme
SkillText[0]=Novice
SkillText[1]=Moyen
SkillText[2]=Initi�
SkillText[3]=Exp�riment�
SkillText[4]=Habile
SkillText[5]=Ma�tre de guerre
SkillText[6]=Inhumain
SkillText[7]=Dieu
CCText="Cr�ation du perso"
TeamNameString=Nom de l'�quipe :
NameText=Nom
SexText=Sexe
TeamText=Equipe
FaceText=Visage
SkillsText=Niveau d'habilet�


[UTIndivBotSetupClient]
SkillText=R�glage Habilet�
SkillHelp=R�glez l'habilet� de ce bot vers le haut ou le bas, � partir de ce niveau de base.
VoicePackText=Voix
VoicePackHelp=Choisissez la voix pour votre joueur.
FavoriteWeaponText=Arme pr�f�r�e :
FavoriteWeaponHelp=S�lectionnez l'arme de pr�dilection de ce bot.
NoFavoriteWeapon=(Aucune)
AccuracyText=Pr�cision :
AccuracyHelp=Changez la pr�cision de l'arme de ce bot. La position centrale correspond au r�glage normal, l'extr�mit� gauche correspond � un faible degr� de pr�cision et l'extr�mit� droite � un haut degr� de pr�cision.
AlertnessText=Vivacit� :
AlertnessHelp=Changez le niveau de vivacit� de ce bot. La position centrale correspond au r�glage normal.
CampingText=Camping :
CampingHelp=Changez la propension de ce bot � camper.
CombatStyleText=Style de combat :
CombatStyleHelp=S�lectionnez le style de combat de ce bot.
CombatStyleValues[1]=0.500000
CombatStyleValues[2]=1.000000
CombatStyleValues[3]=-0.500000
CombatStyleValues[4]=-1.000000
CombatStyleNames[0]=Normal
CombatStyleNames[1]=Agressif
CombatStyleNames[2]=Fou furieux
CombatStyleNames[3]=Prudent
CombatStyleNames[4]=Fuyard
JumpyText=Sauteur :
JumpyHelp=Ce bot a l'habitude de sauter dans tous les sens comme le font certains joueurs.

[UTChallengeHUDConfig]
ShowHUDText=Afficher le HUD
ShowHUDHelp=Afficher l'�cran t�te haute (HUD).
ShowWeaponsText=Afficher les armes
ShowWeaponsHelp=Afficher les armes sur le HUD.
ShowStatusText=Afficher l'�tat du joueur
ShowStatusHelp=Afficher l'indicateur d'�tat du joueur (en haut � droite) sur le HUD.
ShowAmmoText=Afficher les munitions
ShowAmmoHelp="Afficher votre r�serve de munitions actuelle sur le HUD."
ShowTeamInfoText=Afficher les infos sur l'�quipe
ShowTeamInfoHelp=Afficher des informations sur l'�quipe sur le HUD.
ShowFacesText=Afficher zone de conversation
ShowFacesHelp=Afficher la zone de conversation dans le coin sup�rieur gauche de l'�cran, o� les messages et les victimes apparaissent.
ShowFragsText=Afficher les frags
ShowFragsHelp=Afficher votre compte de frags sur le HUD.
UseTeamColorText=Utiliser couleur �quipe/parties en �quipes
UseTeamColorHelp=Dans les parties en �quipes, cette option vous permet d'utiliser la couleur de votre �quipe comme couleur du HUD.
HUDColorText=Couleur du HUD
HUDColorHelp=Changer votre couleur de HUD pr�f�r�e. Dans les parties en �quipes, la couleur de votre �quipe sera utilis�e � la place.
HUDColorNames[0]=Rouge
HUDColorNames[1]=Violet
HUDColorNames[2]=Bleu clair
HUDColorNames[3]=Turquoise
HUDColorNames[4]=Vert
HUDColorNames[5]=Orange
HUDColorNames[6]=Jaune
HUDColorNames[7]=Rose
HUDColorNames[8]=Blanc
HUDColorNames[9]=Bleu fonc� 
HUDColorNames[10]=Personnalis�
HUDColorValues[0]=16,0,0
HUDColorValues[1]=16,0,16
HUDColorValues[2]=0,8,16
HUDColorValues[3]=0,16,16
HUDColorValues[4]=0,16,0
HUDColorValues[5]=16,8,0
HUDColorValues[6]=16,16,0
HUDColorValues[7]=16,0,8
HUDColorValues[8]=16,16,16
HUDColorValues[9]=0,0,16
HUDColorValues[10]=perso
HUDRText=HUD rouge
HUDRHelp=Utilisez les curseurs RVB pour personnaliser la couleur du HUD.
HUDGText=HUD vert
HUDGHelp=Utilisez les curseurs RVB pour personnaliser la couleur du HUD.
HUDBText=HUD bleu
HUDBHelp=Utilisez les curseurs RVB pour personnaliser la couleur du HUD.
OpacityText=HUD transparent
OpacityHelp=R�glez le niveau de transparence du HUD.
HUDScaleText=Taille du HUD
HUDScaleHelp=R�glez la taille des �l�ments affich�s sur le HUD.
WeaponScaleText=Taille des ic�nes des armes
WeaponScaleHelp=R�glez la taille des ic�nes des armes sur le HUD.
StatusScaleText=Taille de l'indicateur d'�tat
StatusScaleHelp=R�glez l'�chelle de l'indicateur d'�tat du joueur dans le coin sup�rieur droit du HUD.
CrosshairText=Style de r�ticule
CrosshairHelp=Choisissez le r�ticule de vis�e affich� au centre de l'�cran.
DefaultsText=R�initialiser
DefaultsHelp=Restaurer les valeurs par d�faut des param�tres du HUD.
CrosshairColorText=Couleur du r�ticule
CrosshairColorHelp=Changer votre couleur de r�ticule pr�f�r�e.
CrosshairRText=R�ticule rouge
CrosshairRHelp=Utilisez les curseurs RVB pour personnaliser la couleur du r�ticule.
CrosshairGText=R�ticule vert
CrosshairGHelp=Utilisez les curseurs RVB pour personnaliser la couleur du r�ticule.
CrosshairBText=R�ticule bleu
CrosshairBHelp=Utilisez les curseurs RVB pour personnaliser la couleur du r�ticule.

[SpeechBinderCW]
LabelText=Associer � la touche :
NotApplicable=-
AllString=Tous
LeaderString=Leader de l'�quipe
PlayerString=Equipier
NotBound=Dissocier
DefaultsText=R�initialiser
DefaultsHelp=Annuler l'association de toutes les touches de conversation.

[TeamBrowser]
NameString=Nom :
ClassString=Classification :
BrowserName=Composition de l'�quipe

[EnemyBrowser]
NameString=Nom :
ClassString=Classification :
BrowserName=Equipe ennemie

[ObjectiveBrowser]
BrowserName=Objectifs de la mission
OrdersTransmissionText=Transmission des ordres

[InGameObjectives]
ObjectiveString=Objectif
BrowserName=Objectifs de la mission
OrdersTransmissionText=Transmission des ordres

[UTPlayerSetupClient]
VoicePackText=Voix
VoicePackHelp=Choisissez la voix de votre joueur (insultes et ordres).
SpectatorText=Participer en spectateur
SpectatorHelp=Cochez cette case si vous souhaitez suivre la partie en tant que spectateur.

[UTTeamRCWindow]
TeamScoreText=Score max de l'�quipe 
TeamScoreHelp=La partie se termine lorsqu'une �quipe obtient ce score.
MaxTeamsText=Nombre max d'�quipes
MaxTeamsHelp=Nombre maximum de joueurs des diff�rentes �quipes autoris�s � se joindre � cette partie.
FFText=Tir ami :
FFHelp=D�terminez l'impact des tirs amis sur les membres d'une m�me �quipe.
BalancePlayersText="Equilibre des �quipes"
BalancePlayersHelp="Si cette option est coch�e, les joueurs qui rejoignent la partie seront int�gr�s � l'�quipe qui convient afin de maintenir l'�quilibre entre les deux �quipes."

[UTRulesCWindow]
JumpMatchText=MatchSaut
JumpMatchHelp=Si vous activez cette option, les joueurs seront capables de sauter tr�s haut.
TourneyText=Tournoi
TourneyHelp=Si vous activez cette option, chaque joueur doit indiquer qu'il est pr�t en cliquant sur le bouton de tir avant le d�but du match.
AllowFOVText=Possib. changer CDV
AllowFOVHelp=Permet aux joueurs de ce serveur de changer de champ de vision (CDV). Cette option pourrait �tre assimil�e � de la triche puisqu'elle permet aux joueurs d'effectuer des zooms avant.
ForceRespawnText="R�g�n�rateur de force"
ForceRespawnHelp="Cochez cette case pour que les joueurs se r�g�n�rent automatiquement sans que l'utilisateur ait besoin d'appuyer sur le bouton de tir. " 

[SpeechChildWindow]
WindowTitle=

[UTBotConfigClient]
BalanceTeamsText=Equilibrer les �quipes
BalanceTeamsHelp=Si vous activez cette option, les bots modifient automatiquement la composition des �quipes pour �quilibrer les effectifs des �quipes.
DumbDownText=IA d'�quipe am�lior�e
DumbDownHelp=Activer les fonctions am�lior�es d'intelligence artificielle d'�quipe pour les bots de cette partie.
MinPlayersText=Nombre min. joueurs
MinPlayersHelp=Les bots se g�n�rent automatiquement de sorte qu'il y ait toujours le m�me nombre de joueurs. Pour d�sactiver les bots, d�finissez la valeur 0.
SkillTaunts[1]=Ils savent tuer.
SkillTaunts[2]=Ne fais pas le malin.
SkillTaunts[3]=Tu te crois costaud ?
SkillTaunts[4]=J'esp�re que tu assures.
SkillTaunts[5]=J'esp�re que tu aimes la r�g�n�ration.
SkillTaunts[6]=Tu es d�j� mort.
SkillTaunts[7]=Je suis l'Alpha et l'Om�ga.
Skills[0]=Novice
Skills[1]=Moyen
Skills[2]=Initi�
Skills[3]=Exp�riment�
Skills[4]=Habile
Skills[5]=Ma�tre de guerre
Skills[6]=Inhumain
Skills[7]=Dieu

[PhysicalChildWindow]
WindowTitle=

[SpeechMiniDisplay]
NameString=Nom :
OrdersString=Ordres :
LocationString=Emplacement :
HumanString=Aucun <Humain>

[OrdersChildWindow]
WindowTitle=

[TargetChildWindow]
AllString=Tous
WindowTitle=

[UTSettingsCWindow]
TranslocText=T�l�porteur
TranslocHelp=Si vous activez cette option, chaque joueur disposera d'un appareil de t�l�portation personnel.
AirControlText=Contr�le a�rien
AirControlHelp=Sp�cifiez la capacit� du joueur � contr�ler ses d�placements en l'air � l'aide de ce curseur.

[UTMenuStartMatchCW]
ChangeLevelsText=Changem. carte auto.
ChangeLevelsHelp=Si vous activez cette option, le serveur change de niveau conform�ment � la liste des cartes pour ce type de partie.

[UTAudioClientWindow]
AutoTauntText=Insulte automatique
AutoTauntHelp=Si vous activez cette option, votre joueur prononce un message d'insulte choisi au hasard d�s que vous abattez quelqu'un.
VoiceTauntsText=Insultes vocales
VoiceTauntsHelp=Si vous activez cette option, vous entendez les insultes prof�r�es par les autres joueurs.
Use3DHardwareText="Utiliser A3D/EAX"
Use3DHardwareHelp="Si cette option est coch�e, UT utilise votre mat�riel 3D audio pour
obtenir
des sons plus riches."
UseSurroundSoundText="Utiliser son Surround "
UseSurroundSoundHelp=" Si cette option est coch�e, UT utilise votre r�cepteur num�rique pour
un meilleur son surround."
ConfirmHardwareTitle="Confirmer utiliser le mat�riel sonore 3D"
ConfirmHardwareText="La fonction mat�riel sonore 3D n�cessite une carte son 3D prenant en charge A3D ou EAX. Si cette option est activ�e, les performances du jeu risquent de se d�grader.\\n\\nVoulez-vous vraiment activer cette fonction ?"
ConfirmSurroundTitle="Confirmer utiliser son Surround "
ConfirmSurroundText="Pour utiliser la fonction de son surround, vous devez disposer d'un r�cepteur sonore surround compatible connect� � votre carte son.  Si vous activez cette option sans le r�cepteur appropri�, la performance sonore sera affect�e.\\n\\nVoulez-vous vraiment activer cette fonction ?"
NoMatureLanguageText="Pas de sarcasmes adultes"
NoMatureLanguageHelp="Si cette option est coch�e, vous n'entendrez pas les sarcasmes r�serv�s aux adultes."
AnnouncerVolumeText="Volume du pr�sentateur"
AnnouncerVolumeHelp="R�gle le volume du pr�sentateur."
MessageSettingsText="Messages vocaux"
MessageSettingsHelp="Ce param�tre commande quels messages vocaux envoy�s par d'autres joueurs seront entendus."
MessageSettings[0]="Tous"
MessageSettings[1]="Pas de sarcasmes automatiques"
MessageSettings[2]="Pas de sarcasmes"
MessageSettings[3]="Aucun"

[UTInputOptionsCW]
InstantRocketText=Tir roquette instant.
InstantRocketHelp="Si vous activez cette option, le lance-roquettes tire la roquette d�s que vous appuyez sur le bouton de tir, et ne charge pas plusieurs roquettes."
SpeechBinderText=Associer Voix/Touches
SpeechBinderHelp=Utilisez cette fen�tre sp�ciale pour associer des insultes et des ordres aux touches.

[ngWorldSecretClient]
DescText[0]=Voici votre mot de passe ngWorldStats. Il est utilis� en association
DescText[1]=avec votre nom de joueur pour conserver vos statistiques de jeu en ligne
DescText[2]=distinctes des autres joueurs. D�s que vous changez votre mot de passe ou nom de joueur, une
DescText[3]=nouvelle identit� est cr��e pour vous dans le syst�me ngWorldStats.
DescText[4]=Vous pouvez jouer sous plusieurs identit�s si vous le souhaitez. Utilisez le mot de passe
DescText[5]=al�atoire ou choisissez le v�tre et conservez-le.
SecretText=Mot de passe :
OKText=OK
SecretDesc="Choisissez un mot de passe pour rendre vos ngWorldStats
uniques."
QuitHelp="S�lectionnez [Oui] pour enregistrer votre nouveau mot de passe."
QuitTitle="Confirmer le changement du mot de passe"
QuitText="Attention ! Si vous jouez avec ce nouveau mot de passe, un nouveau compte ngWorldStats sera cr�� la prochaine fois que vous acc�derez � un serveur ngWorldStats.  Voulez-vous confirmer ?"
EmptyTitle="Confirmer l'annulation du mot de passe"
EmptyText="Remarque : vous ne d�sirez pas de mot de passe ngWorldStats.  Vos stats de parties en ligne ne seront pas enregistr�es. Voulez-vous confirmer ?

[KillGameQueryClient]
QueryText=Etes-vous s�r de vouloir supprimer cette partie enregistr�e ?
YesText=Oui
NoText=Non

[UTLadderChal]
TrophyMap=EOL_Challenge.unr
LadderName=Challenge final

[FreeSlotsClient]
QueryText=Lib�rez d'abord un logement !
DoneText=OK

[UTLadderDM]
TrophyMap=EOL_DeathMatch.unr
LadderName=Match � mort

[UTLadderDOM]
TrophyMap=EOL_Domination.unr
LadderName=Domination

[UTStartGameCW]
DedicatedText=D�di�
DedicatedHelp=Appuyez pour lancer un serveur d�di�.
ServerText=Serveur
ConfirmTitle="Confirmer le d�but de la partie"
ConfirmText="Vous allez activer un serveur d'�coute contenant ngWorldStats
mais
vous n'avez pas entr� le mot de passe ngWorldStats. Si vous n'entrez pas de mot de passe,
les stats de la partie ne seront pas enregistr�es. Voulez-vous
entrer
un mot de passe maintenant ?"

[UTLadderCTF]
ShortTitle=CLD
TrophyMap=EOL_CTF.unr
LadderName=Capturer le drapeau

[UTLadderAS]
TrophyMap=EOL_Assault.unr
LadderName=Assaut

[KillGameQueryWindow]
WindowTitle=V�rifier la partie supprim�e

[UTGameMenu]
NewGameName=&D�marrer le Tournoi Unreal
NewGameHelp=D�marrer une nouvelle partie de Tournoi Unreal !
LoadGameName=&Reprendre Tournoi enregistr�
LoadGameHelp=Reprendre une partie enregistr�e de Tournoi Unreal.
BotmatchName=D�marrer s�ance d'&entra�nement
BotmatchHelp=Commencer une partie d'entra�nement contre les bots.
ReturnToGameName=Revenir � la partie en &cours
ReturnToGameHelp=Quitter les menus et reprendre la partie en cours. Vous pouvez aussi appuyer sur ECHAP pour reprendre la partie.
QuitName=&Quitter
QuitHelp=Enregistrer les pr�f�rences et quitter Unreal.
QuitTitle=Confirmation
QuitText=Etes-vous s�r de vouloir quitter le Tournoi Unreal ?

[SpeechBinderWindow]
WindowTitle=Associer Voix/Touches

[UTConfigIndivBotsWindow]
WindowTitle=Configurer les bots

[FreeSlotsWindow]
WindowTitle=Aucun logement disponible

[UTBotmatchWindow]
WindowTitle=D�marrer s�ance d'entra�nement

[ngWorldSecretWindow]
WindowTitle=S�lection du mot de passe ngWorldStats

[UTCustomizeClientWindow]
LabelList[0]=Commandes,Tir principal
LabelList[1]=Tir secondaire
LabelList[2]=Avancer
LabelList[3]=Reculer
LabelList[4]=Bond � gauche
LabelList[5]=Bond � droite
LabelList[6]=Tourner � gauche
LabelList[7]=Tourner � droite
LabelList[8]=Sauter/Haut
LabelList[9]=S'accroupir/Bas
LabelList[10]=SourisVision
LabelList[11]=Regarder en haut
LabelList[12]=Regarder en bas
LabelList[13]=Centrer la vue
LabelList[14]=Marcher
LabelList[15]=Bond de c�t�
LabelList[16]=Simuler la mort
LabelList[17]="Injures / conversation, Dis"
LabelList[18]="Equipe Dis"
LabelList[19]="Afficher Menu Voix"
LabelList[20]=D�hanchement
LabelList[21]=Signe de la main
LabelList[22]="Moquerie de base"
LabelList[23]=Dance De Victoire
LabelList[24]="Armes,Arme suivante"
LabelList[25]="Arme pr�c�dente"
LabelList[26]="Lancer Arme"
LabelList[27]="S�lectionner Meilleur Arme"
LabelList[28]="T�l�porteur"
LabelList[29]="Tron�onneuse"
LabelList[30]="Marteau"
LabelList[31]="Nettoyeur"
LabelList[32]="Fusil de choc"
LabelList[33]="Fusil Bio"
LabelList[34]="Pistolet � impulsion"
LabelList[35]="Fusil de pr�cision"
LabelList[36]="Eventreur"
LabelList[37]="Minigun"
LabelList[38]="Canon DCA"
LabelList[39]="Lance-roquettes"
LabelList[40]="R�dempteur"
LabelList[41]="Vue Equipier,Equipier 1"
LabelList[42]="Equipier 2"
LabelList[43]="Equipier 3"
LabelList[44]="Equipier 4"
LabelList[45]="Equipier 5"
LabelList[46]="Equipier 6"
LabelList[47]="Equipier 7"
LabelList[48]="Equipier 8"
LabelList[49]="Equipier 9"
LabelList[50]="Equipier 10"
LabelList[51]="HUD,Agrandir HUD"
LabelList[52]="R�duire HUD"

[UTPasswordWindow]
WindowTitle="Entrez le mot de passe serveur"

[UTPasswordCW]
PasswordText="Mot de passe :"

[UTServerSetupPage]
GamePasswordText="Mot de passe la partie"
GamePasswordHelp="S'il a �t� configur�, le joueur doit utiliser ce mot de passe
pour se connecter au serveur."
AdminPasswordText="Mot de passe admin"
AdminPasswordHelp="S'il a �t� configur�, le joueur peut acc�der � votre serveur
en utilisant ce mot de passe et acc�der aux contr�les admin de la console."
EnableWebserverText="Administration Internet"
EnableWebserverHelp="Cochez cette case pour administrer votre serveur UT � distance sans utiliser de navigateur Internet. "
WebAdminUsernameText="Nom d'utilisateur"
WebAdminUsernameHelp="Nom d'utilisateur n�cessaire pour la connexion � l'administration de serveur � distance Internet."
WebAdminPasswordText="Mot de passe Internet "
WebAdminPasswordHelp="Mot de passe n�cessaire pour la connexion � l'administration de serveur � distance Internet."
ListenPortText="Port de serveur"
ListenPortHelp="Num�ro de port pris en compte par le serveur Internet d'administration � distance pour les connexions entrantes."
