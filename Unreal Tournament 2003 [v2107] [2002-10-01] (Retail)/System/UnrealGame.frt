[Public]
Object=(Name=UnrealGame.MutBigHead,Class=Class,MetaClass=Engine.Mutator,Description="Grosse tête, la taille de la tête dépend de vos prouesses.")
Object=(Name=UnrealGame.MutLowGrav,Class=Class,MetaClass=Engine.Mutator,Description="Faible gravité, gravité réduite.")

[UnrealScriptedSequence]
DefensePointDefaultAction1.ActionString="Aller au point"
DefensePointDefaultAction2.ActionString="Attendre chrono"

[CTFGame]
GameName="Capture de drapeau"

[CTFHUDMessage]
YouHaveFlagString="Vous avez le drapeau, regagnez la base !"
EnemyHasFlagString="L'ennemi a votre drapeau, reprenez-le !"

[CTFMessage]
ReturnBlue="rapporte le drapeau bleu !"
ReturnRed="rapporte le drapeau rouge !"
ReturnedBlue="Le drapeau bleu est revenu !"
ReturnedRed="Le drapeau rouge est revenu !"
CaptureBlue="a réussi ! L'équipe rouge marque !"
CaptureRed="a réussi ! L'équipe bleue marque !"
DroppedBlue="a lâché le drapeau bleu !"
DroppedRed="a lâché le drapeau rouge !"
HasBlue="a le drapeau bleu !"
HasRed="a la drapeau rouge !"

[DMMutator]
DMMutPropsDisplayText[0]="Super vitesse"
DMMutPropsDisplayText[1]="Contrôle aérien"

[DeathMatch]
GameName="DeathMatch"
DMPropsDisplayText[0]="Retard lancement réseau"
DMPropsDisplayText[1]="Min. joueurs réseau"
DMPropsDisplayText[2]="Retard relance"
DMPropsDisplayText[3]="Partie de tournoi"
DMPropsDisplayText[4]="Joueurs doivent être prêts"
DMPropsDisplayText[5]="Forcer réapparition"
DMPropsDisplayText[6]="Ajuster compétence bots"
DMPropsDisplayText[7]="Autoriser provoc."
DMPropsDisplayText[8]="Tps protection régénération"

[FirstBloodMessage]
FirstBloodString="a versé le premier sang !"

[GameObjective]
DestructionMessage="Objectif désactivé !"
LocationPrefix="Proche"
ObjectiveStringSuffix=" Base équipe"


[KeyPickup]
PickupMessage="Vous avez ramassé une clé."

[KillingSpreeMessage]
EndSpreeNote=" - sa folie meurtrière a été stoppée par"
EndSelfSpree="faisait fort jusqu'à ce qu'il se tue !"
EndFemaleSpree="faisait fort jusqu'à ce qu'elle se tue !"
SpreeNote[0]="a une folie meurtrière !"
SpreeNote[1]="est en pleine frénésie !"
SpreeNote[2]="domine !"
SpreeNote[3]="est imbattable !"
SpreeNote[4]="se bat comme un dieu !"
SpreeNote[5]="est méchamment GRAVE !"
SelfSpreeNote[0]="Folie meurtrière !"
SelfSpreeNote[1]="Frénésie !"
SelfSpreeNote[2]="Domination !"
SelfSpreeNote[3]="Imbattable !"
SelfSpreeNote[4]="DIVIN !"
SelfSpreeNote[5]="MECHAMMENT GRAVE !"

[MutBigHead]
FriendlyName="Grosse tête"
Description="La taille de la tête dépend de vos prouesses."

[MutLowGrav]
FriendlyName="Faible gravité"
Description="Gravité réduite."

[SquadAI]
SupportString="en soutien"
DefendString="en défense"
AttackString="en attaque"
HoldString="en occupation"
FreelanceString="Balayeur"

[StartupMessage]
Stage[0]="Attente des autres joueurs."
Stage[1]="Attente du signal de départ. Vous êtes prêt."
Stage[2]="La partie est sur le point de commencer... 3"
Stage[3]="La partie est sur le point de commencer... 2"
Stage[4]="La partie est sur le point de commencer... 1"
Stage[5]="La partie a commencé !"
Stage[6]="La partie a commencé !"
Stage[7]="TEMPS SUPPLEMENTAIRE !"
NotReady="Vous n'êtes pas prêt. Appuyez sur [TIR] !"
SinglePlayer="Appuyez sur [TIR] pour commencer !"

[TeamGame]
GameName="Team Deathmatch"
TGPropsDisplayText[0]="Bots équilibrent équipes"
TGPropsDisplayText[1]="Joueurs équilibrent équipes"
TGPropsDisplayText[2]="Taille d'équipe max."
TGPropsDisplayText[3]="Echelle tir fratricide"
NearString="Près de"

[TimerMessage]
CountDownTrailer="..."

[UnrealMPGameInfo]
MPGIPropsDisplayText[0]="Min. joueurs"
MPGIPropsDisplayText[1]="Rounds score équipe"
