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
//{"hello","pau_hh hh_ah ah_l l_ow ow_pau"},
//{"world","pau_w w_uh uh_l l_d"},

// linking words and transistion words
{"the","pau_dh dh_aa aa_ah ah_pau"}, 
{"is","pau_ih ih_z z_z z_pau"},
{"i","pau_ay ay_ay ay_pau"},
{"am", "pau_ae ae_m m_pau"}, // Verb: "I am a synthesizer"
{"my", "pau_m m_ay ay_ay ay_pau"},
{"not", "pau_n n_aa aa_t t_pau"},
{"a", "pau_ax ax_pau"},
{"and", "pau_ae ae_n n_d d_pau"},
{"in","pau_ih ih_n n_pau"},
{"be","pau_b b_iy iy_pau"},
{"by","pau_b b_aa aa_iy iy_pau"},
{"all","pau_al ao_l l_pau"},
{"do","pau_d d_uw uw_pau"},
{"many","pau_m m_eh eh_n n_iy iy_pau"},
{"no","pau_n n_ao ao_uh uh_pau"},
{"of","pau_ah ah_v v_pau"},
{"on","pau_aa aa_aa aa_n n_n n_pau"},
{"or","pau_ao ao_r r_pau"},
{"that","pau_dh dh_ae ae_t t_pau"},
{"then","pau_dh dh_eh eh_eh eh_n n_pau"},
{"this","pau_dh dh_ih ih_s s_pau"},
{"to","pau_t t_uw uw_uw uw_pau"},

//date and time
{"time","pau_t t_ay ay_ay ay_m m_pau"},
{"date","pau_d d_ey ey_ey ey_t t_pau"},
{"o","pau_ao ao_uh uh_ao ao_uh uh_pau"}, // oclock
//dippo
{"name", "pau_n n_ey ey_m m_pau"},
{"dippo", "pau_d d_ih ih_p p_pau pau_ow ow_pau"},
{"hippo" ,"pau_hh hh_ih ih_p p_pau pau_ow ow_pau"},
//dippo is not a hippo
{"speech", "pau_s s_p p_iy iy_iy iy_ch ch_pau"},
{"synthesizer", "pau_s s_ih ih_n n_th th_ah ah_z z_ay ay_er er_pau"},

{"amtime", "pau_ey ey_ey ey_ae ae_m m_m m_m m_pau"}, // Time: "A.M."
{"pmtime","pau_p p_iy iy_iy iy_m m_eh eh_m m_m m_m m_pau"}, //Time P.M.

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

{"thousand", "pau_th th_er er_er er_z z_ae ae_n n_d d_pau"},

