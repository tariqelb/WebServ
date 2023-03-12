#!/bin/bash

curl -X PUT http://127.0.0.1:8080 -H "Content-Type: application/json" --data-binary @- <<DATA
{
  "Id": 12345,
  "Customer": "John Smith",
  "Quantity": 1,
  "Price": 10.00
}
