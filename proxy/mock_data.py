def get_mock_data(path: str) -> dict:
    if '/order/save' in path:
        return ORDER_SUCCESS
        return ORDER_FAIL
    if '/order/status' in path:
        if ORDER_STATUS["order_status"] < 5:
            ORDER_STATUS["order_status"] += 1
        return ORDER_STATUS
    if '/cart/add' in path:
        ORDER_STATUS["order_status"] = -1
        return CART_ADD
    if '/catalog/category-goods' in path:
        return PIZZAS


CART_ADD = {
    "unauthorized_token": "7904e2634334760a642a169c0f7c67f0",
    "cart_id": 123456789,
    "composition": [
        {
            "item": {
                "name": "С ананасами"
            }
        }
    ]
}


ORDER_FAIL = {
    "success": False,
    "message": {
        "composition": [
            "Минимальная стоимость заказа на доставку 99999 ₽"
        ]
    },
    "status": 400
}

ORDER_SUCCESS = {
    "order_id": 69420
}


ORDER_STATUS = {
    "order_status": -1
}


PIZZAS = [
  {
    "goods": [
      {
        "id": 1234,
        "name": "С ананасами",
        "variations": [
          {
            "id": 123430,
            "price": 599,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 23
            }
          },
          {
            "id": 123431,
            "price": 879,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 30
            }
          },
          {
            "id": 123432,
            "price": 1079,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 35
            }
          },
          {
            "id": 123433,
            "price": 1379,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 40
            }
          },
          {
            "id": 123410,
            "price": 879,
            "kind": {
              "id": 1
            },
            "stuffed_crust": "none",
            "size": {
              "value": 30
            }
          },
          {
            "id": 123411,
            "price": 1079,
            "kind": {
              "id": 1
            },
            "stuffed_crust": "none",
            "size": {
              "value": 35
            }
          },
          {
            "id": 123412,
            "price": 1379,
            "kind": {
              "id": 1
            },
            "stuffed_crust": "none",
            "size": {
              "value": 40
            }
          }
        ],
        "good_type": "promotional"
      },
      {
        "id": 4321,
        "name": "С гравием",
        "variations": [
          {
            "id": 223430,
            "price": 429,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 23
            }
          },
          {
            "id": 223431,
            "price": 429,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 30
            }
          },
          {
            "id": 223432,
            "price": 4279,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 35
            }
          },
          {
            "id": 223433,
            "price": 4279,
            "kind": {
              "id": 3
            },
            "stuffed_crust": "none",
            "size": {
              "value": 40
            }
          },
          {
            "id": 223410,
            "price": 429,
            "kind": {
              "id": 1
            },
            "stuffed_crust": "none",
            "size": {
              "value": 30
            }
          },
          {
            "id": 223411,
            "price": 4279,
            "kind": {
              "id": 1
            },
            "stuffed_crust": "none",
            "size": {
              "value": 35
            }
          },
          {
            "id": 223412,
            "price": 4279,
            "kind": {
              "id": 1
            },
            "stuffed_crust": "none",
            "size": {
              "value": 40
            }
          }
        ],
        "good_type": "promotional"
      }
    ]
  }
]
