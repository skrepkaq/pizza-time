#include "menu.h"


Menu::Menu() : lcd(0x3F, 16, 2), enc(ENCPIN1, ENCPIN2, ENCBTNPIN)  {
  Wire.begin(4, 5);
  lcd.init();
  lcd.backlight();
  pinMode(BRBPIN, INPUT_PULLUP);
  pinMode(LEDPIN, OUTPUT);
}


void Menu::setMenuPage(Page page) {
  switch (page) {
    case PRELOAD:
    case PIZZA_SELECT:
      lcd.cursor_off();
      menuPage = page;
      break;
    case DOUGH_SELECT:
      if (menuPage != PIZZA_SELECT && menuPage != SIZE_SELECT) return;
      selectedDough = Pizzas::TRADITIONAL;
      pizzaSizesIndex = 0;
      menuPage = page;
      initVariations();
      break;
    case SIZE_SELECT:
      if (menuPage != DOUGH_SELECT) return;
      menuPage = page;
      break;
    case CONFIRMATION:
      if (menuPage != SIZE_SELECT) return;
      menuPage = page;
      break;
    case PLACING_ORDER: {
      if (menuPage != CONFIRMATION) return;
      lcd.cursor_off();
      Pizzas::Variation var = pizzaSizes[pizzaSizesIndex];
      selectedPizzaId = var.id;
      menuPage = page;
      break;
    }
    case TRACKING:
      if (menuPage != PLACING_ORDER) return;
      menuPage = page;
      break;
  }
  draw();
};

void Menu::setTrackingStatus(TrackingStatus new_status) {
  if (trackingStatus == new_status) return;
  trackingStatus = new_status;
  draw();
}

void Menu::updateIndex() {
  if (menuPage != PIZZA_SELECT) return;
  drawIndex();
}

void Menu::initVariations() {
  Pizzas::Pizza pizza = pizzas.get(curentPizzaIndex);
  for (int i = 0; i < 4; i++) pizzaSizes[i] = {};
  pizzaSizesIndex = 0;
  uint8_t sizesCount;
  for (int i=0; i<pizza.variations_count; i++) {
    if (pizza.variations[i].dough != selectedDough) continue;
    pizzaSizes[sizesCount] = pizza.variations[i];
    sizesCount++;
  }
}

void Menu::tick() {
  animation();
  enc.tick();

  if (enc.click()) {
    switch (menuPage) {
      case PIZZA_SELECT:
        setMenuPage(DOUGH_SELECT);
        break;
      case DOUGH_SELECT:
        setMenuPage(SIZE_SELECT);
        break;
      case SIZE_SELECT:
        setMenuPage(CONFIRMATION);
        break;
      case CONFIRMATION:
        setMenuPage(PIZZA_SELECT);
        break;
      }
    return;
  }

  if (enc.hold()) {
    switch(menuPage) {
      case DOUGH_SELECT:
      case SIZE_SELECT:
      case CONFIRMATION:
        setMenuPage(PIZZA_SELECT);
        break;
    }
  }


  if (enc.turn()) {
    scroll(enc.dir());
  }

  if (!digitalRead(BRBPIN) && !brbflag) {
    if (menuPage == SIZE_SELECT) {
      brbflag = true;
      setMenuPage(CONFIRMATION);
    } else if (menuPage == CONFIRMATION) {
      brbflag = true;
      setMenuPage(PLACING_ORDER);
    }
  }
}


char* cutString(const char* str, size_t len) {
  size_t str_len = strlen(str);
  char* new_str = (char*)malloc((str_len + 1) * sizeof(char));

  strcpy(new_str, str);

  uint8_t c = 0;
  for (size_t i = 0; i < str_len; i++) {
    if (new_str[i] != 0xD0 && new_str[i] != 0xD1) {
      c++;
      if (c >= len) {
        new_str[i+1] = '\0';
        break;
      }
    }
  }
  return new_str;
}


void Menu::drawPrice(uint16_t price) {
  lcd.setCursor(0, 1);
  lcd.print(price, DEC);
  lcd.print("RUB   ");
}

void Menu::drawSizes() {
  lcd.setCursor(0, 1);
  for (int i=0; i<4; i++) {
    Pizzas::Variation pizza = pizzaSizes[i];
    if (pizza.id) {
      lcd.print(pizza.size, DEC);
    } else {
      lcd.print("  ");
    }
    lcd.print("  ");
  }
}

void Menu::drawIndex() {
  uint8_t curentPizza = curentPizzaIndex + 1;
  uint8_t totalPizzas = pizzas.get_count();
  lcd.setCursor(11, 1);
  if (curentPizza < 10) lcd.print(" ");
  if (totalPizzas < 10) lcd.print(" ");
  lcd.print(curentPizza, DEC);
  lcd.print("/");
  lcd.print(totalPizzas, DEC);
}

