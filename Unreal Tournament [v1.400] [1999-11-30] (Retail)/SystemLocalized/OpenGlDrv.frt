[Public]
Object=(Name=OpenGLDrv.OpenGLRenderDevice,Class=Class,MetaClass=Engine.RenderDevice,Autodetect=)
Preferences=(Caption="Rendu",Parent="Options avanc�es")
Preferences=(Caption="Support OpenGL",Parent="Rendu",Class=OpenGLDrv.OpenGLRenderDevice,Immediate=True)

[OpenGLRenderDevice]
ClassCaption="Support OpenGL Tim"
AskInstalled=Est-ce qu'une carte acc�l�ratrice 3D compatible OpenGL est install�e sur votre syst�me ? 
AskUse=Souhaitez-vous qu'Unreal utilise votre carte acc�l�ratrice OpenGL ?

[Errors]
NoFindGL=Pilote %s OpenGL introuvable 
MissingFunc=Fonction %s (%i) OpenGL manquante
ResFailed=Echec du r�glage de la r�solution
