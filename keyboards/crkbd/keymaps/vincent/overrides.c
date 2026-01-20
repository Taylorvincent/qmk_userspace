const key_override_t backspace_delete_key_override = ko_make_basic(MOD_MASK_SHIFT, LGUI_T(KC_BSPC), KC_DEL);
const key_override_t comma_semicolon_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SEMICOLON);
const key_override_t period_colon_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLON);
const key_override_t dollar_at_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOLLAR, KC_AT);
const key_override_t ampersand_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_AMPERSAND, KC_HASH);
const key_override_t pipe_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PIPE, KC_SLASH);
const key_override_t left_parens_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_RPRN);
const key_override_t left_brace_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_RCBR);
const key_override_t left_bracket_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_RBRC);
const key_override_t left_angle_bracket_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_LT, KC_GT);
const key_override_t plus_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_EQL);

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {
	&backspace_delete_key_override,
    &comma_semicolon_key_override,
    &period_colon_key_override,
    // Symbols left hand
    &dollar_at_key_override,
    &ampersand_key_override,
    &pipe_key_override,
    &left_parens_key_override,
    &left_brace_key_override,
    &left_bracket_key_override,
    &left_angle_bracket_key_override,
    &plus_key_override,
};