import numpy as np
import matplotlib.pyplot as plt

# 时域信号绘制函数
def tdraw(s, sample_rate):
    t = np.arange(0, len(s)) * (1.0 / sample_rate)
    # plt.figure(figsize=(20, 5))
    plt.plot(t, s)
    plt.grid()
    plt.xlabel('T/s')
    plt.ylabel('Y')

# 频域信号绘制函数
def fdraw(s, sample_rate, fft_point=512):
    freqs = np.linspace(0, sample_rate/2, int(fft_point/2) + 1)
    # 做FFT变换并转换为分贝
    s_f = np.fft.rfft(s, fft_point) / fft_point
    s_db = 20 * np.log10(np.clip(np.abs(s_f), 1e-20, 1e100))
    plt.plot(freqs, s_db)
    plt.grid()
    plt.xlabel('F/Hz')
    plt.ylabel('dB')

# 功率谱绘制函数
def gdraw(spec, note):
    pic = plt.figure(figsize=(10, 5))
    heatmap = plt.pcolor(spec)
    pic.colorbar(mappable=heatmap)
    plt.xlabel('T/s')
    plt.ylabel(note)
    plt.tight_layout()