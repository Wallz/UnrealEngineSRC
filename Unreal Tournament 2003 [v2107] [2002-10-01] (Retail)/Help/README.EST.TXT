========================================================================
                Unreal Tournament 2003 DEMO "README" notes.
========================================================================

Last Update	: 13-SEP-2002

Desarrollado por	: Digital Extremes & Epic Games
Distribuido por         : Atari


  1  Requerimientos del sistema
     
     1.1  Requerimientos mínimos del sistema
     1.2  Requerimientos recomendados del sistema


  2  Comportamiento
     
     2.1  Resolución
     2.2  Velocidad de CPU
     2.3  Memoria
     2.4  ¿Estás pensando en actualizar?
     2.5  Conseguir un mejor comportamiento


  3  Soluciones
   
     3.1  Colgamientos
     3.2  Cuestiones sobre el sonido
     3.3  Cuestiones sobre la red
     3.4  Cuestiones sobre el control
     3.5  Cuestiones sobre  D3D 
     3.6  Controladores NVIDIA 40.41 
     3.7  Cuestiones sobre GeForce 4 MX/Go 
     3.8  Cuestiones sobre Radeon 7x00 
     3.9  Cuestiones sobre TNT2/ Kyro II/ Voodoo 3 
     3.10 Cuestiones sobre OpenGL 
     3.11 DirectX 8.1b
     3.12 El juego comienza en modo ventana
 

  4  Controles
     
     4.1  Teclado
     4.2  Ratón
     4.3  Menú de habla


  5  Servidores dedicados

     5.1  Explicación
     5.2  Lanzamiento
     5.3  Varios servidores por cada ordenador
     5.4  Líneas generales de comportamiento
     5.5  Más información
  

  6  Pruebas de referencia

     6.1 Resumen
     6.2 Cuestiones posibles
     6.3 Más información


  7  Estadísticas de UT2003 


  8  Enlaces web de interés 


  9  Asistencia técnica

    
  10 Copyright 


========================================================================
 1 Requerimientos del sistema
========================================================================


1.1 Requerimientos mínimos del sistema
--------------------------------------


Sistema operativo:   Windows 98/Me/2000/XP (Linux en descarga separada)

Procesador:          Procesador Pentium III o AMD Athlon 733 MHz 

Memoria:             128 MBytes RAM

Esp.en disco duro:   300 MBytes libres

Vídeo:               16 MB de clase TNT2 u otra versión 6 de DirectX 
                     tarjeta de vídeo compatible* 

Sonido:              Tarjeta de sonido compatible con Windows. NVIDIA nForce u 
                     otra motherboard/tarjeta de sonido que contenga el codificador
                     de contenido interactivo de Dolby requerido para el sistema
                     de audio Dolby.*

DirectX:             DirectX versión 8.1b (en CD)
                                      
Módem:               33.6K baudios (para uso módem/Internet)


Soporte de Internet (TCP/IP) y LAN (TCP/IP).

 

1.2 Requerimientos recomendados del sistema
-------------------------------------------

Sistema operativo:   Windows 98/Me/2000/XP (Linux en descarga separada)

Procesador:          Procesador Pentium III o AMD Athlon 733 MHz 

Memoria:             256 MBytes RAM

Esp. en disco duro:  3 GBytes de espacio libre

Vídeo:               NVIDIA GeForce2 o ATI Radeon (32-128 MB VRAM 
                     recomendado)*

Sonido:              Tarjetas de sonido de la serie Sound Blaster Audigy*

DirectX:             DirectX versión 8.1b (en CD)
                                       
Módem:               56K baudios (para uso módem/Internet)

Soporte de Internet (TCP/IP) y LAN (TCP/IP).



* Indica que el producto debería ser compatible con DirectX. versión 8.1 o
superior.




========================================================================
 2 Comportamiento

========================================================================


2.1 Resolución
--------------

La resolución con la que lances el juego será la determinante en el rendimiento 
del mismo si no tienes la suerte de contar con una tarjeta gráfica de última 
generación. Lanzar el juego a 640x480 en modo 16 bit o incluso a 512x384 
en modo 16 bit debería producir un comportamiento aceptable incluso en hardware más antiguo.



2.2 Velocidad de CPU 
--------------------

