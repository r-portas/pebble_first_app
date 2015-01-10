#include <pebble.h>
static Window *sMainWindow;
static TextLayer *sTimeLayer;

static void updateTime(){
  time_t temp = time(NULL);
  struct tm *tickTime = localtime(&temp);
  
  static char buffer[] = "00:00";
  
  if (clock_is_24h_style() == true){
    // Use 24 hour time
    strftime(buffer, sizeof("00:00"), "%H:%M", tickTime);
  } else {
    // Use 12 hour time
    strftime(buffer, sizeof("00:00"), "%I:%M", tickTime);
  }
  
  text_layer_set_text(sTimeLayer, buffer); // Display the time
}

static void tickHandler(struct tm *tickTime, TimeUnits unitsChanged){
  updateTime();
}

static void main_window_load(Window *window){
  sTimeLayer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(sTimeLayer, GColorBlack);
  text_layer_set_text_color(sTimeLayer, GColorClear);
  
  text_layer_set_font(sTimeLayer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(sTimeLayer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(sTimeLayer));
  
  updateTime();
}

static void main_window_unload(Window *window){
  text_layer_destroy(sTimeLayer);
}


static void init(){
  sMainWindow = window_create();
  window_set_background_color(sMainWindow, GColorBlack);
  window_set_window_handlers(sMainWindow, (WindowHandlers){.load = main_window_load,                                                           .unload = main_window_unload});
  window_stack_push(sMainWindow, true); //Show the window on the watch with animations activated
  tick_timer_service_subscribe(MINUTE_UNIT, tickHandler);
  
}

static void deinit(){
  window_destroy(sMainWindow);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}

