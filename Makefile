.SILENT:

MAKEFLAGS += --no-print-directory

QMK_USERSPACE := $(patsubst %/,%,$(dir $(shell realpath "$(lastword $(MAKEFILE_LIST))")))
ifeq ($(QMK_USERSPACE),)
    QMK_USERSPACE := $(shell pwd)
endif

QMK_FIRMWARE_ROOT = $(shell qmk config -ro user.qmk_home | cut -d= -f2 | sed -e 's@^None$$@@g')
ifeq ($(QMK_FIRMWARE_ROOT),)
    $(error Cannot determine qmk_firmware location. `qmk config -ro user.qmk_home` is not set)
endif

# Custom targets
# Note: keymap_svg.json is used for SVG generation to avoid conflicts with keymap.json (used for QMK modules)
KEYMAP_SVG_JSON := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent/keymap_svg.json
KEYMAP_YAML := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent/keymap.yaml
KEYMAP_SVG := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent/keymap.svg
KEYMAP_C := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent/keymap.c
ADD_COMBOS_SCRIPT := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent/enhance_yaml.py
	
crkbd: $(KEYMAP_SVG)
	qmk compile -kb crkbd -km vincent

$(KEYMAP_SVG_JSON): $(KEYMAP_C)
	@echo "Converting keymap.c to keymap_svg.json using QMK c2json..."
	@qmk c2json -kb crkbd/rev1 -km vincent -o $(KEYMAP_SVG_JSON) $(KEYMAP_C) || (echo "Error: Failed to convert keymap.c to JSON" && exit 1)

$(KEYMAP_YAML): $(KEYMAP_SVG_JSON) $(KEYMAP_C) $(ADD_COMBOS_SCRIPT)
	@echo "Parsing keymap_svg.json to YAML..."
	keymap parse -q $(KEYMAP_SVG_JSON) -o $(KEYMAP_YAML)
	@echo "Adding combos from keymap.c..."
	@PYTHON_CMD=$$(head -1 $$(which keymap) 2>/dev/null | sed 's|^#!||' | head -1) || PYTHON_CMD=python3; \
	 $$PYTHON_CMD $(ADD_COMBOS_SCRIPT) || (echo "Warning: Failed to add combos from keymap.c" && exit 0)

$(KEYMAP_SVG): $(KEYMAP_YAML)
	@echo "Generating SVG visualization..."
	keymap -c my_config.yaml draw $(KEYMAP_YAML) -o $(KEYMAP_SVG)
	@echo "SVG generated: $(KEYMAP_SVG)"
	@echo "Cleaning up keymap_svg.json..."
	@rm -f $(KEYMAP_SVG_JSON)
	@echo "Deleted $(KEYMAP_SVG_JSON)"

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)