El juego también varía mucho dependiendo de la velocidad de la CPU, el ancho
de banda de la memoria, y del caché. Por lo tanto, funcionará mucho mejor con
procesadores punteros como AMD o Intels. Ver sección 1.2 para ver líneas
generales sobre algunas recomendaciones.


2.3 Memoria
-----------

El comportamiento de Unreal Tournament 2003 depende en gran medida de la
cantidad de memoria RAM que tengas en tu máquina, y de la cantidad de
memoria que tengas disponible. Ordenadores con menos memoria tendrán que 
acceder al disco duro más frecuentemente para cargar datos, causando pausas
esporádicas durante el juego. Así que, si tienes un ordenador de 128 MBytes
(o menos), asegúrate de que no tienes otros programas cargados en memoria 
mientras juegas a Unreal Tournament 2003.

Cómo se comportará el juego en diferentes condiciones de RAM:

  * 64 MBytes o menos: intercambios (swappings)

  * 128 MBytes: pocos intercambios con ajustes predeterminados

  * 256 MBytes: podría producir intercambios en raras ocasiones con un nivel de 
    detalle de textura alto

  * 512 MBytes o más: no debería haber intercambios



2.4 ¿Estás pensando en actualizar?
----------------------------------

Para aquellos que estén pensando actualizar sus ordenadores, aquí tienen 
unos trucos basados en nuestra experiencia lanzando Unreal Tournament 2003 
en diferentes ordenadores:

  1 La mayor mejora en Unreal Tournament 2003 viene con un buen chip
    gráfico.

  2 La siguiente actualización que tiende a mejorar las prestaciones de Unreal 
    Tournament 2003 es la actualización de tu CPU.

  3 Finalmente, un montón de RAM ayuda... Con los precios de la memoria cayendo,
    es relativamente barato hacerse con un sistema de memoria de 256 MBytes.



2.5 Conseguir un mejor rendimiento
----------------------------------

Por defecto Unreal Tournament 2003 toma valores predeterminados de hardware
la primera vez que lanzas el juego, lo cual desemboca en un comportamiento decente
aunque con espacio de sobra para mejorar.

Los niveles de resolución y de detalle de texturas tienen un impacto directo en 
el comportamiento del juego si tienes una CPU rápida, así que afina los ajustes
para conseguir un equilibrio entre el nivel de calidad visual y el comportamiento
que deseas.

Si dispones de una tarjeta gráfica rápida pero tienes una CPU lenta puedes
disminuir el detalle de mundos en el menú de ajustes de detalle.

Por último, si tienes tanto una CPU rápida y una tarjeta gráfica rápida no hace
falta que leas esto :)




========================================================================
 3 Soluciones
========================================================================


3.1 Colgamientos
----------------

Si Unreal Tournament 2003 se detiene y muestra un mensaje de "Memoria virtual escasa",
necesitas liberar espacio en el disco duro en tu unidad principal (C:) para poder
jugar. Los mapas de gran tamaño y el alto nivel de texturas de Unreal Tournament 2003 
ocupan mucha memoria virtual. 

En tarjetas sin hardware transform & lighting  acelerado, el juego se colgará informando 
de un problema en "DrawIndexedPrimitive" con DirectX 8.1. Microsoft ha informado de 
este problema con DirectX 8.1b. Por favor actualiza tu versión de DirectX - ver 
sección 3.10 para las instrucciones de cómo hacer esto.



3.2 Cuestiones sobre el sonido
------------------------------

Si utilizas un acelerador de sonido 3D como las tarjetas de sonido de la serie
Sound Blaster Audigy, puedes ir a "Ajustes/Audio" para activar EAX 3.0 seleccionando
las opciones de "Hardware de sonido 3D + EAX".

Necesitas actualizar la última versión de los controladores Sound Blaster Audigy  
para conseguir un rendimiento de sonido 3D aceptable.

El uso de Unreal Tournament 2003 junto con versiones antiguas de los controladores  
PUEDE causar severos problemas de comportamiento (grandes ralentizaciones del 
orden del 30-50% de sonido durante el juego) en cuyo caso deberías cambiar la opción 
a la predeterminada "Software 3D Audio".

Si tu ordenador está conectado a un sistema 5.1 Speaker, deberías ir a "Ajustes" y 
activar "Hardware de audio 3D" para aprovecharte de la panoramización (panning) 
de sonido en 360-grados.



3.3 Cuestiones sobre la red
---------------------------

