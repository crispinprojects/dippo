#  Dippo Diphone Speech Synthesizer

Dippo is a small C diphone speech synthesizer for Linux. It maps words to sequences of recorded diphones, joins the corresponding WAV audio, and writes the result as a WAV file.

## How it works

- `dictionary.c` maps words to diphone sequences.
- `diphone.c` provides the embedded diphone audio and lookup function.
- `wavcat.c` reads and joins WAV data, then writes the output audio.
- `main.c` connects the components.

The WAV helpers support mono, 16-bit PCM audio. The example uses a 16 kHz sample rate.

## Build From Source

The C source code for the diphone speech synthesizer for reading out dates is provided in the src directory and has been developed using Debian Trixie.

### Building on Debian Trixie

To build the diphone speech synthesizer from source you need the gcc compiler and the Make build system. You need to install the following package.

```
sudo apt install build-essential
```

The package:

```
sudo apt install alsa-utils
```

is needed but should be installed by default. 

Use the MAKEFILE to compile the code. Just run "make" inside the source code folder.

```
make
```

To run the diphone speech synthesizer called  dippo from the terminal use

```
./dippo
```
This will read out the current date.

## Current status

The example `main.c` speaks the current date and plays the generated `out.wav` using `aplay`. 

The long-term project roadmap is to improve dippo's voice clarity and to integrate as a lightweight, flexible date-and-time announcer subsystem for the [**Talk Calendar**](https://github.com) project.


## Project files

```
dictionary.h / dictionary.c   Word-to-diphone dictionary
diphone.h / diphone.c         Embedded diphone audio and lookup
wavcat.h / wavcat.c           WAV reading, joining, and writing
main.c                        Example program
Makefile                      Build configuration
```

## License

Dippo licensed under **GPL-3.0**.

## Author

* **Alan Crispin** [Github](https://github.com/crispinprojects)


## Project Status

Active.

## Acknowledgements

* [Diphone Source and License](https://github.com/hypnaceae/DiphoneSynth/blob/master/diphones_license.txt)

* Diphone collection and synthesis Alan W. Black and Kevin Lenzo [2000](https://www.cs.cmu.edu/~awb/papers/ICSLP2000_diphone.pdf)

* [Speech Synthesis](https://www.youtube.com/watch?v=eDjtEsOvouM&t=1459s) CMU Low resource NLP Bootcamp 2020  *Author:* Professor Alan W Black 

* [Geany](https://www.geany.org/) can be used as a source code editor. Geany is lightweight and has an integrated terminal for building the application. To install the Geany IDE use the command below.

* [Talk Calendar](https://github.com/crispinprojects/talkcalendar)



