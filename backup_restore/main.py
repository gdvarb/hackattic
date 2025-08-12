import requests
import json

request = requests.get("https://hackattic.com/challenges/backup_restore/problem?access_token=840aa5334d071243")
print(request.json())
