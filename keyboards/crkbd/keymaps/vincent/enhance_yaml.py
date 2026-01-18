#!/usr/bin/env python3
"""
Parse combos from keymap.c and add them to keymap.yaml
Also parse tap dances from tapdance.c and add them to keymap visualization
"""
import re
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    print("Error: PyYAML is required but not installed.")
    print("Install it using one of these methods:")
    print("  1. brew install pyyaml")
    print("  2. pipx install keymap-drawer  (includes PyYAML)")
    print("  3. python3 -m pip install --user PyYAML")
    print("  4. python3 -m pip install --break-system-packages PyYAML")
    sys.exit(1)

# Mapping from QMK keycodes to their display names in YAML
KEYCODE_MAP = {
    'KC_B': 'B', 'KC_L': 'L', 'KC_D': 'D', 'KC_W': 'W', 'KC_Q': 'Q',
    'KC_J': 'J', 'KC_F': 'F', 'KC_O': 'O', 'KC_U': 'U', 'KC_QUOT': "'",
    'KC_N': 'N', 'KC_R': 'R', 'KC_T': 'T', 'KC_S': 'S', 'KC_G': 'G',
    'KC_Y': 'Y', 'KC_H': 'H', 'KC_A': 'A', 'KC_E': 'E', 'KC_I': 'I',
    'KC_Z': 'Z', 'KC_X': 'X', 'KC_M': 'M', 'KC_C': 'C', 'KC_V': 'V',
    'KC_K': 'K', 'KC_P': 'P', 'TD_COMM': ',', 'KC_DOT': '.', 'KC_SLSH': '/',
    'KC_ESC': 'ESC', 'KC_BSPC': 'BSPC', 'KC_TAB': 'TAB', 'KC_ENTER': 'ENTER',
    'KC_SPC': 'SPC', 'KC_DEL': 'DEL', 'KC_MINUS': '-', 'CW_TOGG': 'CW_TOGG',
    # Symbol keycodes
    'KC_DOLLAR': '$', 'KC_DLR': '$',
    'KC_AMPERSAND': '&', 'KC_AMPR': '&',
    'KC_PIPE': '|',
    'KC_LPRN': '(', 'KC_RPRN': ')',
    'KC_LCBR': '{', 'KC_RCBR': '}',
    'KC_LBRC': '[', 'KC_RBRC': ']',
    'KC_PLUS': '+',
    'KC_EQL': '=',
    'KC_AT': '@',
    'KC_HASH': '#',
    'KC_SEMICOLON': ';',
    'KC_COLON': ':',
    'KC_SLASH': '/',  # Alias for KC_SLSH
}

# Mod-tap keycode patterns
MOD_TAP_PATTERNS = [
    (r'LCTL_T\(KC_(\w+)\)', 'LCTL'),
    (r'LALT_T\(KC_(\w+)\)', 'LALT'),
    (r'LSFT_T\(KC_(\w+)\)', 'LSFT'),
    (r'LGUI_T\(KC_(\w+)\)', 'LGUI'),
]


def extract_keycode(keycode_str):
    """Extract the base keycode from a keycode string, handling mod-taps."""
    keycode_str = keycode_str.strip()
    
    # Check for mod-tap patterns
    for pattern, mod in MOD_TAP_PATTERNS:
        match = re.match(pattern, keycode_str)
        if match:
            base_key = match.group(1)
            return f"KC_{base_key}", mod
    
    # Direct keycode
    return keycode_str, None