//event words
//a
{"accountant","pau_ah ah_k k_aa aa_uh uh_n n_t t_ah ah_n n_t t_pau"},
{"activity","pau_ae ae_k k_t t_ih ih_ih ih_v v_ah ah_t t_iy iy_pau"},
{"adventure","pau ae ae_d d_v v_eh eh_n n_ch ch_uh uh_r r_pau"},
{"afternoon","pau_ae ae_ae ae_ae ae_f f_t t_er er_pau pau_er er_n n_uw uw_uw uw_n n_pau"},
{"agent","pau_eh eh_iy iy_jh jh_ah ah_n n_t t_pau"},
{"agreement","pau_ah ah_g g_r r_iy iy_m m_ah ah_n n_t t_pau"},
{"airport","pau_eh eh_eh eh_r r_p p_ao ao_ao ao_r r_t t_pau"},
{"alarm","pau_ah ah_l l_aa aa_r r_m m_pau"},
{"allotment","pau_ah ah_l l_aa aa_aa aa_t t_m m_ah ah_n n_t t_pau"},
{"amazon","pau_ae ae_ae ae_m m_ah ah_z z_aa aa_aa aa_n n_pau"},
{"anniversary","pau_ae ae_ae ae_n n_ah ah_v v_er er_er er_s s_er er_iy iy_pau"},
{"animal","pau_ae ae_n n_ah ah_m m_ah ah_l l_pau"},
{"appointment","pau_ah ah_p p_oy oy_oy oy_n n_t t_m m_ah ah_n n_t t_pau"},
//{"appointment","pau_ah ah_p p_oy oy_oy oy_n n_t t_m m_ah ah_n n_t t_pau"},
{"archery","pau_aa aa_r r_ch ch_uh uh_r r_iy iy_pau"},
{"art","pau_aa aa_aa aa_aa aa_r r_t t_pau"},
{"assessment","pau_ah ah_s s_eh eh_eh eh_s s_m m_ah ah_pau pau_ah ah_n n_t t_pau"},
{"autumn","pau_ao ao_t t_ah ah_m m_pau"},
//b
{"baby","pau_b b_eh eh_iy iy_b b_iy iy_pau"},
{"badminton","pau_b b_ae ae_d d_m m_ih ih_n n_t t_aa aa_n n_pau"},
{"bakery","pau_b b_eh eh_iy iy_k k_uh uh_r r_iy iy_pau"},
{"bank","pau_b b_ae ae_ae ae_ng ng_k k_pau"},
{"barber","pau_b b_aa aa_aa aa_r r_b b_er er_pau"},
{"bike","pau_b b_ay ay_ay ay_k k_pau"},
{"bin","pau_b b_ih ih_ih ih_n n_pau"},
{"bird","pau_b b_uh uh_r r_d d_pau"},
{"birthday","pau_b b_er er_er er_th th_d d_ey ey_pau"},
//{"birthday","pau_b b_er er_er er_th th_d d_ey ey_pau"},
{"boat","pau_b b_ao ao_uh uh_t t_pau"},
{"book","pau_b b_uh uh_k k_pau"},
{"bowls","pau_b b_ao ao_uh uh_l l_z z_pau"},
{"boxing","pau_b b_aa aa_aa aa_k k_s s_ih ih_ng ng_pau"},
{"break","pau_b b_r r_eh eh_iy iy_k k_pau"},
{"breakfast","pau_b b__r r_eh eh_eh eh_k k_f f_ah ah_s s_t t_pau"},
{"british","pau_b b_r r_ih ih_t t_ih ih_sh sh_pau"},
{"brunch","pau_b b_r r_r r_ah ah_n n_n n_ch ch_ch ch_pau"},
{"bus","pau_b b_ah ah_ah ah_s s_s s_pau"},
{"business","b_ih ih_z z_n n_ah ah_s s_pau"},
{"buy","pau_b b_aa aa_iy iy_pau"},
{"bye","pau_b b_aa aa_iy iy_pau"},


