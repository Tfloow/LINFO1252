import matplotlib.pyplot as plt 
import numpy as np

x = [2**i for i in range(1,7)]
y1 = [102129006,474058573,591005359,1054205194,1958856074,3756842860]
y = [140882751,526277088,1299568775,2769657271,7425189139,17451452634]
fig, ax = plt.subplots()

print((np.array(y)/np.array(y1)).mean())

ax.plot(x,y)

plt.show()