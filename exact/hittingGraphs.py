from matplotlib.pyplot import *
from numpy import *

from time import time, sleep

import os;

def loadEvens(filename):

    return [float(x.rstrip('\n')) for x in open(filename).readlines()[::2]]

if __name__ == '__main__':

    fig = figure(figsize=(14, 10))
    ax = fig.add_subplot(111)    

    ax.set_title("Lower Bound Hitting Time Distributions")
    ax.set_xlabel("sqrt(time)")
    ax.set_ylabel("pdf")

    markers = [7,4,5,6,'o','D','h','H','_','8','p',',','+','.','s','*','d',3,0,1,2,'1','3','4','2','v','<','>','^',',','x']

    
    ax2 = ax.twinx()
    ax2.set_ylabel("cdf")

    bankroll = 100
    ubFactor = 3;
    
    for mu in arange(.025, .037, .002):
        print "./hitting %f -%d %d .000000000000001 > dumpHit%f" % ((mu+1.)/2, bankroll, ubFactor * bankroll, mu)
        os.system("./hitting %f -%d %d .00000000000001 > dumpHit%f" % ((mu+1.)/2, bankroll, ubFactor * bankroll, mu))
        data = loadEvens("dumpHit%f" % mu)
        ax.plot(sqrt(2*arange(len(data))), data, label = str(mu))
        SPACING = len(data)/100
        ax2.plot(sqrt(2*arange(len(data)))[::SPACING],
                 add.accumulate(data)[::SPACING],
                 marker=markers[int(random.rand() * len(markers))], label=str(mu))
        print ("max bust chance at time " + str(data.index(max(data))) + " "
               + "1 bust in "+ str(int(1./add.accumulate(data)[-1])) + " runs" +
               " end bankroll EV / starting bankroll after 10k hands " + str(1 + mu*10000/bankroll)) 
        #sleep(3)

    legend()

    show() 

