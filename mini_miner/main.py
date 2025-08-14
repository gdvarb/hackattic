import json
import hashlib
import requests

GET_URL = "https://hackattic.com/challenges/mini_miner/problem?access_token=840aa5334d071243"

response = requests.get(url=GET_URL)
print(response.json())
data = response.json()

difficulty = data["difficulty"]
block = data["block"]
print("block:", block)
nonce_value = 0

itterating = True

while itterating:
    h = hashlib.sha256()
    block["nonce"] = nonce_value
    # standardize json
    updated_block = json.dumps(block, separators=(',',':'), sort_keys=True)
    
    # encode string so that it can be hashed
    h.update(updated_block.encode("utf-8"))
    hex = h.hexdigest()
    # convert hex string to int
    hex_int = int(hex, 16)
    # convert int to binary and slice off leading 0b
    hex_binary_with_prefix = bin(hex_int)
    hex_binary = hex_binary_with_prefix[2:] 
    # pad the string so it has the proper max length
    padded_hex = hex_binary.zfill(256)
    # check if the required number of bits are zeros
    chars = padded_hex[:difficulty]
    print(chars)
    if (chars == len(chars) * chars[0]) and chars[0] == "0":
        itterating = False
    else:
        nonce_value += 1

data_for_json = {"nonce": nonce_value}
print(data_for_json)
POST_URL = "https://hackattic.com/challenges/mini_miner/solve?access_token=840aa5334d071243"
post_response = requests.post(POST_URL, json=data_for_json)
print(difficulty)
print(post_response.json())
