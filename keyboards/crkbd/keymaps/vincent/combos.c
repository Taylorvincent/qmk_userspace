const uint16_t PROGMEM dw_combo[] = {KC_D, KC_W, COMBO_END};
const uint16_t PROGMEM ld_combo[] = {KC_L, KC_D, COMBO_END};
const uint16_t PROGMEM rt_combo[] = {KC_R, KC_T, COMBO_END};
const uint16_t PROGMEM ha_combo[] = {KC_H, KC_A, COMBO_END};
const uint16_t PROGMEM ae_combo[] = {KC_A, KC_E, COMBO_END};
// const uint16_t PROGMEM ei_combo[] = {KC_E, KC_I, COMBO_END};
combo_t key_combos[] = {
    COMBO(dw_combo, KC_ESC),
    COMBO(ld_combo, KC_ESC),
    COMBO(rt_combo, KC_TAB),
    COMBO(ha_combo, KC_BSPC),
    COMBO(ae_combo, KC_DEL),
    // COMBO(ei_combo, KC_BSPC),
};