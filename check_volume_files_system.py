import os
import shutil
from pathlib import Path
import threading
import time

class VideoSearcher:
    def __init__(self):
        self.stop_search = False
        self.pause_search = False
        
    def stop(self):
        """검색 중단"""
        self.stop_search = True
        print("\n검색을 중단합니다...")

    def pause(self):
        """검색 일시정지"""
        self.pause_search = True

    def resume(self):
        """검색 재개"""
        self.pause_search = False

def get_disk_usage():
    """디스크 용량 정보를 확인하고 크기별로 정렬된 리스트를 반환"""
    disk_info = []
    
    if os.name == 'nt':  # Windows
        import string
        drives = ['%s:' % d for d in string.ascii_uppercase if os.path.exists('%s:' % d)]
        
        for drive in drives:
            try:
                total, used, free = shutil.disk_usage(drive)
                total_gb = round(total / (1024**3), 2)
                used_gb = round(used / (1024**3), 2)
                free_gb = round(free / (1024**3), 2)
                usage_percent = round((used / total) * 100, 1)
                
                disk_info.append({
                    'drive': drive,
                    'total_gb': total_gb,
                    'used_gb': used_gb,
                    'free_gb': free_gb,
                    'usage_percent': usage_percent
                })
            except (OSError, PermissionError):
                continue
    else:  # Linux/Mac
        try:
            total, used, free = shutil.disk_usage('/')
            total_gb = round(total / (1024**3), 2)
            used_gb = round(used / (1024**3), 2)
            free_gb = round(free / (1024**3), 2)
            usage_percent = round((used / total) * 100, 1)
            
            disk_info.append({
                'drive': '/',
                'total_gb': total_gb,
                'used_gb': used_gb,
                'free_gb': free_gb,
                'usage_percent': usage_percent
            })
        except (OSError, PermissionError):
            pass
    
    # 총 용량 기준으로 내림차순 정렬
    disk_info.sort(key=lambda x: x['total_gb'], reverse=True)
    return disk_info

def print_disk_info(disk_list):
    """디스크 정보를 간단하게 출력"""
    print("\n디스크 용량 정보:")
    for disk in disk_list:
        print(f"{disk['drive']} - 총:{disk['total_gb']}GB, 사용:{disk['used_gb']}GB, 여유:{disk['free_gb']}GB")

def show_continue_dialog():
    """계속 검색할 것인지 확인하는 창"""
    print("\n" + "="*50)
    print("검색 진행 상황 확인")
    print("="*50)
    print("현재까지 검색된 결과가 있습니다.")
    print("계속 검색하시겠습니까?")
    print("1. 계속 검색 (y/yes)")
    print("2. 검색 중단 (n/no)")
    print("3. 현재까지 결과 보기 (r/result)")
    print("="*50)
    
    while True:
        choice = input("선택하세요 (y/n/r): ").strip().lower()
        if choice in ['y', 'yes', '계속', '1']:
            return 'continue'
        elif choice in ['n', 'no', '중단', '2']:
            return 'stop'
        elif choice in ['r', 'result', '결과', '3']:
            return 'result'
        else:
            print("올바른 선택을 입력하세요 (y/n/r)")

