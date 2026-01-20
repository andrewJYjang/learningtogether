import whisper
import time
import os
from pathlib import Path

# 설정
INPUT_FOLDER = "input_audio"  # 음성 파일이 있는 폴더
OUTPUT_FOLDER = "transcripts"  # 전사 결과를 저장할 폴더
MODEL_SIZE = "medium"  # tiny, base, small, medium, large
LANGUAGE = "ko"

# 지원하는 오디오/비디오 확장자
SUPPORTED_EXTENSIONS = {'.mp3', '.mp4', '.wav', '.m4a', '.flac', '.ogg', '.avi', '.mkv', '.mov'}

def main():
    # 출력 폴더 생성
    os.makedirs(OUTPUT_FOLDER, exist_ok=True)
    
    # 입력 폴더에서 음성 파일 찾기
    input_path = Path(INPUT_FOLDER)
    audio_files = [f for f in input_path.iterdir() 
                   if f.is_file() and f.suffix.lower() in SUPPORTED_EXTENSIONS]
    
    if not audio_files:
        print(f"❌ '{INPUT_FOLDER}' 폴더에 음성 파일이 없습니다.")
        return
    
    audio_files.sort()  # 파일명 순으로 정렬
    total_files = len(audio_files)
    
    print(f"📁 총 {total_files}개의 파일을 발견했습니다.")
    print(f"📝 처리할 파일 목록:")
    for i, f in enumerate(audio_files, 1):
        print(f"  {i}. {f.name}")
    
    # 모델 로딩
    print(f"\n🔄 Whisper {MODEL_SIZE} 모델 로딩 중...")
    model_start = time.time()
    model = whisper.load_model(MODEL_SIZE)
    print(f"✅ 모델 로딩 완료 ({time.time() - model_start:.1f}초)\n")
    
    # 전체 시작 시간
    total_start = time.time()
    success_count = 0
    
    # 각 파일 처리
    for idx, audio_file in enumerate(audio_files, 1):
        print(f"{'='*60}")
        print(f"[{idx}/{total_files}] 처리 중: {audio_file.name}")
        print(f"{'='*60}")
        
        file_start = time.time()
        
        try:
            # 전사 실행
            result = model.transcribe(
                str(audio_file),
                language=LANGUAGE,
                fp16=True,
                verbose=True
            )
            
            # 결과 저장
            output_filename = f"{audio_file.stem}_transcript.txt"
            output_path = Path(OUTPUT_FOLDER) / output_filename
            
            with open(output_path, "w", encoding="utf-8") as f:
                f.write(result["text"])
            
            file_elapsed = time.time() - file_start
            success_count += 1
            
            print(f"\n✅ 완료: {output_filename}")
            print(f"⏱️  소요 시간: {file_elapsed/60:.1f}분")
            print(f"📊 진행률: {idx}/{total_files} ({idx/total_files*100:.1f}%)\n")
            
        except Exception as e:
            print(f"\n❌ 오류 발생: {audio_file.name}")
            print(f"   에러 메시지: {str(e)}\n")
    
    # 최종 결과
    total_elapsed = time.time() - total_start
    print(f"{'='*60}")
    print(f"🎉 전체 작업 완료!")
    print(f"{'='*60}")
    print(f"✅ 성공: {success_count}/{total_files}개")
    print(f"⏱️  총 소요 시간: {total_elapsed/60:.1f}분")
    print(f"📁 결과 저장 위치: {OUTPUT_FOLDER}/")
    print(f"{'='*60}")

if __name__ == "__main__":
    main()
