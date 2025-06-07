[Public]
Object=(Name=UnrealShare.DeathmatchGame,Class=Class,MetaClass=Engine.GameInfo)
Object=(Name=UnrealShare.TeamGame,Class=Class,MetaClass=Engine.GameInfo)
Object=(Name=UnrealShare.CoopGame,Class=Class,MetaClass=Engine.GameInfo)
Object=(Name=UnrealShare.DispersionPistol,Class=Class,MetaClass=Engine.Weapon)
Object=(Name=UnrealShare.Automag,Class=Class,MetaClass=Engine.Weapon)
Object=(Name=UnrealShare.Stinger,Class=Class,MetaClass=Engine.Weapon)
Object=(Name=UnrealShare.ASMD,Class=Class,MetaClass=Engine.Weapon)
Object=(Name=UnrealShare.Eightball,Class=Class,MetaClass=Engine.Weapon)
Object=(Name=UnrealShare.FemaleOne,Class=Class,MetaClass=UnrealShare.UnrealiPlayer,Description="Femme 1")
Object=(Name=UnrealShare.MaleThree,Class=Class,MetaClass=UnrealShare.UnrealiPlayer,Description="Homme 3")
Object=(Name=UnrealShare.UnrealSpectator,Class=Class,MetaClass=UnrealShare.UnrealiPlayer,Description="Spectateur")
Object=(Name=UnrealShare.FemaleOneBot,Class=Class,MetaClass=UnrealShare.Bots,Description="Femme 1")
Object=(Name=UnrealShare.MaleThreeBot,Class=Class,MetaClass=UnrealShare.Bots,Description="Homme 3")
Object=(Name=UnrealShare.SinglePlayer,Class=Class,MetaClass=UnrealShare.SinglePlayer)
Preferences=(Caption="Types de partie",Parent="Options avanc�es")
Preferences=(Caption="Coop�ratif",Parent="Types de partie",Class=UnrealShare.CoopGame,Immediate=True)
Preferences=(Caption="Match � mort",Parent="Types de partie",Class=UnrealShare.DeathmatchGame,Immediate=True)
Preferences=(Caption="Partie en �quipe",Parent="Types de partie",Class=UnrealShare.TeamGame,Immediate=True)
Preferences=(Caption="R�seau",Parent="Options avanc�es")
Preferences=(Caption="Listes de cartes",Parent="R�seau")
Preferences=(Caption="Cartes matchs � mort",Parent="Listes de cartes",Class=UnrealShare.DmMapList,Immediate=True)

[DeathmatchGame]
ClassCaption=Match � mort
GlobalNameChange=" a chang� son nom en "
NoNameChange=" est d�j� en cours d'utilisation"
MaxedOutMessage= Le serveur est d�j� en pleine capacit�.
GameName=Match � mort
TimeMessage[0]=Plus que 5 minutes avant la fin de la partie ! 
TimeMessage[1]=Plus que 4 minutes avant la fin de la partie !
TimeMessage[2]=Plus que 3 minutes avant la fin de la partie !
TimeMessage[3]=Plus que 2 minutes avant la fin de la partie !
TimeMessage[4]=Plus qu'1 minute avant la fin de la partie !
TimeMessage[5]=Plus que 30 secondes !
TimeMessage[6]=Plus que 10 secondes !
TimeMessage[7]=5 secondes...
TimeMessage[8]=4...
TimeMessage[9]=3...
TimeMessage[10]=2...
TimeMessage[11]=1...
TimeMessage[12]=Termin� !

[TeamGame]
ClassCaption=Partie en �quipe
NewTeamMessage=" a rejoint "
TeamColor[0]=Rouge
TeamColor[1]=Bleue
TeamColor[2]=Verte
TeamColor[3]=Or
GameName=Partie par �quipe

[CoopGame]
ClassCaption= Mode coop�ratif 
GameName=Coop�ratif

