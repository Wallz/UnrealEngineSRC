[Public]
Object=(Name=UnrealGame.MutBigHead,Class=Class,MetaClass=Engine.Mutator,Description="Grosse t�te, la taille de la t�te d�pend de vos prouesses.")
Object=(Name=UnrealGame.MutLowGrav,Class=Class,MetaClass=Engine.Mutator,Description="Faible gravit�, gravit� r�duite.")

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
CaptureBlue="a r�ussi ! L'�quipe rouge marque !"
CaptureRed="a r�ussi ! L'�quipe bleue marque !"
DroppedBlue="a l�ch� le drapeau bleu !"
DroppedRed="a l�ch� le drapeau rouge !"
HasBlue="a le drapeau bleu !"
HasRed="a la drapeau rouge !"

[DMMutator]
DMMutPropsDisplayText[0]="Super vitesse"
DMMutPropsDisplayText[1]="Contr�le a�rien"

[DeathMatch]
GameName="DeathMatch"
DMPropsDisplayText[0]="Retard lancement r�seau"
DMPropsDisplayText[1]="Min. joueurs r�seau"
DMPropsDisplayText[2]="Retard relance"
DMPropsDisplayText[3]="Partie de tournoi"
DMPropsDisplayText[4]="Joueurs doivent �tre pr�ts"
DMPropsDisplayText[5]="Forcer r�apparition"
DMPropsDisplayText[6]="Ajuster comp�tence bots"
DMPropsDisplayText[7]="Autoriser provoc."
DMPropsDisplayText[8]="Tps protection r�g�n�ration"

[FirstBloodMessage]
FirstBloodString="a vers� le premier sang !"

[GameObjective]
DestructionMessage="Objectif d�sactiv� !"
LocationPrefix="Proche"
ObjectiveStringSuffix=" Base �quipe"


[KeyPickup]
PickupMessage="Vous avez ramass� une cl�."

[KillingSpreeMessage]
EndSpreeNote=" - sa folie meurtri�re a �t� stopp�e par"
EndSelfSpree="faisait fort jusqu'� ce qu'il se tue !"
EndFemaleSpree="faisait fort jusqu'� ce qu'elle se tue !"
SpreeNote[0]="a une folie meurtri�re !"
SpreeNote[1]="est en pleine fr�n�sie !"
SpreeNote[2]="domine !"
SpreeNote[3]="est imbattable !"
SpreeNote[4]="se bat comme un dieu !"
SpreeNote[5]="est m�chamment GRAVE !"
SelfSpreeNote[0]="Folie meurtri�re !"
SelfSpreeNote[1]="Fr�n�sie !"
SelfSpreeNote[2]="Domination !"
SelfSpreeNote[3]="Imbattable !"
SelfSpreeNote[4]="DIVIN !"
SelfSpreeNote[5]="MECHAMMENT GRAVE !"

[MutBigHead]
FriendlyName="Grosse t�te"
Description="La taille de la t�te d�pend de vos prouesses."

[MutLowGrav]
FriendlyName="Faible gravit�"
Description="Gravit� r�duite."

[SquadAI]
SupportString="en soutien"
DefendString="en d�fense"
AttackString="en attaque"
HoldString="en occupation"
FreelanceString="Balayeur"

[StartupMessage]
Stage[0]="Attente des autres joueurs."
Stage[1]="Attente du signal de d�part. Vous �tes pr�t."
Stage[2]="La partie est sur le point de commencer... 3"
Stage[3]="La partie est sur le point de commencer... 2"
Stage[4]="La partie est sur le point de commencer... 1"
Stage[5]="La partie a commenc� !"
Stage[6]="La partie a commenc� !"
Stage[7]="TEMPS SUPPLEMENTAIRE !"
NotReady="Vous n'�tes pas pr�t. Appuyez sur [TIR] !"
SinglePlayer="Appuyez sur [TIR] pour commencer !"

[TeamGame]
GameName="Team Deathmatch"
TGPropsDisplayText[0]="Bots �quilibrent �quipes"
TGPropsDisplayText[1]="Joueurs �quilibrent �quipes"
TGPropsDisplayText[2]="Taille d'�quipe max."
TGPropsDisplayText[3]="Echelle tir fratricide"
NearString="Pr�s de"

[TimerMessage]
CountDownTrailer="..."

[UnrealMPGameInfo]
MPGIPropsDisplayText[0]="Min. joueurs"
MPGIPropsDisplayText[1]="Rounds score �quipe"
