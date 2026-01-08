
// Tap Dance keycodes
enum td_keycodes {
    OS_SHFT_4_TD, // Oneshot shift on tap, layer 4 on hold
};

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP
} td_state_t;


// Create a global instance of the tapdance state type
static td_state_t td_state;

// Declare your tapdance functions:
td_state_t cur_dance(tap_dance_state_t *state);
void os_shft_4_finished(tap_dance_state_t *state, void *user_data);
void os_shft_4_reset(tap_dance_state_t *state, void *user_data);

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    }

    if (state->count == 2) return TD_DOUBLE_SINGLE_TAP;
    else return TD_UNKNOWN;
}

// Handle the possible states for OS_SHFT_4 tapdance
void os_shft_4_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            // On tap: send oneshot shift
            set_oneshot_mods(MOD_BIT(KC_LSFT));
            break;
        case TD_SINGLE_HOLD:
            // On hold: activate layer 4
            layer_on(4);
            break;
        default:
            break;
    }
}

void os_shft_4_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            // Nothing to unregister for oneshot mods
            break;
        case TD_SINGLE_HOLD:
            // On release: turn off layer 4
            layer_off(4);
            break;
        default:
            break;
    }
}

// Define tapdance actions
tap_dance_action_t tap_dance_actions[] = {
    [OS_SHFT_4_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, os_shft_4_finished, os_shft_4_reset)
};