/* dictionary.c
 *
 * Copyright 2026 Alan Crispin <crispinalan@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
/**
 * @file dictionary.c
 * @brief Implementation of word lookup functionality for Dippo's speech synthesizer.
 * 
 * This file contains functions for finding words in a dictionary table and retrieving their phonetic transcriptions.
 */

#include "dictionary.h"

/**
 * @brief A static array of WordEntry structs containing words and 
 * their diphone phonetic transcriptions.
 */
static WordEntry word_table[] = {

//words
{"hello","pau_hh hh_ah ah_l l_ow ow_pau"},
{"world","pau_w w_uh uh_l l_d"},

{"the","pau_dh dh_ah ah_ah ah_pau"},
{"time","pau_t t_ay ay_ay ay_m m_pau"},
{"date","pau_d d_ey ey_ey ey_t t_pau"},
{"is","pau_ih ih_z z_z z_pau"},
{"o","pau_ao ao_uh uh_ao ao_uh uh_pau"}, //

{"my", "pau_m m_ay ay_ay ay_pau"},
{"name", "pau_n n_ey ey_m m_pau"},
{"dippo", "pau_d d_ih ih_p p_pau pau_ow ow_pau"},
{"hippo" ,"pau_hh hh_ih ih_p p_pau pau_ow ow_pau"},
{"not", "pau_n n_aa aa_t t_pau"},
{"a", "pau_ax ax_pau"},
//dippo is not a hippo
{"i","pau_ay ay_ay ay_pau"},
{"am", "pau_ae ae_m m_pau"}, // Verb: "I am a synthesizer"
{"amtime", "pau_ey ey_ey ey_ae ae_m m_m m_m m_pau"}, // Time: "A.M."
{"pmtime","pau_p p_iy iy_iy iy_m m_eh eh_m m_m m_m m_pau"}, //Time P.M.


{"thousand", "pau_th th_er er_er er_z z_ae ae_n n_d d_pau"},
{"and", "pau_ae ae_n n_d d_pau"},


//event words
{"anniversary","pau_ae ae_ae ae_n n_ah ah_v v_er er_er er_s s_er er_iy iy_pau"},
{"appointment","pau_ah ah_p p_oy oy_oy oy_n n_t t_m m_ah ah_n n_t t_pau"},
{"birthday","pau_b b_er er_er er_th th_d d_ey ey_pau"},
{"car","pau_k k_aa aa_aa aa_r r_pau"},
{"clinic","pau_k k_l l_ih ih_ih ih_n n_ih ih_k k_pau"},
{"clock","pau_k k_l l_aa aa_k k_pau"},
{"delivery","pau_d d_ih ih_l l_ih ih_ih ih_v v_er er_iy iy_pau"},
{"dentist","pau_d d_eh eh_eh eh_n n_t t_ah ah_s s_t t_pau"},
{"doctor","pau_d d_aa aa_aa aa_k k_t t_er er_pau"},
{"driver","pau_d d_r r_ay ay_ay ay_v v_er er_pau"},
{"family","pau_f f_ae ae_ae ae_m m_ah ah_l l_iy iy_pau"},
{"funeral","pau_f f_y y_uw uw_uw uw_n n_er er_ah ah_l l_pau"},
{"happy", "pau_hh hh_ae ae_p p_iy iy_pau"},
{"holiday","pau_hh hh_aa aa_aa aa_l l_ah ah_d d_ey ey_ey ey_pau"},
{"home","pau_hh hh_ow ow_ow ow_m m_pau"},
//{"home","pau_h h_ow ow_m m_pau"},
{"maintenance","pau_m m_ey ey_ih ih_n n_t t_ah ah_n n_ae ae_n n_s s_pau"},
{"hospital","pau_hh hh_aa aa_aa aa_s s_p p_ih ih_ih ih_t t_ah ah_l l_pau"},
{"medical","pau_m m_eh eh_eh eh_d d_ah ah_k k_ah ah_l l_pau"},
{"meeting","pau_m m_iy iy_iy iy_t t_ih ih_ng ng_pau"},
{"meetup","pau_m m_iy iy_t t_pau pau_pau pau_ah ah_p p_pau"},
{"memo","pau_m m_eh eh_eh eh_m m_ao ao_uh uh_pau"},
{"party","pau_p p_aa aa_aa aa_r r_t t_iy iy_pau"},
{"payment","pau_p p_ey ey_ey ey_m m_ah ah_n n_t t_pau"},
{"project","pau_p p_r r_aa aa_aa aa_jh jh_eh eh_k k_t t_pau"},
{"reminder","pau_r r_iy iy_m m_ay ay_ay ay_n n_d d_er er_pau"},
{"restaurant","pau_r r_eh eh_eh eh_s s_t t_er er_aa aa_aa aa_n n_t t_pau"},
{"sport","pau_s s_p p_ao ao_ao ao_r r_t t_pau"},
{"task","pau_t t_ae ae_ae ae_s s_k k_pau"},
{"television","pau_t t_eh eh_eh eh_l l_ah ah_v v_ih ih_ih ih_zh zh_ah ah_n n_pau"},
{"theater","pau_th th_iy iy_iy iy_ey ey_t t_er er_pau"},
{"travel","pau_t t_r r_ae ae_ae ae_v v_ah ah_l l_pau"},
{"visit","pau_v v_ih ih_ih ih_z z_ih ih_t t_pau"},
{"walk","pau_w w_w w_ao ao_ao ao_k k_k k_pau"},
{"work","pau_w w_uh uh_r r_k k_pau"},

{"speech", "pau_s s_p p_iy iy_iy iy_ch ch_pau"},
{"synthesizer", "pau_s s_ih ih_n n_th th_ah ah_z z_ay ay_er er_pau"},

//days of week
{"monday","pau_m m_ah ah_ah ah_n n_n n_d d_iy iy_pau"},
{"tuesday","pau_t t_uw uw_uw uw_z z_d d_iy iy_pau"},
{"wednesday","pau_w w_eh eh_eh eh_n n_z z_d d_iy iy_pau"},
{"thursday","pau_th th_er er_er er_z z_d d_iy iy_pau"},
{"friday","pau_f f_r r_ay ay_ay ay_d d_iy iy_pau"},
{"saturday","pau_s s_ae ae_ae ae_t t_ih ih_ih ih_d d_ey ey_ey ey_pau"},
{"sunday","pau_s s_ah ah_ah ah_n n_d d_ey ey_ey ey_pau"},

//months
{"january","pau_jh jh_ae ae_ae ae_n n_y y_uw uw_eh eh_eh eh_r r_iy iy_pau"},
{"february","pau_f f_eh eh_eh eh_b b__y y_ah ah_w w_eh eh_eh eh_r r_iy iy_pau"},
{"march","pau_m m_aa aa_aa aa_r r_ch ch_pau"},
{"april","pau_ey ey_ey ey_p p_r r_ah ah_l l_pau"},
{"may","pau_m m_m m_ey ey_ey ey_pau"},
{"june","pau_jh jh_uw uw_uw uw_n n_pau"},
{"july","pau_jh jh_uw uw_uw uw_l l_ay ay_ay ay_pau"},
{"august","pau_aa aa_aa aa_g g_ah ah_s s_t t_pau"},
{"september","pau_s s_eh eh_p p_t t_eh eh_eh eh_m m_b b_er er_pau"},
{"october","pau_aa aa_k k_t t_ow ow_ow ow_b b_er er_pau"},
{"november","pau_n n_ow ow_v v_eh eh_eh eh_m m_b b_er er_pau"},
{"december","pau_d d_ih ih_s s_eh eh_eh eh_m m_b b_er er_pau"},

//ordinal numbers (e.g., "first", "second") 
{"first","pau_f f_er er_er er_s s_t t_pau pau_t"},
{"second","pau_s s_eh eh_eh eh_k k_ah ah_n n_d d_pau"},
{"third","pau_th th_er er_er er_d d_pau"},
{"fourth","pau_f f_ao ao_ao ao_r r_th th_pau"},
{"fifth","pau_f f_ih ih_ih ih_f f_th th_pau"},
{"sixth","pau_s s_ih ih_ih ih_k k_s s_th th_pau"},
{"seventh","pau_s s_eh eh_eh eh_v v_ah ah_n n_th th_pau"},
{"eighth","pau_eh eh_ey ey_ey ey_t t_th th_pau"},
{"ninth","pau_n n_ay ay_ay ay_n n_th th_pau"},
{"tenth","pau_t t_eh eh_eh eh_n n_th th_pau"},
{"eleventh","pau_ih ih_l l_eh eh_eh eh_v v_ah ah_n n_th th_pau"},
{"twelfth","pau_t t_w w_eh eh_eh eh_l l_f f_th th_th th_pau"},
{"thirteenth","pau_th th_er er_er er_t t_iy iy_iy iy_n n_th th_pau"},
{"fourteenth","pau_f f_ao ao_ao ao_r r_t t_iy iy_iy iy_n n_th th_th th_pau"},
{"fifteenth","pau_f f_ih ih_f f_t t_iy iy_iy iy_n n_th th_pau"},
{"sixteenth","pau_s s_ih ih_k k_s s_t t_iy iy_iy iy_n n_th th_pau"},
{"seventeenth","pau_s s_eh eh_eh eh_v v_ah ah_n n_t t_iy iy_iy iy_n n_th th_pau"},
{"eighteenth","pau_ey ey_ey ey_t t_iy iy_iy iy_n n_th th_pau"},
{"nineteenth","pau_n n_ay ay_ay ay_n n_t t_iy iy_iy iy_n n_th th_th th_pau"},
{"twentieth","pau_t t_w w_eh eh_eh eh_n n_t t_iy iy_eh eh_eh eh_th th_pau"},

{"thirtieth","pau_th th_er er_er er_t t_iy iy_eh eh_eh eh_th th_pau"},

//cardinal numbers one, two,three
{"zero","pau_z z_iy iy_iy iy_r r_ow ow_pau"},
{"one","w_ah ah_ah ah_n n_pau"},
{"two","pau_t t_uw uw_uw uw_pau"},
{"three","pau_t th_r r_iy iy_iy iy_pau"},
{"four","pau_f f_ao ao_ao ao_r r_pau"},
{"five","pau_f f_ay ay_ay ay_v v_pau"},
{"six","pau_s s_ih ih_ih ih_k k_s s_pau"},
{"seven","pau_s s_eh eh_eh eh_v v_ah ah_n n_pau"},
{"eight","pau_ey ey_ey ey_t t_pau"},
{"nine","pau_n n_ay ay_ay ay_n n_pau"},
{"ten","pau_t t_eh eh_eh eh_n n_pau"},
{"eleven","pau_ih ih_l l_eh eh_eh eh_v v_ah ah_n n_pau"},
{"twelve","pau_t t_w w_eh eh_eh eh_l l_v v_pau"},
{"thirteen","pau_th th_er er_er er_t t_iy iy_iy iy_n n_pau"},
{"fourteen","pau_f f_ao ao_ao ao_r r_t t_iy iy_iy iy_n n_pau"},
{"fifteen","pau_f f_ih ih_ih ih_f f_t t_iy iy_iy iy_n n_pau"},
{"sixteen","pau_s s_ih ih_k k_s s_t t_iy iy_iy iy_n n_pau"},
{"seventeen","pau_s s_eh eh_eh eh_v v_ah ah_n n_t t_iy iy_iy iy_n n_pau"},
{"eighteen","pau_ey ey_ey ey_t t_iy iy_iy iy_n n_pau"},
{"nineteen","pau_n n_ay ay_ay ay_n n_t t_iy iy_iy iy_n n_pau"},
{"twenty","pau_t t_w w_eh eh_eh eh_n n_t t_iy iy_pau"},
{"thirty","pau_th th_er er_er er_d d_iy iy_iy iy_pau"},
{"forty","pau_f f_ao ao_ao ao_r r_t t_iy iy_pau"},
{"fifty","pau_f f_ih ih_ih ih_f f_t t_iy iy_pau"},
	
};

