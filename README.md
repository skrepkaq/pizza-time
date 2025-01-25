# PizzaTime - кнопка для заказа пиццы с помощью ESP32
[![тут должно было быть видео](https://img.youtube.com/vi/6Nl7IEtmYt4/maxresdefault.jpg)](https://youtu.be/6Nl7IEtmYt4)

## Описание
PizzaTime button - это устройство на базе ESP32 для заказа пиццы из Papa John's одним нажатием кнопки.

## Возможности
- Выбор пиццы с помощью энкодера и LCD дисплея
- Заказ выбранной пиццы одним нажатием большой красной кнопки
- Отслеживание статуса заказа на дисплее

## Схема подключения
- LCD дисплей по I2C (SDA - pin 4, SCL - pin 5)
- Энкодер (DT - pin 8, CLK - pin 6, BUTTON - pin 7)
- Большая красная кнопка (pin 10)
- Светодиод кнопки (pin 9)

## Навигация по меню
- Выбор пиццы производится с помощью крутилки
- Для подтверждения выбора нажмите на крутилку
- Чтобы вернуться назад зажмите крутилку на 2 сек
- Чтобы заказать пиццу  нажмите на большую красную кнопку когда она загорится

## Настройка
1. Склонируйте репозиторий с подмодулями:

```bash
git clone --recursive https://github.com/skrepkaq/PizzaTime.git
```
2.  Откройте проект в [PlatformIO](https://platformio.org/install/integration/)

3. Нажмите "build" или скопируйте пример конфига вручную

```bash
cp config.example include/config.h
```

4. В файле `include/config.h` укажите:
- Данные WiFi сети
- IP прокси-сервера
- ID города
- Пины для подключения компонентов
- Данные для заказа (имя, телефон и т.д.)

5. Соберите и загрузите прошивку на Arduino

6. Запустите прокси-сервер:

```bash
cd proxy
docker-compose up -d
```

7. Включите кнопку и заказывайте пиццу