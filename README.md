# Dippo: A Diphone Speech Synthesiser

Dippo is a lightweight, concatenative diphone speech synthesiser written in pure standard ISO C for Linux systems with a command-line interface (CLI).

```
./dippo "dippo is not a hippo" -tempo 1.4 -av 1.5  -wav out.wav && aplay out.wav
```
The long-term goal of the project is to serve as a high-clarity, context-aware audio announcement subsystem for the [Talk Calendar](https://github.com/crispinprojects/talkcalendar) project.

## Technical Overview & Core Concepts

Dippo is built completely from scratch using a concatenative diphone layout:

• **Phonemes vs. Diphones:** A phoneme is the smallest unit of sound that distinguishes one word from another (English features 44 baseline phonemes). Phonemes capture "steady-state" sounds. A diphone is a sound unit composed of two adjacent partial phonemes i.e. the second half of the first phoneme and the first half of the second phoneme and so captures the acoustic transition from the middle of one phoneme to the middle of the next. The synthesizer uses a set of pre-recorded diphone sound samples and concatenates diphone audio to produce speech output for a given text input.

• **Coarticulation Handling:** The most complex part of synthetic speech is the boundary transition between sounds. By recording these transitions (diphones) and "gluing" them together at the stable, stationary centres of phonemes, Dippo achieves a much more natural speech flow than basic phoneme playback as used in my [TOM](https://github.com/crispinprojects/tom) project.

• **Phonetic Matrix:** With 44 phonemes, there are theoretically \(44^2 = 1,936\) possible diphone permutations. Because many transitions never occur naturally in English, Dippo utilizes a streamlined collection of approximately 1,600 standard diphones.

• **Syllable-Free Synthesis:** Unlike alternative syllable-based engines (such as the [TOM](https://github.com/crispinprojects/tom) speech processor), Dippo handles phonetic transitions mid-syllable natively through predefined phoneme boundary cuts, eliminating the need for complex runtime syllable segmentation algorithms.

## Project Structure & Architecture

Source Directory Layout

```
dippo/
├── main.c              # Application entry point, CLI flag parsing, and execution loop
├── text_processor.c    # Normalisation, number expansion, and smart punctuation stripping
├── text_processor.h    
├── dictionary.c        # Case-insensitive static dictionary lookup table
├── dictionary.h        
├── g2p_engine.c        # Context-aware Grapheme-to-Phoneme predictive rule tree
├── g2p_engine.h        
├── diphone.c           # Embedded audio waveform data and acoustic indexing arrays
├── diphone.h           
├── synthesizer.c       # Overlap-add audio concatenation and vocoder execution
├── synthesizer.h       
└── Makefile            # Standard POSIX compliance build routines
```

## Synthesis Pipeline Chain

```
[Input Text String] 
       │
       ▼
 [text_processor]  ──► Normalises strings, strips structural punctuation, expands numbers/years
       │
       ▼
 [dictionary]      ──► Performs lookups; falls back to plural truncation checks if missing
       │
  ┌────┴───────────────┐
  │ Match Found        │ Dictionary Miss
  ▼                    ▼
[diphone_str]     [g2p_engine] ──► Generates phonetic transitions via sliding context rules 
  │                    │
  └────┬───────────────┘
       │
       ▼
 [synthesizer]     ──► Concatenates waveforms using windowed overlap-add (OLA)
       │
       ▼
[Output WAV Audio] ──► Exports 16 kHz Mono 16-bit PCM Audio file 
```

Pipeline component details below.

1. **CLI (main.c)**

The main.c file provides the command-line interface (CLI) entry point. It processes initial arguments, handles option flags, and uses the system components, dictionary, G2P engine, text processing and the synthesizer. 
```
usage: dippo "Phrase Text" [options];
```
Options are:
```
-wav <filename> (Output directly to a WAV audio file)
-tempo <value> (default=1)
-av <factor> (Audio volume amplification factor default=1.0)
-v (Print version information)
```

2. **Text Processing (text_processor.c)**

Implements preprocessing to clean input strings and expand numeric data before vocabulary matching:

• **Punctuation Filtering:** Replaces structural punctuation (such as :, ., ,) with space literals to isolate tokens cleanly, while preserving internal punctuation like hyphens (-) and apostrophes (') to maintain single-word token boundaries.

• **Chronological Normalisation:** Translates dates and calendar numbers (e.g., 4:01 expands to "four zero one", 24th turns into "twenty fourth").

• **Smart Year Handling:** Explicitly parses calendar eras (e.g., 2026 turns into "twenty twenty six", while 2005 expands to "two thousand and five").

3. **Dictionary (dictionary.c)**

Performs case-insensitive matching against a compiled static table mapping target text keys directly to custom diphone sequence strings.

• **Dynamic Plural Truncation:** To avoid dictionary bloat, if a plural word ending in 's' fails direct lookup, Dippo automatically truncates the terminal character, confirms the singular root, strips its closing silence boundaries, and appends a direct phonetic bridge (_s s_pau) smoothly.

4. **G2P Engine (g2p_engine.c)**

If a word is not found in the dictionary, the G2P engine is invoked. A **G2P engine** converts written spelling units (graphemes) into a sequence of phonemes that represents how the text should be pronounced. The phoneme sequence is then converted into a diphone sequence.

Grapheme-to-Phoneme predictive rule tree calculates pronunciation on the fly using a sliding window context:

```
char p = previous_char; // 1 character behind
char c = current_char;  // Target character
char n = next_char;     // 1 character ahead
char nn = char_2_ahead; // 2 characters ahead
char nnn = char_3_ahead;// 3 characters ahead
```

The  predict functions `predictA` through `predictZ` functions are the pronunciation rules.

**Example: predictE function**
```
const char* predictE(char p, char c, char n, char nn, char nnn) {
    // Context-sensitive vowel pronunciation:
    
    if (n == '_') return "silent";        // Final 'e' that's silent
    
    if (p == 'h' && n == 'l') return "ah";  // "he" -> ah sound
    if (c == 'o' && n == '_') return "ow";   // "go", "hello" -> ow sound
    
    // Magic E patterns:
    if (n != '_' && nn == 'e' && nnn == '_') return "ey";  // "cake" -> ey
    
    // Default case for 'e':
    return "eh";
}
```

The patterns in the predict rules were learned from the [CMU Pronouncing Dictionary](http://www.speech.cs.cmu.edu/cgi-bin/cmudict#about)  which was used as the pronunciation corpus. This contains 134,000 words and their phoneme transcriptions. My CART-Based Grapheme-to-Phoneme (G2P) Rule Generator & Tester [project](https://github.com/crispinprojects/g2p-rule-generator) explains how I developed the G2P engine.

The G2P engine while not perfect always tries to achieve a pronunciation for a word not in the dictionary (unknown word).


5. **Synthesizer & Vocoder (synthesizer.c)**

The vocoder is the component responsible for taking a diphone sequence and generating the final audio samples. Early versions of dippo used a basic  **overlap-add** (OLA) method to concatenate the recorded diphone raw audio units. OLA smooths the joins between recorded diphone units to eliminate audible clicks and digital discontinuities. At each diphone boundary join, it overlaps a short trailing tail of the outgoing unit with the opening segment of the incoming unit using complementary cross-fade windows.

Dippo version 0.3 has moved from basic OLA to PSOLA (Pitch-Synchronous Overlap-Add). OLA windows audio at fixed intervals. PSOLA windows audio dynamically, centering each frame precisely on a pitch mark (glottal pulse epoch). Dippo defaults to PSOLA.

The **Pitch-Synchronous Overlap-and-Add (PSOLA)** is a speech synthesis technique used to modify the **pitch** and **duration** of diphone-based speech segments while preserving naturalness. It works by:

1. **Pitch Marking**:
   - The input waveform is analyzed using an **AMDF (Average Magnitude Difference Function)** algorithm to detect pitch periods.
   - These periods are marked as **epochs** (instantaneous points of maximum excitation in the vocal tract).
   - Epochs define where each pitch cycle begins, enabling precise manipulation.

2. **Time-Pitch Modification**:
   - For **time-stretching**, epochs are spaced further apart or closer together.
   - For **pitch-shifting**, the spacing between epochs is adjusted while maintaining waveform integrity.
   - A **triangular window** is applied around each epoch to smoothly overlap and add segments.

3. **Overlap-and-Add (OLA)**:
   - Adjacent pitch periods are overlapped with a fading window to avoid discontinuities.
   - The amplitude scaling factor (`av`) controls loudness, while the tempo factor adjusts speed.
   
In `synthesizer.c`, PSOLA is implemented via:
- `amdf_pitch_tracker()` → Detects epochs.
- `vocoder_psola()` → Applies time-pitch modifications using epoch spacing and windowing.
- `create_calendar_intonation_envelope()`  → Intonation contour mapping.

### **Key Advantages PSOLA**

- Preserves natural speech quality by synchronising modifications to natural pitch cycles.
- Enables dynamic intonation changes (e.g., rising/falling pitch in phrases).
- Computationally efficient compared to phase vocoders for real-time applications.

## Compilation & Usage

## Prerequisites

Developed and tested on Debian 13 (Trixie). Requires standard GCC compilation tools and ALSA utility packages:

```
sudo apt install build-essential alsa-utils
```


## Building From Source

Compile using the provided Makefile:

```
make
```
To run the diphone speech synthesizer called  dippo from the terminal use:

```
./dippo
```
This will show the CLI options.

### Execution Flags

```
usage: dippo "Phrase Text" [options]
options:
 -wav  <filename>  Output directly to a specified WAV audio file (default: out.wav)
 -tempo <value>    Audio playback speed scaling multiplier (default: 1.0)
 -av    <factor>   Audio volume amplification scaling factor (default: 1.0)
 -v                Print synthesiser release version information
```

## Practical Verification Examples


1. **Baseline Text and Pre-processing Verification**

Dippo defaults to the PSOLA mode.
```
 ./dippo "Dippo, is not a Hippo!" -tempo 2.0 -av 1.5 -wav out.wav && aplay out.wav
```
Using the OLA mode.
```
./dippo "Dippo, is not a Hippo!" -mode ola -tempo 2.0 -av 1.5 -wav out.wav && aplay out.wav
```

2. **Automatic Date Transformation**

```
./dippo "Friday 30th October" -tempo 2.0 -av 1.5 -wav out.wav && aplay out.wav

```
3. **Time**

With time entries there is a clash with "am" the verb and "am" meaning time A.M. 
Consequently there are two dictionary entries "am" and "amtime" to distinguish between the two cases.
```
{"am", "pau_ae ae_m m_pau"}, // Verb: "I am a synthesizer"
{"amtime", "pau_ey ey_ey ey_ae ae_m m_m m_m m_pau"}, // Time: "A.M."
```
There is also a "pmtime" dictionary entry.
```
./dippo "the time is 11:07 amtime" -tempo 2.0 -av 1.5  -wav out.wav && aplay out.wav
```
```
./dippo "the time is 6:04 pmtime" -tempo 2.0 -av 1.5  -wav out.wav && aplay out.wav
```

4. **Dynamic Plural Aggregation**

```
./dippo "doctors appointment" -tempo 1.75 -av 1.5
```
Output Trace Log:
```
Looking up: 'doctors'
Found singular dictionary match for plural root 'doctor'
Combined diphone string: pau_d d_aa aa_aa aa_k k_t t_er er_r r_s s_pau ...
```

5. **Unknown word (G2P Engine)**

When dippo encounters a word not in the dictionary (unknown word) it uses the G2P engine to predict its pronunciation.
The word "London"  is not in the dictionary and so is an unknown word.

```
 ./dippo "visit London" -tempo 1.75 -av 1.5 -wav out.wav && aplay out.wav 
```
Output trace log:

```
Processing phrase: "Visit London"
DEBUG: Processing token 'Visit'
DEBUG: Processing token 'London'
DEBUG: Final preprocessed text: 'Visit London'
After preprocessing: "Visit London"
Looking up: 'Visit'
Found diphones for 'Visit': pau_v v_ih ih_ih ih_z z_ih ih_t t_pau
Looking up: 'London'
Warning: No diphone sequence found for word 'London'
G2P result for 'London': pau_l l_aa aa_n n_d d_aa aa_n n_pau
```

The G2P engine while not perfect always tries to achieve a pronunciation for an unknown word.

## License

Distributed under the **GNU General Public License v3.0**.

## Author

* **Alan Crispin** [Github](https://github.com/crispinprojects)


## Project Status

Active.

## Acknowledgements

* Acoustic Data: Uses the public base [diphone collection](https://github.com/hypnaceae/DiphoneSynth/blob/master/diphones_license.txt) compiled by Professor Alan W Black and Kevin Lenzo.

* Diphone collection and synthesis Alan W. Black and Kevin Lenzo [2000](https://www.cs.cmu.edu/~awb/papers/ICSLP2000_diphone.pdf)

* [Speech Synthesis](https://www.youtube.com/watch?v=eDjtEsOvouM&t=1459s) CMU Low resource NLP Bootcamp 2020  *Author:* Professor Alan W Black 

* Development managed using the [Geany](https://www.geany.org/) IDE

* [Talk Calendar](https://github.com/crispinprojects/talkcalendar)


