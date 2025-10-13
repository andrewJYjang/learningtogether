# faster-whisper 대신 원래 openai-whisper 사용
!pip install -U openai-whisper

import whisper
import time
from google.colab import files

# 파일명 확인
filename = list(uploaded.keys())[0]
print(f"처리 파일: {filename}")

start = time.time()
print("\n모델 로딩 중...")
#model = whisper.load_model("base")  # GPU 자동 감지
# medium 모델 (더 정확, 10~15분 소요)
model = whisper.load_model("medium")

print("전사 시작... (진행상황이 표시됩니다)\n")
result = model.transcribe(
    filename, 
    language="ko",
    verbose=True,  # 진행상황 표시
    fp16=True  # GPU 사용
)

# 결과 저장
# 타임스탬프 포함 버전
with open("transcript_with_time.txt", "w", encoding="utf-8") as f:
    for segment in result["segments"]:
        start_time = segment["start"]
        end_time = segment["end"]
        text = segment["text"]
        f.write(f"[{start_time:.2f}s - {end_time:.2f}s] {text}\n")


# 다운로드
files.download("transcript.txt")
