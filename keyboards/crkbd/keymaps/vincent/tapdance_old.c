// Tap Dance keycodes
enum td_keycodes {
    OS_SHFT_4_TD, // OS_Shift L4 
    COMM_TD, // , ; 
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

// void sentence_end(tap_dance_state_t *state, void *user_data) {
//     switch (state->count) {

//         // Double tapping TD_DOT produces
//         // ". <one-shot shift>" i.e. dot, space and capitalize next letter.
//         // This helps to quickly end a sentence and begin another one
//         // without having to hit shift.
//         case 2:
//             tap_code(KC_SPC);
//             // Calling one shot shift here produces unreliable results. More
//             // reliable results are achieved by calling one shot shift in
//             // sentence_end_finished.
//             break;

//         // Since `sentence_end` is called on each tap
//         // and not at the end of the tapping term,
//         // the third tap needs to cancel the effects
//         // of the double tap in order to get the expected
//         // three dots ellipsis.
//         case 3:
//             // remove the added space of the double tap case
//             tap_code(KC_BSPC);
//             // replace the space with a second dot
//             tap_code(KC_DOT);
//             // tap the third dot
//             tap_code(KC_DOT);
//             break;

//         // send KC_DOT on every normal tap of TD_DOT
//         default:
//             tap_code(KC_DOT);
//     }
// };

// void sentence_end_finished (tap_dance_state_t *state, void *user_data) {
//     if (state->count == 2) {
//         /* Internal code of OSM(MOD_LSFT) */
//         add_oneshot_mods(MOD_BIT(KC_LEFT_SHIFT));
//     }
// } 

// Define tapdance actions
tap_dance_action_t tap_dance_actions[] = {
    [OS_SHFT_4_TD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, os_shft_4_finished, os_shft_4_reset),
    [COMM_TD] = ACTION_TAP_DANCE_DOUBLE(KC_COMM, KC_SEMICOLON),
    // [DOT_TD] = ACTION_TAP_DANCE_FN_ADVANCED(sentence_end, sentence_end_finished, NULL),
    // [SLASH_TD] = ACTION_TAP_DANCE_DOUBLE(KC_SLASH, KC_BACKSLASH),
};