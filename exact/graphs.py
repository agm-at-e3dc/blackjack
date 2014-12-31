from matplotlib.pyplot import *
from numpy import *

from time import sleep

import os;

def myPlot(fig = None, filename='dump', label='Count = 0', xData=None):

    if (fig == None):
        fig = figure()
    
    data = [float(x.rstrip('\n')) for x in open(filename).readlines()]

    markers = [7,4,5,6,'o','D','h','H','_','8','p',',','+','.','s','*','d',3,0,1,2,'1','3','4','2','v','<','>','^',',','x']

    if (xData is not None):
        slope = (data[1] - data[0]) / (xData[1] - xData[0])
        slopeAvg = (data[-1] - data[0]) / (xData[-1] - xData[0])
        print slope, slopeAvg
    else:
        slope = (data[1] - data[0])
        slopeAvg = (data[-1] - data[0])
        print slope, slopeAvg

    if ('T' in label):
        slope /= 4 # we choose to consider fractional removal of 1 ten card, not all
    
    
    #plot(data, marker=markers[int(random.rand() * len(markers))], label=(label + " : s = %.2f" % (slope * 100)))
    if (xData is not None):
        print "wooooo"
        plot(xData, data, marker=markers[int(random.rand() * len(markers))], label=(label + " : s_0 = " + (("%.2f" % slope).rjust(5)) 
                                                                         + " s_avg = " + (("%.2f" % slopeAvg)).rjust(5)))
    else:
        plot(data, marker=markers[int(random.rand() * len(markers))], label=(label + " : s_0 = " + (("%.2f" % slope).rjust(5)) 
                                                                         + " s_avg = " + ("%.2f" % slopeAvg).rjust(5)))

    
    return fig;


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
        
        #hilo
        # if (popNum == 0 or popNum == 9):
        #     self.count -= 1
        # if (popNum in [1,2,3,4,5]):
        #     self.count += 1

        #true change
        if popNum == 0: self.count += -2.08 -.0777
        if popNum == 1: self.count +=  1.15 -.0777
        if popNum == 2: self.count +=  1.47 -.0777
        if popNum == 3: self.count +=  2.18 -.0777
        if popNum == 4: self.count +=  2.91 -.0777
        if popNum == 5: self.count +=  1.63 -.0777
        if popNum == 6: self.count +=  1.12 -.0777
        if popNum == 7: self.count += -0.02 -.0777
        if popNum == 8: self.count += -0.59 -.0777
        if popNum == 9: self.count += -1.69 -.0777

            
    def toProb(self):

        return self.cards/sum(self.cards)

    def trueCount(self):

        return self.count / (sum(self.cards)/52)

def corr(x, y):

    return (mean([x[i]*y[i] for i in range(len(x))]) - mean(x)*mean(y)) / (std(x) * std(y))
    
    
