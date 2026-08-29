import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

IS_SHOW = 1
SCRIPT_PATH = os.path.abspath(sys.argv[0])
SCRIPT_DIR = os.path.dirname(SCRIPT_PATH)


if not os.path.exists(SCRIPT_DIR + "/charts"):
    os.mkdir(SCRIPT_DIR + "/charts")

df = pd.read_csv(SCRIPT_DIR + '/data/speed.csv', sep=';', encoding="utf-8")
plt.figure(figsize=(12, 8))

print(df)
df["time"] /= 1000
methods = df['method'].unique()

# Для каждого метода строим свою линию
markers = ['o', 's', 'D', '^', '*']
for method in methods:
    method_data = df[df['method'] == method]
    plt.plot(method_data['size'], method_data['time'], 
             marker=markers.pop(), linewidth=2, markersize=6, label=method)

# Настройка графика
plt.xlabel('Размер матрицы', fontsize=12)
plt.ylabel('Время выполнения (мс)', fontsize=12)
# plt.title('Сравнение времени выполнения методов', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)

plt.savefig(SCRIPT_DIR + '/charts/AA4_1.jpg', format='jpg', bbox_inches='tight')

if IS_SHOW:
    plt.show()

print("График сохранен в папке charts с именем chart.svg")