La conexión de velocidad mínima para un buen resultado jugando en Internet es
de un módem de 33.3K a tu proveedor de servicios de Internet.

Cuestiones ya conocidas sobre el juego en red:

  * Cuando un jugador nuevo entra en un juego de red, los clientes pueden experimentar 
    una pausa de 1/4-segundos mientras que la malla, la piel y otros datos son cargados
    para este jugador. Esto es normal, y está hecho a propósito.

  * El comportamiento de Unreal Tournament 2003 en Internet depende altamente del 
    bando de ancha de tu conexión, de la latencia (tiempo de ping), y de la pérdida de 
    paquetes. El juego está diseñado para ser jugable en un tiempo de ping de 300 mseg, 
    con un 5% de pérdida de paquetes, y a una velocidad de conexión de 33.3K.
    El rendimiento baja gravemente con una peor latencia, pérdida de paquetes y una peor
    conexión.

  * Si tienes activado "Internet Connection Firewall" de Windows XP,
    no podrás ver los juegos LAN ya comenzados.  Para desactivarlo,
    ve al menú de Inicio de Windows XP / Conexiones de red / Conexión
    de área local / Propiedades/ Avanzadas y vacía el campo
    "Internet Connection Firewall".


3.4 Cuestiones sobre el control
-------------------------------

Algunos teclados de PC no pueden reconocer ciertas combinaciones de 3 o más teclas
presionadas simultáneamente.



3.5 Cuestiones de D3D
---------------------

Por favor asegúrate de que estás utilizando los últimos controladores para tu
tarjeta gráfica puesto que Unreal Tournament exprime el hardware y los
controladores más que la mayoría de los juegos disponibles y confiamos en
una serie de fabricantes de hadware con correcciones incorporadas en sus
últimos controladores. Para conseguir los últimos controladores visita
el sitio web del fabricante de tu tarjeta gráfica.  



3.6 Controladores NVIDIA 40.41 
------------------------

Se sabe que los controladores NVIDIA 40.41 tienen defectos visuales y problemas
de rendimiento con la demo de Unreal Tournament 2003 que no están presentes 
en los controladores 30.82. NVIDIA ha informado de este problema y controladores
NVIDIA lanzados en el futuro contendrán las respectivas correcciones. 

Si vienes utilizando controladores 40.41 por favor, o bien rebájalos
hasta 30.82 o instala nuevos controladores.

Los últimos controladores NVIDIA pueden encontrarse en la siguiente dirección:

http://www.nvidia.com/content/drivers/drivers.asp



3.7 Cuestiones sobre GeForce 4 MX/Go 
------------------------------------

Con tarjetas GF4 MX/ Go puede que encuentres gráficos corruptos en modo
32 bit. NVIDIA ha informado del problema y controladores NVIDIA lanzados 
en el futuro contendrán las respectivas correcciones. Una medida temporal 
es jugar con un modo 16 bit.



3.8 Cuestiones sobre Radeon 7x00
--------------------------------

Con los controladores 7.74 las tarjetas Radeon 7x00 muestran un par de 
problemas visuales como las pancartas colgantes en BR-Anubis, la introducción 
de NVIDIA o la retícula de cierre del lanzabolas, mostrando un movimiento 
de rotación incorrecto.

Recomendamos el uso de los controladores 7.76 (Catalyst 2.3) ya que
contienen las correciones pertinentes para lanzar Unreal Tournament 2003.


3.9  Cuestiones sobre TNT2/ Kyro II/ Voodoo 3 
---------------------------------------------

Debido a la falta de ciertas funcionalidades como la compresión de textura 
y el mapa de cubos, las tarjetas TNT2, Kyro II, Voodoo 3 y G400 no serán 
capaces de lanzar el juego con una calidad visual plena. Como las texturas 
no comprimidas son mucho más grandes que las comprimidas, debemos bajar 
el nivel de tamaño y profundida de color de las texturas para poder 
adaptarlas a la memoria de vídeo. Eso significa que notarás objetos 
ondulantes - especialmente en los menús. 

La falta de soporte para mapa de cubos significa que los reflejos no se 
verán bien, aunque durante el juego probablemente apenas tengas tiempo 
de apreciar la diferencia.



3.10 Cuestiones sobre OpenGL 
----------------------------

