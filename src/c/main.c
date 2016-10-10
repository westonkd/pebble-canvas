#include <pebble.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*************************************************
* Event struct
*************************************************/
struct Event {
  char name[128];
  bool isAssignment;
  char date[32];
  char time[32];
};

Window *my_window;
TextLayer *text_layer;

/*************************************************
* split_string
* Split string at "|" and store in array
*************************************************/
void split_string(char* buffer, char array[][256], int array_size) {
  char word[64] = ""; 
  int i = 0;
  
  for (char* p = buffer; *p; p++) {
    if (*p != '|') {
      word[strlen(word)] = *p;
    } else {
      APP_LOG(APP_LOG_LEVEL_ERROR, word);
      strcpy(array[i], word);
      i++;
      memset(&word[0], 0, sizeof(word));
    }
  }
}

/*************************************************
* inbox_received_callback
*************************************************/
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Buffers for incoming messages
  static char event_names_buffer[256];
  static char event_types_buffer[256];
  static char event_dates_buffer[256];
  static char event_times_buffer[256];
  
  int event_count = 0;
  
  // Read tuples for data
  Tuple *names_tuple = dict_find(iterator, MESSAGE_KEY_EVENT_NAMES);
  Tuple *types_tuple = dict_find(iterator, MESSAGE_KEY_EVENT_TYPES);
  Tuple *dates_tuple = dict_find(iterator, MESSAGE_KEY_EVENT_DATES);
  Tuple *times_tuple = dict_find(iterator, MESSAGE_KEY_EVENT_TIMES);
  Tuple *count_tuple = dict_find(iterator, MESSAGE_KEY_EVENT_COUNT);
  
  // Use all data if it is available
  if (names_tuple && types_tuple && dates_tuple && times_tuple && count_tuple) {
    snprintf(event_names_buffer, sizeof(event_names_buffer), "%s", names_tuple->value->cstring);
    snprintf(event_types_buffer, sizeof(event_types_buffer), "%s", types_tuple->value->cstring);
    snprintf(event_dates_buffer, sizeof(event_dates_buffer), "%s", dates_tuple->value->cstring);
    snprintf(event_times_buffer, sizeof(event_times_buffer), "%s", times_tuple->value->cstring);
    event_count = atoi(count_tuple->value->cstring);
    
    char names_array[event_count][256];
    for (int i = 0; i < event_count; i++)
      memset(&names_array[i], 0, sizeof(names_array[i]));
    
    split_string(event_names_buffer, names_array, 1);
    for (int i = 0; i < event_count; i++) {
      APP_LOG(APP_LOG_LEVEL_ERROR, names_array[i]);
    }
  }
}

/*************************************************
* inbox dropped, failed, and sent callbacks
*************************************************/
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message Dropped");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox Send Failed");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/*************************************************
* init and deinit
*************************************************/
void handle_init(void) {
  // Register received callbacks
  app_message_register_inbox_received(inbox_received_callback);

  // Open AppMessage
  const int inbox_size = 256;
  const int outbox_size = 256;
  app_message_open(inbox_size, outbox_size);
  
  // Register other callbacks
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  my_window = window_create();
  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}