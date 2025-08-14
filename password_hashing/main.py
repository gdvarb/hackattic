import json
import hashlib
import requests
import base64

GET_URL = "https://hackattic.com/challenges/password_hashing/problem?access_token=840aa5334d071243"

response = requests.get(url=GET_URL).json() 
print(response)

salt = response["salt"]
decoded_salt = base64.b64decode(salt)
print(f"decoded salt: {decoded_salt}")
combined = response["password"] + str(decoded_salt)
print(f"combined password and decoded_salt: {combined}")


# sha256
m = hashlib.sha256()
m.update(combined.encode("utf-8"))
m.digest()
sha_res = m.hexdigest()
print(sha_res)



result = {
    "sha256": sha_res
}

POST_URL = "https://hackattic.com/challenges/password_hashing/solve?access_token=840aa5334d071243"
post_response = requests.post(url=POST_URL, json=result)
print(post_response.json())
