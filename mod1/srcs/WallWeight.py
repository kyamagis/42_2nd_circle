

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# CSVファイルを読み込む（1列目がX, 2列目がYのデータを想定）
df = pd.read_csv("./WallWeight.csv")

# X, Y データを取得（列名が不明な場合、df.columns で確認）
x_data = df.iloc[:, 0]  # 1列目
y_data = df.iloc[:, 1]  # 2列目

# 線形補完（NaNが含まれている場合）
x_data_interp = np.linspace(x_data.min(), x_data.max(), num=100)
y_data_interp = np.interp(x_data_interp, x_data, y_data)

# 線形回帰を実行（最小二乗法）
slope, intercept, r_value, p_value, std_err = stats.linregress(x_data, y_data)

# 回帰直線を計算
y_regression = slope * x_data_interp + intercept

# 補完されたデータと回帰直線を描画
plt.figure(figsize=(8, 6))
plt.plot(x_data, y_data, 'o', label='Original Data', color='blue')
plt.plot(x_data_interp, y_data_interp, '-', label='Interpolated Data', color='green')
plt.plot(x_data_interp, y_regression, '--', label='Fitted Line (Linear Regression)', color='red')


# グラフを描画し、画像として保存
plt.figure(figsize=(6, 4))
plt.plot(x_data, y_data, marker="o", linestyle="-", color="b", label="Data")
plt.xlabel("distance")
plt.ylabel("weight")
plt.title("distance - weight")
plt.legend()
plt.grid(True)

# グラフを画像として保存
plt.savefig('./wall_weight.png')