El renderer OpenGL no es soportado oficialmente por Windows, pero podría ser
una buena solución en ciertas combinaciones de hardware/ controlador, puesto que
podría crear menos errores en los controladores. A menos que experimentes problemas
visuales serios, no hay razón para que cambies a PenGL, en cualquier modo. 
En contraste con Unreal Tournament, Unreal Tournament 2003 ha sido diseñado 
en torno a D3D y ofrece el mejor comportamiento y la mayor calidad visual con 
el renderer D3D.

Además debes tener en cuenta que el renderer OpenGL está todavía por desarrollar y 
tiene unos requisitos de sistema mayores que los del D3D. Por ejemplo, depende de 
la presencia de compresión de textura que rige, por ejemplo, las tarjetas TNT2.

Se sabe que OpenGL no trabaja con controladores previos a 7.76 ATI.



3.11 Cómo obtener DirectX 8.1b
------------------------------

DirectX 8.1b contiene correcciones en el tiempo de lanzamiento de 
DirectX que son requeridos por el juego para ser lanzado con tarjetas sin 
hardware transform & lighting. E.g. TNT2, Kyro I/II, Voodoo 3, Radeon 7000, ... 
DirectX 8.1b está incluido en el CD y se instalará automáticamente durante
el proceso de instalación del juego.

En cualquier caso aquí tienes información sobre cómo conseguirlo.


Si tienes instalado Windows 98/ME/2000 puedes descargar DirectX 8.1b 
usando el siguiente enlace:

	http://www.microsoft.com/windows/directx/downloads/drx81.asp


Para Windows XP tienes que instalar QFE 321178 para actualizar tu versión de
DirectX to 8.1b:

	http://support.microsoft.com/default.aspx?scid=kb;en-us;Q321178


Nótese que Windows XP Service Pack 1 ya lo posee.

	http://www.microsoft.com/WindowsXP/pro/downloads/servicepacks/sp1/default.asp



3.12 El juego comienza a modo ventana
-------------------------------------

Si el juego no consigue obtener una visión del teclado al lanzarse se iniciará en
modo ventana. En ese caso puedes hacer clic en la ventana y presionar ALT-TAB
para cambiar a modo pantalla-completa.




========================================================================
 4 Controles
========================================================================


4.1 Teclado
------------

Puedes asignar nuevas teclas a los controles haciendo clic en la opción "Controles" del 
menú de Preferencias dentro de Unreal Tournament 2003 . Hay un montón de controles 
por asignar.


4.2 Ratón
---------

Aunque puedes depender del teclado para moverte e integrarte en el universo en 3D de 
Unreal Tournament 2003, el uso simultáneo de ratón y teclado te ofrece una mayor fluidez y
un gran control de respuesta.

Cuando utilizas el ratón para tus movimientos de rotación y para apuntar, ganas un grado 
de precisión y rapidez que jugadores de sólo-teclado no pueden alcanzar. El teclado se 
usa mejor para movimientos laterales, adelante/atrás, y para saltar.

Para dominar los controles predeterminados de Unreal Tournament 2003, mantén tu mano
izquierda en el teclado, utilizando las teclas de dirección para el movimiento, las 
teclas 0-9 para la selección del arma, y la barra espaciadora para el salto. Tu mano 
derecha opera con el ratón, controlando la rotación, el apuntado y el disparo. Claro 
que, puedes personalizar estos controles a través del menú de Ajustes.

4.3 Menú de habla
-----------------

Presiona la tecla 'V' para abrir el menú de habla. Puedes seleccionar varias 
órdenes o burlas. Bajo el submenú de Órdenes puedes escoger una tarea que asignar 
a un compañero de equipo. Si tu compañero es un bot, obedecerá automáticamente 
tus órdenes. Las órdenes disponibles son:

  Defender la base.
        El bot acudirá de inmediato a vuestra base para protegerla de un posible 
        ataque enemigo.

  Cúbreme.
        El bot te cubrirá y te seguirá ante los ataques que te encuentres.

  Asaltar la base.
        Le ordenas al bot que sea ofensivo. En CLB esta orden es remplazada por 
        "Capturar la bandera enemiga."

  Mantener la posición.
        El bot encontrará tu posición e intentará protegerla de cualquier 
        ataque enemigo.

  Mercenario.
        Libera al bot the órdenes anteriores. Un bot mercenario tomará sus 
        propias decisiones en combate.