def find_key_position(keycode, yaml_data):
    """Find the position index of a keycode in the YAML layers."""
    # Use L0 layer as reference
    layer = yaml_data['layers'].get('L0', [])
    
    keycode_str, mod = extract_keycode(keycode)
    base_key = KEYCODE_MAP.get(keycode_str)
    
    # Ensure KC_QUOT has the correct base_key
    # YAML '''' parses to a single quote, so we need to match that
    if keycode_str == "KC_QUOT":
        base_key = "'"
    
    if not base_key:
        # Try to handle special cases
        if keycode_str == "KC_COMM":
            base_key = ','
        elif keycode_str == "KC_DOT":
            base_key = '.'
        else:
            return None
    
    # Search for the key in the layer
    for idx, key in enumerate(layer):
        if isinstance(key, str):
            # Special handling for KC_QUOT - YAML '''' actually parses to a single quote
            if keycode_str == "KC_QUOT":
                # Match a single quote character
                if key == "'" or key == base_key:
                    if not mod:
                        return idx
            elif key == base_key:
                # If we're looking for a mod-tap but found a plain key, skip
                if not mod:
                    return idx
        elif isinstance(key, dict):
            # Check tap field
            tap_val = key.get('t')
            if tap_val == base_key:
                # If mod-tap, also check the mod matches
                if mod:
                    mod_map = {'LCTL': 'LCTL', 'LALT': 'LALT', 'LSFT': 'LSFT', 'LGUI': 'LGUI'}
                    if key.get('h') == mod_map.get(mod):
                        return idx
                else:
                    # Looking for plain key, found mod-tap - that's okay
                    return idx
    
    return None


def parse_combos_from_c(combos_c_path):
    """Parse combo definitions from combos.c file."""
    with open(combos_c_path, 'r') as f:
        content = f.read()
    
    combos = []
    
    # Find all combo array definitions like: const uint16_t PROGMEM xxx_combo[] = {KC_X, KC_Y, COMBO_END};
    combo_array_pattern = r'const uint16_t PROGMEM (\w+)\[\]\s*=\s*\{([^}]+)\};'
    combo_arrays = {}
    
    for match in re.finditer(combo_array_pattern, content):
        combo_name = match.group(1)
        keys_str = match.group(2)
        # Extract keycodes, handling nested parentheses for mod-taps
        keys = []
        current_key = ""
        paren_depth = 0
        for char in keys_str:
            if char == '(':
                paren_depth += 1
                current_key += char
            elif char == ')':
                paren_depth -= 1
                current_key += char
            elif char == ',' and paren_depth == 0:
                key = current_key.strip()
                if key and 'COMBO_END' not in key:
                    keys.append(key)
                current_key = ""
            else:
                current_key += char
        # Add last key if any
        if current_key.strip() and 'COMBO_END' not in current_key:
            keys.append(current_key.strip())
        combo_arrays[combo_name] = keys
    
    # Find combo_t key_combos[] array - extract the entire array content
    combo_array_start = content.find('combo_t key_combos[] = {')
    if combo_array_start == -1:
        return combos
    
    combo_array_end = content.find('};', combo_array_start)
    if combo_array_end == -1:
        return combos
    
    combo_array_content = content[combo_array_start:combo_array_end]
    
    # Parse COMBO() calls
    combo_pattern = r'COMBO\s*\(\s*(\w+)\s*,\s*(\w+)\s*\)'
    for match in re.finditer(combo_pattern, combo_array_content):
        combo_array_name = match.group(1)
        result_keycode = match.group(2)
        keys = combo_arrays.get(combo_array_name, [])
        if keys:
            combos.append({
                'keys': keys,
                'result': result_keycode,
                'action': False
            })
    
    # Parse COMBO_ACTION() calls
    combo_action_pattern = r'\[\s*(\w+)\s*\]\s*=\s*COMBO_ACTION\s*\(\s*(\w+)\s*\)'
    for match in re.finditer(combo_action_pattern, combo_array_content):
        combo_event = match.group(1)
        combo_array_name = match.group(2)
        keys = combo_arrays.get(combo_array_name, [])
        if keys:
            # Check process_combo_event for the action
            action_result = find_combo_action(content, combo_event)
            combos.append({
                'keys': keys,
                'result': action_result or f"ACTION_{combo_event}",
                'action': True
            })
    
    return combos


def find_combo_action(content, combo_event):
    """Find what action a combo event performs."""
    # Look for process_combo_event function
    # Match case statement and SEND_STRING
    pattern = rf'case\s+{combo_event}:\s*(?:if\s*\([^)]+\)\s*{{)?\s*SEND_STRING\("([^"]+)"\)'
    match = re.search(pattern, content, re.DOTALL)
    if match:
        email = match.group(1)
        return f"EMAIL: {email}"
    return None


