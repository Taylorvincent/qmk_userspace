#pragma once

#include QMK_KEYBOARD_H

#ifdef TAP_DANCE_ENABLE
enum {
    DOT_TD,
    Q_ESC,
    COMM_TD,
};

#define TD_DOT TD(DOT_TD)
#define TD_QESC TD(Q_ESC)
#define TD_COMM TD(COMM_TD)

#else

#define TD_DOT KC_DOT
#define TD_QESC KC_Q
#define TD_COMM KC_COMM

#endif