Si das una orden a "Todos" cada uno de los bots intentarán cumplir esa orden. 
Si miras a uno de los bots el menú de habla mostrará la opción "Ordenar a este bot".



========================================================================
 5 Servidores de red dedicados
========================================================================


5.1 Explicación
---------------

Para un comportamiento de red óptimo, puedes lanzar una copia dedicada del 
servidor de Unreal Tournament 2003 en un ordenador. Esto mejora el comportamiento
comparado con el uso de un servidor no dedicado pero, claro está, esto 
atará a un PC.

Cualquiera puede lanzar libremente servidores dedicados en la red; no 
necesitas ningún permiso o rellenar un formulario.  



5.2 Lanzamiento
---------------

Puedes lanzar un servidor dedicado a través del menú de Unreal Tournament 2003, 
seleccionar "Dirigir juego Multijugador | Mapa y modo, seleccionar las opciones
apropiadas en la sección 'Servidor', y asegurarse de escoger 'Servidor dedicado'. 
Esto es lo que deberás hacer para partidas rápidas en LAN donde tienes un ordenador
extra que puede actuar como servidor dedicado.

Además, puedes lanzar tu servidor dedicado desde la línea de comandos lanzando
UnrealTournament2003.exe directamente (que normalmente se encuentra en el directorio
c:\UnrealTournament2003\System, o en el subdirectorio System del directorio donde
hayas instalado el juego).  Por ejemplo, para lanzar el nivel "DM-Antalus", lanza:

   ucc server DM-Antalus.ut2



5.3 Varios servidores por cada ordenador
----------------------------------------

Cada copia del servidor dedicado sólo puede lanzar un nivel de una vez.

Sin embargo, puedes lanzar servidores de varios niveles en un ordenador. 
Para hacer esto, debes dar a cada servidor un único número de puerto TCP/IP. 
El número de puerto predeterminado es 7777. Para especificar un puerto, 
utiliza el siguiente comando:

   ucc server DM-Antalus.ut2 port=7778



5.4 Líneas generales de rendimiento
-----------------------------------

Una CPU de 600 MHz puede manejar unos 16 jugadores con un rendimiento 
decente. El rendimiento varía con el nivel de complejidad y con otros 
factores de velocidad del equipo, así que todo depende. El rendimiento
disminuye cuanto mayor es el número de jugadores.

Si estás lanzando varios servidores a la vez, Windows XP/2k/NT
actúa como Windows 9x debido a su 'multitasking' superior y a su capacidad
de procesamiento TCP/IP.

Para un mejor rendimiento, recomendamos tener 128 MBytes de memoria por
servidor. Por ejemplo, para lanzar dos servidores simultáneamente tu equipo
debería tener al menos 256 MBytes de memoria.

El servidor de Unreal Tournament 2003 utiliza hasta 33.38 bits por 
segundo de ancho de banda por jugador (en Internet), así que si lanzas
el servidor a un equipo conectado a un módem de 28.8K, sólo podrás soportar
un cliente con un rendimiento decente. Servidores dedicados que soportan 
a muchos jugadores requieren por lo general un ancho de banda de salida
de una línea T1 o superior.


5.5 Más información
-------------------

Visita la Página de tecnología Unreal para más información sobre el 
lanzamiento de servidores Unreal Tournament 2003. Algunos temas que puedes 
encontrar aquí son:

  * Administración de servidor remoto

  * Versión Linux del servidor (descarga separada incluyendo el Léeme especial 
    para Linux)

  * Descarga de servidores únicamente

  * Servicio de niveles y de modos de juego

  * Administración de servidores públicos que aparecen en la lista de servidores



========================================================================
 6 Pruebas de referencia
========================================================================

6.1 Resumen
-----------

La Demo de Unreal Tournament 2003 viene con una gran variedad de pruebas
de referencia de funcionalidad. La manera más fácil de utilizarlas consiste
en lanzar Benchmark.exe del directorio System de tu directorio de instalación
de Unreal Tournament 2003. Aparecerá una ventana de diálogo pidiéndote
que escojas una resolución, y cuando presiones 'ok' lanzará 4 tests consecutivos
(dos vistas preliminares de niveles y dos combates de bots). Es importante que
no toques el ratón hasta que aparezca la última ventana de diálogo con los
resultados en pantalla. Ésta mostrará el nivel medio de cuadros/segundo de
los 4 tests.



6.2 Cuestiones posibles
-----------------------