def parse_key_overrides_from_c(overrides_c_path):
    """Parse key override definitions from overrides.c file.
    Extracts ko_make_basic(MOD_MASK_SHIFT, KC_XXX, KC_YYY) patterns.
    Returns a dictionary mapping base keycodes to their shifted equivalents.
    Example: {KC_BSPC: KC_DEL, KC_COMM: KC_SEMICOLON, ...}
    """
    with open(overrides_c_path, 'r') as f:
        content = f.read()
    
    overrides = {}
    
    # Pattern to match: ko_make_basic(MOD_MASK_SHIFT, KC_XXX, KC_YYY)
    # We only care about MOD_MASK_SHIFT overrides
    pattern = r'ko_make_basic\s*\(\s*MOD_MASK_SHIFT\s*,\s*(KC_\w+)\s*,\s*(KC_\w+)\s*\)'
    
    for match in re.finditer(pattern, content):
        base_keycode = match.group(1)
        shifted_keycode = match.group(2)
        overrides[base_keycode] = shifted_keycode
    
    return overrides


def find_keys_by_tap_value(yaml_data, tap_value):
    """Find all keys in YAML that match a given tap value.
    Searches across all layers.
    Works with both plain keys (string format) and mod-tap keys (dict with t: and h:).
    Returns list of (layer_name, key_idx, key) tuples.
    """
    matching_keys = []
    
    # Search through all layers
    for layer_name, layer_keys in yaml_data.get('layers', {}).items():
        for idx, key in enumerate(layer_keys):
            if isinstance(key, str):
                # Plain key - check if it matches the tap value
                if key == tap_value:
                    matching_keys.append((layer_name, idx, key))
            elif isinstance(key, dict):
                # Mod-tap or other dict key - check tap field
                tap_val = key.get('t')
                if tap_val == tap_value:
                    matching_keys.append((layer_name, idx, key))
    
    return matching_keys


def convert_keycode_to_yaml(keycode_str):
    """Convert QMK keycode to YAML display format.
    First checks KEYCODE_MAP, then handles special cases,
    then strips KC_ prefix if not found.
    Examples:
    - KC_DEL -> DEL
    - KC_BSPC -> BSPC
    - KC_SEMICOLON -> SEMICOLON (not in map, strips KC_)
    - KC_COMM -> , (special case)
    """
    # Check KEYCODE_MAP first
    if keycode_str in KEYCODE_MAP:
        return KEYCODE_MAP[keycode_str]
    
    # Handle special cases
    if keycode_str == "KC_COMM":
        return ','
    elif keycode_str == "KC_DOT":
        return '.'
    elif keycode_str == "KC_QUOT":
        return "'"
    
    # If not in map and not a special case, strip KC_ prefix
    if keycode_str.startswith('KC_'):
        return keycode_str[3:]  # Remove 'KC_' prefix
    
    # Return as-is if it doesn't start with KC_
    return keycode_str


def add_shifted_codes_to_yaml(yaml_data, overrides):
    """Add shifted codes (s: field) to YAML keys based on key overrides.
    overrides is a dict mapping base keycodes to shifted keycodes.
    Example: {KC_BSPC: KC_DEL, KC_COMM: KC_SEMICOLON, ...}
    """
    if not overrides:
        return 0
    
    updated_count = 0
    
    for base_keycode, shifted_keycode in overrides.items():
        # Convert base keycode to YAML format
        base_yaml = convert_keycode_to_yaml(base_keycode)
        
        # Find all keys in YAML that match the base keycode
        matching_keys = find_keys_by_tap_value(yaml_data, base_yaml)
        
        # Convert shifted keycode to YAML format
        shifted_yaml = convert_keycode_to_yaml(shifted_keycode)
        
        for layer_name, key_idx, key in matching_keys:
            layer = yaml_data['layers'][layer_name]
            
            # Convert key to dict if it's a string
            if isinstance(key, str):
                # Create dict with tap and shifted fields
                new_key = {'t': key}
                new_key['s'] = shifted_yaml
                layer[key_idx] = new_key
                updated_count += 1
                print(f"  Added shifted code to {layer_name}[{key_idx}]: {base_yaml} -> s:'{shifted_yaml}'")
            elif isinstance(key, dict):
                # Update or add s: field (overwrite if exists)
                key['s'] = shifted_yaml
                updated_count += 1
                print(f"  Updated shifted code on {layer_name}[{key_idx}]: {base_yaml} -> s:'{shifted_yaml}'")
    
    return updated_count