//c
{"cabbie","pau_k k_ae ae_ae ae_b b_b b_iy iy_pau"},
{"cafe","pau_k k_ah ah_f f_ey ey_ey"},
{"calendar","pau_k k_ae ae_ae ae_l l_ah ah_n n_d d_er er_pau"},
{"call","pau_k k_ao ao_l l_pau"},
{"campsite","pau_k k_ae ae_m m_p p_s s_aa aa_iy iy_t t_s s_pau"},
{"car","pau_k k_aa aa_aa aa_r r_pau"},
{"card","pau_k k_aa aa_r r_d d_pau"},
{"care","pau_k k_eh eh_r r_pau"},
{"cat","pau_k k_ae ae_t t_pau"},
{"certificate","pau_s s_uh uh_r r_t t_ih ih_f f_ih ih_k k_ah ah_t t_pau"},
{"check","pau_ch ch_eh eh_k k_pau"},
{"chemist","pau_k k_eh eh_m m_ih ih_s s_t t_pau"},
{"chips","pau_ch ch_ih ih_ih ih_p p_s s_pau"}, //dog name  walk chips
{"choir","pau_k k_w w_aa aa_iy iy_uh uh_r r_pau"},
{"chore","pau_ch ch_ao ao_ao ao_r r_pau"},
{"christmas","pau_k k_r r_ih ih_ih ih_s s_m m_ah ah_s s_pau"},
{"church","pau_ch ch_er er_er er_ch ch_pau"},
{"cinema","s_ih ih_ih ih_n n_ah ah_m m_ah ah_pau"},
{"circle","pau_s s_uh uh_r r_k k_ah ah_l l_pau"},
{"circus","pau_s s_uh uh_r r_k k_ah ah_s s_pau"},
{"city","pau_s s_ih ih_t t_iy iy_pau"},
{"clean","pau_k k_l l_iy iy_n n_pau"},
{"cleaner","pau_k k_l l_iy iy_n n_uh uh_r r_pau"},
{"climbing","pau_k k_l l_aa aa_iy iy_m m_ih ih_ng ng_pau"},
{"clinic","pau_k k_l l_ih ih_ih ih_n n_ih ih_k k_pau"},
{"clock","pau_k k_l l_aa aa_k k_pau"},
{"club","pau_k k_l l_ah ah_b b_pau"},
{"coach","pau_k k_ow ow_ow ow_ch ch_ch ch_pau"},
{"coast","pau_k k_ao ao_uh uh_s s_t t_pau"},
{"cobbler","pau_k k_aa aa_aa aa_b b_b b__l l_er er_pau"},
{"code","pau_k k_ao ao_uh uh_d d_pau"},
{"coffee","pau_k k_aa aa_f f_iy iy_pau"},
{"collection","pau_k k_ah ah_l l_eh eh_k k_sh sh_ah ah_n n_pau"},
{"college","pau_k k_aa aa_l l_ih ih_jh jh_pau"},
{"coming","pau_k k_ah ah_ah ah_m m_ih ih_ng ng_pau"},
{"computer","pau_k k_ah ah_m m_p p_y y_uw uw_t t_uh uh_r r_pau"},
{"concert","pau_k k_aa aa_n n_s s_uh uh_r r_t t_pau"},
{"contact","pau_k k_aa aa_n n_t t_ae ae_k k_t t_pau"},
{"cottage","pau_k k_aa  aa_aa aa_t t_ah ah_jh jh_pau"},
{"court","pau_k k_ao ao_r r_t t_pau"},
//d
{"dad","pau_d d_ae ae_ae ae_d d_pau"},
{"date","pau_d d_ey ey_ey ey_t t_pau"},
{"day","pau_d d_d d_ey ey_ey ey_pau"},
{"deadline","pau_d d_eh eh_eh eh_d d_l l_ay ay_ay ay_n n_pau"},
{"dealer","pau_d d_iy iy_l l_uh uh_r r_pau"},
{"death","pau_d d_eh eh_th th_pau"},
{"decorate","pau_d d_eh eh_k k_uh uh_r r_eh eh_iy iy_t t_pau"},
{"decorating","pau_d d_eh eh_k k_uh uh_r r_eh eh_iy iy_t t_ih ih_ng ng_pau"},
{"delivery","pau_d d_ih ih_l l_ih ih_ih ih_v v_er er_iy iy_pau"},
{"dentist","pau_d d_eh eh_eh eh_n n_t t_ah ah_s s_t t_pau"},
{"development","pau_d d_ih ih_v v_eh eh_l l_ah ah_p p_m m_ah ah_n n_t t_pau"},
{"diary","pau_d d_aa aa_iy iy_uh uh_r r_iy iy_pau"},
{"dinner","pau_d d_ih ih_ih ih_n n_er er_pau"},
{"doctor","pau_d d_aa aa_aa aa_k k_t t_er er_pau"},
{"dog","pau_d d_ao ao_g g_pau"},
{"donkey","pau_d d_aa aa_ng ng_k k_iy iy_pau"},
{"driver","pau_d d_r r_ay ay_ay ay_v v_er er_pau"},
{"dustbin","pau_d d_ah ah_ah ah_s s_t t_b b_ih ih_n n_pau"},
//e
{"ear","pau_iy iy_iy iy_r r_pau"},
{"easter","pau_iy iy_iy iy_s s_t t_er er_pau"},
{"education","pau_eh eh_jh jh_ah ah_k k_eh eh_iy iy_sh sh_ah ah_n n_pau"},
{"electrician","pau_ih ih_ih ih_l l_eh eh_k k_t t_r r_ih ih_ih ih_sh sh_ah ah_n n_pau"},
{"electricity","pau_ih ih_l l_eh eh_k k_t t_r r_ih ih_s s_ah ah_t t_iy iy_pau"},
{"engagement","pau_eh eh_n n_g g_eh eh_iy iy_jh jh_m m_ah ah_n n_t t_pau"},
{"evening","pau_iy iy_iy iy_v v_n n_ih ih_ng ng_pau"},
{"event","pau_ih ih_v v_eh eh_eh eh_n n_t t_pau"},
{"events","pau_ih ih_v v_eh eh_n n_t t_z z_pau"},
{"eye","pau_aa aa_iy iy_pau"},
//f
{"family","pau_f f_ae ae_ae ae_m m_ah ah_l l_iy iy_pau"},
{"friend","pau_f f_r r_eh eh_eh eh_n n_d d_pau"},
{"funeral","pau_f f_y y_uw uw_uw uw_n n_er er_ah ah_l l_pau"},
//g
{"gallery","pau_g g_ae ae_ae ae_l l_er er_iy iy_pau"},
{"game","pau_g g_eh eh_iy iy_m m_pau"},
{"garage","pau_g g_uh uh_r r_aa aa_zh zh_pau"},
{"garbage","pau_g g_aa aa_aa aa_r r_b b_ih ih_jh jh_pau"},
{"garden","pau_g g_aa aa_r r_d d_ah ah_n n_pau"},
{"gas","pau_g g_ae ae_ae ae_s s_s s_pau"},
{"glasses","pau_g g_l l_ae ae_ae ae_s s_ah ah_z z_pau"},
{"go","pau_g g_ao ao_uh uh_pau"},
{"golf","pau_g g_aa aa_l l_f f_pau"},
{"good","pau_g g_uh uh_d d_pau"},
{"grandad","pau_g g_r r_ae ae_ae ae_n n_d d_ae ae_ae ae_d d_pau"},
{"grandfather","pau_g g_r r_ae ae_n n_d d_f f_aa aa_dh dh_uh uh_r r_pau"},
{"grandmother","pau_g g_r r_ae ae_n n_d d_m m_ah ah_dh dh_uh uh_r r_pau"},
{"granny","pau_g g_r r_ae ae_ae ae_n n_iy iy_pau"},
//h
{"hairdresser","pau_hh hh_eh eh_eh eh_r r_d d_r r_eh eh_eh eh_s s_er er_pau"},
{"happy", "pau_hh hh_ae ae_p p_iy iy_pau"},
{"halloween","pau_hh hh_ae ae_ae ae_l l_ah ah_w w_iy iy_iy iy_n n_pau"},
{"health","pau_hh hh_eh eh_eh eh_l l_th th_pau"},
{"heating","pau_hh hh_iy iy_iy iy_t t_ih ih_ng ng_pau"},
{"hello","pau_hh hh_ah ah_l l_ow ow_ow ow_pau"},
{"help","pau_hh hh_eh eh_eh eh_l l_p p_pau"},
{"high","pau_hh hh_aa aa_iy iy_pau"},
{"hobby","pau_hh hh_aa aa_b b_iy iy_pau"},
{"hockey","pau_hh hh_aa aa_aa aa_k kiy iy_pau"},
{"holiday","pau_hh hh_aa aa_aa aa_l l_ah ah_d d_ey ey_ey ey_pau"},
{"home","pau_hh hh_ow ow_ow ow_m m_pau"},
{"hospital","pau_hh hh_aa aa_aa aa_s s_p p_ih ih_ih ih_t t_ah ah_l l_pau"},
{"hotel","pau_hh hh_ow ow_t t_eh eh_eh eh_l l_pau"},
{"house","pau_hh hh_hh hh_aw aw_aw aw_s s_s s_pau"},
//i
{"industry","pau_ih ih_n n_d d_ah ah_s s_t t_r r_iy iy_pau"},
{"information","pau_ih ih_n n_f f_uh uh_r r_m m_eh eh_iy iy_sh sh_ah ah_n n_pau"},
{"inn","pau_ih ih_ih ih_ih ih_n n_pau"},
{"insurance","pau_ih ih_n n_sh sh_uh uh_r r_ah ah_n n_s s_pau"},
{"investment","pau_ih ih_n n_v v_eh eh_s s_t t_m m_ah ah_n n_t t_pau"},
{"invoice","pau_ih ih_n n_v v_ao ao_iy iy_s s_pau"},
//j
{"jeweler","pau_jh jh_uw uw_ah ah_l l_uh uh_r r_pau"},
{"job","pau_jh jh_aa aa_aa aa_b b_b b_pau"},
{"journey","pau_jh jh_er er_er er_n n_iy iy_pau"},
//k
{"king","pau_k k_ih ih_ng ng_pau"},
{"kingdom","pau_k k_ih ih_ng ng_d d_ah ah_m m_pau"},
//l
{"laboratory","pau_l l_ae ae_b b_r r_ah ah_t t_ao ao_r r_iy iy_pau"},
{"last","pau_l l_ae ae_s s_t t_pau"},
{"lawyer","pau_l l_ao ao_y y_uh uh_r r_pau"},
{"lecture","pau_l l_eh eh_eh eh_k k_ch ch_er er_pau"},
{"lesson","pau_l l_eh eh_s s_ah ah_n n_pau"},
{"library","pau_l l_aa aa_iy iy_b b_r r_eh eh_r r_iy iy_pau"},
{"license","pau_l l_aa aa_iy iy_s s_ah ah_n n_s s_pau"},
{"lift","pau_l l_ih ih_ih ih_f f_t t_pau"},
{"location","pau_l l_ao ao_uh uh_k k_eh eh_iy iy_sh sh_ah ah_n n_pau"},
{"lodge","pau_l l_aa aa_jh"},
{"long","pau_l l_ao ao_ng"},
{"look","pau_l l_uh uh_k"},
{"lunch","pau_l l_ah ah_ah ah_n n_ch ch_pau"},