[UnrealGameInfo]
DeathVerb=" a �t� "
DeathPrep=" par "
DeathTerm=tu�
DeathArticle= un(e)  
ExplodeMessage=" a explos�"
SuicideMessage=" a fait un infarctus."
FallMessage=" a laiss� un petit crat�re."
DrownedMessage=" a oubli� de respirer � la surface."
BurnedMessage=" a �t� incin�r�"
CorrodedMessage=" a �t� englu�"
HackedMessage=" a �t� taill� en pi�ces "
DeathMessage[0]=tu�
DeathMessage[1]=gagn� 
DeathMessage[2]=fum�
DeathMessage[3]=d�pec�
DeathMessage[4]=annihil�
DeathMessage[5]=descendu 
DeathMessage[6]=�cr�m� 
DeathMessage[7]=perfor� 
DeathMessage[8]=d�chiquet� 
DeathMessage[9]=d�truit 
DeathMessage[10]=ridiculis� 
DeathMessage[11]=mis aux ordures 
DeathMessage[12]=extermin� 
DeathMessage[13]=�cras� 
DeathMessage[14]=an�anti 
DeathMessage[15]=broy� 
DeathMessage[16]=battu 
DeathMessage[17]=�crabouill� 
DeathMessage[18]=r�duit � l'�tat de pur�e 
DeathMessage[19]=tranch� 
DeathMessage[20]=coup� en morceaux 
DeathMessage[21]=�ventr� 
DeathMessage[22]=explos� 
DeathMessage[23]=d�chir� 
DeathMessage[24]=fess� 
DeathMessage[25]=�visc�r� 
DeathMessage[26]=neutralis� 
DeathMessage[27]=fouett� 
DeathMessage[28]=roul� 
DeathMessage[29]=recycl� 
DeathMessage[30]=crev� 
DeathMessage[31]=tron�onn� 
DeathModifier[0]=enti�rement 
DeathModifier[1]=compl�tement 
DeathModifier[2]=absolument 
DeathModifier[3]=totalement 
DeathModifier[4]=d�finitivement 
MajorDeathMessage[0]=�ventr� un autre 
MajorDeathMessage[1]=fait des d�g�ts 
MajorDeathMessage[2]=red�fini le concept de douleur 
MajorDeathMessage[3]=
MajorDeathMessage[4]=
MajorDeathMessage[5]=
MajorDeathMessage[6]=
MajorDeathMessage[7]=
HeadLossMessage[0]=d�capit� 
HeadLossMessage[1]=�t�t� 

[UnrealQuitMenu]
YesSelString=" [OUI]  Non"
NoSelString="  Oui  [NON]"
MenuTitle=Quitter ?
HelpMessage[1]=S�lectionnez Oui et appuyez sur Entr�e pour retrouver la fadeur et l'inutilit� de votre existence, puisque vous n'avez pas l'�toffe d'UNREAL.
MenuList[0]=Digital Extremes/Epic Megagames 
MenuList[1]=Collaboration
MenuList[2]="Conception : James Schmalz"
MenuList[3]=Cliff Bleszinski
MenuList[4]=Niveaux : Cliff Bleszinski
MenuList[5]=T. Elliot Cannon  Pancho Eekels
MenuList[6]=Jeremy War  Cedric Fiorentino
MenuList[7]=Shane Caudle
MenuList[8]=Anim.: Dave Carter
MenuList[9]=Graph. : James Schmalz 
MenuList[10]=Mike Leatham  Artur Bialas
MenuList[11]=Prog. : Tim Sweeney  Steven Polge
MenuList[12]=Erik de Neve  James Schmalz
MenuList[13]=Carlo Vogelsang  Nick Michon
MenuList[14]=Musique : Alexander Brandon
MenuList[15]=Michiel van den Bos
MenuList[16]=Effets son. : Dave Ewing
MenuList[17]=Producteur GT: Jason Schreiber
MenuList[18]=Biz:Mark Rein Nigel Kent Craig Lafferty

[DispersionPistol]
PickupMessage=Vous avez le pistolet � dispersion 
ItemName=Pistolet � dispersion 
DeathMessage=%o a �t� tu� par %w de %k. Quel nul !

[UnrealGameOptionsMenu]
AdvancedString=Options avanc�es
AdvancedHelp=Modifiez les options de configuration avanc�es du jeu.
MenuTitle=OPTIONS DE JEU 
HelpMessage[1]=R�glez la vitesse � laquelle le temps passe dans le jeu.
HelpMessage[2]=Avec l'option Vrai, vous r�duisez l'aspect gore du jeu.
MenuList[1]=Vitesse de jeu 
MenuList[2]=Moins de gore 