def update_combo_trigger_keys(yaml_data):
    """Update combo trigger keys to match the structure of layer keys.
    If a layer key has an s: field, the corresponding combo trigger key should also have it.
    This ensures the keymap drawer can properly match combo trigger keys to layer keys.
    """
    if 'combos' not in yaml_data:
        return
    
    updated_count = 0
    
    # Update combo trigger keys
    for combo in yaml_data.get('combos', []):
        trigger_keys = combo.get('tk', [])
        combo_layers = combo.get('l', [])
        
        for tk in trigger_keys:
            if not isinstance(tk, dict):
                continue
            
            tap_val = tk.get('t')
            hold_val = tk.get('h')
            
            # Find the corresponding layer key for each combo layer
            for layer_name in combo_layers:
                layer = yaml_data['layers'].get(layer_name, [])
                for layer_key in layer:
                    # Check if this layer key matches the trigger key
                    if isinstance(layer_key, str):
                        if layer_key == tap_val and hold_val is None:
                            # Plain key matches - no s: field to copy
                            break
                    elif isinstance(layer_key, dict):
                        layer_tap = layer_key.get('t')
                        layer_hold = layer_key.get('h')
                        if layer_tap == tap_val and layer_hold == hold_val:
                            # Found matching key - copy s: field if it exists and not already in tk
                            if 's' in layer_key:
                                if 's' not in tk:
                                    tk['s'] = layer_key['s']
                                    updated_count += 1
                            break
    
    if updated_count > 0:
        print(f"  Updated {updated_count} combo trigger keys with shifted codes")


def parse_tapdances_from_c(tapdance_c_path):
    """Parse tap dance definitions from tapdance.c file.
    Reads comments from the td_keycodes enum and splits them:
    - First segment -> tap field (keypress)
    - Second segment -> shifted/top field
    """
    with open(tapdance_c_path, 'r') as f:
        content = f.read()
    
    tapdances = {}
    
    # Find the enum to parse comments from it
    enum_pattern = r'enum\s+td_keycodes\s*\{([^}]+)\}'
    enum_match = re.search(enum_pattern, content)
    if not enum_match:
        return tapdances
    
    enum_content = enum_match.group(1)
    
    # Parse each enum entry: ENUM_NAME, // comment
    for line in enum_content.split('\n'):
        line = line.strip()
        if not line or line.startswith('//'):
            continue
        
        # Extract enum name and comment
        # Pattern: ENUM_NAME, // comment or ENUM_NAME // comment
        parts = line.split('//', 1)
        if len(parts) < 2:
            continue
        
        enum_name_part = parts[0].strip()
        comment = parts[1].strip()
        
        # Extract enum name (remove trailing comma)
        enum_name = enum_name_part.rstrip(',').strip()
        if not enum_name:
            continue
        
        # Split comment on space - first part is tap, second part is top
        comment_parts = comment.split(None, 1)  # Split on whitespace, max 1 split
        tap_text = comment_parts[0] if len(comment_parts) > 0 else ""
        top_text = comment_parts[1] if len(comment_parts) > 1 else ""
        
        # Store both tap and top text
        tapdances[enum_name] = {
            'tap': tap_text,
            'top': top_text
        }
    
    return tapdances


