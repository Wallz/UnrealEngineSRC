[Public]
Object=(Name=IpDrv.TcpNetDriver,Class=Class,MetaClass=Engine.NetDriver)
Object=(Name=IpDrv.UdpBeacon,Class=Class,MetaClass=Engine.Actor)
Preferences=(Caption="R�seau",Parent="Options avanc�es")
Preferences=(Caption="Partie en r�seau TCP/IP",Parent="R�seau",Class=IpDrv.TcpNetDriver)
Preferences=(Caption="Balise de serveur",Parent="R�seau",Class=IpDrv.UdpBeacon,Immediate=True)

[TcpNetDriver]
ClassCaption="Partie en r�seau TCP/IP"

[UdpBeacon]
ClassCaption="Balise serveur r�seau local"