[UnrealIndivBotMenu]
MenuTitle=Configuration intelligence artificielle
HelpMessage[1]=Type de configuration de Bot. Utilisez les touches gauche et droite pour changer.
HelpMessage[2]=Appuyez sur Entr�e pour changer le nom de ce bot.
HelpMessage[3]=Utilisez les touches gauche et droite pour changer la classe de ce bot.
HelpMessage[4]=Utilisez les touches gauche et droite pour changer l'aspect de ce bot.
HelpMessage[5]="R�glez le niveau d'habilet� g�n�rale de ce bot dans cette proportion (par rapport au niveau d'habilet� de base des bots)."
HelpMessage[6]=" Indiquez dans quelle �quipe ce bot joue (Rouge, Bleue, Verte ou Jaune)."
MenuList[1]="Configuration"
MenuList[2]=Nom
MenuList[3]=Classe
MenuList[4]=Aspect
MenuList[5]=Habilet�
MenuList[6]=Equipe

[IntroNullHud]
ESCMessage=Appuyez sur ECHAP pour commencer 

[UnrealFavoritesMenu]
MenuTitle=FAVORIS 
EditList[0]=Nom du serveur:
EditList[1]=Adresse :
HelpMessage[1]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[2]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[3]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[4]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[5]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[6]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[7]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[8]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[9]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[10]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[11]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.
HelpMessage[12]=Appuyez sur Entr�e pour acc�der � ce serveur. Appuyez sur la touche de direction vers la droite pour modifier cette entr�e.

[Translator]
PickupMessage=Appuyez sur F2 pour activer le Traducteur 
NewMessage=Traducteur universel

[UnrealVideoMenu]
MenuTitle=AUDIO/VIDEO
HelpMessage[1]="R�glez le niveau de luminosit� � l'aide des touches gauche et droite."
HelpMessage[2]=Affichez Unreal dans une fen�tre. Note : un mode d'affichage logiciel peut faire dispara�tre des d�tails visibles avec l'acc�l�ration graphique.
HelpMessage[3]=" S�lectionnez la r�solution � l'aide des touches gauche et droite, puis appuyez sur Entr�e pour valider votre choix."
HelpMessage[4]=D�finissez le param�tre "bas" pour optimiser l'ex�cution du jeu. Toute modification prendra effet au niveau suivant.
HelpMessage[5]=R�glez le volume musical � l'aide des touches gauche et droite.
HelpMessage[6]=R�glez le volume des effets sonores � l'aide des touches gauche et droite.
HelpMessage[7]=D�finissez l'option "bas" pour optimiser l'ex�cution du jeu sur des machines ne disposant pas de plus de 32 Mo de RAM. Toute modification prendra effet au niveau suivant.
MenuList[1]=Luminosit� 
MenuList[2]=Mode Plein �cran 
MenuList[3]=R�solution 
MenuList[4]=D�tail des textures 
MenuList[5]=Volume Musique 
MenuList[6]=Volume Son 
MenuList[7]=Qualit� audio 
LowText=Bas
HighText=Haut
HelpMessage[8]=Avec l'option vrai, vous entendrez des messages dans les types de parties qui mettent en sc�ne ces messages.
MenuList[8]=Messages
HelpMessage[9]=Si vous s�lectionnez Vrai, vous entendrez un bip lorsque vous recevrez un message.
MenuList[9]=Bip activation

[UnrealMeshMenu]
HelpMessage[4]=Changez votre classe � l'aide des touches gauche et droite.
HelpMessage[5]=Entrez le mot de passe admin ici, ou le mot de passe du jeu si n�cessaire.
MenuList[4]=Classe :
MenuList[5]=Mot de passe :
HelpMessage[6]=Appuyez sur Entr�e pour commencer la partie.
MenuList[6]=D�marrer la partie 

