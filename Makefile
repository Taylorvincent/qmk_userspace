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
crkbd:
	keymapviz $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent/keymap.c -r -c $(QMK_USERSPACE)/keymapviz.ini
	qmk compile -kb crkbd -km vincent


KEYMAP_JSON := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_json/keymap.json
KEYMAP_YAML := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_json/keymap.yaml
KEYMAP_SVG := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_json/keymap.svg
KEYMAP_C := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_json/keymap.c
ADD_COMBOS_SCRIPT := $(QMK_USERSPACE)/keyboards/crkbd/keymaps/vincent_json/add_combos_to_yaml.py
	
crkbd_json: $(KEYMAP_SVG)
	qmk compile -kb crkbd -km vincent_json

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

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)