//m
{"magazine","pau_m m_ae ae_g g_ah ah_z z_iy iy_n n_pau"},
{"magistrate","pau_m m_ae ae_jh jh_ah ah_s s_t t_r r_eh eh_iy iy_t t_pau"},
{"maintenance","pau_m m_ey ey_ey ey_n n_t t_ah ah_n n_ah ah_n n_s s_s s_pau"},
{"management","pau_m m_ae ae_ae ae_n n_ah ah_jh jh_m m_ah ah_n n_t t_pau"},
{"meal","pau_m m_iy iy_iy iy_l l_pau"},
{"mechanic","pau_m m_ah ah_k k_ae ae_n n_ih ih_k k_pau"},
{"medical","pau_m m_eh eh_eh eh_d d_ah ah_k k_ah ah_l l_pau"},
{"meeting","pau_m m_iy iy_iy iy_t t_ih ih_ng ng_pau"},
{"meetup","pau_m m_iy iy_t t_pau pau_pau pau_ah ah_p p_pau"},
{"member","pau_m m_eh eh_m m_b b_uh uh_r r_pau"},
{"memo","pau_m m_eh eh_eh eh_m m_ao ao_uh uh_pau"},
{"memorandum","pau_m m_eh eh_eh eh_m m_er er_ae ae_ae ae_n n_d d_ah ah_m m_pau"},
{"message","pau_m m_eh eh_s s_ah ah_jh jh_pau"},
{"midnight","pau_m m_ih ih_d d_n n_aa aa_iy iy_t t_pau"},
{"month","pau_m m_ah ah_n n_th th_pau"},
{"morning","pau_m m_ao ao_ao ao_r r_n n_ih ih_ng ng_pau"},
{"mother","pau_m m_ah ah_ah ah_dh dh_er er_pau"},
{"motorcycle","pau_m m_ow ow_ow ow_t t_er er_s s_ay ay_ay ay_k k_ah ah_l l_pau"},
{"moving","pau_m m_uw uw_v v_ih ih_ng ng_pau"},
{"movie","pau_m m_uw uw_uw uw_v v_iy iy_pau"},
{"mum","pau_m m_ah ah_ah ah_m m_pau"},
{"music","pau_m m_y y_uw uw_uw uw_z z_ih ih_k k_pau"},
//n
{"national","pau_n n_ae ae_sh sh_ah ah_n n_ah ah_l l_pau"},
{"neighbour","pau_n n_eh eh_iy iy_b b_uh uh_r r_pau"},
{"netflix","pau_n n_eh eh_eh eh_t t_f f_l l_ih ih_ih ih_k k_k k_pau"},
{"new","pau_n n_uw uw_uw uw_pau"}, //new year
{"news","pau_n n_uw uw_z z_pau"},
{"newspaper","pau_n n_uw uw_z z_p p_eh eh_iy iy_p p_uh uh_r r_pau"},
{"night","pau_n n_aa aa_iy iy_t t_pau"},
{"north","pau_n n_ao ao_r r_th th_pau"},
{"notable","pau_n n_ao ao_uh uh_t t_ah ah_b b_ah ah_l l_pau"},
{"notification","pau_n n_ow ow_ow ow_t t_ah ah_f f_ah ah_k k_ey ey_ey ey_sh sh_ah ah_n n_pau"},
{"now","pau_n n_aw aw_aw aw_pau"},
{"nurse","pau_n n_uh uh_r r_s s_pau"},
{"nursery","pau_n n_er er_er er_s s_er er_iy iy_pau"},
//o
{"off","pau_ao ao_f f_pau"},
{"offer","pau_ao ao_f f_uh uh_r r_pau"},
{"office","pau_ao ao_ao ao_f f_ah ah_s s_pau"},
{"online","pau_aa aa_aa aa_n n_n n_l l_aa aa_iy iy_n n_pau"},
{"open","pau_ao ao_uh uh_p p_ah ah_n n_pau"},
{"orchestra","pau_ao ao_r r_k k_ah ah_s s_t t_r r_ah ah_pau"},
{"out","pau_aa aa_uh uh_t t_pau"},
{"outbreak","pau_aa aa_uh uh_t t_b b_r r_eh eh_iy iy_k k_pau"},
{"outcome","pau_aa aa_uh uh_t t_k k_ah ah_m m_pau"},
{"outhouse","pau_aa aa_uh uh_t t_hh hh_aa aa_uh uh_s s_pau"},
{"outing","pau_aa aa_uh uh_t t_ih ih_ng ng_pau"},
{"over","pau_ao ao_uh uh_v v_uh uh_r r_pau"},
{"own","pau_ow ow_ow_ow_ow ow_n n_pau"},
//p
{"pancake","pau_p p_ae ae_ae ae_n n_k k_ey ey_ey ey_k k_pau"},
{"paper","pau_p p_eh eh_iy iy_p p_uh uh_r r_pau"},
{"parent","pau_p p_eh eh_r r_ah ah_n n_t t_pau"},
{"party","pau_p p_aa aa_aa aa_r r_t t_iy iy_pau"},
{"passport","pau_p p_ae ae_ae ae_s s_p p_ao ao_ao ao_r r_t t_pau"},
{"payment","pau_p p_ey ey_ey ey_m m_ah ah_n n_t t_pau"},
{"piano","pau_p p_iy iy_ae ae_n n_ao ao_uh uh_pau"},
{"picture","pau_p p_ih ih_ih ih_k k_ch ch_er er_pau"},
{"pilates","pau_p p_ih ih_ih ih_l l_aa aa_aa aa_t t_iy iy_iy iy_s s_pau"},
{"point","pau_p p_oy oy_oy oy_n n_t t_pau"},
{"postoffice","pau_p p_ao ao_uh uh_s s_t t_ao ao_f f_ah ah_s s_pau"},
{"practice","pau_p p_r r_ae ae_k k_t t_ah ah_s s_pau"},
{"premier","pau_p p_r r_eh eh_m m_ih ih_ih ih_r r_pau"},
{"prescription","pau_p p_r r_ah ah_s s_k k_r r_ih ih_p p_sh sh_ah ah_n n_pau"},
{"present","pau_p p_r r_eh eh_z z_ah ah_n n_t t_pau"},
{"pressure","pau_p p_r r_eh eh_sh sh_uh uh_r r_pau"},
{"priority","pau_p p_r r_aa aa_iy iy_ao ao_r r_ah ah_t t_iy iy_pau"},
{"prison","pau_p p_r r_ih ih_z z_ah ah_n n_pau"},
{"private","pau_p p_r r_aa aa_iy iy_v v_ah ah_t t_pau"},
{"professional","pau_p p_r ah_ah ah_f f_eh eh_eh eh_sh sh_ah ah_n n_ah ah_l l_pau "},
{"professor","pau_p p_r ah_ah ah_f f_eh eh_eh eh_s s_er er_pau"},
{"program","pau_p p_r r_ow ow_ow ow_g g_r r_ae ae_ae ae_m m_pau"},
{"programme","pau_p p_r r_ow ow_ow ow_g g_r r_ae ae_ae ae_m m_pau"},
{"project","pau_p p_r r_aa aa_aa aa_jh jh_eh eh_k k_t t_pau"},
{"pub","pau_p p_ah ah_ah ah_b b_pau"},
{"public","pau_p p_ah ah_ah ah_b b_l l_ih ih_k k_pau"},
{"purchase","pau_p p_uh uh_r r_ch ch_ah ah_s s_pau"},
//q
{"queen","pau_k k_w w_iy iy_n n_pau"},
{"quick","pau_k k_w w_ih ih_k k_pau"},
{"quiz","pau_k k_w w_ih ih_ih ih_z z_pau"},
{"quote","pau_k k_w w_w w_ow ow_ow ow_t t_pau"},