def find_tapdance_keys_in_yaml(yaml_data, td_enum_name):
    """Find all keys in YAML that use TD(td_enum_name).
    Note: keymap parse converts underscores to spaces, so we need to match both.
    """
    td_keys = []
    
    # Create pattern that matches both underscores and spaces
    # COMM_TD should match both "TD(COMM_TD)" and "TD(COMM TD)"
    # keymap parse converts underscores to spaces in YAML output
    pattern_underscore = re.escape(td_enum_name)
    pattern_space = re.escape(td_enum_name.replace('_', ' '))
    
    # Search through all layers
    for layer_name, layer_keys in yaml_data.get('layers', {}).items():
        for idx, key in enumerate(layer_keys):
            # Check if key is a string containing TD(ENUM_NAME)
            if isinstance(key, str):
                # Match TD(ENUM_NAME) with underscores or spaces
                if (re.search(rf'TD\s*\(\s*{pattern_underscore}\s*\)', key) or
                    re.search(rf'TD\s*\(\s*{pattern_space}\s*\)', key)):
                    td_keys.append((layer_name, idx, key))
            # Check if key is a dict with tap field containing TD(ENUM_NAME)
            elif isinstance(key, dict):
                tap_val = key.get('t') or key.get('tap') or key.get('center')
                if tap_val:
                    tap_str = str(tap_val)
                    if (re.search(rf'TD\s*\(\s*{pattern_underscore}\s*\)', tap_str) or
                        re.search(rf'TD\s*\(\s*{pattern_space}\s*\)', tap_str)):
                        td_keys.append((layer_name, idx, key))
    
    return td_keys


def add_tapdances_to_yaml(yaml_data, tapdances, tapdance_c_path):
    """Add tap dance labels to YAML keys.
    tapdances is a dict mapping enum_name -> {'tap': '...', 'top': '...'}
    """
    if not tapdances:
        return 0
    
    updated_count = 0
    
    for td_enum_name, td_data in tapdances.items():
        # Find keys using this tap dance
        td_keys = find_tapdance_keys_in_yaml(yaml_data, td_enum_name)
        
        tap_text = td_data.get('tap', '')
        top_text = td_data.get('top', '')
        
        for layer_name, key_idx, key in td_keys:
            layer = yaml_data['layers'][layer_name]
            
            # Convert key to dict if it's a string
            if isinstance(key, str):
                # Create dict with tap and top fields
                new_key = {}
                # Set tap field - use tap_text if available, otherwise keep original keycode
                if tap_text:
                    new_key['t'] = tap_text
                else:
                    new_key['t'] = key  # Keep original if no tap text
                # Set top field (shifted) if we have top text
                if top_text:
                    new_key['s'] = top_text
                layer[key_idx] = new_key
                updated_count += 1
                print(f"  Added tap dance to {layer_name}[{key_idx}]: {td_enum_name} -> tap:'{tap_text}' top:'{top_text}'")
            elif isinstance(key, dict):
                # Update tap and top fields
                updated = False
                # Update tap field if we have tap_text
                if tap_text:
                    if key.get('t') != tap_text:
                        key['t'] = tap_text
                        updated = True
                # Update top field if we have top_text
                if top_text:
                    if key.get('s') != top_text and key.get('shifted') != top_text and key.get('top') != top_text:
                        key['s'] = top_text
                        updated = True
                if updated:
                    updated_count += 1
                    print(f"  Updated tap dance on {layer_name}[{key_idx}]: {td_enum_name} -> tap:'{tap_text}' top:'{top_text}'")
    
    return updated_count


