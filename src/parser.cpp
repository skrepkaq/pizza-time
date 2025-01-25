#include "parser.h"


void PizzaHandler::startDocument() {
}

void PizzaHandler::startArray(ElementPath path) {
}

void PizzaHandler::startObject(ElementPath path) {
}

void PizzaHandler::value(ElementPath path, ElementValue value) {
  char fullPath[200] = "";
  const char* currentKey = path.getKey();
  Menu &menu = Menu::getInstance();
  menu.tick();
  if (path.getCount() > 2) {
    char grand[50] = "";
    path.get(-2)->toString(grand);
    char parent[50] = "";
    path.get(-1)->toString(parent);
    if (currentKey[0] == '\0') {
      return;
    }
    if (!strcmp(grand, "goods")) {
        if(!strcmp(currentKey, "name")) {
        value.toString(last_pizza.name);
        uint8_t len = strlen(last_pizza.name);

        for (int i = 0; i < len - 1; ++i) {
          last_pizza.name[i] = last_pizza.name[i + 1];
        }
        last_pizza.name[len - 2] = '\0';

      } else if (!strcmp(currentKey, "id")) {
        last_pizza.id = value.getInt();
        ElementSelector* parent_selector = path.getParent();

        char key[50] = "";
        parent_selector->toString(key);
      } else if (!strcmp(currentKey, "good_type")) {
        if (strlen(last_pizza.name) && last_pizza.id && last_pizza.variations_count) {
          pizzas.add_pizza(last_pizza);
          if (menu.getMenuPage() == Menu::PRELOAD) {
            menu.setMenuPage(Menu::PIZZA_SELECT);
          } else {
            menu.updateIndex();
          }
        }
        last_pizza = {};
      }
    } else if (!strcmp(grand, "variations")) {
      if(!strcmp(currentKey, "id")) {
        char var_str[10];
        value.toString(var_str);
        new_variation.id = value.getInt();
      } else if(!strcmp(currentKey, "price")) {
        new_variation.price = value.getInt();
        #ifdef MINPRICE
        if (new_variation.price < MINPRICE) {
          new_variation = {};
        }
        #endif
        if (new_variation.price < last_pizza.price || last_pizza.price == 0) {
          last_pizza.price = new_variation.price;
        }
      } else if (!strcmp(currentKey, "stuffed_crust")) {
        char crust[10];
        value.toString(crust);
        if(strcmp(crust, "\"none\"")) {
          new_variation = {};
        }
      }
    } else if (!strcmp(parent, "kind")) {
      if(!strcmp(currentKey, "id")) {
        new_variation.dough = static_cast<Pizzas::Dough>(value.getInt());
      }
    } else if (!strcmp(parent, "size")) {
      if(!strcmp(currentKey, "value")) {
        new_variation.size = value.getInt();
        if (new_variation.id) {
          last_pizza.variations[last_pizza.variations_count] = new_variation;
          last_pizza.variations_count++;
        }
        new_variation = {};
      }
    }
  }
}

void PizzaHandler::endArray(ElementPath path) {
}

void PizzaHandler::endObject(ElementPath path) {
}

void PizzaHandler::endDocument() {
}

void PizzaHandler::whitespace(char c) {
}