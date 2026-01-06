#include QMK_KEYBOARD_H
#include "combos.c"

// Tri-layer: when both L1 and R1 are active, activate ADJUST layer
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, 1, 3, 4);
}

