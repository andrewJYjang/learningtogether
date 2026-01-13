import whisper #whisper 모듈 에러 발생 원인은 open ai whisper 따로 설치를 했어야 함
import time

start = time.time()

print("모델 로딩 중...")
model = whisper.load_model("large") #medium

print("전사 시작...")
result = model.transcribe(
    "260112_140221.m4a",
    language="ko",
    fp16=True,
    verbose=True
)

with open("transcript.txt", "w", encoding="utf-8") as f:
    f.write(result["text"])

elapsed = time.time() - start

print(f"\n✅ 완료! {elapsed/60:.1f}분 소요")
