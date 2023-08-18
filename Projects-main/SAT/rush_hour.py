import sys
import numpy as np
from z3 import *


#---------------------

def Clause_only1(L): #List of N vars... it gives a clause DNF  so that only 1 Var is True.
    Outer = []
    for i in range(len(L)):
        inner = [ x  if L.index(x) == i else Not(x) for x in L ]
        case_clause = And(inner)
        Outer.append(case_clause)
    return(Or(Outer))


def Red_Reached(L,N):
    if L[0] == N-2:
        return True
    return False

#--------------------


f = open(sys.argv[1])


N = 0  #'Grid Size'
MAX = 0  #'MAX_num_moves'


num_cars = 0  # Excluding RED car.
num_Xcars = 0 # Excluding RED car.
num_Ycars = 0

INIT_POS = [] #2d array for storing initial Type,row,col info of all CARS, including Red one.

MINES = [] #Arrayfor storing initial & forever (r,c) of Mines.

FS = 1 
for line in f:
    if FS == 1: # first line of input
        N,MAX = [int(x) for x in line.split(',')]
        FS = 2
    elif FS == 2: #second line of input
        INIT_POS.append([1] + [int(x) for x in line.split(',')])
        FS = 3
    else:
        curr_pos = [int(x) for x in line.split(',')]
        if curr_pos[0] == 0:
            num_Ycars += 1
            INIT_POS.append(curr_pos)

        elif curr_pos[0] == 1:
            num_Xcars += 1
            INIT_POS.append(curr_pos)

        else:
            mine_pos = ([int(x) for x in line.split(',')])
            MINES.append(mine_pos)


M = MAX + 1     #'Number of timeframes... INITIAL + M moves... TOTAL M+1.
num_cars = num_Xcars + num_Ycars


CAR_VARS = [ [ Int('C{}_{}'.format(c,m) )   for c in range(num_cars + 1) ] for m in range(M) ]

SOLUTION = [[ 0 for c in range(1+num_cars)] for m in range(M)]

S = Solver()



#--Initial Conmdition.
for c in range(1+num_cars):
    if INIT_POS[c][0] == 0:
        row = (INIT_POS[c][1])
        S.add(CAR_VARS[0][c] == row)
    elif INIT_POS[c][0] == 1:
        col = (INIT_POS[c][2])
        S.add(CAR_VARS[0][c]  == col)



#--condition that at Each TIME, EACH CAR, has just 2 adjacent positions occupied in its given row/col.
for c in range(1+num_cars):
    for m in range(M):
        S.add(CAR_VARS[m][c] < N-1, CAR_VARS[m][c] >= 0 )

#--condition for MINES, CARS not to collide.
for m in range(M):
    for x in MINES:
        row,col = x[1:]
        for c in range(1+num_cars):
            if INIT_POS[c][0] == 0 and INIT_POS[c][2] == col:
                S.add( CAR_VARS[m][c] != row-1, CAR_VARS[m][c] != row )
            elif INIT_POS[c][0] == 1 and INIT_POS[c][1] == row:
                S.add( CAR_VARS[m][c] != col-1, CAR_VARS[m][c] != col )


#--condition for CARS not to Collide.
for m in range(M):
    for i in range(N):
        for j in range(N):
            carlist_ij = []
            for num in range(1+num_cars):
                if INIT_POS[num][0] == 0 and INIT_POS[num][2] == j:
                    carlist_ij.append( Or(CAR_VARS[m][num] == i,CAR_VARS[m][num] == i-1)   )
                elif INIT_POS[num][0] == 1 and INIT_POS[num][1] == i:
                    carlist_ij.append( Or( CAR_VARS[m][num] == j, CAR_VARS[m][num] == j-1)   )
            if len(carlist_ij) > 1:
                C = Or( Clause_only1(carlist_ij) , Not( Or(carlist_ij) ) )
                S.add(C)                             




#--condition between two time frames.
for m in range(M-1):
    moves_list = []
    for c in range(1+num_cars):
        moves_list.append( Or( CAR_VARS[m+1][c] == CAR_VARS[m][c] + 1 , CAR_VARS[m+1][c] == CAR_VARS[m][c] - 1  ) )
        S.add( Or( CAR_VARS[m+1][c] == CAR_VARS[m][c] + 1 , CAR_VARS[m+1][c] == CAR_VARS[m][c] - 1 , CAR_VARS[m+1][c] == CAR_VARS[m][c] )  )
    C = Clause_only1(moves_list)
    S.add(C)
    





#--RED_CAR condition
time_cases = []
for m in range(M):
    C = CAR_VARS[m][0] == N-2 
    time_cases.append(C)
S.add ( Or(time_cases) )




if S.check() == sat:
    Sol = S.model()
    for m in range(M):
        for c in range(1+num_cars):
            SOLUTION[m][c] = (Sol[CAR_VARS[m][c]].as_long())
    

    for i in range(M-1):
        if Red_Reached(SOLUTION[i],N):
            break
        Diff_arr = [ (SOLUTION[i+1][x] - SOLUTION[i][x]) for x in range(len(SOLUTION[i])) ]
        for car in range(len(Diff_arr)):
            if Diff_arr[car] == 1:
                if INIT_POS[car][0] == 0:
                    col = INIT_POS[car][2]
                    row = SOLUTION[i+1][car]
                    print(row,col,sep = ',')
                elif INIT_POS[car][0] == 1:
                    row = INIT_POS[car][1]
                    col = SOLUTION[i+1][car]
                    print(row,col,sep = ',')

            elif Diff_arr[car] == -1:
                if INIT_POS[car][0] == 0:
                    col = INIT_POS[car][2]
                    row = SOLUTION[i][car]
                    print(row,col,sep = ',')
                elif INIT_POS[car][0] == 1:
                    row = INIT_POS[car][1]
                    col = SOLUTION[i][car]
                    print(row,col,sep = ',')



else:
    print("unsat")