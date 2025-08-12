import json
import hashlib
import requests

GET_URL = "https://hackattic.com/challenges/mini_miner/problem?access_token=840aa5334d071243"

response = requests.get(url=GET_URL)
print(response.json())

difficulty = response["difficulty"]
block = response["block"]
nonce_value = 0
h = hashlib.new("sha256")

itterating = True

while itterating:
    block["nonce"] = nonce_value
    h.update(str(block))
    val = h.hexdigest()

    for i in range(difficulty):
        if val[i].isdigit() and int(val[i]) == 0:
        continue
        else:
            nonce_value += 1
            break
        

