SRC += boboysdadda.c \
		process_records.c

# Common flags
SPACE_CADET_ENABLE      = no
LTO_ENABLE              = yes
UNICODE_ENABLE          = yes
NKRO_ENABLE             = yes
EXTRAKEY_ENABLE         = yes
LEADER_ENABLE           = yes
TAP_DANCE_ENABLE        = no

ifneq ($(strip $(NO_SECRETS)), yes)
    ifneq ("$(wildcard $(USER_PATH)/secrets.c)","")
        SRC += secrets.c
    endif
    ifeq ($(strip $(NO_SECRETS)), lite)
        OPT_DEFS += -DNO_SECRETS
    endif
endif

ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
    SRC += tap_dances.c
endif

ifeq ($(strip $(OLED_DRIVER_ENABLE)), yes)
    SRC += oled.c
endif

ifeq ($(strip $(LEADER_ENABLE)), yes)
    SRC += leader.c
endif

ifeq ($(strip $(ENCODER_ENABLE)), yes)
    SRC += encoder_stuff.c
endif

ifeq ($(strip $(RGBLIGHT_ENABLE)), yes)
    SRC += rgb_lighting_user.c
    ifeq ($(strip $(INDICATOR_LIGHTS)), yes)
        OPT_DEFS += -DINDICATOR_LIGHTS
    endif
    ifeq ($(strip $(RGBLIGHT_TWINKLE)), yes)
        OPT_DEFS += -DRGBLIGHT_TWINKLE
    endif
    ifeq ($(strip $(RGBLIGHT_NOEEPROM)), yes)
        OPT_DEFS += -DRGBLIGHT_NOEEPROM
    endif
    ifeq ($(strip $(RGBLIGHT_STARTUP_ANIMATION)), yes)
        OPT_DEFS += -DRGBLIGHT_STARTUP_ANIMATION
    endif
endif

# At least until build.mk or the like drops, this is here to prevent
# VUSB boards from enabling NKRO, as they do not support it. Ideally
# this should be handled per keyboard, but until that happens ...
ifeq ($(strip $(PROTOCOL)), VUSB)
    NKRO_ENABLE       = no
endif