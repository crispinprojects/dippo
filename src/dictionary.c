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
#include "dictionary.h"

//static WordEntry word_table[] = {
	//{"birthday","b_uh uh_r r_th th_d d_eh eh_iy"},
	//{"happy","hh_ae ae_p p_iy"},
    //{ "hello", "hh_ah ah_l l_ao ao_uh" },
    //{ "world", "w_uh uh_r r_l l_d" },
   
//};

//small dictionary for experimentation


static WordEntry word_table[] = {
	//A-words

//amm
{"amm","pau_ey ey_ey ey_ae ae_m m_m m_m m_pau"}, //A.M.
//april
{"april","pau_ey ey_ey ey_p p_r r_ah ah_l l_pau"},
//august
{"august","pau_aa aa_aa aa_g g_ah ah_s s_t t_pau"},

 
//B_words
//birthday BIRTHDAY  B ER1 TH D EY2
{"birthday","pau_b b_er er_er er_th th_d d_ey ey_pau"},

//C_words
//D_words
//december
{"december","pau_d d_ih ih_s s_eh eh_eh eh_m m_b b_er er_pau"},
//dentist
{"dentist","pau_d d_eh eh_eh eh_n n_t t_ah ah_s s_t t_pau"},
//doctor
{"doctor","pau_d d_aa aa_aa aa_k k_t t_er er_pau"},

//E_words
//eight
{"eight","pau_ey ey_ey ey_t t_pau"},
//eighteen
{"eighteen","pau_ey ey_ey ey_t t_iy iy_iy iy_n n_pau"},
//eighteenth
{"eighteenth","pau_ey ey_ey ey_t t_iy iy_iy iy_n n_th th_pau"},
//eighth
{"eighth","pau_eh eh_ey ey_ey ey_t t_th th_pau"},
//eightieth 
{"eightieth","pau_eh ey_ey ey_t t_iy iy_ih ih_th th_pau"},
//eighty
{"eighty","pau_eh eh_iy iy_t t_iy iy_pau"},
//eleven
{"eleven","pau_ih ih_l l_eh eh_eh eh_v v_ah ah_n n_pau"},
//eleventh
{"eleventh","pau_ih ih_l l_eh eh_eh eh_v v_ah ah_n n_th th_pau"},

//F_words
//february
{"february","pau_f f_eh eh_eh eh_b b__y y_ah ah_w w_eh eh_eh eh_r r_iy iy_pau"},
//fifteen
{"fifteen","pau_f f_ih ih_ih ih_f f_t t_iy iy_iy iy_n n_pau"},
//fifteenth
{"fifteenth","pau_f f_ih ih_f f_t t_iy iy_iy iy_n n_th th_pau"},
//fifth
{"fifth","pau_f f_ih ih_ih ih_f f_th th_pau"},
//fifty
{"fifty","pau_f f_ih ih_ih ih_f f_t t_iy iy_pau"},
//first
{"first","pau_f f_er er_er er_s s_t t_pau pau_t"},
//four
{"four","pau_f f_ao ao_ao ao_r r_pau"},
//fourteen
{"fourteen","pau_f f_ao ao_ao ao_r r_t t_iy iy_iy iy_n n_pau"},
//fourteenth
//{"fourteenth","pau_f f_ao ao_ao ao_r r_t t_iy iy_iy iy_n n_th th_pau pau_th th_pau"},
{"fourteenth","pau_f f_ao ao_ao ao_r r_t t_iy iy_iy iy_n n_th th_th th_pau"},
//fourth
{"fourth","pau_f f_ao ao_ao ao_r r_th th_pau"},
//friday
{"friday","pau_f f_r r_ay ay_ay ay_d d_iy iy_pau"},

//G_words

//H_words
//hello
{"hello","pau_hh hh_ah ah_l l_ow ow_ow ow_pau"},

//I_words

//J_words
//january
{"january","pau_jh jh_ae ae_ae ae_n n_y y_uw uw_eh eh_eh eh_r r_iy iy_pau"},
//july
{"july","pau_jh jh_uw uw_uw uw_l l_ay ay_ay ay_pau"},
//june
{"june","pau_jh jh_uw uw_uw uw_n n_pau"},

//K_words
//L_words
//M_words
//march
{"march","pau_m m_aa aa_aa aa_r r_ch ch_pau"},
//may
{"may","pau_m m_m m_ey ey_ey ey_pau"},
//monday
{"monday","pau_m m_ah ah_ah ah_n n_n n_d d_iy iy_pau"},
//month
{"month","pau_m m_ah ah_n n_th th_pau"},

//N_words
//naomi

//nine
{"nine","pau_n n_ay ay_ay ay_n n_pau"},
//nineteen
{"nineteen","pau_n n_ay ay_ay ay_n n_t t_iy iy_iy iy_n n_pau"},
//nineteenth
{"nineteenth","pau_n n_ay ay_ay ay_n n_t t_iy iy_iy iy_n n_th th_th th_pau"},
//ninetieth
{"ninetieth","pau_n n_aa aa_iy iy_n n_t t_iy iy_ih ih_th th_pau"},
//ninety
{"ninety","pau_n n_aa aa_iy iy_n n_t t_iy iy_pau"},
//ninth
{"ninth","pau_n n_ay ay_ay ay_n n_th th_pau"},
//no
{"no","pau_n n_ao ao_uh uh_pau"},

//november
{"november","pau_n n_ow ow_v v_eh eh_eh eh_m m_b b_er er_pau"},

//O_words

//october
{"october","pau_aa aa_k k_t t_ow ow_ow ow_b b_er er_pau"},
//one
{"one","w_ah ah_ah ah_n n_pau"},

//P_words

//pauses
{"pause1","pau_pau"},
{"pause2","pau_pau pau_pau"},
//pmm
{"pmm","pau_p p_iy iy_iy iy_m m_eh eh_m m_m m_m m_pau"},
//priority
{"priority","pau_p p_r r_aa aa_iy iy_ao ao_r r_ah ah_t t_iy iy_pau"},
//public
{"public","pau_p p_ah ah_ah ah_b b_l l_ih ih_k k_pau"},

//Q_words


//R_words
//radio
//reminder
{"reminder","pau_r r_iy iy_m m_ay ay_ay ay_n n_d d_er er_pau"},

//S_words

//saturday
{"saturday","pau_s s_ae ae_ae ae_t t_ih ih_ih ih_d d_ey ey_ey ey_pau"},
{"season","pau_s s_iy iy_z z_ah ah_n n_pau"},
//second
{"second","pau_s s_eh eh_eh eh_k k_ah ah_n n_d d_pau"},
//september
{"september","pau_s s_eh eh_p p_t t_eh eh_eh eh_m m_b b_er er_pau"},
//seven
{"seven","pau_s s_eh eh_eh eh_v v_ah ah_n n_pau"},
//seventeen
{"seventeen","pau_s s_eh eh_eh eh_v v_ah ah_n n_t t_iy iy_iy iy_n n_pau"},
//seventeenth
{"seventeenth","pau_s s_eh eh_eh eh_v v_ah ah_n n_t t_iy iy_iy iy_n n_th th_pau"},
//seventh
{"seventh","pau_s s_eh eh_eh eh_v v_ah ah_n n_th th_pau"},
//seventieth
{"seventieth","pau_s s_eh eh_v v_ah ah_n n_t t_iy iy_ih ih_th th_pau"},
//seventy
{"seventy","pau_s s_eh eh_v v_ah ah_n n_t t_iy iy_pau"},
//six
{"six","pau_s s_ih ih_ih ih_k k_s s_pau"},
//sixteen
{"sixteen","pau_s s_ih ih_k k_s s_t t_iy iy_iy iy_n n_pau"},
//sixteenth
{"sixteenth","pau_s s_ih ih_k k_s s_t t_iy iy_iy iy_n n_th th_pau"},
//sixth
{"sixth","pau_s s_ih ih_ih ih_k k_s s_th th_pau"},
//sixtieth
//sixty
{"sixty","pau_s s_ih ih_k k_s s_t t_iy iy_pau"},
//spring
{"spring","pau_s s_p p_r r_ih ih_ih ih_ng ng_pau"},
//summer
{"summer","pau_s s_ah ah_m m_uh uh_r r_pau"},
//summertime
{"summertime","pau_s s_ah ah_m m_uh uh_r r_t t_aa aa_iy iy_m m_pau"},
//sunday 
{"sunday","pau_s s_ah ah_ah ah_n n_d d_ey ey_ey ey_pau"},

//T_words

//ten
{"ten","pau_t t_eh eh_eh eh_n n_pau"},
//tenth
{"tenth","pau_t t_eh eh_eh eh_n n_th th_pau"},
//third
{"third","pau_th th_er er_er er_d d_pau"},
//thirteen
{"thirteen","pau_th th_er er_er er_t t_iy iy_iy iy_n n_pau"},
//thirteenth
{"thirteenth","pau_th th_er er_er er_t t_iy iy_iy iy_n n_th th_pau"},
//thirtieth
{"thirtieth","pau_th th_er er_er er_t t_iy iy_eh eh_eh eh_th th_pau"},
//thirty
{"thirty","pau_th th_er er_er er_d d_iy iy_iy iy_pau"},
//three
{"three","pau_t th_r r_iy iy_iy iy_pau"},
//thursday
{"thursday","pau_th th_er er_er er_z z_d d_iy iy_pau"},
//time
{"time","pau_t t_ay ay_ay ay_m _pau"},
//tomorrow
{"tomorrow","pau_t t_ah ah_m m_aa aa_r r_ao ao_uh uh_pau"},
//tuesday
{"tuesday","pau_t t_uw uw_uw uw_z z_d d_iy iy_pau"},
//twelfth
{"twelfth","pau_t t_w w_eh eh_eh eh_l l_f f_th th_th th_pau"},
//twelve
{"twelve","pau_t t_w w_eh eh_eh eh_l l_v v_pau"},
//twentieth
{"twentieth","pau_t t_w w_eh eh_eh eh_n n_t t_iy iy_eh eh_eh eh_th th_pau"},
//twenty
{"twenty","pau_t t_w w_eh eh_eh eh_n n_t t_iy iy_pau"},
//two
{"two","pau_t t_uw uw_uw uw_pau"},

//U_words
//V_words
//w_words
//wednesday
{"wednesday","pau_w w_eh eh_eh eh_n n_z z_d d_iy iy_pau"}, //or iy_iy
//week
{"week","pau_w w_iy iy_k k_pau"},
//winter
{"winter","pau_w w_ih ih_ih ih_n n_t t_er er_pau"},

//world
{"world","pau_w w_er er_er er_l l_d d_d d_pau"},

//X_words
//Y_words
//year
{"year","pau_y y_ih ih_ih ih_r r_pau"},
//yearly
{"yearly","pau_y y_ih ih_ih ih_r r_l l_iy iy_pau"},
//Z_words
//zero
{"zero","pau_z z_iy iy_iy iy_r r_ow ow_pau"},
	
};

// Number of entries in the lookup table
const unsigned int word_table_size = sizeof(word_table) / sizeof(word_table[0]);


//WordEntry* find_diphone_str(char* word)
//{
    //if (word == NULL) {
        //return NULL;
    //}
    //for (unsigned int i = 0; i < word_table_size; ++i) {
        //if (strcmp(word_table[i].name, word) == 0) {
            //return &word_table[i];
        //}
    //}
    //return NULL; // word not found
//}

WordEntry* find_word_entry(const char* word) {
    for (unsigned int i = 0; i < sizeof(word_table) / sizeof(word_table[0]); i++) {
        if (strcmp(word_table[i].word, word) == 0) {
            return &word_table[i];
        }
    }
    return NULL;
}

