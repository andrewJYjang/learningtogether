import cv2
from ultralytics import YOLO

esp32_cam_url = "http://esp32 웹서버 ip address:81/stream"
model = YOLO("yolov8n.pt")
cap = cv2.VideoCapture(esp32_cam_url)

if not cap.isOpened():
    print("스트림을 열 수 없습니다.")
    exit()

print("스트리밍을 종료하려면 'q' 키를 누르세요.")

while True:
    ret, frame = cap.read()
    if not ret:
        print("프레임을 읽을 수 없습니다.")
        continue

    results = model(frame)
    annotated_frame = results[0].plot()
    cv2.imshow("ESP32-CAM YOLO Detection", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        print("스트리밍을 종료합니다.")
        break

cap.release()
cv2.destroyAllWindows()
