import ctypes
import time

# .so 파일 로드
lib = ctypes.CDLL(r'/home/mini/MB1414/src/libMB1414.so')

# 필요한 함수 호출 설정
lib.create_sensor.restype = ctypes.c_void_p
lib.destroy_sensor.argtypes = [ctypes.c_void_p]
lib.sensor_run.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
lib.sensor_stop.argtypes = [ctypes.c_void_p]
lib.sensor_get_info.restype = ctypes.c_char_p
lib.sensor_get_value.restype = ctypes.c_short
lib.sensor_get_value.argtypes = [ctypes.c_void_p]

# 센서 객체 생성
sensor = lib.create_sensor()

try:
    # 센서 실행
    lib.sensor_run(sensor, b"/dev/ttyUSB0")

    # 센서 정보 출력
    info = lib.sensor_get_info(sensor)
    decoded_info = info.decode('utf-8')  # UTF-8 디코딩
    print("Decoded Info:")
    print(decoded_info)

    # 센서 값 지속적으로 읽기
    print("Reading sensor values continuously. Press Ctrl+C to stop.")
    while True:
        value = lib.sensor_get_value(sensor)
        there_is_people = value <= 35  # 값이 35 이하인 경우 True
        print(f"Sensor Value: {value}, There is people: {there_is_people}")
        time.sleep(0.1)  # 0.1초 대기
except KeyboardInterrupt:
    # Ctrl+C를 눌렀을 때 루프 종료
    print("\nStopping sensor...")
finally:
    # 센서 중지 및 삭제
    lib.sensor_stop(sensor)
    lib.destroy_sensor(sensor)
    print("Sensor stopped and resources released.")
