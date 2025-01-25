#pragma once
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
#include <EncButton.h>

#include "pizzas.h"
#include "config.h"


class Menu {
public:
    static Menu& getInstance() {
        static Menu instance;
        return instance;
    }

    enum Page {
        WIFI_CONNECT,
        PRELOAD,
        PIZZA_SELECT,
        DOUGH_SELECT,
        SIZE_SELECT,
        CONFIRMATION,
        PLACING_ORDER,
        TRACKING
    };

    enum TrackingStatus {
        UNKNOWN = -1,
        PROCESSING,
        ACCEPTED,
        COOKING,
        COURIER_WAIT,
        DELIVERY,
        FINISHED,
        CANCELLED
    };

    void tick();
    void setMenuPage(Page page);
    uint8_t getMenuPage() {return menuPage;}
    void setTrackingStatus(TrackingStatus status);
    void updateIndex();
    uint32_t getSelectedPizzaId() {return selectedPizzaId;}
private:
    LCD_1602_RUS lcd;
    EncButton enc;
    void animation();
    void scroll(int8_t direction);
    void initVariations();
    void drawSizes();
    void drawSizeSelectUpdate();
    void drawTrackingStatus();
    void drawPizzaName(char* name);
    void drawVariation(Pizzas::Variation var);
    void draw();
    void drawPrice(uint16_t price);
    void drawPriceShort(uint16_t price);
    void drawIndex();
    void drawDough(Pizzas::Dough dough);
    const char* trackingStatusStrings[7] = {
        "Обработка",
        "Принят",
        "Готовится",
        "Ожидание курьера",
        "Курьер в пути",
        "Доставлено",
        "Отменён"
    };
    boolean brbflag;
    uint32_t animationTick;
    boolean animationState;
    Pizzas& pizzas = Pizzas::getInstance();
    uint8_t curentPizzaIndex;
    Page menuPage;
    Pizzas::Dough selectedDough;
    Pizzas::Variation pizzaSizes[4];
    uint8_t pizzaSizesIndex;
    uint32_t selectedPizzaId;
    TrackingStatus trackingStatus = UNKNOWN;

    Menu();

};
