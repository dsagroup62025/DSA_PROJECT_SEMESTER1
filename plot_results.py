import pandas as pd
import matplotlib.pyplot as plt
import os

# 1. Đọc dữ liệu từ file CSV vừa tạo
try:
    df = pd.read_csv('results.csv')
except FileNotFoundError:
    print("Lỗi: Không tìm thấy file results.csv!")
    exit()

# 2. Cấu hình biểu đồ
plt.style.use('ggplot') # Dùng giao diện đẹp hơn
fig, ax1 = plt.subplots(figsize=(10, 6))

# Vẽ cột cho thời gian xử lý (Total Time)
color = 'tab:blue'
ax1.set_xlabel('Chunk Size (MB)')
ax1.set_ylabel('Total Time (s)', color=color)
bars = ax1.bar(df['ChunkSize_MB'].astype(str) + " MB", df['TotalTime_s'], color=color, alpha=0.6, label='Execution Time')
ax1.tick_params(axis='y', labelcolor=color)

# Tạo trục thứ 2 để vẽ đường cho Thông suất (Throughput)
ax2 = ax1.twinx() 
color = 'tab:red'
ax2.set_ylabel('Throughput (elements/sec)', color=color)
ax2.plot(df['ChunkSize_MB'].astype(str) + " MB", df['Throughput_elements_sec'], color=color, marker='o', linewidth=2, label='Throughput')
ax2.tick_params(axis='y', labelcolor=color)

# 3. Trang trí
plt.title('External Merge Sort Performance Analysis', fontsize=14, fontweight='bold')
fig.tight_layout()

# Tạo thư mục charts nếu chưa có và lưu ảnh
if not os.path.exists('charts'):
    os.makedirs('charts')

plt.savefig('charts/performance_analysis.png')
print("Thành công! Biểu đồ đã được lưu trong thư mục charts/performance_analysis.png")

plt.show()