def get_video_files(searcher):
    """동영상 파일을 빠르게 검색"""
    video_extensions = ['.mp4', '.avi', '.mkv', '.mov', '.wmv', '.flv', '.webm', '.m4v']
    all_videos = []
    large_videos = []
    search_count = 0
    
    # 주요 폴더만 검색
    search_paths = []
    user_home = os.path.expanduser('~')
    search_paths.extend([
        os.path.join(user_home, 'Videos'),
        os.path.join(user_home, 'Downloads'),
        os.path.join(user_home, 'Desktop'),
        os.path.join(user_home, 'Documents')
    ])
    
    # Windows의 경우 추가 경로
    if os.name == 'nt':
        search_paths.extend(['C:\\Users\\Public\\Videos'])
    
    print("동영상 파일 검색 중... ('q' 입력시 중단)")
    
    for search_path in search_paths:
        if searcher.stop_search:
            break
            
        if os.path.exists(search_path):
            print(f"검색 중: {search_path}")
            try:
                for root, dirs, files in os.walk(search_path):
                    if searcher.stop_search:
                        break
                    
                    # 일시정지 체크
                    while searcher.pause_search and not searcher.stop_search:
                        time.sleep(0.1)
                        
                    for file in files:
                        if searcher.stop_search:
                            break
                            
                        if os.path.splitext(file)[1].lower() in video_extensions:
                            try:
                                file_path = os.path.join(root, file)
                                file_size = os.path.getsize(file_path)
                                file_size_gb = round(file_size / (1024**3), 2)
                                
                                video_info = {
                                    'name': file,
                                    'path': file_path,
                                    'size_gb': file_size_gb
                                }
                                
                                all_videos.append(video_info)
                                search_count += 1
                                
                                if file_size_gb >= 1.0:
                                    large_videos.append(video_info)
                                
                                # 100개마다 확인 창 표시
                                if search_count % 100 == 0 and search_count > 0:
                                    searcher.pause()
                                    print(f"\n현재까지 {search_count}개의 동영상 파일을 찾았습니다.")
                                    choice = show_continue_dialog()
                                    
                                    if choice == 'stop':
                                        searcher.stop()
                                        break
                                    elif choice == 'result':
                                        # 현재까지 결과 보기
                                        temp_categories = categorize_videos(all_videos)
                                        print_intermediate_results(all_videos, large_videos, temp_categories)
                                        
                                        # 다시 선택
                                        choice2 = show_continue_dialog()
                                        if choice2 == 'stop':
                                            searcher.stop()
                                            break
                                    
                                    searcher.resume()
                                    print("검색을 계속합니다...")
                                    
                            except (OSError, PermissionError):
                                continue
                                
            except (OSError, PermissionError):
                continue
    
    # 크기순 정렬
    all_videos.sort(key=lambda x: x['size_gb'], reverse=True)
    large_videos.sort(key=lambda x: x['size_gb'], reverse=True)
    
    return all_videos, large_videos

def categorize_videos(video_list):
    """동영상을 크기별로 간단 분류"""
    small = [v for v in video_list if v['size_gb'] < 0.5]
    medium = [v for v in video_list if 0.5 <= v['size_gb'] < 1.0]
    large = [v for v in video_list if v['size_gb'] >= 1.0]
    
    return {'small': small, 'medium': medium, 'large': large}

def print_intermediate_results(all_videos, large_videos, categories):
    """중간 결과를 출력"""
    print(f"\n현재까지 검색 결과:")
    print(f"총 동영상 파일: {len(all_videos)}개")
    print(f"소형(0.5GB미만): {len(categories['small'])}개")
    print(f"중형(0.5-1GB): {len(categories['medium'])}개") 
    print(f"대형(1GB이상): {len(categories['large'])}개")
    
    if large_videos:
        print(f"\n1GB 이상 파일 상위 5개:")
        for i, video in enumerate(large_videos[:5], 1):
            print(f"{i}. {video['name']} ({video['size_gb']}GB)")

def print_results(all_videos, large_videos, categories):
    """최종 결과를 출력"""
    print(f"\n최종 검색 결과:")
    print(f"총 동영상 파일: {len(all_videos)}개")
    print(f"소형(0.5GB미만): {len(categories['small'])}개")
    print(f"중형(0.5-1GB): {len(categories['medium'])}개") 
    print(f"대형(1GB이상): {len(categories['large'])}개")
    
    if large_videos:
        print(f"\n1GB 이상 파일 {len(large_videos)}개:")
        for i, video in enumerate(large_videos[:10], 1):
            print(f"{i}. {video['name']} ({video['size_gb']}GB)")
    
    if all_videos:
        print(f"\n가장 큰 파일 TOP 10:")
        for i, video in enumerate(all_videos[:10], 1):
            print(f"{i}. {video['name']} ({video['size_gb']}GB)")

def input_monitor(searcher):
    """입력 모니터링"""
    try:
        while not searcher.stop_search:
            user_input = input().strip().lower()
            if user_input == 'q':
                searcher.stop()
                break
    except:
        pass

def main():
    """메인 함수"""
    try:
        # 디스크 용량 확인
        disk_info = get_disk_usage()
        if disk_info:
            print_disk_info(disk_info)
        
        # 검색 객체 생성
        searcher = VideoSearcher()
        
        # 입력 모니터링 스레드
        input_thread = threading.Thread(target=input_monitor, args=(searcher,), daemon=True)
        input_thread.start()
        
        try:
            # 동영상 검색
            all_videos, large_videos = get_video_files(searcher)
            
            if all_videos:
                categories = categorize_videos(all_videos)
                print_results(all_videos, large_videos, categories)
            else:
                print("\n동영상 파일을 찾을 수 없습니다.")
                
        except KeyboardInterrupt:
            searcher.stop()
            print("\n검색이 중단되었습니다.")
            
    except Exception as e:
        print(f"오류: {e}")

if __name__ == '__main__':
    main()






