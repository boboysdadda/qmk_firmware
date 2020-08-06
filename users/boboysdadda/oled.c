#include "boboysdadda.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_keyboard_master()) {
    return OLED_ROTATION_270;  // flips the display 270 degrees if master
  }
  return rotation;
}

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);

static uint8_t zero_bar_count = 0;
static uint8_t bar_count = 0;

static void render_wpm_graph(void) {
  uint8_t bar_height = 0;
  uint8_t bar_segment = 0;

  if (wpm_graph_timer == 0) {
    wpm_graph_timer = timer_read();
    return;
  }
  if(timer_elapsed(wpm_graph_timer) > 500) {
    wpm_graph_timer = timer_read();

    if(OLED_DISPLAY_HEIGHT == 64)
      bar_height = get_current_wpm() / 2;
    if(OLED_DISPLAY_HEIGHT == 32)
      bar_height = get_current_wpm() / 4;
    if(bar_height > OLED_DISPLAY_HEIGHT)
      bar_height = OLED_DISPLAY_HEIGHT;

    if(bar_height == 0) {
        // keep track of how many zero bars we have drawn.  If
        // there is a whole screen worth, turn the display off and
        // wait until there is something to do
        if (zero_bar_count > OLED_DISPLAY_WIDTH) {
      oled_off();
      return;
        }
        zero_bar_count++;
    } else
        zero_bar_count=0;

    oled_pan(false);
    bar_count++;
    for (uint8_t i = (OLED_DISPLAY_HEIGHT / 8); i > 0; i--) {
        if (bar_height > 7) {
      if (i % 2 == 1 && bar_count % 3 == 0)
          bar_segment = 254;
      else
          bar_segment = 255;
      bar_height -= 8;
        } else {
      switch (bar_height) {
          case 0:
        bar_segment = 0;
        break;
          case 1:
        bar_segment = 128;
        break;
          case 2:
        bar_segment = 192;
        break;
          case 3:
        bar_segment = 224;
        break;
          case 4:
        bar_segment = 240;
        break;
          case 5:
        bar_segment = 248;
        break;
          case 6:
        bar_segment = 252;
        break;
          case 7:
        bar_segment = 254;
        break;
      }
      bar_height = 0;

      if (i % 2 == 1 && bar_count % 3 == 0)
          bar_segment++;
        }
        oled_write_raw_byte(bar_segment, (i-1) * OLED_DISPLAY_WIDTH);
    }
  }
}

void render_default_layer_state(void) {
    oled_write_P(PSTR("Lyout"), false);
    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR(" QRTY"), false);
            break;
        case _COLEMAK:
            oled_write_P(PSTR(" COLE"), false);
            break;
        case _DVORAK:
            oled_write_P(PSTR(" DVRK"), false);
            break;
        case _WORKMAN:
            oled_write_P(PSTR(" WRKM"), false);
            break;
    }
}

void render_layer_state(void) {
  oled_write_ln_P(PSTR("LAYER"), false);
  oled_write_ln_P(PSTR("DFLT"), layer_state_is(_QWERTY));
  oled_write_ln_P(PSTR("RSE"), layer_state_is(_RAISE));
  oled_write_ln_P(PSTR("LWR"), layer_state_is(_LOWER));
}

void render_status_secondary(void) {
  render_layer_state();
  oled_write_ln_P(PSTR(""), false);
  oled_write_ln_P(PSTR(""), false);
  oled_write_ln_P(PSTR(""), false);
  oled_write_ln_P(PSTR(""), false);
  oled_write_ln_P(PSTR(""), false);
  oled_write_ln_P(PSTR(""), false);
  oled_write_ln_P(PSTR(""), false);
#ifdef WPM_ENABLE
  // Write WPM
  sprintf(wpm_str, "WPM: %03d", get_current_wpm());
  oled_write(wpm_str, false);
#endif
}

void oled_task_user(void) {
    if (timer_elapsed32(oled_timer) > 30000) {
        oled_off();
        return;
    }
#if !defined(SPLIT_KEYBOARD)
    else {
        oled_on();
    }
#endif
    if (!is_keyboard_master()) {
        render_wpm_graph();
    } else {
        render_status_secondary();
    }
}
