import whisper
import time

start = time.time()

print("모델 로딩 중...")
model = whisper.load_model("medium")  # 또는 "base전사 시작...")
result = model.transcribe(
    "P250_0620.mp3",
    language="ko",
    fp16=True,  # GPU 사용!
    verbose=True  # 진행상황 표시
)

# 결과 저장
with open("transcript.txt", "w", encoding="utf-8") as f:
    f.write(result["text"])

elapsed = time.time() - start

print(f"\n✅ 전사 완료! {elapsed/60:.1f}분 소요")
