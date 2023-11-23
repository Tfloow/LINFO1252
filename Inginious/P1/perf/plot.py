import numpy as np 
import pandas as pd 
import matplotlib.pyplot as plt 
import os 

for f in os.listdir("perf"):
    if f.split(".")[-1] == "csv":
        data = pd.read_csv(f"perf/{f}")
        fig, ax = plt.subplots()
        maximum = 0
        width = data.shape[1]
        
        av = np.zeros(width)
        t = np.zeros(width)
        
        for i,col in enumerate(data.columns):
            nb_threads = int(col.strip().split()[0])
            ax.plot(np.ones(len(data[col]))*nb_threads, data[col], "o")
            maximum = max(maximum, data[col].max())
            av[i] = data[col].mean()
            t[i] = nb_threads
            
        ax.plot(t, av, linewidth=3)
        
        ax.set_title(f"Rapidité d'exécution de {f.split(".")[0]}")
        ax.set_xlabel("Nombre de Threads")
        ax.set_xlim(0, 70)
        
        ax.set_ylabel("Vitesse d'exécution [ns]")
        ax.set_ylim(0, maximum*1.1)

        ax.minorticks_on()
        ax.grid(which = "major", linewidth = 1)
        ax.grid(which = "minor", linewidth = 0.2)
        
        fig.savefig(f"perf/{f.split(".")[0]}_plot.png")
        data.plot()
