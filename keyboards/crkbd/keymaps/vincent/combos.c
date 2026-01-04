/**
 b l d w q   j f o u ' 
 n r t s g   y h a e i 
 z x m c v   k p , . /    
*/

const uint16_t PROGMEM ld_combo[] = {KC_L, KC_D, COMBO_END};
const uint16_t PROGMEM dw_combo[] = {KC_D, KC_W, COMBO_END};
const uint16_t PROGMEM xm_combo[] = {KC_X, KC_M, COMBO_END};
const uint16_t PROGMEM mc_combo[] = {KC_M, KC_C, COMBO_END};
const uint16_t PROGMEM fo_combo[] = {KC_F, KC_O, COMBO_END};
const uint16_t PROGMEM ou_combo[] = {KC_O, KC_U, COMBO_END};
const uint16_t PROGMEM p_comma_combo[] = {KC_P, KC_COMMA, COMBO_END};
const uint16_t PROGMEM comma_period_combo[] = {KC_COMMA, KC_PERIOD, COMBO_END};

const uint16_t PROGMEM gy_combo[] = {KC_G, KC_Y, COMBO_END};
combo_t key_combos[] = {
    COMBO(dw_combo, KC_ESC),
    COMBO(ld_combo, KC_ESC),
    COMBO(rt_combo, KC_TAB),
    COMBO(ha_combo, KC_BSPC),
    COMBO(ae_combo, KC_DEL),
    combo(fo_combo, KC_BSPC),
    combo(ou_combo, KC_BSPC),
    combo(p_comma_combo, KC_DEL),
    combo(comma_period_combo, KC_ESC),

    COMBO(gy_combo, CW_TOGG),
};