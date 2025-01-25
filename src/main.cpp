#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "parser.h"
#include "requests.h"
#include "config.h"
#include "menu.h"


ArudinoStreamParser parser;
PizzaHandler handler;

Request r;
uint32_t order_id;
uint32_t trackLastUpdate;
char unauthorized_token[33];


void setup() {
  WiFi.disconnect();
  delay(100);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Menu &menu = Menu::getInstance();
  while (WiFi.status() != WL_CONNECTED) {
    menu.tick();
    delay(100);
  }

  menu.setMenuPage(Menu::PRELOAD);
  parser.setHandler(&handler);
  char path[70];
  sprintf(path, "/catalog/category-goods?category=pizza&page_size=100&city_id=%d", CITY_ID);
  r.request("GET", path, parser);
}

bool addToCart(uint32_t pizzaId) {
  JsonDocument add_pizza;

  JsonObject composition = add_pizza["composition"].add<JsonObject>();
  composition["good_id"] = pizzaId;
  composition["type"] = "good";
  add_pizza["city_id"] = CITY_ID;

  char payload[350];

  serializeJson(add_pizza, payload);
  char* responce = r.request("POST", "/cart/add", payload);


  JsonDocument pizza_add_responce;

  DeserializationError error = deserializeJson(pizza_add_responce, responce);
  free(responce);

  if (error) {
    pizza_add_responce.clear();
    return 1;
  }

  strcpy(unauthorized_token, pizza_add_responce["unauthorized_token"]);

  pizza_add_responce.clear();
  return 0;
}

bool placeOrder() {
  JsonDocument order;


  JsonObject user_data = order["user_data"].to<JsonObject>();
  user_data["username"] = ORDER_NAME;
  user_data["phone"] = ORDER_PHONE;
  user_data["email"] = ORDER_EMAIL;
  user_data["subscription_state"] = false;
  user_data["sms_state"] = false;

  JsonObject address_coordinates = order["address_coordinates"]["coordinates"].to<JsonObject>();
  address_coordinates["latitude"] = ORDER_GPS_LAT;
  address_coordinates["longitude"] = ORDER_GPS_LON;

  order["unauthorized_token"] = unauthorized_token;
  order["pay_type"] = "card_to_courier";
  order["city_id"] = CITY_ID;
  #ifdef ORDER_RESTAURANT_ID
  order["restaurant_id"] = ORDER_RESTAURANT_ID;
  #endif


  char payload[350];

  serializeJson(order, payload);

  char* responce = r.request("POST", "/order/save", payload);
  JsonDocument order_place_responce;
  DeserializationError error = deserializeJson(order_place_responce, responce);
  free(responce);
  if (error) {
    order_place_responce.clear();
    return 1;
  } else if (order_place_responce["success"] == false) {
    order_place_responce.clear();
    return 1;
  }
  order_id = order_place_responce["order_id"];
  order_place_responce.clear();
  return 0;
}

Menu::TrackingStatus getStatus() {
  char path[100];
  sprintf(path, "/order/status?city_id=%d&unauthorized_token=%s&order_id=%d", CITY_ID, unauthorized_token, order_id);
  char* responce = r.request("GET", path);
  JsonDocument orderStatus;

  DeserializationError error = deserializeJson(orderStatus, responce);
  free(responce);

  if (error) {
    return Menu::UNKNOWN;
  }

  Menu::TrackingStatus status = static_cast<Menu::TrackingStatus>(orderStatus["order_status"]);
  orderStatus.clear();
  return status;
}

void loop() {
  Menu &menu = Menu::getInstance();
  menu.tick();

  if (menu.getMenuPage() == Menu::PLACING_ORDER) {
    // питса выбрана, можно заказывать
    uint32_t selectedPizzaId = menu.getSelectedPizzaId();

    bool error = addToCart(selectedPizzaId);
    if (error) {
      menu.setMenuPage(Menu::PIZZA_SELECT);
      return;
    }

    error = placeOrder();
    if (error) {
      menu.setMenuPage(Menu::PIZZA_SELECT);
      return;
    }
    menu.setMenuPage(Menu::TRACKING);

  } else if (menu.getMenuPage() == Menu::TRACKING) {
    if (millis() - trackLastUpdate > TRACK_UPDATE_INTERVAL) {
      Menu::TrackingStatus status = getStatus();
      if (status != Menu::UNKNOWN) {
        menu.setTrackingStatus(status);
      }
      trackLastUpdate = millis();
    }
  }
}