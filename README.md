PAV - P5: síntesis musical polifónica
=====================================

Obtenga su copia del repositorio de la práctica accediendo a [Práctica 5](https://github.com/albino-pav/P5) 
y pulsando sobre el botón `Fork` situado en la esquina superior derecha. A continuación, siga las
instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para crear una rama con el apellido de
los integrantes del grupo de prácticas, dar de alta al resto de integrantes como colaboradores del proyecto
y crear la copias locales del repositorio.

Como entrega deberá realizar un *pull request* con el contenido de su copia del repositorio. Recuerde que
los ficheros entregados deberán estar en condiciones de ser ejecutados con sólo ejecutar:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.sh
  make release
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A modo de memoria de la práctica, complete, en este mismo documento y usando el formato *markdown*, los
ejercicios indicados.

Ejercicios.
-----------

### Envolvente ADSR.

Tomando como modelo un instrumento sencillo (puede usar el InstrumentDumb), genere cuatro instrumentos que
permitan visualizar el funcionamiento de la curva ADSR.

Para este apartado, hemos generado diferentes .wav que siguieran los diferentes instrumentos que se piden. 
Cuando hemos obtenido el .wav en cada caso caso, y junto con la aplicación de Matlab hemos puesto los 
gráficos correspondientes con una leyenda dónde se ve los diferentes valores de ADSR.

* Un instrumento con una envolvente ADSR genérica, para el que se aprecie con claridad cada uno de sus
  parámetros: ataque (A), caída (D), mantenimiento (S) y liberación (R).
  
  <img src="Pics/Pic1.png" witdth="500" align="center">
  
* Un instrumento *percusivo*, como una guitarra o un piano, en el que el sonido tenga un ataque rápido, no
  haya mantenimiemto y el sonido se apague lentamente.
  - Para un instrumento de este tipo, tenemos dos situaciones posibles:
    * El intérprete mantiene la nota *pulsada* hasta su completa extinción.
    
    <img src="Pics/Pic2.png" witdth="500" align="center">
    
    * El intérprete da por finalizada la nota antes de su completa extinción, iniciándose una disminución
	  abrupta del sonido hasta su finalización.
	  
    <img src="Pics/Pic3.png" witdth="500" align="center">
    
  - Debera representar en esta memoria **ambos** posibles finales de la nota.
* Un instrumento *plano*, como los de cuerdas frotadas (violines y semejantes) o algunos de viento. En
  ellos, el ataque es relativamente rápido hasta alcanzar el nivel de mantenimiento (sin sobrecarga), y la
  liberación también es bastante rápida.
  
  <img src="Pics/Pic4.2.png" witdth="500" align="center">

Para los cuatro casos, deberá incluir una gráfica en la que se visualice claramente la curva ADSR. Deberá
añadir la información necesaria para su correcta interpretación, aunque esa información puede reducirse a
colocar etiquetas y títulos adecuados en la propia gráfica (se valorará positivamente esta alternativa).

### Instrumentos Dumb y Seno.

Implemente el instrumento `Seno` tomando como modelo el `InstrumentDumb`. La señal **deberá** formarse
mediante búsqueda de los valores en una tabla.

- Incluya, a continuación, el código del fichero `seno.cpp` con los métodos de la clase Seno.

seno.h
```
#ifndef INSTRUMENT_SENO
#define INSTRUMENT_SENO

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentDumb: public upc::Instrument {
    EnvelopeADSR adsr;
    unsigned int index, prev, back;
	  float f0, theta, nota, step;
    float A;
    std::vector<float> tbl;
  public:
    seno(const std::string &param = "");
    void command(long cmd, long note, long velocity=1);
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;}
  };
}

#endif
```

seno.cpp
```
#include <iostream>
#include <math.h>
#include "seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

seno::seno(const std::string &param)
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h
  */
  KeyValue kv(param);
  int N;

  if (!kv.to_int("N",N))
    N = 40; //default value

  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
}


void seno::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    f0 = pow(2, ((note-69.0)/12.0)) * 440.0;
    nota = f0/SamplingRate;
    step = 2 * M_PI * nota;
	  A = vel / 127.;
    index = 0;
    prev = 0;
    back = 0;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & seno::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    //x[i] = A * tbl[index];
    x[i] = A * 0.5(tbl[prev] + tbl[back])

    prev = index;
    back = round(theta + nota*tbl.size());
    index = round(theta);

    theta = theta + nota*tbl.size();

    while (back >= tbl.size()){
      back = 0;
    }
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
```
- Explique qué método se ha seguido para asignar un valor a la señal a partir de los contenidos en la tabla,
  e incluya una gráfica en la que se vean claramente (use pelotitas en lugar de líneas) los valores de la
  tabla y los de la señal generada.
  
  Hemos calculado el valor f0 y el de la nota (valor discretizado de f0) a tarvés de la fórmula mediante la cual se calcula la nota N:
  
  <img src="Pics/Pic5.PNG">
  
  En la parte correpondiente al synthesize, hemos generado cada valor de x[i] a través de la amplitud multiplicada por el valor de la tabla marcado por el índice. Una vez añadida la ampliación (interpolación de los valores anterior (prev) y posterior (back)), cada valor de x[i] se ha calculado como la amplitud multiplicada por la media de el valor anterior y posterior. Seguidamente se han actualizado dichos valores. Finalmente se ha reseteado el valor de la muestra posterior a cero cuando este valor había superado el incremento (el valor de la nota discretizado por el tamaño de la tabla).
  
  A continuación añadimos una imagen del seno:
  
  <img src="Pics/Pic5.1.PNG" witdth="500" align="center">
  
  Y ahora un imagen con los valores que usamos en la tabla:
  
  <img src="PIC12.png" witdth="500" align="center">
  
- Si ha implementado la síntesis por tabla almacenada en fichero externo, incluya a continuación el código
  del método `command()`.

### Efectos sonoros.

- Incluya dos gráficas en las que se vean, claramente, el efecto del trémolo y el vibrato sobre una señal
  sinusoidal. Deberá explicar detalladamente cómo se manifiestan los parámetros del efecto (frecuencia e
  índice de modulación) en la señal generada (se valorará que la explicación esté contenida en las propias
  gráficas, sin necesidad de *literatura*).
  
  En primera instancia incluimos una imagen de las cuatro primeras notas cuando no hay efectos aplicados.
  
  <img src="Pics/Pic7.0.PNG" witdth="500" align="center">
  
  Tremolo:
  <img src="Pics/Pic7.1.PNG" witdth="500" align="center">
  
  En esta imagen tenemos el efecto de Tremolo que se ha implementado a partir de la segunda nota, como se puede
  ver en la imagen hay una clara diferencia en cuanto a la forma, además cuando se escucha por wavesurfer 
  los cambios también son notables.
  
  Vibrato:
  <img src="Pics/Pic8.0.PNG" witdth="500" align="center">
  
  Seguidamente, tenemos el efecto de Vibrato en el cual tambien podemos observar diferencia entre las dos primeras notas aunque no es una diferencia tan notable como en el efecto de tremolo.
  
- Si ha generado algún efecto por su cuenta, explique en qué consiste, cómo lo ha implementado y qué
  resultado ha producido. Incluya, en el directorio `work/ejemplos`, los ficheros necesarios para apreciar
  el efecto, e indique, a continuación, la orden necesaria para generar los ficheros de audio usando el
  programa `synth`.

### Síntesis FM.

Construya un instrumento de síntesis FM, según las explicaciones contenidas en el enunciado y el artículo
de [John M. Chowning](https://ccrma.stanford.edu/sites/default/files/user/jc/fm_synthesispaper-2.pdf). El
instrumento usará como parámetros **básicos** los números `N1` y `N2`, y el índice de modulación `I`, que
deberá venir expresado en semitonos.

- Use el instrumento para generar un vibrato de *parámetros razonables* e incluya una gráfica en la que se
  vea, claramente, la correspondencia entre los valores `N1`, `N2` e `I` con la señal obtenida.
  
  A continuació mostrem diferents gràfiques obtingudes a partir de a modificació dels diferents paràmetres:
  
  I = 2, N1 = 3, N2 = 2:
  <img src="Pics/FM1.PNG" witdth="500" align="center">
  
  I = 2, N1 = 15, N2 = 2:
  <img src="Pics/FM2.PNG" witdth="500" align="center">
  
  I = 2, N1 = 3, N2 = 10:
  <img src="Pics/FM3.PNG" witdth="500" align="center">
  
  I = 10, N1 = 3, N2 = 2:
  <img src="Pics/FM4.PNG" witdth="500" align="center">
  
  I = 10, N1 = 15, N2 = 2:
  <img src="Pics/FM5.PNG" witdth="500" align="center">
  
  I = 20, N1 = 15, N2 = 2:
  <img src="Pics/FM6.PNG" witdth="500" align="center">
  
- Use el instrumento para generar un sonido tipo clarinete y otro tipo campana. Tome los parámetros del
  sonido (N1, N2 e I) y de la envolvente ADSR del citado artículo. Con estos sonidos, genere sendas escalas
  diatónicas (fichero `doremi.sco`) y ponga el resultado en los ficheros `work/doremi/clarinete.wav` y
  `work/doremi/campana.work`.
  
  La forma de la señal para cada uno de los instrumentos es la siguiente:
  
  Clarinete (ADSR_A=0.1; ADSR_D=0; ADSR_S=1; ADSR_R=0.05; I=4; N1 = 3; N2= 2):
  <img src="Pics/Pic10.PNG" witdth="500" align="center">
  
  Campana (ADSR_A=0.02; ADSR_D=0.4; ADSR_S=0.0; ADSR_R=0.1; I=10; N1 = 5; N2= 7):
  <img src="Pics/Pic11.PNG" witdth="500" align="center">
  
  * También puede colgar en el directorio work/doremi otras escalas usando sonidos *interesantes*. Por
    ejemplo, violines, pianos, percusiones, espadas láser de la
	[Guerra de las Galaxias](https://www.starwars.com/), etc.

### Orquestación usando el programa synth.

Use el programa `synth` para generar canciones a partir de su partitura MIDI. Como mínimo, deberá incluir la
*orquestación* de la canción *You've got a friend in me* (fichero `ToyStory_A_Friend_in_me.sco`) del genial
[Randy Newman](https://open.spotify.com/artist/3HQyFCFFfJO3KKBlUfZsyW/about).

- En este triste arreglo, la pista 1 corresponde al instrumento solista (puede ser un piano, flautas,
  violines, etc.), y la 2 al bajo (bajo eléctrico, contrabajo, tuba, etc.).
- Coloque el resultado, junto con los ficheros necesarios para generarlo, en el directorio `work/music`.
- Indique, a continuación, la orden necesaria para generar la señal (suponiendo que todos los archivos
  necesarios están en directorio indicado).
  
  synth toy.orc ToyStory_A_Friend_in_me.sco toystory.wav

También puede orquestar otros temas más complejos, como la banda sonora de *Hawaii5-0* o el villacinco de
John Lennon *Happy Xmas (War Is Over)* (fichero `The_Christmas_Song_Lennon.sco`), o cualquier otra canción
de su agrado o composición. Se valorará la riqueza instrumental, su modelado y el resultado final.
- Coloque los ficheros generados, junto a sus ficheros `score`, `instruments` y `efffects`, en el directorio
  `work/music`.
- Indique, a continuación, la orden necesaria para generar cada una de las señales usando los distintos
  ficheros.
