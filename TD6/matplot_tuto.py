

# Exemple d'utilisation de la librairie python matplotlib pour LINFO1252
import matplotlib
matplotlib.use("qtagg")
# On importe numpy (pour se faciliter la vie) et matplotlib
import matplotlib.pyplot as plt
import numpy as np

# On crée une première figure, par défaut toutes les commandes plt qui vont suivre vont s'appliquer sur cette figure (qui est la dernière déclarée).
fig1 = plt.figure()

# température moyenne l'année passée à Bruxelles
T_b = [2.1,3.7,6.4,9.7,13.3,16.6,17.6,17.6,15.8,11.5,6.3,3.2]
# température moyenne l'année passée à Marseille
T_m = [6.3,7.3,10.1,12.7,16.6,20.4,23.1,22.6,19.3,15.2,10.3,7]
# Les mois correspondant
M = [i for i in range(1,13)]

# On trace la température moyenne en fonction du mois en bleu avec un trait plein de 1 pixel d'épaisseur
plt.plot(M, T_b, color="blue", linewidth=1.0, linestyle="-")

# Possibles linestyles pour le plot : [ '-' | '--' | '-.' | ':' | ...]
# Pour vous amusez avec les couleurs disponibles : https://matplotlib.org/stable/gallery/color/named_colors.html
# idem avec les températures de Marseille
plt.plot(M, T_m, color="red", linewidth=1.0, linestyle="-")

# Limiter le range de valeurs affichées pour l'axe des x
plt.xlim(1,12)

# Forcer la graduation en y. np.linspace découpe l'intervalle [1,12] en parties égales
# Il renvoie 12 graduations [1,2,3...12]. (peut aussi être fait manuellement)
plt.xticks(np.linspace(1,12,12))

# Donner un label à l'axe x
plt.xlabel('Mois de l\'année')

# Limiter le range de valeur affiché pour y
plt.ylim(0,30)

# Forcer la graduation en y. np.linspace découpe l'intervalle [0,30] en parties égales et renvoie 7 graduations (5 graduations + 2 pour '0' et '30')
plt.yticks(np.linspace(0,30,7))

# Donner un label à l'axe y
plt.ylabel('Température moyenne [°C]')

# Donner un titre à votre graphe.
plt.title('Température moyenne observée durant l\'année')

# Permet d'ajouter une grille au graphe, rendant la lecture de vos données plus facile.
plt.grid(True)

# Ajouter une légende, loc peut prendre différentes valeurs (https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.legend.html)
plt.legend(['Bruxelles','Marseille'], loc = 'upper right')

# on enregistre le graphique. L'extension est directement déduite du nom donné en argument (png par défault).
plt.savefig("Exemple.png")
plt.savefig("Exemple.pdf")

# Optionnel : on affiche le graphe à l'écran (note: show est un appel bloquant, tant que le graphe n'est pas fermé, on est bloqué)
plt.show()

# On ferme proprement le plot.
plt.close()
