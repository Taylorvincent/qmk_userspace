#include QMK_KEYBOARD_H
#include "combos.c"
 
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT(
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R', 
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R', 
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R', 
        // allow thumb cmd key to trigger same hand faster
                  'L', '*', 'L',  'L', '*', 'L'
    );

// Tri-layer: when both L1 and R1 are active, activate ADJUST layer
// layer_state_t layer_state_set_user(layer_state_t state) {
//     return update_tri_layer_state(state, 2, 3, 4);
// }
 