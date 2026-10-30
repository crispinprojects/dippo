/**
 * @file synthesizer.h
 * @brief Header file for the voice synthesizer with diphone synthesis.
 *
 * This file contains the function declarations and data structures used for
 * overlap-add logic in diphone-based speech synthesis. It supports PSOLA (Pitch
 * Synchronous Overlap and Add) and OLA (Overlap and Add) vocoding techniques.
 */

#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <stdio.h>
#include <stdint.h>

/**
 * @struct DiphoneData
 * @brief Represents a diphone entry with metadata.
 *
 * Contains the name, raw data pointer, and size of a diphone sample.
 */
typedef struct {
    const char* name;          /**< Name identifier of the diphone */
    const unsigned char* data; /**< Pointer to raw diphone waveform data */
    unsigned int size;         /**< Size of the diphone data in bytes */
} DiphoneData;

/**
 * @struct PitchMarks
 * @brief Stores pitch period markers (epoch indices) for a diphone.
 *
 * Used in PSOLA vocoding to track pitch periods for time-scale modification.
 */
typedef struct {
    int* sample_indices; /**< Array of epoch indices (pitch marks) */
    int num_marks;       /**< Number of detected epochs */
} PitchMarks;

/**
 * @struct PSOLA_DiphoneEntry
 * @brief Enhanced diphone entry with pitch information for PSOLA synthesis.
 *
 * Extends DiphoneData with pitch period markers for dynamic pitch manipulation.
 */
typedef struct {
    const char* name;          /**< Name identifier of the diphone */
    const int16_t* data;       /**< Pointer to processed waveform data (int16) */
    int size;                  /**< Size of the waveform data in bytes */
    PitchMarks epochs;         /**< Pitch period markers for PSOLA */
} PSOLA_DiphoneEntry;

/**
 * @brief Computes pitch periods using Average Magnitude Difference Function (AMDF).
 *
 * Analyzes a waveform segment to detect pitch periods by finding local minima
 * in the AMDF function. Returns an array of epoch indices.
 *
 * @param data Input audio waveform (int16)
 * @param size Number of samples in the input waveform
 * @param min_period Minimum expected pitch period (in samples)
 * @param max_period Maximum expected pitch period (in samples)
 * @param marks Output array of detected epoch indices (allocated by function)
 * @return Number of detected epochs, or -1 on failure.
 */
int amdf_pitch_tracker(const int16_t* data, size_t size, int min_period, int max_period, int** marks);

/**
 * @brief Locates and processes a diphone entry for PSOLA synthesis.
 *
 * Loads waveform data from a diphone entry and computes its pitch periods using
 * AMDF. Returns a PSOLA_DiphoneEntry structure with processed data.
 *
 * @param name Name of the diphone to retrieve
 * @return Pointer to PSOLA_DiphoneEntry, or NULL if not found.
 */
PSOLA_DiphoneEntry* find_psola_diphone(const char* name);

/**
 * @brief Writes a WAV file header to the specified file.
 *
 * Formats the RIFF/WAVE header with given data size for mono 16-bit PCM audio
 * at 16 kHz sample rate.
 *
 * @param file Output file handle
 * @param data_size Size of audio data in bytes
 */
void write_wav_header(FILE* file, int data_size);

/**
 * @brief Overlap-and-Add (OLA) vocoder for diphone concatenation.
 *
 * Synthesizes speech by overlapping and adding diphone segments with adjustable
 * tempo and amplitude. Supports dynamic buffer expansion for long utterances.
 *
 * @param diphone_string Space-separated list of diphones to synthesize
 * @param output_file Output WAV file handle
 * @param tempo Time-stretch factor (1.0 = normal speed)
 * @param av Amplitude scaling factor (1.0 = no change)
 */
void vocoder_ola(char* diphone_string, FILE* output_file, float tempo, float av);

/**
 * @brief Pitch-Synchronous Overlap-and-Add (PSOLA) vocoder.
 *
 * Synthesizes speech with dynamic pitch control by adjusting epoch spacing and
 * applying a triangular window. Supports phrase-level intonation modulation via
 * an optional pitch array.
 *
 * @param diphone_string Space-separated list of diphones to synthesize
 * @param output_file Output WAV file handle
 * @param tempo Time-stretch factor (1.0 = normal speed)
 * @param av Amplitude scaling factor (1.0 = no change)
 * @param pitch_array Optional array of target pitches (Hz) per diphone
 * @param pitch_array_len Length of the pitch array
 */
void vocoder_psola(char* diphone_string, FILE* output_file, float tempo, float av, const float* pitch_array, int pitch_array_len);

#endif // SYNTHESIZER_H