La prueba de referencia intentará siempre lanzar el juego al nivel más alto
de detalles, así que los resultados no podrán compararse directamente con
tarjetas en las que el juego no puede renderizarse al máximo nivel de
detalle (e.g. tarjetas TNT2 y Kyro I/II).

Debido al modo de funcionamiento de la prueba de referencia, tendrás que
marcar manualmente los siguientes archivos como 'sólo lectura' antes de lanzar 
Benchmark.exe en dieferentes tarjetas.

INSTALL_DIR\Benchmark\Stuff\MaxDetail.ini
INSTALL_DIR\Benchmark\Stuff\MaxDetailUser.ini

Para la prueba de los partidos de bots, el Benchmark.exe lanza un juego
con sólo bots combatiendo y con el jugador en modo espectador. En caso de
que el equipo no pueda lanzarse al máximo nivel de detalle, el resultado del
encuentro podría diferir, llevándonos a un encuentro diferente y por consiguiente,
mostrando números diferentes otros resultados. Esto también podría ocurrir
al comparar dos tipos diferentes de CPUs, así que o bien presta atención a los
encuentros durante la prueba para asegurarse de que son los mismos de siempre, o
compara el número entre corchetes en benchmark.log. Si el número es el mismo, se 
renderizó la misma secuencia.



6.3 Más información
-------------------

En el directorio Benchmark encontrarás un par de archivos por lotes
que pueden ser usados para lanzar tests individuales y para crear
un archivo por lotes grande para automatizar todo el proceso de
lanzamiento en múltiples resoluciones.

El juego registrará un montón de información y estadísticas mientras
esté en modo benchmark y los resultados pueden verse en los subdirectorios
de la carpeta Benchmark. Uno de los más interesantes es la carpeta CSV, la
cual contiene una miríada de estad. que el motor recoge según las frames
en un archivo en forma de "variables separadas por coma", el cual, 
por ejemplo, puede ser abierto con Microsoft Excel.



========================================================================
 7 Estadísticas UT2003 
========================================================================

7.1 Genreal
-----------

Unreal Tournament 2003 ha sido diseñado con la inclusión de un servicio
disponible de estad. UT2003. Estad. UT2003 te permite registrar y mostrar
tus puntuaciones y clasificaciones y ofrece un análisis estadístico de
juego para tus juegos en Internet.

Para una visión en detalle de tus estadísticas visita estad. 2003 en:

	http://ut2003stats.epicgames.com


7.2 Activar *tu* registro de estadísticas
-----------------------------------------

Entra en el menú dentro del juego Ajustes > Red > Estad. UT2003. Activa el
regitro de tus estad. UT2003 haciendo clic en el botón de 'Registrar estad.',
y escoge un único nombre de usuario y una contraseña de estad. De este modo
asignarás un único PlayerID para ti.

Nota: Tus estad. UT2003 *sólo* estarán seguras como lo sea tu contraseña.


========================================================================
 8 Enlaces web de interés
========================================================================


Visita esta página para actualizaciones, parches y eventos de la comunidad:

	http://www.unrealtournament2003.com


Información sobre otros juegos de Digital Extremes:

	http://www.digitalextremes.com


Información sobre otros juegos de Epic:

	http://www.epicgames.com


Últimas noticias sobre el equipo de desarrollo de Epic. Fantásticos recursos
para programadores, autores de mods, y fans en general:

	http://unreal.epicgames.com


Encuentra más información sobre Productos Atari:

	http://www.atari.com


========================================================================
 9 Asistencia técnica
========================================================================

Consultar el manual.


========================================================================
 10 Copyright
========================================================================

Ogg Vorbis Copyright © 2001, Xiphophorus

MathEngine and Karma and the MathEngine and Karma logos are 
registered trade marks or trade marks of MathEngine PLC, used under 
license.

Unreal® Tournament 2003 ©2002 Epic Games, Inc. Raleigh, N.C. USA. 

Unreal and the Unreal logo are registered trademarks of Epic 
Games, Inc. ALL RIGHTS RESERVED. 

All other trademarks and trade names are the property of their 
respective owners. 

Unreal Tournament 2003 was created by Digital Extremes and Epic Games,
Inc.. 

Manufactured and marketed by Infogrames, Inc., New York, New York, 
a subsidiary of Infogrames Entertainment, S.A., under license from 
Epic Games, Inc.