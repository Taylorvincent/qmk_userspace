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
crkbd_old:
	keymapviz $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent/keymap.c -r -c $(QMK_USERSPACE)/keymapviz.ini
	qmk compile -kb crkbd -km vincent


KEYMAP_JSON := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_2/keymap.json
KEYMAP_YAML := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_2/keymap.yaml
KEYMAP_SVG := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_2/keymap.svg
KEYMAP_C := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_2/keymap.c
ADD_COMBOS_SCRIPT := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_2/add_combos_to_yaml.py
	
crkbd: $(KEYMAP_SVG)
	qmk compile -kb crkbd -km vincent_2

$(KEYMAP_JSON): $(KEYMAP_C)
	@echo "Converting keymap.c to keymap.json using QMK c2json..."
	@qmk c2json -kb crkbd/rev1 -km vincent_2 -o $(KEYMAP_JSON) $(KEYMAP_C) || (echo "Error: Failed to convert keymap.c to JSON" && exit 1)

$(KEYMAP_YAML): $(KEYMAP_JSON) $(KEYMAP_C) $(ADD_COMBOS_SCRIPT)
	@echo "Parsing keymap.json to YAML..."
	keymap parse -q $(KEYMAP_JSON) -o $(KEYMAP_YAML)
	@echo "Adding combos from keymap.c..."
	@PYTHON_CMD=$$(head -1 $$(which keymap) 2>/dev/null | sed 's|^#!||' | head -1) || PYTHON_CMD=python3; \
	 $$PYTHON_CMD $(ADD_COMBOS_SCRIPT) || (echo "Warning: Failed to add combos from keymap.c" && exit 0)

$(KEYMAP_SVG): $(KEYMAP_YAML)
	@echo "Generating SVG visualization..."
	keymap draw $(KEYMAP_YAML) -o $(KEYMAP_SVG)
	@echo "SVG generated: $(KEYMAP_SVG)"
	@echo "Cleaning up keymap.json to avoid interference with keymap generation..."
	@rm -f $(KEYMAP_JSON)
	@echo "Deleted $(KEYMAP_JSON)"

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)