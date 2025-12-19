#pragma once

#include QMK_KEYBOARD_H

#ifdef TAP_DANCE_ENABLE
enum {
    DOT_TD,
};

#define TD_DOT TD(DOT_TD)
#else
#define TD_DOT KC_DOT
#endif