if __name__ == '__main__':

    nDecks = 1
    penetration = .8
    nShoes = 1
    
    deck = SimpleDeck(nDecks)

    N = int(nDecks*52*penetration)
    
    print "running %d full simulations" % (N * nShoes)

    SPREAD = 1
    
    spreadEv = []
    baseEv = []
    totWager = 0

    firstSpreadEv = []
    firstBaseEv = []
    firstTotWager = 0

    secondSpreadEv = []
    secondBaseEv = []
    secondTotWager = 0

    # thirdSpreadEv = []
    # thirdBaseEv = []
    # thirdTotWager = 0
    
    for run in range(nShoes):

        print run
        
        deck.shuffle();
        
        os.system("rm dumpZeroth")
        os.system("rm dumpFirst")
        os.system("rm dumpSecond")
        os.system("rm dumpThird")
        for i in range(int(N)):
            p = deck.toProb()
            print ("./zeroth %f %f %f %f %f %f %f %f %f %f %d" %
                      (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], sum(deck.cards)))
            os.system("./zeroth %f %f %f %f %f %f %f %f %f %f >> dumpZeroth" %
                      (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]))
            os.system("./first %f %f %f %f %f %f %f %f %f %f %d >> dumpFirst" %
                      (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], sum(deck.cards)))
            os.system("./second %f %f %f %f %f %f %f %f %f %f %d >> dumpSecond" %
                      (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], sum(deck.cards)))
            # os.system("./third %f %f %f %f %f %f %f %f %f %f %d >> dumpThird" %
            #           (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], sum(deck.cards)))
            deck.pop()
        
        data = [float(x.rstrip('\n')) for x in open("dumpZeroth").readlines()]
        ev = []
        for datum in data:
            if datum > 1: # min % edge 
                totWager += SPREAD
                ev.append(SPREAD*datum)
            else:
                ev.append(datum)
                totWager += 1
        baseEv += data
        spreadEv += ev

        data = [float(x.rstrip('\n')) for x in open("dumpFirst").readlines()]
        ev = []
        for datum in data:
            if datum > 1: # min % edge 
                firstTotWager += SPREAD
                ev.append(SPREAD*datum)
            else:
                ev.append(datum)
                firstTotWager += 1
        firstBaseEv += data
        firstSpreadEv += ev

        data = [float(x.rstrip('\n')) for x in open("dumpSecond").readlines()]
        ev = []
        for datum in data:
            if datum > 1: # min % edge 
                secondTotWager += SPREAD
                ev.append(SPREAD*datum)
            else:
                ev.append(datum)
                secondTotWager += 1
        secondBaseEv += data
        secondSpreadEv += ev

        # data = [float(x.rstrip('\n')) for x in open("dumpThird").readlines()]
        # ev = []
        # for datum in data:
        #     if datum > 1: # min % edge 
        #         thirdTotWager += SPREAD
        #         ev.append(SPREAD*datum)
        #     else:
        #         ev.append(datum)
        #         thirdTotWager += 1
        # thirdBaseEv += data
        # thirdSpreadEv += ev

        
    #print corrcoef(deck.pastCounts, data)[0,1]
    #print corr(deck.pastCounts, data)

    # accSpreadEv = add.accumulate(spreadEv);
    # firstAccSpreadEv = add.accumulate(spreadEv);
        
    # fig = figure()
    # ax = fig.add_subplot(111)
    # ax.plot(baseEv,'b')
    # ax.plot(spreadEv, 'g')
    # ax.plot(range(N*nShoes), zeros(N*nShoes), '--', color="black")
    # ax.set_ylabel("optimal play %")
    # ax.set_xlabel("cards removed")

    # ax2 = ax.twinx()
    # #ax2.plot(range(N*nShoes), zeros(N*nShoes), '--', color="green")        
    # ax2.plot(accSpreadEv, 'r')        
    # ax2.set_ylabel("accumulated ev %")

    # fig.show()

    fig = figure(figsize = (14, 10));
    ax = fig.add_subplot(111)
    ax.plot(baseEv)
    ax.plot(firstBaseEv)
    ax.plot(secondBaseEv)
    #ax.plot(thirdBaseEv)
    show()
        
    print "mean flat ev (in unit bet %) : "+  str(mean(baseEv))

    print "mean spreaded ev (in unit bet %) : " + str(mean(spreadEv))
    
    print "true ev with spread (%) : " + str(sum(spreadEv)/totWager)

    print "FIRST: mean flat ev (in unit bet %) : "+  str(mean(firstBaseEv))

    print "FIRST: mean spreaded ev (in unit bet %) : " + str(mean(firstSpreadEv))
    
    print "FIRST: true ev with spread (%) : " + str(sum(firstSpreadEv)/firstTotWager)

    print "SECOND: mean flat ev (in unit bet %) : "+  str(mean(secondBaseEv))

    print "SECOND: mean spreaded ev (in unit bet %) : " + str(mean(secondSpreadEv))
    
    print "SECOND: true ev with spread (%) : " + str(sum(secondSpreadEv)/secondTotWager)

    # print "THIRD: mean flat ev (in unit bet %) : "+  str(mean(thirdBaseEv))

    # print "THIRD: mean spreaded ev (in unit bet %) : " + str(mean(thirdSpreadEv))
    
    # print "THIRD: true ev with spread (%) : " + str(sum(thirdSpreadEv)/thirdTotWager)
    
    
    #sleep(100)

        
    # fig = None
    # labels = ['A', '2', '3', '4', '5', '6', '7', '8', '9', 'T']

    # for k in [9]:#range(1, 10) + [0]:
    #     for j in range(1, 10) + [0]:
    #         if (k == j):
    #             continue
    #         print labels[k], labels[j]
    #         os.system("rm dump")
    #         for i in range(100, -1, -1):
    #             p = array([1./13]*10)
    #             p[-1] = 4./13
    #             p[k] *= i/100.
    #             p[8] *= i/100;
    #             p[j] *= i/100.
    #             p /= sum(p)
    #             os.system("./foo %f %f %f %f %f %f %f %f %f %f >> dump" %
    #                       (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]))
    #         fig = myPlot(fig, label=labels[j])


    #for j in range(8, -1, -1):

    # N = 26
    # os.system("rm dump")
    # for i in range(0, N, 1):

    #     p = array([1./13]*10)
    #     p[-1] = 4./13
    #     #for k in range(8, j-1,-1):
    #     #   p[k] *= i/100.
    #     p[9] *= (100+2*i)/100.
    #     p[8] *= (100)/100.
    #     p[7] *= (100)/100.
    #     p[6] *= (100)/100.
    #     p[5] *= (100-2*i)/100.
    #     p[4] *= (100-i)/100.
    #     p[3] *= (100)/100.
    #     p[2] *= (100)/100.
    #     p[1] *= (100)/100.
    #     p[0] *= (100+2*i)/100.
    #     if (i == 50): print p
    #     p /= sum(p)
    #     os.system("./foo %f %f %f %f %f %f %f %f %f %f >> dump" %
    #                   (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]))

    # fig = myPlot(fig, label="hilo", xData = arange(0, N)/2.5)

    # plot(arange(0, N)/2.5, zeros(N), '--', color="black")
    # #plot(range(N), zeros(N), '--', color="black")

    # title("The optimal return a given hilo shift")
    # legend(loc="upper left")
    # #xlabel("% removed")
    # xlabel("approx true count")
    # ylabel("optimal outcome %")
        
    # show()
        
    #p = array([1./13]*10)
    #p[-1] = 4./13


    # name = "dumpy"
    # print "./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name)
    # os.system("./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name))
    
    # fig = myPlot(filename=name, label="whole deck")


    #for i in range(len(p)):
     #   p[i] = p[i] *(0 + 1 * random.rand())

    #p[9] *= 0;
    #p /= sum(p)
    # name = "dump1"
    #print "./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name)
    #os.system("./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name))
    #print "./foo %f %f %f %f %f %f %f %f %f %f" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9])
    #
    
    # fig = myPlot(fig=fig, filename=name, label='no twos')

    # p[2] = 0
    # p *= 12./11
    # name = "dump2"
    # print "./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name)
    # os.system("./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name))
    
    # fig = myPlot(fig=fig, filename=name, label='no 2s/3s')

    # p = array([1./13]*10)
    # p[-1] = 0
    # p *= 13./9
    # name = "dump3"
    # print "./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name)
    # os.system("./foo %f %f %f %f %f %f %f %f %f %f > %s" % (p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], name))
    
    # fig = myPlot(fig=fig, filename=name, label='no tens')

    
    # title("Probability of dealer busting as a function of his upcard")
    
    # xlabel("Dealer Upcard")
    # ylabel("Bust Probability")

    # xticks(arange(1, 11), ('A', '2', '3', '4', '5', '6', '7', '8', '9', 'T'))
    
    # xlim(0, 11)
    
    # legend()
    
    # show()
