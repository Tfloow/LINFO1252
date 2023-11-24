import numpy as np 
import pandas as pd 
import matplotlib.pyplot as plt 
import os 

timeScale = ["ns", "µs", "ms", "s"]

for f in os.listdir("perf/data"):
    if f.split(".")[-1] == "csv":
        data = pd.read_csv(f"perf/data/{f}")        
        fig, ax = plt.subplots()
        maximum = data.to_numpy().max()
        width = data.shape[1]
        
        track = 0
        time_scale = timeScale[track]
        big = maximum
        while big // 1000 != 0 and track < 3:
            big = big // 1000
            track+=1
    
        
        av = np.zeros(width)
        t = np.zeros(width)
        
        for i,col in enumerate(data.columns):
            nb_threads = int(col.strip().split()[0])
            ax.plot(np.ones(len(data[col]))*nb_threads, data[col]/(1000**track), "o", label=col.strip())
            av[i] = data[col].mean()
            t[i] = nb_threads
            
    
        ax.legend()
        ax.plot(t, av/(1000**track), linewidth=3)
        
        ax.set_title(f"Rapidité d'exécution de {f.split(".")[0]}")
        ax.set_xlabel("Nombre de Threads")
        ax.set_xlim(0, 70)
        
        ax.set_ylabel(f"Vitesse d'exécution [${timeScale[track]}$]")
        ax.set_ylim(0, maximum/(1000**track)*1.1)

        ax.minorticks_on()
        ax.grid(which = "major", linewidth = 1)
        ax.grid(which = "minor", linewidth = 0.2)
                
        fig.savefig(f"perf/plot/{f.split(".")[0]}_plot.png")
        data.plot()
