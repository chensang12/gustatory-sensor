import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

ser = serial.Serial('COM8', baudrate=115200, timeout=1)
CCD_LENGTH = 3800
CCD_SIZE = CCD_LENGTH * 2  # 每个元素是8位，总长度是16位的两倍
DISPLAY_SIZE = 500  # 要显示的数据点数量
s = np.zeros(DISPLAY_SIZE, dtype=np.uint16)
plt.pause(0.01)  # 可以尝试使用更小的值

fig, ax = plt.subplots()
line, = ax.plot(s)
ax.set_xlim([0, 3800])  # 设置x轴范围，CCDSize是数据的总长度
ax.set_ylim([0, 4096])  # 根据你的ADC范围调整y轴范围

def update(frame):
    data = ser.read(CCD_SIZE)
    Uart_CCD_DataBUff = np.frombuffer(data, dtype=np.uint8)
    
    # 将8位数据重新组合成16位，调整顺序
    low_bits = Uart_CCD_DataBUff[::2]
    high_bits = Uart_CCD_DataBUff[1::2]
    new_data = (low_bits.astype(np.uint16)) + (high_bits.astype(np.uint16) << 8)
    printf()
    # 移动数据，去掉第一个元素，添加新的元素
    s[:-1] = s[1:]
    s[-1] = new_data[0]

    line.set_ydata(s)

    return line,

ani = FuncAnimation(fig, update, blit=True, cache_frame_data=False)

try:
    plt.show()

except KeyboardInterrupt:
    print("Interrupted by user. Stopping the acquisition.")

finally:
    ser.close()
