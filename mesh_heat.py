import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

mesh = pd.read_csv("mesh.csv", header=None)
print(np.array(mesh))
fig, ax = plt.subplots()
im = ax.imshow(mesh)
plt.show()

sns.heatmap(mesh, cmap="crest")
plt.show()