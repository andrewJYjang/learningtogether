import pyupbit

# API 키 설정
access_key = "****************"
secret_key = "****************"

# 업비트 객체 생성
upbit = pyupbit.Upbit(access_key, secret_key)

# 잔액 조회 (가장 간단한 방법)
balances = upbit.get_balances()

# 예쁘게 출력하기
def print_balances(balances):
    """보유 자산을 좀 더 유연한 디자인을 요구하여 출력"""
    if not balances:
        print("❌ 보유 자산이 없거나 API 오류")
        return
    
    print("\n" + "=" * 70)
    print("🏦                    업비트 보유 자산                    ")
    print("=" * 70)
    print(f"{'💰 코인':<15} {'📊 보유량':<20} {'💵 평단가':<15} {'💎 상태'}")
    print("-" * 70)
    
    total_krw = 0
    crypto_list = []
    
    # 데이터 정리
    for balance in balances:
        currency = balance['currency']
        amount = float(balance['balance']) + float(balance['locked'])
        avg_price = float(balance['avg_buy_price'])
        
        if amount > 0:  # 0보다 큰 잔액만 표시
            if currency == 'KRW':
                total_krw = amount
            else:
                crypto_list.append({
                    'currency': currency,
                    'amount': amount,
                    'avg_price': avg_price
                })
    
    # KRW 먼저 출력
    if total_krw > 0:
        print(f"{'💵 KRW':<15} {f'{total_krw:,.0f}원':<20} {'-':<15} {'💰 현금'}")
    
    # 암호화폐 출력 (보유량 순으로 정렬)
    crypto_list.sort(key=lambda x: x['amount'], reverse=True)
    
    for crypto in crypto_list:
        currency = crypto['currency']
        amount = crypto['amount']
        avg_price = crypto['avg_price']
        
        # 현재가 조회
        try:
            current_price = pyupbit.get_current_price(f"KRW-{currency}")
            if current_price and avg_price > 0:
                profit_rate = ((current_price - avg_price) / avg_price) * 100
                if profit_rate > 5:
                    status = f"🚀 +{profit_rate:.1f}%"
                elif profit_rate > 0:
                    status = f"📈 +{profit_rate:.1f}%"
                elif profit_rate < -5:
                    status = f"💥 {profit_rate:.1f}%"
                else:
                    status = f"📉 {profit_rate:.1f}%"
            else:
                status = "❓ 정보없음"
        except:
            status = "❓ 조회실패"
        
        # 소수점 자리수 조정
        if amount >= 1:
            amount_str = f"{amount:,.4f}"
        else:
            amount_str = f"{amount:.8f}"
        
        avg_price_str = f"{avg_price:,.0f}원" if avg_price > 0 else "-"
        
        print(f"{'🪙 ' + currency:<15} {amount_str:<20} {avg_price_str:<15} {status}")
    
    print("-" * 70)
    print(f"총 {len([b for b in balances if float(b['balance']) + float(b['locked']) > 0])}개 자산 보유중")
    print("=" * 70)

# 실행
if access_key == "본인의_Access_Key":
    print("⚠️  API 키를 설정해주세요!")
    print("1. 업비트에서 API 키 발급 (자산조회 권한)")
    print("2. access_key, secret_key 값 변경")
else:
    # 예쁘게 출력
    print_balances(balances)
    
    # 특정 코인만 조회 (기존 방식)
    btc_balance = upbit.get_balance("BTC")
    krw_balance = upbit.get_balance("KRW")
    
    print(f"\n🔍 특정 자산 조회:")
    print(f"   BTC: {btc_balance if btc_balance else 0}")
    print(f"   KRW: {krw_balance if krw_balance else 0:,.0f}원")


# 더 간단한 버전
def simple_print(balances):
    """정말 간단한 출력"""
    print("\n📋 보유 자산 목록:")
    for balance in balances:
        currency = balance['currency']
        amount = float(balance['balance']) + float(balance['locked'])
        if amount > 0:
            if currency == 'KRW':
                print(f"  💰 {currency}: {amount:,.0f}원")
            else:
                print(f"  🪙 {currency}: {amount:.8f}")

# 간단 버전 실행 예시
# simple_print(balances)
