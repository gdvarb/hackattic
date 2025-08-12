import json
import requests
import cv2

URL = 'https://hackattic.com/challenges/reading_qr/problem?access_token=840aa5334d071243'
TOKEN='840aa5334d071243'

x = requests.get(url=URL)
data = x.json()
print(data)
image_url = data["image_url"]
save_path = "hackattic_qr_image.png"

try:
    response = requests.get(image_url, stream=True)
    response.raise_for_status()

    with open(save_path, "wb") as file:
        for chunk in response.iter_content(chunk_size=8192):
            file.write(chunk)
        print(f"image downloaded to {save_path}")

except requests.exceptions.RequestException as e:
    print(f"error downloading image: {e}")


img = cv2.imread("hackattic_qr_image.png")

#qr code detector
detector = cv2.QRCodeDetector()

decoded_data, bbox, staight_qrcode = detector.detectAndDecode(img)

if decoded_data:
    print(f"decoded data: {decoded_data}")
else:
    print("decoded data could not be detected")

data_for_json = {"code": decoded_data}

POST = "https://hackattic.com/challenges/reading_qr/solve?access_token=840aa5334d071243"

post_response = requests.post(POST, json=data_for_json)
print(post_response.json())