def enhance_yaml(yaml_path, combos, combos_c_path):
    """Add parsed combos to the YAML file."""
    with open(yaml_path, 'r') as f:
        yaml_data = yaml.safe_load(f)
    
    if 'combos' not in yaml_data:
        yaml_data['combos'] = []
    
    # Parse combos from C file
    parsed_combos = parse_combos_from_c(combos_c_path)
    print(f"Found {len(parsed_combos)} combos in combos.c")
    
    # Parse tap dances from tapdance.c
    script_dir = Path(yaml_path).parent
    tapdance_c_path = script_dir / 'tapdance.c'
    tapdances = {}
    if tapdance_c_path.exists():
        tapdances = parse_tapdances_from_c(tapdance_c_path)
        print(f"Found {len(tapdances)} tap dances in tapdance.c")
    else:
        print(f"Warning: {tapdance_c_path} not found, skipping tap dance parsing")
    
    # Parse key overrides from overrides.c
    overrides_c_path = script_dir / 'overrides.c'
    overrides = {}
    if overrides_c_path.exists():
        overrides = parse_key_overrides_from_c(overrides_c_path)
        print(f"Found {len(overrides)} key overrides in overrides.c")
    else:
        print(f"Warning: {overrides_c_path} not found, skipping key override parsing")
    
    # Convert to YAML format
    yaml_combos = []
    for combo in parsed_combos:
        keys = combo['keys']
        if len(keys) < 2:
            continue
        
        # Find positions for the keys
        positions = []
        trigger_keys = []
        
        for keycode in keys:
            pos = find_key_position(keycode, yaml_data)
            if pos is not None:
                positions.append(pos)
                # Get the key display name
                keycode_str, mod = extract_keycode(keycode)
                base_key = KEYCODE_MAP.get(keycode_str, keycode_str)
                if mod:
                    trigger_keys.append({'t': base_key, 'h': mod})
                else:
                    trigger_keys.append(base_key)
            else:
                print(f"  Warning: Could not find position for keycode {keycode}")
        
        if len(positions) == len(keys):
            # Use trigger_keys format as it's more readable
            result = combo['result']
            # Convert result keycode to display format
            result_display = KEYCODE_MAP.get(result, result)
            
            combo_spec = {
                'tk': trigger_keys,
                'k': result_display,
                'l': ['L0']  # Only on base layer
            }
            
            # Add align: left for VI_EMAIL combo
            if isinstance(result_display, str) and result_display.startswith('EMAIL:'):
                combo_spec['slide'] = '-.5'
            
            yaml_combos.append(combo_spec)
    
    # Initialize combos list if it doesn't exist
    if 'combos' not in yaml_data:
        yaml_data['combos'] = []
    
    # Add new combos (avoid duplicates)
    # Create a set of existing combo trigger keys for comparison
    def normalize_tk(tk_list):
        """Normalize trigger keys for comparison."""
        if not tk_list:
            return tuple()
        normalized = []
        for item in tk_list:
            if isinstance(item, dict):
                normalized.append((item.get('t'), item.get('h')))
            else:
                normalized.append((item, None))
        return tuple(sorted(normalized))
    
    existing_tk_sets = {normalize_tk(c.get('tk', c.get('p', []))) for c in yaml_data.get('combos', [])}
    added_count = 0
    for combo in yaml_combos:
        tk_set = normalize_tk(combo.get('tk', []))
        if tk_set not in existing_tk_sets:
            yaml_data['combos'].append(combo)
            existing_tk_sets.add(tk_set)
            added_count += 1
            print(f"  Added combo: {combo.get('tk')} -> {combo.get('k')}")
    
    # Add tap dances to YAML
    if tapdances:
        td_count = add_tapdances_to_yaml(yaml_data, tapdances, tapdance_c_path)
        print(f"Updated {td_count} keys with tap dance labels")
    
    # Add shifted codes to YAML
    if overrides:
        sc_count = add_shifted_codes_to_yaml(yaml_data, overrides)
        print(f"Updated {sc_count} keys with shifted codes")
    
    # Update combo trigger keys to include shifted codes if layer keys have them
    update_combo_trigger_keys(yaml_data)
    
    # Write back to file
    with open(yaml_path, 'w') as f:
        yaml.dump(yaml_data, f, default_flow_style=False, sort_keys=False, allow_unicode=True)
    
    print(f"Added {added_count} new combos to {yaml_path} (total: {len(yaml_data['combos'])})")


def main():
    script_dir = Path(__file__).parent
    combos_c_path = script_dir / 'combos.c'
    yaml_path = script_dir / 'keymap.yaml'
    
    if not combos_c_path.exists():
        print(f"Error: {combos_c_path} not found")
        sys.exit(1)
    
    if not yaml_path.exists():
        print(f"Error: {yaml_path} not found")
        sys.exit(1)
    
    enhance_yaml(yaml_path, [], combos_c_path)


if __name__ == '__main__':
    main()

