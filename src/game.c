#include "pebble.h"

static Window *window;

static BitmapLayer *image_layer;
static TextLayer * title_layer;
static GBitmap *image1;
static GBitmap *image2;
static GBitmap *image3;
static GBitmap *image_win;
static GBitmap *image_draw;
static GBitmap *image_lose;
static GBitmap *result_image[3];
static AppTimer *timer;
static int idx;
Layer *window_layer;
#define SCISSORS  0
#define ROCK  1
#define PAPER 2
#define WIN  1
#define DRAW 0
#define FAIL  -1
  
static void timer_callback(void *data) {
  
  
  text_layer_set_text(title_layer, "가위~ 바위~ 보~");
  text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  switch (idx%3) {
    case (0):
      bitmap_layer_set_bitmap(image_layer, image2);
      bitmap_layer_set_alignment(image_layer, GAlignCenter);
      layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
      timer = app_timer_register(100, (AppTimerCallback) timer_callback, NULL);
      idx++;
      break;
    case 1:
      bitmap_layer_set_bitmap(image_layer, image3);
      bitmap_layer_set_alignment(image_layer, GAlignCenter);
      layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
      timer = app_timer_register(100, (AppTimerCallback) timer_callback, NULL);
      idx++;
      break;
    case 2:
      bitmap_layer_set_bitmap(image_layer, image1);    
      bitmap_layer_set_alignment(image_layer, GAlignCenter);
      layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
      timer = app_timer_register(100, (AppTimerCallback) timer_callback, NULL);
      idx++;
      break;
  }
}
static void display(int result) {
   switch (result) {
    case (-1):
      bitmap_layer_set_bitmap(image_layer, image_lose);
      bitmap_layer_set_alignment(image_layer, GAlignCenter);
      text_layer_set_text(title_layer, "졌네요~ ㅠㅠ");
      text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
      text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
      layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
      layer_add_child(window_layer, text_layer_get_layer(title_layer));  
      break;
    case 0:
      bitmap_layer_set_bitmap(image_layer, image_draw);
      bitmap_layer_set_alignment(image_layer, GAlignCenter);
      text_layer_set_text(title_layer, "무승부~ 다시");
      text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
      text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
      layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
      layer_add_child(window_layer, text_layer_get_layer(title_layer));  
     break;
    case 1:
      bitmap_layer_set_bitmap(image_layer, image_win);
      bitmap_layer_set_alignment(image_layer, GAlignCenter);
      text_layer_set_text(title_layer, "이겼다~!!!");
      text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
      text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
      layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
      layer_add_child(window_layer, text_layer_get_layer(title_layer));  
      break;
  }
  
  
  
  //
    
}
int calculateWin(int person) {
  int ret= WIN;
  int com = rand()%3;
  if(person == com)
    return DRAW;
  if((person == SCISSORS && com == ROCK) || (person == ROCK && com == PAPER) || (person == PAPER && com == SCISSORS))
    return FAIL;
  return ret;
}
static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  app_timer_cancel(timer);
  int ret=calculateWin(SCISSORS);
  display(ret);
  //psleep(3000);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SCISSORS %d",ret);
  timer = app_timer_register(500, (AppTimerCallback) timer_callback, NULL);
}
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  app_timer_cancel(timer);
  int ret=calculateWin(ROCK);
  display(ret);
  //psleep(3000);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ROCK %d",ret);
  timer = app_timer_register(500, (AppTimerCallback) timer_callback, NULL);
}
static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  app_timer_cancel(timer);
  int ret=calculateWin(PAPER);
  display(ret);
  //psleep(3000);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "PAPER %d",ret);
  timer = app_timer_register(500, (AppTimerCallback) timer_callback, NULL);
}

static void config_provider(void *context) {
  const uint16_t repeat_interval_ms = 5000;
  window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, up_single_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, repeat_interval_ms, select_single_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, down_single_click_handler);

}
static void init() {
  window = window_create(); 
  window_stack_push(window, true);
  
  srand(time(NULL));
  idx = 0;

  window_layer = window_get_root_layer(window);
  title_layer =  text_layer_create(GRect(0,0,144,30));
  image1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
  image2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_2);
  image3 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_3);
  
  image_lose = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOSE);
  image_win = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_VIC);
  image_draw = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DRAW);

  image_layer = bitmap_layer_create(GRect(0, 30, 144, 168));
  bitmap_layer_set_bitmap(image_layer, image1);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
  text_layer_set_text(title_layer, "가위~ 바위~ 보~");
  text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(title_layer));                              

  timer = app_timer_register(100 /* milliseconds */, timer_callback, NULL);
  window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
 
}

static void deinit() {
  gbitmap_destroy(image1);
  gbitmap_destroy(image2);
  gbitmap_destroy(image3);
  
  gbitmap_destroy(image_win);
  gbitmap_destroy(image_draw);
  gbitmap_destroy(image_lose);

  app_timer_cancel(timer);
  bitmap_layer_destroy(image_layer);
  // Destroy Window
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