//r
{"radio","pau_r r_ey ey_ey ey_d d_iy iy_ow ow_ow ow_pau"},
{"railway","pau_r r_eh eh_iy iy_l l_w w_eh eh_iy iy_pau"},
{"rain","pau_r r_eh eh_iy iy_n n_pau"},
{"reminder","pau_r r_iy iy_m m_ay ay_ay ay_n n_d d_er er_pau"},
{"renewal","pau_r r_ih ih_n n_uw uw_ah ah_l l_pau"},
{"repair","pau_r r_ih ih_p p_eh eh_r r_pau"},
{"rest","pau_r r_eh eh_eh eh_s s_t t_pau"},
{"restaurant","pau_r r_eh eh_eh eh_s s_t t_er er_aa aa_aa aa_n n_t t_pau"},
{"retirement","pau_r r_iy iy_t t_ay ay_er er_m m_ah ah_n n_t t_pau"},
{"royal","pau_r r_ao ao_iy iy_ah ah_l l_pau"},
{"rugby","pau_r r_ah ah_g g_b b_iy iy_pau"},


//s
{"saint","pau_s s_ey ey_ey ey_n n_t t_pau"},
{"say","pau_s s_eh eh_iy iy_pau"},
{"school","pau_s s_k k_uw uw_l l_pau"},
{"seaside","pau_s s_iy iy_s s_aa aa_iy iy_d d_pau"},
{"season","pau_s s_iy iy_z z_ah ah_n n_pau"},
{"shoe","pau_sh sh_uw uw_pau"},
{"shop","pau_sh sh_aa aa_aa aa_p p_pau"},
{"shopping","pau_sh sh_aa aa_aa aa_p p_ih ih_ng n_pau"},
{"short","pau_sh sh_ao ao_r r_t t_pau"},
{"show","pau_sh sh_ow ow_ow ow_pau"},
{"small","pau_s s_m m_ao ao_l l_pau"},
{"society","pau_s s_ow ow_s s_ay ay_ay ay_ih ih_t t_iy iy_pau"},
{"special","pau_s s_p p_eh eh_eh eh_sh sh_ah ah_l l_pau"},
{"sport","pau_s s_p p_ao ao_ao ao_r r_t t_pau"},
{"spring","pau_s s_p p_r r_ih ih_ih ih_ng ng_pau"},
{"stag","pau_s s_t t_ae ae_g g_pau"},
{"star","pau_s s_t t_aa aa_r r_pau"},
{"start","pau_s s_t t_aa aa_r r_t t_pau"},
{"station","pau_s s_t t_ey ey_ey ey_sh sh_ah ah_n n_pau"},
{"stay","pau_s s_t t_eh eh_iy iy_pau"},
{"stock","pau_s s_t t_aa aa_k k_pau"},
{"store","pau_s s_t t_ao ao_r r_pau"},
{"study","pau_s s_t t_ah ah_d d_iy iy_pau"},
{"summer","pau_s s_ah ah_m m_uh uh_r r_pau"},
{"summertime","pau_s s_ah ah_m m_uh uh_r r_t t_aa aa_iy iy_m m_pau"},
{"system","pau_s s_ih ih_s s_t t_ah ah_m m_pau"},
//t
{"talk","pau_t t_ao ao_ao ao_k k_pau"},
{"task","pau_t t_ae ae_ae ae_s s_k k_pau"},
{"tax","pau_t t_ae ae_ae ae_k k_s s_pau"},
{"taxi","pau_t t_ae ae_ae ae_k k_s s_iy iy_pau"},
{"tea","pau_t t_t t_iy iy_iy iy_pau"},
{"tearoom","pau_t t_r r_uh uh_r r_uw uw_m m_pau"},
{"team","pau_t t_iy iy_m m_pau"},
{"telephone","pau_t t_eh eh_eh eh_l l_ah ah_f f_f f_ow ow_ow ow_ow ow_n n_pau"},
{"television","pau_t t_eh eh_eh eh_l l_ah ah_v v_ih ih_ih ih_zh zh_ah ah_n n_pau"},
{"temperature","pau_t t_eh eh_m m_p p_r r_ah ah_ch ch_uh uh_r r_pau"},
{"tennis","pau_t t_eh eh_n n_ah ah_s s_pau"},
{"test","pau_t t_eh eh_eh eh_s s_t t_pau"},
{"thank","pau_th th_ae ae_ng ng_k k_pau"},
{"theater","pau_th th_iy iy_iy iy_ey ey_t t_er er_pau"},
{"today","pau_t t_ah ah_d d_ey ey_ey ey_pau"},
{"toll","pau_t t_ao ao_uh uh_l l_pau"},
{"tomorrow","pau_t t_ah ah_m m_aa aa_r r_ao ao_uh uh_pau"},
{"train","pau_t t_r r_eh eh_iy iy_n n_pau"},
{"transport","pau_t t_r r_ae ae_n n_s s_p p_ao ao_r r_t t_pau"},
{"travel","pau_t t_r r_ae ae_ae ae_v v_ah ah_l l_pau"},
{"travelodge","pau_t t_r r_ae ae_ae ae_v v_ah ah_l l_aa aa_aa aa_jh jh_pau"},
{"trip","pau_t t_r r_ih ih_ih ih_p p_p p_pau"},
{"tunnel","pau_t t_ah ah_n n_ah ah_l l_pau"},
{"turn","pau_t t_uh uh_r r_n n_pau"},
{"tv","pau_t t_iy iy_iy iy_v v_iy iy_iy iy_pau"},
//u
{"uncle","pau_ah ah_ah ah_ng ng_k k_ah ah_l l_pau"},
{"university","pau_y y_uw uw_n n_ah ah_v v_uh uh_r r_s s_ah ah_t t_iy iy_pau"},
{"unknown","pau_ah ah_n n_n n_ow ow_ow ow_n n_n n_pau"},
{"upcoming","pau_ah ah_p p_k k_ah ah_ah ah_m m_ih ih_ng ng_pau"},
{"use","pau_y y_uw uw_uw uw_z z_pau"},
{"user","pau_y y_uw uw_uw uw_z z_er er_pau"},
//v
{"vacation","pau_v v_ey ey_k k_ey ey_ey ey_sh sh_ah ah_n n_pau"},
{"valentine","pau_v v_ae ae_ae ae_l l_ah ah_n n_t t_ay ay_ay ay_n n_pau"},
{"vehicle","pau_v v_iy iy_iy iy_hh hh_ih ih_k k_ah ah_l l_pau"},
{"venue","pau_v v_eh eh_n n_y y_uw uw_pau"},
{"version","pau_v v_er er_er er_zh zh_ah ah_n n_pau"},
{"vicar","pau_v v_ih ih_k k_uh uh_r r_pau"},
{"vicarage","pau_v v_ih ih_k k_uh uh_r r_r r_aa aa_jh jh_pau"},
{"visit","pau_v v_ih ih_ih ih_z z_ih ih_t t_pau"},
//w
{"walk","pau_w w_w w_ao ao_ao ao_k k_k k_pau"},
{"warning","pau_w w_ao ao_r r_n n_ih ih_ng ng_pau"},
{"washing","pau_w w_aa aa_sh sh_ih ih_ng ng_pau"},
{"water","pau_w w_ao ao_ao ao_t t_er er_pau"},
{"way","pau_w w_ey ey_iy iy_pau"},
{"weather","pau_w w_eh eh_dh dh_uh uh_r r_pau"},
{"web","pau_w w_eh eh_eh eh_b b_b b_pau"},
{"wedding","pau_w w_eh eh_eh eh_d d_ih ih_ng ng_pau"},
{"week","pau_w w_iy iy_k k_pau"},
{"weekend","pau_w w_iy iy_k k_eh eh_n n_d d_pau"},
{"west","pau_w w_eh eh_s s_t t_pau"},
{"window","pau_w w_ih ih_n n_d d_ao ao_uh uh_pau"},
{"winter","pau_w w_ih ih_ih ih_n n_t t_er er_pau"},
{"work","pau_w w_uh uh_r r_k k_pau"},
{"workman","pau_w w_uh uh_r r_k k_m m_ah ah_n n_pau"},
{"world","pau_w w_uh uh_l l_d"},

//y
{"yacht","pau_y y_aa aa_aa aa_t t_pau"},
{"year","pau_y y_ih ih_ih ih_r r_pau"},
{"yearly","pau_y y_ih ih_ih ih_r r_l l_iy iy_pau"},
{"yesterday","pau_y y_eh eh_s s_t t_er er_d d_ey ey_pau"},
{"yodel","pau_y y_ao ao_uh uh_d d_ah ah_l l_pau"},
{"you","pau_y y_uw uw_uw uw_pau"},
{"youth","pau_y y_uw uw_th th_pau"},

//z_words
{"zone","pau_z z_ao ao_uh uh_n n_pau"},
{"zoo","pau_z z_uw uw_pau"},

	
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
