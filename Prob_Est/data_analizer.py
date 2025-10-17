import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

datas = []

nome = input("Digite o nome do arquivo: ")
with open(f"{nome.strip()}.txt", "r", encoding="UTF=8") as file:
    lines = file.readlines()
    for line in lines:
        if line.startswith("INFO:"):
            line = line.strip().replace("INFO:", "").replace("\n", "")
            info = line.split(" | ")
            continue
        for n in line.split(" "):
            if "," in n:
                n = n.replace(",", ".")
            datas.append(float(n))

data = np.array(datas)

media = np.mean(data)
mediana = np.median(data)
moda = pd.Series(data).mode().tolist()
variancia = np.var(data, ddof=1)
desvio_padrao = np.std(data, ddof=1)
data.sort()
q_size = len(data) // 4

q1 = (data[(1 * q_size) - 1] + data[(1 * q_size)]) / 2
q3 = (data[(3 * q_size) - 1] + data[(3 * q_size)]) / 2

iqr = q3 - q1

min = min(data)
max = max(data)

print(
    f"Média: {media:.2f}\nMediana (Segundo Quartil): {mediana:.2f}\nModa: {moda if len(moda) < len(datas) else 'N/A'}\nVariância: {variancia:.2f}\nDesvio Padrão: {desvio_padrao:.2f}\n\
Primeiro Quartil: {q1:.2f}\nTerceiro Quartil: {q3:.2f}\nDistância Interquartil: {iqr:.2f}"
)

plt.figure(figsize=(16,7))
plt.subplot(1, 2, 1)
sns.histplot(data, bins="auto", kde=True, color="lightgreen")
plt.title(f"Histograma sobre {info[0]}")
plt.xlabel(info[1])
plt.ylabel(info[2])

plt.subplot(1, 2, 2)
sns.boxplot(x=data, color="red")
plt.title(f"Boxplot sobre {info[0]}")

plt.tight_layout()
plt.savefig(f"{nome}.png")



