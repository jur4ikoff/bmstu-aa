import pandas as pd
import matplotlib.pyplot as plt
import os
import sys

IS_SHOW = 1
SCRIPT_PATH = os.path.abspath(sys.argv[0])
SCRIPT_DIR = os.path.dirname(SCRIPT_PATH)
DATA_PATH = f"{SCRIPT_DIR}/charts/res.csv"


if not os.path.exists(SCRIPT_DIR + "/charts"):
    os.mkdir(SCRIPT_DIR + "/charts")


df = pd.read_csv(DATA_PATH, sep=";")

case_titles = {
    "random": "Случайный порядок",
    "straight": "Прямой порядок",
    "reverse": "Обратный порядок",
}

algorithms = [
    "Итерационный метод поиска максимума",
    "Рекурсивный метод поиска максимума",
]

markers = ["o", "s"]  # круг и квадрат

# Построение графиков
for case in ["random", "straight", "reverse"]:
    case_data = df[df["case"] == case]
    if case_data.empty:
        print(f"Нет данных для случая: {case}")
        continue

    plt.figure(figsize=(8, 5))

    for idx, algo in enumerate(algorithms):
        algo_data = case_data[case_data["algorithm"] == algo]
        if algo_data.empty:
            print(f"Нет данных для алгоритма '{algo}' в случае '{case}'")
            continue

        # Берём все значения "как есть", без группировки и усреднения
        x = algo_data["size"]
        y = algo_data["time"]

        plt.plot(
            x,
            y,
            marker=markers[idx],
            markersize=5,
            label=algo,
        )

    plt.title(case_titles[case])
    plt.xlabel("Размер")
    plt.ylabel("Время")
    plt.grid(True, which="both", linewidth=0.5)
    plt.legend()
    plt.tight_layout()

    filename = f"{SCRIPT_DIR}/charts/{case}.png"
    plt.savefig(filename, dpi=300, bbox_inches="tight")
    plt.close()

    print(f"График для '{case}' сохранён как {filename}")