[UnrealPlayerMenu]
MenuTitle=S�lectionner repr�sentation num�rique
HelpMessage[1]=Appuyez sur Entr�e pour taper votre nom. Veillez � le faire avant de rejoindre une partie multijoueur.
HelpMessage[2]=Utilisez les touches de direction pour changer la couleur de votre �quipe (Rouge, Bleu, Vert ou Jaune).
HelpMessage[3]=Modifiez votre apparence � l'aide des touches de direction gauche et droite.
MenuList[1]=Nom : 
MenuList[2]=Equipe :
MenuList[3]=Aspect :
HelpMessage[4]=Vous ne pouvez modifier votre classe/mod�le qu'au moment o� vous rejoignez une partie multijoueur ou lorsque vous commencez une nouvelle partie � un seul joueur.
MenuList[4]=Classe :
Teams[0]=Rouge
Teams[1]=Bleu
Teams[2]=Jaune
Teams[3]=Vert

[UnrealJoinGameMenu]
InternetOption="Internet (28.8 - 56K) "
FastInternetOption=" Internet rapide (RNIS, c�ble modem) "
LANOption=" RESEAU LOCAL "
MenuTitle=REJOINDRE UNE PARTIE 
HelpMessage[1]=Recherchez les serveurs locaux 
HelpMessage[2]=Choisissez un serveur dans la liste des favoris 
HelpMessage[3]=Appuyez sur Entr�e pour taper une adresse de serveur. Appuyez � nouveau sur Entr�e pour acc�der � ce serveur.
HelpMessage[4]=R�glez la vitesse du r�seau.
HelpMessage[5]=Ouvrez la page Web de la liste des serveurs Unreal Epic 
MenuList[1]=Trouvez les serveurs locaux 
MenuList[2]=Choisissez parmi les favoris 
MenuList[3]=Ouvrez 
MenuList[4]=Optimis� pour 
MenuList[5]="Acc�s liste serveurs Unreal Epic"

[UnrealOptionsMenu]
MenuTitle=OPTIONS DU MENU
HelpMessage[1]=Activez ou d�sactivez aide de vis�e verticale.
HelpMessage[2]=Basculez l'activation du joystick.
HelpMessage[3]=R�glez la sensibilit� de la souris, ou la distance que vous devez faire parcourir � la souris pour produire un mouvement donn� dans le jeu.
HelpMessage[4]=Inversez l'axe X de la souris. Avec l'option Vrai, vous regarderez vers le bas plut�t que vers le haut si vous poussez la souris vers l'avant.
HelpMessage[5]=Avec l'option Vrai, la vue se centre automatiquement lorsque vous l�chez la touche VueSouris.
HelpMessage[6]=Avec l'option Vrai, la souris sert toujours � regarder en haut et en bas, sans n�cessiter de touche VueSouris.
HelpMessage[7]= Avec l'option Vrai, la vue s'ajuste automatiquement pour regarder en haut et en bas lorsque vous n'�tes pas en mode VueSouris.
HelpMessage[8]=Avec l'option Vrai, votre �cran se mettra � clignoter lorsque vous ferez feu.
HelpMessage[9]=Choisissez le r�ticule de vis�e qui sera affich� au centre de l'�cran. 
HelpMessage[10]=S�lectionnez l'endroit de l'�cran o� appara�tra votre arme.
HelpMessage[11]=" Si cette option est activ�e, vous ferez un �cart rapide en double-cliquant sur les touches de d�placement (avant, arri�re, gauche et droite)."
HelpMessage[12]=Appuyez sur Entr�e pour personnaliser la configuration du clavier, de la souris et du joystick.
HelpMessage[13]=Appuyez sur Entr�e pour �tablir l'ordre d'apparition des armes.
HelpMessage[14]=Utilisez les touches gauche et droite pour s�lectionner une configuration Visualisation T�te haute (HUD).
HelpMessage[15]=R�glez la quantit� de mouvement de la cam�ra lorsque vous vous d�placez.
HelpMessage[16]=Ouvrez le menu de configuration des options avanc�es.
MenuList[1]=Vis�e automatique 
MenuList[2]=Joystick activ� 
MenuList[3]=Sensib. souris 
MenuList[4]=Inverser souris 
MenuList[5]=VueAuto 
MenuList[6]=Toujours VueSouris 
MenuList[7]=Vue AutoHorizon 
MenuList[8]=Clignotement arme
MenuList[9]=R�ticule 
MenuList[10]=Main arm�e 
MenuList[11]=Esquive 
MenuList[12]=Personnaliser les commandes 
MenuList[13]=Priorit� des armes 
MenuList[14]=Configuration HUD 
MenuList[15]=Cam�ra � l'�paule 
MenuList[16]=Options avanc�es 
HideString=Cach�

