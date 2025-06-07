[Language]
Language="Fran�ais"
LangId=12
SubLangId=0
Locked=True

[Public]
Object=(Name=Core.HelloWorldCommandlet,Class=Class,MetaClass=Core.Commandlet)
Preferences=(Caption="Advanced",Parent="Advanced Options")
Preferences=(Caption="File System",Parent="Advanced",Class=Core.System,Immediate=True)

[Errors]
Unknown="Erreur inconnue"
Aborted="Annul�"
ExportOpen="Erreur d'exportation de %s : impossible d'ouvrir le fichier '%s'"
ExportWrite="Erreur d'exportation de %s : impossible d'�crire dans le fichier '%s'"
FileNotFound="Fichier '%s' introuvable"
ObjectNotFound="Objet '%s %s.%s' introuvable"
PackageNotFound="Fichier de l'ensemble '%s' introuvable"
PackageResolveFailed="Impossible de r�soudre le nom de l'ensemble"
FilenameToPackage="Impossible de convertir le nom de fichier '%s' vers le nom d'ensemble"
Sandbox="Ensemble '%s' inaccessible dans cette sandbox"
PackageVersion="Disparit� de version de l'ensemble '%s'"
FailedLoad="Echec du chargement de '%s' : %s"
ConfigNotFound="'%s' introuvable dans le fichier de configuration"
LoadClassMismatch="%s n'est pas une classe fille de %s.%s"
NotDll="'%s' n'est pas un ensemble DLL ; exportation '%s' introuvable"
NotInDll="'%s' introuvable dans '%s.dll'"
FailedLoadPackage="Echec du chargement de l'ensemble : %s"
FailedLoadObject="Echec du chargement de '%s %s.%s': %s"
TransientImport="Objet transitoire import� : %s"
FailedSavePrivate="Impossible de sauvegarder %s : graphe li� � un objet externe priv� %s"
FailedImportPrivate="Importation de l'objet priv� %s %s impossible"
FailedCreate="%s %s introuvable pour cr�ation"
FailedImport="%s introuvable dans le fichier '%s'"
FailedSaveFile="Erreur de sauvegarde du fichier '%s' : %s"
SaveWarning="Erreur de sauvegarde de '%s'"
NotPackaged="Objet hors ensemble : %s %s"
NotWithin="Objet %s %s cr�� dans %s au lieu de %s"
Abstract="Cr�ation de l'objet %s impossible : classe %s abstraite"
NoReplace="Remplacement de %s par %s impossible"
NoFindImport="Fichier '%s' introuvable pour importation"
ReadFileFailed="Echec de lecture du fichier '%s' pour importation"
SeekFailed="Erreur de recherche de fichier"
OpenFailed="Erreur d'ouverture de fichier"
WriteFailed="Erreur d'�criture de fichier"
ReadEof="Lecture au-del� de la fin de fichier"
IniReadOnly="Fichier %s prot�g� en �criture ; sauvegarde des param�tres impossible"
UrlFailed="Echec de lancement d'URL"
Warning="Avertissement"
Question="Question"
OutOfMemory="M�moire virtuelle insuffisante. Pour �viter cela, lib�rez de l'espace sur le disque dur principal."
History="Historique"
Assert="Echec de d�claration : %s [Fichier : %s] [Ligne : %i]"
Debug="Echec de d�claration de d�boguage : %s [Fichier : %s] [Ligne : %i]"
LinkerExists="Liaison de '%s' d�j� existante"
BinaryFormat="Le fichier '%s' contient des donn�es non reconnaissables"
SerialSize="%s : Disparit� de taille s�rie : %i re�u, %i attendu"
ExportIndex="Mauvais index d'exportation %i/%i"
ImportIndex="Mauvais index d'importation %i/%i"
Password="Mot de passe non reconnu"
Exec="Commande non reconnue"
BadProperty="'%s' : Propri�t� mauvaise ou manquante '%s'"
MisingIni="Fichier .ini manquant : %s"

[Query]
OldVersion="Le fichier %s a �t� sauvegard� par une version ant�rieure non compatible avec la version actuelle. Sa lecture va probablement �chouer et peut causer un plantage. Voulez-vous cependant la tenter ?"
Name="Nom :"
Password="Mot de passe :"
PassPrompt="Tapez votre nom et votre mot de passe :"
PassDlg="V�rification d'identit�"
Overwrite="Le fichier '%s' doit �tre mis � jour. Ecraser la version existante ?"
OverwriteReadOnly="'%s' est en lecture seule. Tenter de l'�craser ?"
CheckoutPerforce="'%s' est en lecture seule.  Essayer de le v�rifier avec Perforce ?"

[Progress]
Saving="Sauvegarde du fichier %s..."
Loading="Chargement du fichier %s..."
Closing="Fermeture"

[General]
Product="Unreal"
Engine="Unreal Engine"
Copyright="Copyright 2001 Epic Games, Inc."
True="Vrai"
False="Faux"
None="N�ant"
Yes="Oui"
No="Non"
