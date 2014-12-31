from matplotlib.pyplot import *
from numpy import *

from time import time

import os;

class SimpleDeck:

    def __init__(self, nDecks):

        self.nDecks = nDecks
        self.shuffle()
        
    def shuffle(self):
        
        self.cards = self.nDecks * array([4., 4., 4., 4., 4., 4., 4., 4., 4., 16.])
        self.count = 0
        self.pastCounts = []
        
    def pop(self):

        accProb = add.accumulate(self.toProb())

        popNum = 0
        r = random.rand()
        for i in range(9):
            if accProb[i] < r:
                popNum += 1

        self.applyCount(popNum)
                
        if self.cards[popNum] > 0:
            self.cards[popNum] -= 1
            return popNum
        else:
            print "Fuuuuuu"
            exit(0)

    def applyCount(self, popNum):

        self.pastCounts.append(self.count)
        
        #true change
        # if popNum == 0: self.count += -2.08 -.0777
        # if popNum == 1: self.count +=  1.15 -.0777
        # if popNum == 2: self.count +=  1.47 -.0777
        # if popNum == 3: self.count +=  2.18 -.0777
        # if popNum == 4: self.count +=  2.91 -.0777
        # if popNum == 5: self.count +=  1.63 -.0777
        # if popNum == 6: self.count +=  1.12 -.0777
        # if popNum == 7: self.count += -0.02 -.0777
        # if popNum == 8: self.count += -0.59 -.0777
        # if popNum == 9: self.count += -1.69 -.0777

        #hilo
        if popNum == 0: self.count += -1
        if popNum == 1: self.count +=  1
        if popNum == 2: self.count +=  1
        if popNum == 3: self.count +=  1
        if popNum == 4: self.count +=  1
        if popNum == 5: self.count +=  1
        if popNum == 6: self.count +=  0
        if popNum == 7: self.count +=  0
        if popNum == 8: self.count +=  0
        if popNum == 9: self.count += -1
        
            
    def toProb(self):

        return self.cards/sum(self.cards)

    def trueCount(self):

        return self.count / (sum(self.cards)/52)

def corr(x, y):

    return (mean([x[i]*y[i] for i in range(len(x))]) - mean(x)*mean(y)) / (std(x) * std(y))

if __name__ == '__main__':

    # d = SimpleDeck(1)
    # d.cards = array([2., 4., 4., 4., 4., 4., 4., 4., 4., 8.])

    # probs = d.toProb();
    # s = "{"
    # for p in probs:
    #     s += str(p) + ','
    # s += "}"
    # print s
    # print sum(probs)

    start = time()
    
    nDecks = 6
    penetration = .83
    nShoes = 20

    def betFunction(x):

        return 1
        
        # if x <= 1:
        #     return 1
        # if x <= 3:
        #     return 1 + .5 * (x-1)
        # return 2
        
        # if x <= -3:
        #     return 1./4;
        # elif x <= -1:
        #     return 1./(1-x)
        # elif x <= 1:
        #     return 1.
        # elif x<=3:
        #     return 19.5*x-18.5
        # else:
        #     return 40.

    numStrats = 1;

    dumpStr = "dumpSingle"
    
    deck = SimpleDeck(nDecks)

    nSeenCards = int(nDecks*52*penetration)
    
    print "running %d simulations per order" % (nSeenCards * nShoes )

    spreadEV = []
    baseEV = []
    wager = []
    for i in range(numStrats):
        spreadEV.append([])
        baseEV.append([])
        wager.append([])

    #for order in range(numStrats):
     #   os.system("rm dump%d" % order);
    os.system("rm " + dumpStr)
        
    handsPlayed = 0.

    count = []
    
    for run in range(nShoes):

        print run, handsPlayed
        
        deck.shuffle();

        play = False
        
        for i in range(nSeenCards):

            p = deck.toProb()

            if deck.trueCount() > -99:
                play = True

            if (play):
                handsPlayed += 1
                count.append(deck.trueCount())

            os.system("./switch %f %f %f %f %f %f %f %f %f %f %d %f >> %s" %
                      (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], sum(deck.cards), count[-1], dumpStr))

                
                # for order in range(-3, maxOrder + 1):
                #     # print ("./order %f %f %f %f %f %f %f %f %f %f %d %d >> dump%d" %
                #     #       (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], sum(deck.cards), order, order))
                #     os.system("./order %f %f %f %f %f %f %f %f %f %f %d %d >> dump%d" %
                #               (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], sum(deck.cards), order, order))

            if (play and deck.trueCount() < -99):
                break;
                
            deck.pop()

    print "% hands played : " + str(handsPlayed / (nSeenCards * nShoes))
            
    print "Running took " + str(time()-start) + " seconds"
            
    labels = ["basic", "low count", "high count", "comp dependent", "mixed"]

    deviations = []

    data = [float(x.rstrip('\n')) for x in open(dumpStr).readlines()]

    print range(312)[2::2*(numStrats)]
    
    for order in range(numStrats):
        for hand, datum in enumerate(data[2*order:-1:2*(numStrats)]):
            bet = betFunction(count[hand])
            wager[order].append(bet)
            baseEV[order].append(datum)
            spreadEV[order].append(datum * bet)
        dev = 0
        for datum in data[1+2*order::2*(numStrats)]:
            dev += datum
        deviations.append(dev)
                
    for order in range(numStrats):
        print ""
        print "strategy : " + labels[order]
        print "deviations % : " + str(100*(deviations[order] / handsPlayed))
        print "base EV (in unit bet %) : " + str(mean(baseEV[order]))
        print "spread EV (in unit bet %) : " + str(mean(spreadEV[order]))
        print "average wager : " + str(mean(wager[order]))
        print "total profit : " + str((sum(spreadEV[order]))/100)
        print "total wager : " + str(sum(wager[order]))
        print "spread EV (in total wager %) : " + str(mean(spreadEV[order])/mean(wager[order]))

    fig = figure(figsize=(14, 10))
    ax = fig.add_subplot(111)    
    
    for order in range(numStrats):

        ax.plot(spreadEV[order], label = labels[order])

    ax.set_xlabel("cards seen")
    ax.set_ylabel("EV")
    ax.legend()

    ax2 = ax.twinx()
    ax2.plot(count, 's')
    ax2.set_ylabel("true count")
    
    show()