[EndgameHud]
Message1=La capsule d'�vacuation Skaarj s'est lib�r�e de l'attraction gravitationnelle de la plan�te... Quelle victoire ! Cependant, vous n'avez presque plus de carburant et vous d�rivez dans l'espace...
Message2=Vous avez r�ussi � vous �chapper de l� o� tant y ont laiss� la vie. Vous riez int�rieurement. Il s'est pass� tant de choses, mais rien n'a chang�.
Message3=Avant de vous �craser sur la plan�te, vous �tiez enferm� dans une cellule et vous voil� � nouveau confin� dans une prison.
Message4=Mais vous gardez l'espoir que quelqu'un vous rep�rera dans votre petit vaisseau... un jour.
Message5=En attendant, vous d�rivez et continuez � esp�rer.
Message6=A suivre...

[UnrealWeaponMenu]
MenuTitle=PRIORITES 

[Eightball]
PickupMessage=Vous avez le lanceur 8 coups 
ItemName=Lanceur 8 coups 
DeathMessage=%o a �t� frapp� � plusieurs reprises par %w de %k.

[UnrealKeyboardMenu]
OrString=" ou "
MenuTitle=COMMANDES 
HelpMessage[1]=
MenuList[1]=Tirer 
MenuList[2]=Alterner tir 
MenuList[3]=Avancer 
MenuList[4]=Reculer 
MenuList[5]=Tourner � gche 
MenuList[6]=Tourner � dte 
MenuList[7]=Mitrail. � gche 
MenuList[8]=Mitrail. � dte 
MenuList[9]=Sauter/Haut
MenuList[10]=Ramper/Bas 
MenuList[11]="Vue Souris"
MenuList[12]=Activ. �l�ment 
MenuList[13]=El�ment suiv.
MenuList[14]=El�ment pr�c.
MenuList[15]=Regarder haut 
MenuList[16]=Regarder bas 
MenuList[17]=Centrer vue 
MenuList[18]=Marcher 
MenuList[19]=Mitrailler 
MenuList[20]=Arme suivante 
MenuList[21]=REINITIALISER 

[TranslatorEvent]
M_NewMessage=Nouveau message du Traducteur 
M_TransMessage=Message du Traducteur 

[Flashlight]
ExpireMessage=Les piles de votre lampe torche sont �puis�es.
PickupMessage=Vous avez r�cup�r� la lampe torche 

[ParentBlob]
BlobKillMessage=a �t� endommag� par un Blob 

[FlakCannon]
PickupMessage=Vous avez le canon antia�rien 
ItemName=Canon antia�rien 

[Rifle]
PickupMessage=Vous avez le fusil 
ItemName=Sniper 

[UnrealBotConfigMenu]
MenuTitle=BOTS
HelpMessage[1]=Avec l'option Vrai, les bots adaptent leur niveau d'habilet� en fonction de leur efficacit� face aux joueurs.
HelpMessage[2]=Niveau d'habilet� de base des bots (entre 0 et 3).
HelpMessage[3]="Avec l'option Vrai, les bots entrent dans la partie dans un ordre al�atoire. Avec l'option Faux, les bots entrent dans leur ordre de configuration."
HelpMessage[4]=Modifiez la configuration des bots voulus.
MenuList[1]=Auto-adapt. habilet� 
MenuList[2]=Habilet� de base 
MenuList[3]=Ordre al�atoire 
MenuList[4]=Configurer les bots 
HelpMessage[5]=Nombre de bots au d�but de la partie (max 15).
HelpMessage[6]=Utilisez les bots dans des parties contre d'autres personnes.
MenuList[5]=Nombre de bots
MenuList[6]=Bots dans les parties 

[UnrealLoadMenu]
RestartString=Red�marrer 
MenuTitle=CHARGER UNE PARTIE 

[UnrealSlotMenu]
MonthNames[0]=Janv
MonthNames[1]=F�v
MonthNames[2]=Mars
MonthNames[3]=Avril
MonthNames[4]=Mai
MonthNames[5]=Juin
MonthNames[6]=Juil
MonthNames[7]=Ao�t
MonthNames[8]=Sept
MonthNames[9]=Oct
MonthNames[10]=Nov
MonthNames[11]=D�c

