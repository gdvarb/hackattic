import json
import hashlib
import requests
import base64

GET_URL = "https://hackattic.com/challenges/password_hashing/problem?access_token=840aa5334d071243"

response = requests.get(url=GET_URL).json() 
print(response)


# combine password and decoded salt into byte object
encoded_password = response["password"].encode("utf-8") # encode password string into bytes
decoded_salt = base64.b64decode(response["salt"])
combined_bytes = encoded_password + decoded_salt

# sha256
m = hashlib.sha256()
m.update(combined_bytes)
sha_res = m.hexdigest()
print(sha_res)

# hmac




result = {
    "sha256": sha_res
}

POST_URL = "https://hackattic.com/challenges/password_hashing/solve?access_token=840aa5334d071243"
post_response = requests.post(url=POST_URL, json=result)
print(post_response.json())