/**
 * @brief Number of entries in the lookup table
 */
const unsigned int word_table_size = sizeof(word_table) / sizeof(word_table[0]);

/**
 * @brief Get the diphone sequence for a word
 * 
 * This function searches for a given word in the dictionary and returns its diphone transcription.
 * The search is case-insensitive.
 * 
 * @param word Pointer to the null-terminated string containing the word to find
 * @return Pointer to WordEntry if found, NULL otherwise
 */
WordEntry* find_word_entry(const char* word) {
    // Handle case-insensitive lookup
    char lower_word[64];
    strcpy(lower_word, word);
    
    // Convert to lowercase for comparison (simplified version)
    for (int i = 0; lower_word[i]; i++) {
        if (lower_word[i] >= 'A' && lower_word[i] <= 'Z') {
            lower_word[i] = lower_word[i] + ('a' - 'A');
        }
    }
    
    for (unsigned int i = 0; i < word_table_size; i++) {
        if (strcmp(word_table[i].word, lower_word) == 0) {
            return &word_table[i];
        }
    }
    return NULL;
}


/**
 * @brief Get the total number of entries in dictionary
 * 
 * This function returns the total number of words stored in the dictionary table.
 * 
 * @return Number of entries in the dictionary
 */
int get_dictionary_size(void) {
    return word_table_size;
}