void Menu::drawTrackingStatus() {
  lcd.setCursor(0, 1);
  if (trackingStatus == UNKNOWN) {
    lcd.print("Получение...");
  } else {
    lcd.print(trackingStatusStrings[trackingStatus]);
  }
}

void Menu::drawDough(Pizzas::Dough dough) {
  switch (dough) {
    case Pizzas::TRADITIONAL:
      lcd.print("обычн");
      break;
    case Pizzas::THIN:
      lcd.print("тонк");
      break;
  }
  lcd.print(" ");
}

void Menu::drawPriceShort(uint16_t price) {
  lcd.setCursor(11, 0);
  for (int d=1000; price<d; d/=10) lcd.print(" ");;
  lcd.print(price, DEC);
  lcd.print("p");
}

void Menu::drawSizeSelectUpdate() {
  Pizzas::Variation var = pizzaSizes[pizzaSizesIndex];
  drawPriceShort(var.price);
  uint8_t cursor = pizzaSizesIndex * 4;
  lcd.setCursor(cursor, 1);
}

void Menu::drawPizzaName(char* name) {
  char* shortName = cutString(name, 16);
  lcd.setCursor(0, 0);
  lcd.print(shortName);
  free(shortName);
}

void Menu::drawVariation(Pizzas::Variation var) {
  lcd.setCursor(0, 1);
  lcd.write(var.dough == Pizzas::THIN ? '_' : 0xFF);
  lcd.print(" ");
  lcd.print(var.size, DEC);
  lcd.print(" ");
  lcd.print(var.price, DEC);
  lcd.print("p");
  lcd.setCursor(11, 1);
  lcd.print("conf?");
}


void Menu::scroll(int8_t direction) {
  switch (menuPage) {
    case PIZZA_SELECT: {
      uint8_t newIndex constrain(curentPizzaIndex + direction, 0, pizzas.get_count() - 1);
      if (curentPizzaIndex == newIndex) return;
      curentPizzaIndex = newIndex;
      draw();
      break;
    }
    case DOUGH_SELECT: {
      selectedDough = (selectedDough == Pizzas::THIN) ? Pizzas::TRADITIONAL : Pizzas::THIN;
      initVariations();
      draw();
      break;
    }
    case SIZE_SELECT: {
      uint8_t sizesCount;
      for (int i=0; i<4; i++) {
        if (pizzaSizes[i].id) sizesCount++;
      }
      pizzaSizesIndex = constrain(pizzaSizesIndex + direction, 0, sizesCount - 1);
      drawSizeSelectUpdate();
      break;
    }
  }
};


void Menu::animation() {
  switch (menuPage) {
    case WIFI_CONNECT:
      if (millis() - animationTick > 500) {
        animationState = !animationState;
        animationTick = millis();
        draw();
      }
      break;
    case DOUGH_SELECT:
    case SIZE_SELECT:
      if (millis() - animationTick > 300) {
        if (animationState) {
            lcd.cursor_on();
        } else {
            lcd.cursor_off();
        }
        animationState = !animationState;
        animationTick = millis();
      }
      break;
  }
}


void Menu::draw() {
  switch (menuPage) {
    case WIFI_CONNECT:
      lcd.setCursor(0, 0);
      if (animationState) {
          lcd.print("WIFI CONNECTING ");
      } else {
          lcd.print("WIFI CONNECTING.");
      }
      break;
    case PRELOAD:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Загрузка питс...");
      break;
    case PIZZA_SELECT: {
      Pizzas::Pizza pizza = pizzas.get(curentPizzaIndex);
      lcd.clear();
      drawPizzaName(pizza.name);
      drawPrice(pizza.price);
      drawIndex();
      digitalWrite(LEDPIN, LOW);
      brbflag = false;
      break;
    }
    case DOUGH_SELECT: {
      lcd.clear();
      lcd.setCursor(0, 0);
      drawDough(Pizzas::TRADITIONAL);
      drawDough(Pizzas::THIN);
      Pizzas::Variation var = pizzaSizes[pizzaSizesIndex];
      drawPriceShort(var.price);
      drawSizes();
      uint8_t cursor = (selectedDough == Pizzas::TRADITIONAL) ? 0 : 6;
      lcd.setCursor(cursor, 0);
      break;
    }
    case SIZE_SELECT:
      lcd.clear();
      lcd.setCursor(0, 0);
      drawDough(selectedDough);
      drawSizes();
      drawSizeSelectUpdate();
      break;
    case CONFIRMATION: {
      Pizzas::Pizza pizza = pizzas.get(curentPizzaIndex);
      Pizzas::Variation var = pizzaSizes[pizzaSizesIndex];
      lcd.clear();
      drawPizzaName(pizza.name);
      drawVariation(var);
      digitalWrite(LEDPIN, HIGH);
      delay(3000);
      brbflag = false;
      break;
    }
    case PLACING_ORDER:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Заказываем...");
      digitalWrite(LEDPIN, LOW);
      break;
    case TRACKING:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Статус:");
      drawTrackingStatus();
      break;
  }
}
