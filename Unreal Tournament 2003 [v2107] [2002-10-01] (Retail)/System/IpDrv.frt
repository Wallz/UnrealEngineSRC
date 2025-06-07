[Public]
Object=(Name=IpDrv.UpdateServerCommandlet,Class=Class,MetaClass=Core.Commandlet)
Object=(Name=IpDrv.MasterServerCommandlet,Class=Class,MetaClass=Core.Commandlet)
Object=(Name=IpDrv.CompressCommandlet,Class=Class,MetaClass=Core.Commandlet)
Object=(Name=IpDrv.DecompressCommandlet,Class=Class,MetaClass=Core.Commandlet)
Object=(Name=IpDrv.TcpNetDriver,Class=Class,MetaClass=Engine.NetDriver)
Object=(Name=IpDrv.UdpBeacon,Class=Class,MetaClass=Engine.Actor)
Preferences=(Caption="Networking",Parent="Advanced Options")
Preferences=(Caption="TCP/IP Network Play",Parent="Networking",Class=IpDrv.TcpNetDriver)
Preferences=(Caption="Server Beacon",Parent="Networking",Class=IpDrv.UdpBeacon,Immediate=True)

[UpdateServerCommandlet]
HelpCmd=updateserver
HelpOneLiner="R�pondre aux demandes de mise � jour auto. d'Unreal Engine."
HelpUsage=updateserver [-option...] [parm=value]
HelpParm[0]=ConfigFile
HelpDesc[0]="Fichier de configuration � employer. Par d�faut : UpdateServer.ini"

[MasterServerCommandlet]
HelpCmd=masterserver
HelpOneLiner="Entretenir la liste ma�tresse des serveurs."
HelpUsage=masterserver [-option...] [parm=value]
HelpParm[0]=ConfigFile
HelpDesc[0]="Fichier de configuration � employer. Par d�faut : MasterServer.ini"

[CompressCommandlet]
HelpCmd=compress
HelpOneLiner="Compresser un ensemble Unreal pour t�l�chargement auto. Un fichier d'extension .uz sera cr��."
HelpUsage=compress File1 [File2 [File3 ...]]
HelpParm[0]=Files
HelpDesc[0]="Le wildcard ou les noms de fichier � compresser."

[DecompressCommandlet]
HelpCmd=decompress
HelpOneLiner="D�compresser un fichier compress� avec ucc."
HelpUsage=decompress CompressedFile
HelpParm[0]=CompressedFile
HelpDesc[0]="Fichier .uz � d�compresser."

[TcpNetDriver]
ClassCaption="TCP/IP Network Play"

[UdpBeacon]
ClassCaption="Balise de serveur LAN"

