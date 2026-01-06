#include QMK_KEYBOARD_H

enum combo_events {
    VI_EMAIL,
};

/**
 b l d w q   j f o u ' 
 n r t s g   y h a e i 
 z x m c v   k p , . /    
*/

// 1 hand combos
const uint16_t PROGMEM ld_combo[] = {KC_L, KC_D, COMBO_END};
const uint16_t PROGMEM dw_combo[] = {KC_D, KC_W, COMBO_END};
const uint16_t PROGMEM xm_combo[] = {KC_X, KC_M, COMBO_END};
const uint16_t PROGMEM mc_combo[] = {KC_M, KC_C, COMBO_END};
const uint16_t PROGMEM rt_combo[] = {LALT_T(KC_R), LSFT_T(KC_T), COMBO_END};
const uint16_t PROGMEM fo_combo[] = {KC_F, KC_O, COMBO_END};
const uint16_t PROGMEM ou_combo[] = {KC_O, KC_U, COMBO_END};
const uint16_t PROGMEM ha_combo[] = {LGUI_T(KC_H), LSFT_T(KC_A), COMBO_END};
const uint16_t PROGMEM ae_combo[] = {LSFT_T(KC_A), LALT_T(KC_E), COMBO_END};
const uint16_t PROGMEM p_comma_combo[] = {KC_P, KC_COMMA, COMBO_END};
const uint16_t PROGMEM comma_period_combo[] = {KC_COMMA, KC_DOT, COMBO_END};

// 2 hand combos
const uint16_t PROGMEM gy_combo[] = {KC_G, KC_Y, COMBO_END};
const uint16_t PROGMEM vi_email_combo[] = {KC_V, KC_I, COMBO_END};

combo_t key_combos[] = {
    COMBO(dw_combo, KC_ESC),
    COMBO(ld_combo, KC_ESC),
    COMBO(rt_combo, KC_TAB),
    COMBO(ha_combo, KC_BSPC),
    COMBO(ae_combo, KC_ENTER),
    COMBO(fo_combo, KC_BSPC),
    COMBO(ou_combo, KC_BSPC),
    COMBO(p_comma_combo, KC_DEL),
    COMBO(comma_period_combo, KC_MINUS),
    COMBO(gy_combo, CW_TOGG),
    [VI_EMAIL] = COMBO_ACTION(vi_email_combo),
};

// Tri-layer: when both L1 and R1 are active, activate ADJUST layer
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, 1, 3, 4);
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