[ASMD]
PickupMessage=Vous avez l'ASMD 
ItemName=ASMD
DeathMessage=%k a bless� mortellement %o avec %w.

[UnrealMainMenu]
HelpMessage[1]=Appuyez sur Entr�e pour modifier les options de jeu, y compris le chargement et l'enregistrement des parties, le niveau de difficult�, et le d�marrage d'un BotMatch.
HelpMessage[2]=Appuyez sur Entr�e pour modifier les options de configuration multijoueur, y compris d�marrer ou rejoindre une partie en r�seau, changer d'aspect, de nom ou d'�quipe.
HelpMessage[3]=Appuyez sur Entr�e pour personnaliser les commandes.
HelpMessage[4]=Changez les options audio et vid�o 
HelpMessage[5]=Appuyez sur Entr�e pour quitter la partie.
HelpMessage[7]=Appelez le num�ro gratuit (aux USA) 1-877-4UNREAL pour commander UNREAL !
MenuList[1]=PARTIE 
MenuList[2]=MULTIJOUEUR 
MenuList[3]=OPTIONS
MenuList[4]=AUDIO/VIDEO
MenuList[5]=QUITTER
MenuList[7]=INFOS COMMANDES 

[UnrealServerMenu]
BotTitle=BOTMATCH
MenuTitle=MULTIJOUEUR 
HelpMessage[1]="Choisir le type de partie."
HelpMessage[2]=Choisir la carte.
HelpMessage[3]=Modifier les options de jeu.
HelpMessage[4]=D�marrer la partie.
MenuList[1]=S�lect.partie 
MenuList[2]=S�lect. carte 
MenuList[3]=Configurer la partie 
MenuList[4]=D�marrer la partie 
MenuList[5]=Lancer le serveur d�di� 
HelpMessage[5]=D�marrer un serveur d�di� sur cette machine.

[UnrealGameMenu]
HelpMessage[1]=Appuyez sur Entr�e pour enregistrer la partie en cours.
HelpMessage[2]=Appuyez sur Entr�e pour charger la partie en cours.
HelpMessage[3]= S�lectionnez un niveau de difficult� et d�marrez une nouvelle partie.
HelpMessage[4]="Appuyez sur Entr�e pour modifier les options de jeu. Remarque : Vous ne pouvez le faire si vous jouez une partie multijoueur."
HelpMessage[5]=Match � mort contre les Bots.
MenuList[1]=ENREGISTRER UNE PARTIE 
MenuList[2]=CHARGER UNE PARTIE 
MenuList[3]=NOUVELLE PARTIE 
MenuList[4]=OPTIONS DE JEU 
MenuList[5]=BOTMATCH

[Health]
PickupMessage=Vous avez r�cup�r� un kit m�dical +

[AutoMag]
PickupMessage=Vous avez l'AutoMag 
ItemName=Automag
DeathMessage=%o a �t� aplati par %w de %k.

[UnrealChooseGameMenu]
MenuTitle=CHOISIR UNE PARTIE 
HelpMessage[1]=Choisissez une partie.
HelpMessage[2]=Choisissez une partie.
HelpMessage[3]=Choisissez une partie.
HelpMessage[4]=Choisissez une partie.
HelpMessage[5]=Choisissez une partie.
HelpMessage[6]=Choisissez une partie.
HelpMessage[7]=Choisissez une partie.
HelpMessage[8]=Choisissez une partie.
HelpMessage[9]=Choisissez une partie.
HelpMessage[10]=Choisissez une partie.
HelpMessage[11]=Choisissez une partie.
HelpMessage[12]=Choisissez une partie.
HelpMessage[13]=Choisissez une partie.
HelpMessage[14]=Choisissez une partie.
HelpMessage[15]=Choisissez une partie.
HelpMessage[16]=Choisissez une partie.
HelpMessage[17]=Choisissez une partie.
HelpMessage[18]=Choisissez une partie.
HelpMessage[19]=Choisissez une partie.

[UnrealMultiPlayerMenu]
HelpMessage[1]=Rejoignez une partie en r�seau.
HelpMessage[2]=Configurez et d�marrez une partie en r�seau.
HelpMessage[3]=Configurez votre apparence, nom et nom d'�quipe.
MenuList[1]=REJOINDRE UNE PARTIE 
MenuList[2]=DEMARRER LA PARTIE 
MenuList[3]=CONFIG. JOUEUR 

