enum combo_events {
    COMBO_LD,
    COMBO_RT,
    COMBO_DW,
    COMBO_P_COMMA,
    COMBO_GY,
    VI_EMAIL,
    COMBO_THUMB,
    COMBO_U_QUOTE,
    COMBO_YH,
    COMBO_SG,
    COMBO_COMMA_DOT,
    COMBO_DOT_SLASH,
};

/**
 b l d w q   j f o u ' 
 n r t s g   y h a e i 
 z x m c v   k p , . /    
*/

// 1 hand combos
const uint16_t PROGMEM ld_combo[] = {KC_L, KC_D, COMBO_END};
const uint16_t PROGMEM rt_combo[] = {LALT_T(KC_R), LSFT_T(KC_T), COMBO_END};
const uint16_t PROGMEM dw_combo[] = {KC_D, KC_W, COMBO_END};
const uint16_t PROGMEM p_comma_combo[] = {KC_P, KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_thumb[] = {LGUI_T(KC_BSPC), LGUI_T(KC_SPC), COMBO_END};
const uint16_t PROGMEM u_quote_combo[] = {KC_U, KC_QUOT, COMBO_END};
const uint16_t PROGMEM yh_combo[] = {KC_Y, LT(3, KC_H), COMBO_END};
const uint16_t PROGMEM sg_combo[] = {LT(1, KC_S), KC_G, COMBO_END};
const uint16_t PROGMEM comma_dot_combo[] = {TD_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM dot_slash_combo[] = {KC_DOT, KC_SLASH, COMBO_END};

// 2 hand combos
const uint16_t PROGMEM gy_combo[] = {KC_G, KC_Y, COMBO_END};
const uint16_t PROGMEM vi_email_combo[] = {KC_V, LCTL_T(KC_I), COMBO_END};

combo_t key_combos[] = {
    [COMBO_LD] = COMBO(ld_combo, KC_ESC),
    [COMBO_RT] = COMBO(rt_combo, KC_TAB),
    [COMBO_DW] = COMBO(dw_combo, KC_BSPC),
    [COMBO_P_COMMA] = COMBO(p_comma_combo, KC_MINUS),
    [COMBO_GY] = COMBO(gy_combo, CW_TOGG),
    [VI_EMAIL] = COMBO_ACTION(vi_email_combo),
    [COMBO_THUMB] = COMBO(combo_thumb, KC_LSFT),
    [COMBO_U_QUOTE] = COMBO(u_quote_combo, KC_DQUO),
    [COMBO_YH] = COMBO(yh_combo, KC_MINUS),
    [COMBO_SG] = COMBO(sg_combo, KC_MINUS),
    [COMBO_COMMA_DOT] = COMBO(comma_dot_combo, KC_EXLM),
    [COMBO_DOT_SLASH] = COMBO(dot_slash_combo, KC_EXLM),
};

// Per-combo terms: lower for homerow mod combos to avoid accidental triggers
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case COMBO_LD:  // ld_combo (KC_L, KC_D)
        case COMBO_RT:  // rt_combo (LALT_T(KC_R), LSFT_T(KC_T)) - homerow mods
        case COMBO_THUMB:  // combo_thumb (LGUI_T(KC_BSPC), LGUI_T(KC_SPC))
            return 20;
        default:
            return COMBO_TERM;  // Default 35ms
    }
}

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case VI_EMAIL:
            if (pressed) {
                SEND_STRING("vincent.sam.taylor@gmail.com");
            }
            break;
    }
}
