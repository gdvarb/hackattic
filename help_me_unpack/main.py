import json
import requests
import base64
import struct

URL = 'https://hackattic.com/challenges/help_me_unpack/problem?access_token=840aa5334d071243'
TOKEN='840aa5334d071243'

x = requests.get(url=URL)
data = x.json()
print(data)
print(data["bytes"])
test = base64.b64decode(data["bytes"])


signed_int = struct.unpack("<i",test[:4] ) 
print(signed_int[0])

unsigned_int = struct.unpack("<I",test[4:8])
print(unsigned_int[0])

signed_short = struct.unpack("<h",test[8:10])
print(signed_short[0])

float_int = struct.unpack("<f",test[12:16])
print(float_int[0])

double = struct.unpack("<d",test[16:24])
print(double[0])

double_big_endien= struct.unpack(">d",test[24:32])
print(double_big_endien[0])

data_for_json = {
    "int":signed_int[0],
    "uint": unsigned_int[0],
    "short": signed_short[0],
    "float": float_int[0],
    "double": double[0],
    "big_endian_double":double_big_endien[0]
}

json_string = json.dumps(data)
print(json_string)

POST_URL = "https://hackattic.com/challenges/help_me_unpack/solve?access_token=840aa5334d071243"
y = requests.post(POST_URL, json=data_for_json)

print(y.json())