[UnrealHelpMenu]
MenuTitle=AIDE
HelpMessage[1]=Ouvrez le document de d�pannage.
MenuList[1]=D�pannage

[YesNoMenu]
YesSelString=[OUI]  Non
NoSelString=" Oui  [NON]"

[NaliFruit]
PickupMessage=Vous avez cueilli le fruit de gu�rison Nali +

[Minigun]
PickupMessage=Vous avez le minigun 
ItemName=Minigun

[UnrealListenMenu]
MenuTitle=SERVEURS LOCAUX
HelpMessage[1]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[2]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[3]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[4]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[5]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[6]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[7]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[8]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[9]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[10]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[11]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[12]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[13]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[14]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[15]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[16]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[17]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[18]=Appuyez sur Entr�e pour s�lectionner ce serveur.
HelpMessage[19]=Appuyez sur Entr�e pour s�lectionner ce serveur.

[GESBioRifle]
PickupMessage=Vous avez le fusil Bio GES 
ItemName=Fusil Bio GES 

[UnrealDMGameOptionsMenu]
HelpMessage[3]=Nombre d'ennemis � abattre par le joueur pour remporter la partie. Si 0, pas de limite.
HelpMessage[4]=Dur�e (en minutes) de la partie. Si 0, pas de limite.
HelpMessage[5]=Nombre maximum de joueurs autoris�s dans la partie.
HelpMessage[6]=Si le mode coop�ratif est activ�, les armes ressurgissent automatiquement, mais ne peuvent �tre r�cup�r�es qu'une fois par un joueur donn�.
HelpMessage[7]=Configurez une partie bot et des param�tres individuels.
HelpMessage[8]=Choisissez le style de jeu : Hardcore est plus rapide, et les armes sont plus dangereuses qu'en mode Classique. Turbo ressemble � Hardcore, avec des d�placements tr�s rapides.
HelpMessage[9]=Messages de mort classiques ou d'un style nouveau (bas� sur les armes).
MenuList[3]=Nombre des abats
MenuList[4]=Dur�e
MenuList[5]=Joueurs max 
MenuList[6]=Garder armes
MenuList[7]=Configurer Bots
MenuList[8]=Style de jeu
MenuList[9]=Messages de mort
GameStyle[0]=Classique
GameStyle[1]=Hardcore
GameStyle[2]=Turbo

[Amplifier]
ExpireMessage=L'amplificateur n'a plus d'�nergie.
PickupMessage="Vous avez l'amplificateur d'�nergie"

[SearchLight]
PickupMessage=Vous avez r�cup�r� le projecteur.

[SCUBAGear]
PickupMessage=Vous avez r�cup�r� le scaphandre 

[UnrealNewGameMenu]
HelpMessage[1]=Mode touriste.
HelpMessage[2]=Pr�t � s'�clater !

HelpMessage[3]=Ne convient pas aux d�ficients coronariens.
HelpMessage[4]=Instinct suicidaire.
MenuList[1]=FACILE
MenuList[2]=MOYEN
MenuList[3]=DIFFICILE
MenuList[4]=IRREEL

[Stinger]
PickupMessage=Vous avez r�cup�r� le stinger 
ItemName=Stinger
DeathMessage=%o a �t� perfor� par %w de %k.

[RazorAmmo]
PickupMessage=Vous avez r�cup�r� le lance-lames 

[RocketCan]
PickupMessage=Vous avez r�cup�r� douze 8-coups 

[ShellBox]
PickupMessage=Vous avez r�cup�r� 50 balles 

[VoiceBox]
PickupMessage=Vous avez r�cup�r� la bo�te vocale 

[UnrealSaveMenu]
CantSave=IMPOSSIBLE D'ENREGISTRER SI VOUS ETES MORT 
MenuTitle=ENREGISTRER UNE PARTIE 

[Seeds]
PickupMessage=Vous avez r�cup�r� les graines de fruit Nali 

[RifleAmmo]
PickupMessage=Vous avez 8 cartouches de fusil.

[FlakBox]
PickupMessage=Vous avez r�cup�r� 10 obus de canon antia�rien 

