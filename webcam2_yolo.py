import cv2
import time
from ultralytics import YOLO

def find_webcam():
    """웹캠을 찾습니다."""
    print("웹캠 검색 중...")
    
    # 0부터 3까지 카메라 인덱스 확인 (Thonny에서는 간단하게)
    for i in range(4):
        cap = cv2.VideoCapture(i)
        if cap.isOpened():
            ret, frame = cap.read()
            if ret and frame is not None:
                print(f"웹캠 발견! 인덱스: {i}")
                cap.release()
                return i
            cap.release()
    
    print("웹캠을 찾을 수 없습니다.")
    return None

def main():
    print("=== 로지텍 웹캠 YOLO 객체 탐지 ===")
    print("Thonny에서 실행 중...")
    
    # 1. 웹캠 찾기
    camera_index = find_webcam()
    if camera_index is None:
        print("웹캠 연결을 확인하고 다시 시도해주세요.")
        return
    
    # 2. 웹캠 열기
    print(f"웹캠 {camera_index}을 사용합니다.")
    cap = cv2.VideoCapture(camera_index)
    
    # 웹캠 설정
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)  # 지연 최소화
    
    if not cap.isOpened():
        print("웹캠을 열 수 없습니다.")
        return
    
    # 3. YOLO 모델 로드
    print("YOLO 모델 로딩 중... (처음에는 시간이 걸릴 수 있습니다)")
    try:
        model = YOLO("yolov8n.pt")  # 가장 작고 빠른 모델
        print("YOLO 모델 로드 완료!")
    except Exception as e:
        print(f"YOLO 모델 로드 실패: {e}")
        print("인터넷 연결을 확인하세요. (처음 실행시 모델 다운로드)")
        cap.release()
        return
    
    # 5. 실시간 탐지 시작
    print("\n실시간 객체 탐지 시작!")
    print("- 종료: 'q' 키")
    print("- 일시정지: 스페이스바")
    print("- 스크린샷: 's' 키")
    
    frame_count = 0
    start_time = time.time()
    last_fps_time = time.time()
    
    while True:
        ret, frame = cap.read()
        if not ret:
            print("웹캠에서 영상을 읽을 수 없습니다.")
            break
        
        # YOLO로 객체 탐지
        results = model(frame, verbose=False)
        
        # 탐지 결과를 영상에 그리기
        annotated_frame = results[0].plot()
        
        # 탐지된 객체 수 표시
        detections = results[0].boxes
        if detections is not None:
            object_count = len(detections)
            # 화면 왼쪽 상단에 객체 수 표시
            cv2.putText(annotated_frame, f"Objects: {object_count}", 
                       (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            
            # Thonny 콘솔에도 출력 (5초마다)
            if time.time() - last_fps_time > 5:
                if object_count > 0:
                    # 탐지된 객체 이름들 가져오기
                    class_names = model.names
                    detected_objects = []
                    for box in detections:
                        class_id = int(box.cls[0])
                        confidence = float(box.conf[0])
                        if confidence > 0.5:  # 50% 이상 확신할 때만
                            detected_objects.append(f"{class_names[class_id]}({confidence:.2f})")
                    
                    if detected_objects:
                        print(f"탐지된 객체: {', '.join(detected_objects)}")
                
                last_fps_time = time.time()
        
        # 영상 화면에 표시
        cv2.imshow("Webcam YOLO - Thonny", annotated_frame)
        
        # 키 입력 처리
        key = cv2.waitKey(1) & 0xFF
        
        if key == ord('q'):
            print("프로그램 종료")
            break
        elif key == ord('s'):
            # 스크린샷 저장
            filename = f"screenshot_{int(time.time())}.jpg"
            cv2.imwrite(filename, annotated_frame)
            print(f"스크린샷 저장: {filename}")
        elif key == ord(' '):
            # 일시정지
            print("일시정지... 아무 키나 누르면 계속")
            cv2.waitKey(0)
        
        frame_count += 1
    
    # 종료 처리
    cap.release()
    cv2.destroyAllWindows()
    
    # 실행 통계
    total_time = time.time() - start_time
    print(f"\n=== 실행 완료 ===")
    print(f"총 {frame_count}프레임 처리")
    print(f"실행 시간: {total_time:.1f}초")
    print(f"평균 FPS: {frame_count/total_time:.1f}")

# Thonny에서 실행
if __name__ == "__main__":
    main()