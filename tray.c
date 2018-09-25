#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define TRAY_WINAPI 1
#define MAX_BUFFER 2048
#include "tray.h"
#include "cJSON.h"


// {"tip":"Lark","icon":"icon.ico","items":[{"text":"Open Lark  (Ctrl+Shift+L)","id":"1"}]}


int first = 1;
struct tray tray;
char json_string[MAX_BUFFER] = { 0 };

void click_cb() {
  puts("event:click");
}

void item_cb(struct tray_menu* item) {
  char* id = (char *)item->context;
  printf("event:item:%s\n", id);
}

DWORD WINAPI do_tray_loop(void* p) {
  if (first) {
    tray_init(&tray);
    first = 0;
  }
  while (tray_loop(1) == 0);
}

void init() {
  tray.icon = NULL;
  tray.onclick = click_cb;
  tray.menu = (struct tray_menu *)malloc(sizeof(struct tray_menu) * 100);
}

int main(int argc, char** argv) {
  init();
  int menu_count = 0;
  cJSON* root = NULL;
  cJSON* item = NULL;
  while (fgets(json_string, MAX_BUFFER, stdin)) {
    if (strstr(json_string, "exit") == json_string) {
      cJSON_Delete(root);
      free(tray.menu);
      tray_exit();
      return 0;
    }
    if (!first) cJSON_Delete(root);
    menu_count = 0;
    root = cJSON_Parse(json_string);
    tray.tip = cJSON_GetObjectItemCaseSensitive(root, "tip")->valuestring;
    tray.icon = cJSON_GetObjectItemCaseSensitive(root, "icon")->valuestring;
    cJSON* items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON_ArrayForEach(item, items) {
      char* text = cJSON_GetObjectItemCaseSensitive(item, "text")->valuestring;
      char* id = cJSON_GetObjectItemCaseSensitive(item, "id")->valuestring;
      tray.menu[menu_count++] = (struct tray_menu){ text, 0, 0, item_cb, id, NULL };
    }
    tray.menu[menu_count++] = (struct tray_menu){ NULL, 0, 0, NULL, NULL, NULL };
    if (first) {
      CreateThread(NULL, 0, do_tray_loop, NULL, 0, NULL);
    } else {
      tray_update(&tray);
    }
  }
  return 0;
}

