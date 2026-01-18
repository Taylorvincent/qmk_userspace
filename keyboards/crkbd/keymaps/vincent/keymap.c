#include QMK_KEYBOARD_H
#include "tapdance.c"

#define TD_COMM TD(COMM_TD)
#define TD_DOT TD(DOT_TD)
#define TD_SLSH TD(SLASH_TD)
#define TD_OS_SHFT_4 TD(OS_SHFT_4_TD)

#include "combos.c"
#include "overrides.c"


const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT(
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R', 
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R', 
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R', 
        // allow thumb cmd key to trigger same hand faster
                  '*', '*', '*',  '*', '*', '*'
    );


// Layer names
enum layer_names {
    _BASE,
    _L1,
    _L2,
    _L3,
    _L4,
};

// Keymap definition
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x5_3(
        KC_B,    KC_L,    KC_D,    KC_W,    KC_Q,         KC_J,    KC_F,    KC_O,    KC_U,    KC_QUOT,
        LCTL_T(KC_N), LALT_T(KC_R), LSFT_T(KC_T), LT(1, KC_S), KC_G,         KC_Y,    LT(3, KC_H), LSFT_T(KC_A), LALT_T(KC_E), LCTL_T(KC_I),
        KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,         KC_K,    KC_P,    TD_COMM, KC_DOT,  KC_SLSH,
                          KC_TRNS, LGUI_T(KC_BSPC), LT(2, KC_ENTER),         TD_OS_SHFT_4, LGUI_T(KC_SPC), KC_TRNS
    ),
    [_L1] = LAYOUT_split_3x5_3(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_PLUS, KC_7,    KC_8,    KC_9,    KC_ASTR,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_MINS, KC_0,    KC_1,    KC_2,    KC_3,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_EQL,  KC_4,    KC_5,    KC_6,    KC_BSLS,
                          KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_L2] = LAYOUT_split_3x5_3(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_MPLY, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,
                          KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS
    ),
										
    [_L3] = LAYOUT_split_3x5_3(
        KC_GRV, KC_DLR,KC_AMPR, KC_PIPE, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_ASTR, KC_LPRN, KC_LCBR, KC_LBRC, KC_CIRC,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_PERC, KC_MINS, KC_PLUS, KC_LT, KC_BSLS,       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS, KC_TRNS,  KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS
    ),
    [_L4] = LAYOUT_split_3x5_3(
        RM_TOGG, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU,      KC_TRNS, KC_F7,   KC_F8,   KC_F9,   KC_F10,
        QK_RBT,  RM_PREV, RM_HUED, RM_SATD, RM_VALD,       KC_TRNS, KC_F12,  KC_F1,   KC_F2,   KC_F3,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_F4,   KC_F5,   KC_F6,   KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS
    ),
};

// Tri-layer: when both L1 and R1 are active, activate ADJUST layer
// layer_state_t layer_state_set_user(layer_state_t state) {
//     return update_tri_layer_state(state, 2, 3, 4);
// }
 