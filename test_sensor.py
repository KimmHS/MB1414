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

# 센서 실행
lib.sensor_run(sensor, b"/dev/ttyUSB0")

# 센서 정보 출력
info = lib.sensor_get_info(sensor)
decoded_info = info.decode('utf-8')  # UTF-8 디코딩
print("Decoded Info:")
print(decoded_info)
time.sleep(0.1)
for _ in range(40):  # 20번 반복
    value = lib.sensor_get_value(sensor)
    time.sleep(0.1)
    print("Sensor Value:", value)
    

# 센서 중지 및 삭제
lib.sensor_stop(sensor)
lib.destroy_sensor(sensor)