[Flare]
PickupMessage=Vous avez r�cup�r� une fus�e �clairante 

[JumpBoots]
ExpireMessage=Les bottes de saut ont cess� de fonctionner 
PickupMessage=Vous avez r�cup�r� les bottes de saut 

[WeaponPowerUp]
PickupMessage="Vous avez r�cup�r� la recharge du pistolet � dispersion"

[Razorjack]
PickupMessage=Vous avez le lance-lames 
ItemName=Lance-lames 

[ForceField]
M_NoRoom=Manque d'espace pour activer le champ de force.
PickupMessage= Vous avez r�cup�r� le champ de force

[ASMDAmmo]
PickupMessage=Vous avez r�cup�r� un coeur ASMD.

[ShieldBelt]
PickupMessage="Vous avez r�cup�r� la ceinture protectrice"

[UnrealTeamGameOptionsMenu]
HelpMessage[10]="Pourcentage de dommages encaiss�s lorsque vous �tes atteint par un tir ami. Remarque : vous prenez toujours la dose maximale de vos propres armes."
MenuList[10]=Echelle du tir ami 

[OKMenu]
OKString=[OK]

[StingerAmmo]
PickupMessage=Vous avez r�cup�r� 40 �clats de Tarydium 

[UpgradeMenu]
MenuList[1]=Il vous faut une version de Unreal plus r�cente pour jouer sur ce serveur. Souhaitez-vous acc�der au site Web de Unreal pour y r�cup�rer une nouvelle version?

[Sludge]
PickupMessage=Vous avez r�cup�r� 25 kilos de boue de Tarydium 

[Invisibility]
ExpireMessage=L'invisibilit� a cess� de faire effet.
PickupMessage=Vous �tes invisible 

[ToxinSuit]
PickupMessage=Vous avez r�cup�r� la combinaison anti-toxique 

[Shells]
PickupMessage=Vous avez r�cup�r� 15 cartouches 

[RifleRound]
PickupMessage=Vous avez une cartouche de fusil.

[QuadShot]
PickupMessage=Vous avez r�cup�r� le QuadShot 

[PowerShield]
PickupMessage= Vous avez r�cup�r� le bouclier d'�nergie 

[Dampener]
ExpireMessage=L'isolateur acoustique a cess� de faire effet.
PickupMessage=Vous avez r�cup�r� l'isolateur acoustique 

[KevlarSuit]
PickupMessage=Vous avez r�cup�r� la combinaison en kevlar 

[FlakShellAmmo]
PickupMessage=Vous avez r�cup�r� un obus de canon antia�rien.

[Fell]
Name=est tomb� 
AltName=est tomb� 

[Drowned]
Name=s'est noy� 
AltName=s'est noy� 

[Decapitated]
Name=d�capit� 
AltName=�t�t�

[Corroded]
Name=endommag� 
AltName=englu�

[Clip]
PickupMessage=Vous avez r�cup�r� une cartouche 

[Burned]
Name=r�ti
AltName=br�l� vif 

[SuperHealth]
PickupMessage=Vous avez r�cup�r� le super kit m�dical 

[Bandages]
PickupMessage=Vous avez r�cup�r� des bandages +

[AsbestosSuit]
PickupMessage=Vous avez r�cup�r� la combinaison en amiante 

[Armor]
PickupMessage=Vous avez r�cup�r� la combinaison de combat 

[UnrealHUD]
IdentifyName=Nom
IdentifyHealth=Sant�
VersionMessage=Version
TeamName[0]=Equipe rouge :
TeamName[1]=Equipe bleue :
TeamName[2]=Equipe verte :
TeamName[3]=Equipe or :

[UnrealCoopGameOptions]
Difficulties[0]=Facile
Difficulties[1]=Moyen
Difficulties[2]=Difficile
Difficulties[3]=Irr�el
HelpMessage[3]=R�glage du niveau.
MenuList[3]=Difficult�

[UnrealTeamScoreBoard]
TeamName[0]=Equipe rouge :
TeamName[1]=Equipe bleue :
TeamName[2]=Equipe verte :
TeamName[3]=Equipe or :

[SinglePlayer]
GameName=Unreal

[MaleThree]
MenuName=Homme 3

[FemaleOne]
MenuName=Femme 1
