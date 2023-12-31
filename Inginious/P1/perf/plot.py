import numpy as np 
import pandas as pd 
import matplotlib.pyplot as plt 
import os 

timeScale = ["ns", "µs", "ms", "s"]

files = os.listdir("perf/data")
files.sort()

for f in files:
    if f.split(".")[-1] == "csv":        
        data = pd.read_csv(f"perf/data/{f}")  
        if not (f.split(".")[0] == "test-test-and-set" or f.split(".")[0].split("_")[-1] == "home"):
            maxima = 0
            fig, ax = plt.subplots()
        maximum = data.to_numpy().max()
        width = data.shape[1]
        standard = [0 for _ in range(width)]
        postProc = []
        
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
            #ax.plot(np.ones(len(data[col]))*nb_threads, data[col]/(1000**track), "o", label=col.strip()) # To plot all the dots
            av[i] = data[col].mean()
            t[i] = nb_threads
        
            standard[i] = (data[col].to_numpy()/(1000**track)).std()
            
            postProc.append(data[col]/(1000**track))
            
        #ax.plot(t, av/(1000**track), label=f"Moyenne sur {len(data[col])} essais")
        #ax.fill_between(t, av/(1000**track) - standard, av/(1000**track) + standard, alpha=0.4, color="orange", label="Déviation Standard")

        if f.split(".")[0] == "test-test-and-set" and False:
            print(postProc)
            break
            tmp = []
            for s in postProc:
                tmp.append(1000*s)
            postProc = tmp

        ax.boxplot(postProc, labels=data.columns)
        
        ax.set_title(f"Rapidité d'exécution de {f.split('.')[0].split('_')[0]} ({len(data[col])} essais)")
        ax.set_xlabel("Nombre de Threads [-]")
        
        
        if not (f.split(".")[0] == "test-test-and-set" or f.split(".")[0].split("_")[-1] == "home"):
            ax.set_ylabel(f"Vitesse d'exécution [${timeScale[track]}$]")
            maxima = max(maxima, maximum/(1000**track)*1.1)
            ax.set_ylim(0, maxima)
        else:
            maxima = max(maxima, maximum/(1000**track)*1.1)
            ax.set_ylim(0, maxima)

        
        ax.minorticks_on()
        ax.grid(which = "major", linewidth = 1)
        ax.grid(which = "minor", linewidth = 0.2)
        
        l=f.split(".")[0]
        if(l.split("_")[-1] == "home"):
            l = "notre " + l.split("_")[0]
        
        ax.plot(np.arange(1,len(postProc)+1), [pt.mean() for pt in postProc], label=l)
        ax.legend()
        
        if(f.split(".")[0] == "test-test-and-set" or f.split(".")[0].split("_")[-1] == "home"):
            fig.savefig(f"perf/plot/{f.split('.')[0]}_plot.png")
            fig.savefig(f"perf/plot/pdf/{f.split('.')[0]}_plot.pdf")
