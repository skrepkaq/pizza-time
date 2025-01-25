#pragma once


class Pizzas {
  public:
    static Pizzas& getInstance() {
        static Pizzas instance;
        return instance;
    }
    enum Dough {
        THIN = 1,
        TRADITIONAL = 3
    };

    struct Variation {
      uint32_t id;
      uint16_t price;
      Dough dough;
      uint8_t size;
    };

    struct Pizza {
      uint16_t id;
      char name[100];
      uint16_t price;
      Variation variations[20];
      uint8_t variations_count = 0;
    };

    Pizza* get_all() {
      return pizzas;
    }
    Pizza get(uint8_t index) {
      return pizzas[index];
    }
    uint8_t get_count() {
      return pizzas_count;
    }

    void add_pizza(Pizza pizza) {
      pizzas[pizzas_count] = pizza;
      pizzas_count++;
    }
  private:
    Pizzas() {};
    Pizza pizzas[100];
    uint8_t pizzas_count = 0;
};