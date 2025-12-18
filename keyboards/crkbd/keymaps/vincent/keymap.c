#include QMK_KEYBOARD_H
#include "sm_td.h"

// Colemak DH layout
#define CM_Q    KC_Q
#define CM_W    KC_W
#define CM_F    KC_F
#define CM_P    KC_P
#define CM_B    KC_B
#define CM_J    KC_J
#define CM_L    KC_L
#define CM_U    KC_U
#define CM_Y    KC_Y
#define CM_SCLN KC_SCLN

#define CM_A    KC_A
#define CM_R    KC_R
#define CM_S    KC_S
#define CM_T    KC_T
#define CM_G    KC_G
#define CM_M    KC_M
#define CM_N    KC_N
#define CM_E    KC_E
#define CM_I    KC_I
#define CM_O    KC_O

#define CM_Z    KC_Z
#define CM_X    KC_X
#define CM_C    KC_C
#define CM_D    KC_D
#define CM_V    KC_V
#define CM_K    KC_K
#define CM_H    KC_H
#define CM_COMM KC_COMM
#define CM_DOT  KC_DOT
#define CM_SLSH KC_SLSH

// Home row mods using SM_TD
// Mac modifiers: GUI=Command, ALT=Option, CTRL=Control
smtd_resolution on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        // Left hand home row mods (Colemak DH positions)
        SMTD_MT(KC_A, KC_LGUI)   // A -> Command (GUI)
        SMTD_MT(KC_R, KC_LALT)   // R -> Option (ALT)
        SMTD_MT(KC_S, KC_LCTL)   // S -> Control
        SMTD_MT(KC_T, KC_LSFT)   // T -> Shift
        
        // Right hand home row mods (Colemak DH positions)
        SMTD_MT(KC_N, KC_LSFT)   // N -> Shift
        SMTD_MT(KC_E, KC_LCTL)   // E -> Control
        SMTD_MT(KC_I, KC_LALT)   // I -> Option (ALT)
        SMTD_MT(KC_O, KC_LGUI)   // O -> Command (GUI)
    }
    return SMTD_RESOLUTION_UNHANDLED;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
     * │ Q │ W │ F │ P │ B │       │ J │ L │ U │ Y │ ; │
     * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
     * │ A │ R │ S │ T │ G │       │ M │ N │ E │ I │ O │
     * ├───┴───┼───┼───┼───┤       ├───┼───┼───┼───┴───┤
     * │ Z │ X │ C │ D │ V │       │ K │ H │ , │ . │ / │
     * └───────┴───┴───┴───┘       └───┴───┴───┴───────┘
     *               ┌─────┐       ┌─────┐
     *               │ BSP │       │ SPC │
     *               └─────┘       └─────┘
     */
    [0] = LAYOUT_split_3x5_3(
        CM_Q,    CM_W,    CM_F,    CM_P,    CM_B,         CM_J,    CM_L,    CM_U,    CM_Y,    CM_SCLN,
        CM_A,    CM_R,    CM_S,    CM_T,    CM_G,         CM_M,    CM_N,    CM_E,    CM_I,    CM_O,
        CM_Z,    CM_X,    CM_C,    CM_D,    CM_V,         CM_K,    CM_H,    CM_COMM, CM_DOT,  CM_SLSH,
                          KC_ESC,  KC_BSPC,  KC_TAB,      KC_ENT,  KC_SPC,  KC_DEL
    ),
};

