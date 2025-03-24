import json

with open('simulator.json','r',encoding='utf-8') as file:
    data=json.load(file)
# data = {"nom": "John Doe", "age": 30, "ville": "Paris"}
print(json.dumps(data))