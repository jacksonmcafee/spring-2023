'''
Name: Jackson McAfee
Date: April 27, 2023
Assignment: Module 14 Project
Due Date: April 30, 2023
About this project: This program creates a MM based on the data from PRSA_Data_Nongzhanguan[].csv 
Assumptions: N/A
All work below was performed by Jackson McAfee
'''

import random
import pandas as pd
import numpy as np
from MarkovChain import *
from MChainDraw import *
from statistics import mean

random.seed("aima-python")

# read file into dataframe
file = r"C:\\Users\\Jackson McAfee\\Documents\\Programming\\Project\\PRSA_Data_Nongzhanguan_20130301-20170228.csv"
df = pd.read_csv(file)

# separate data into 4 range-based options
def label_dewp(row):
    if row['DEWP'] <= -10:
        return "Very Low"
    elif row['DEWP'] > -10 and row['DEWP'] <= 0:
        return "Low"
    elif row['DEWP'] > 0 and row['DEWP'] <= 10:
        return "High"
    elif row['DEWP'] > 10:
        return "Very High"
    return "Other"

# add label column to dataframe
df['dewp_label'] = df.apply(lambda row: label_dewp(row), axis=1)

# create variables for transition probabilities for 4 states
CountVLowVLow, CountVLowLow, CountVLowHigh, CountVLowVHigh = 0, 0, 0, 0
CountLowVLow, CountLowLow, CountLowHigh, CountLowVHigh = 0, 0, 0, 0
CountHighVLow, CountHighLow, CountHighHigh, CountHighVHigh = 0, 0, 0, 0
CountVHighVLow, CountVHighLow, CountVHighHigh, CountVHighVHigh = 0, 0, 0, 0

# assign transition probabilities for 4 states
indxDewpLabel = df.columns.get_loc("dewp_label")
for i in range(1, df.shape[0]-1):
    if df.iat[i, indxDewpLabel]=="Very Low":
        if df.iat[i+1, indxDewpLabel]=="Very Low":
            CountVLowVLow += 1
        elif df.iat[i+1, indxDewpLabel]=="Low":
            CountVLowLow += 1
        elif df.iat[i+1, indxDewpLabel]=="High":
            CountVLowHigh += 1
        elif df.iat[i+1, indxDewpLabel]=="Very High":
            CountVLowVHigh += 1
    if df.iat[i, indxDewpLabel]=="Low":
        if df.iat[i+1, indxDewpLabel]=="Very Low":
            CountLowVLow += 1
        elif df.iat[i+1, indxDewpLabel]=="Low":
            CountLowLow += 1
        elif df.iat[i+1, indxDewpLabel]=="High":
            CountLowHigh += 1
        elif df.iat[i+1, indxDewpLabel]=="Very High":
            CountLowVHigh += 1
    if df.iat[i, indxDewpLabel]=="High":
        if df.iat[i+1, indxDewpLabel]=="Very Low":
            CountHighVLow += 1
        elif df.iat[i+1, indxDewpLabel]=="Low":
            CountHighLow += 1
        elif df.iat[i+1, indxDewpLabel]=="High":
            CountHighHigh += 1
        elif df.iat[i+1, indxDewpLabel]=="Very High":
            CountHighVHigh += 1
    if df.iat[i, indxDewpLabel]=="Very High":
        if df.iat[i+1, indxDewpLabel]=="Very Low":
            CountVHighVLow += 1
        elif df.iat[i+1, indxDewpLabel]=="Low":
            CountVHighLow += 1
        elif df.iat[i+1, indxDewpLabel]=="High":
            CountVHighHigh += 1
        elif df.iat[i+1, indxDewpLabel]=="Very High":
            CountVHighVHigh += 1

# compute transition probabilities for 4 states
ProbVLowVLow = CountVLowVLow / ( CountVLowVLow + CountVLowLow + CountVLowHigh + CountVLowVHigh )
ProbVLowLow = CountVLowLow / ( CountVLowVLow + CountVLowLow + CountVLowHigh + CountVLowVHigh )
ProbVLowHigh = CountVLowHigh / ( CountVLowVLow + CountVLowLow + CountVLowHigh + CountVLowVHigh )
ProbVLowVHigh = CountVLowVHigh / ( CountVLowVLow + CountVLowLow + CountVLowHigh + CountVLowVHigh )
ProbLowVLow = CountLowVLow / ( CountLowVLow + CountLowLow + CountLowHigh + CountLowVHigh )
ProbLowLow = CountLowLow / ( CountLowVLow + CountLowLow + CountLowHigh + CountLowVHigh )
ProbLowHigh = CountLowHigh / ( CountLowVLow + CountLowLow + CountLowHigh + CountLowVHigh )
ProbLowVHigh = CountLowVHigh / ( CountLowVLow + CountLowLow + CountLowHigh + CountLowVHigh )
ProbHighVLow = CountHighVLow / ( CountHighVLow + CountHighLow + CountHighHigh + CountHighVHigh )
ProbHighLow = CountHighLow / ( CountHighVLow + CountHighLow + CountHighHigh + CountHighVHigh )
ProbHighHigh = CountHighHigh / ( CountHighVLow + CountHighLow + CountHighHigh + CountHighVHigh )
ProbHighVHigh = CountHighVHigh / ( CountHighVLow + CountHighLow + CountHighHigh + CountHighVHigh )
ProbVHighVLow = CountVHighVLow / ( CountVHighVLow + CountVHighLow + CountVHighHigh + CountVHighVHigh )
ProbVHighLow = CountVHighLow / ( CountVHighVLow + CountVHighLow + CountVHighHigh + CountVHighVHigh )
ProbVHighHigh = CountVHighHigh / ( CountVHighVLow + CountVHighLow + CountVHighHigh + CountVHighVHigh )
ProbVHighVHigh = CountVHighVHigh / ( CountVHighVLow + CountVHighLow + CountVHighHigh + CountVHighVHigh )

# create transition probability matrix
transition_prob = {'Very Low': {'Very Low': ProbVLowVLow, 'Low': ProbVLowLow, 'High': ProbVLowHigh, 'Very High': ProbVLowVHigh},
                   'Low': {'Very Low': ProbLowVLow, 'Low': ProbLowLow, 'High': ProbLowHigh, 'Very High': ProbLowVHigh},
                   'High': {'Very Low': ProbHighVLow, 'Low': ProbHighLow, 'High': ProbHighHigh, 'Very High': ProbHighVHigh},
                   'Very High': {'Very Low': ProbVHighVLow, 'Low': ProbVHighLow, 'High': ProbVHighHigh, 'Very High': ProbVHighVHigh}}

# create MarkovChain object
weatherChain = MarkovChain(transition_prob=transition_prob)

# create MarkovChainDraw transtiion probability matrix
P = np.array([[round(ProbVLowVLow,2),round(ProbVLowLow,2),round(ProbVLowHigh,2),round(ProbVLowVHigh,2)],
              [round(ProbLowVLow,2),round(ProbLowLow,2),round(ProbLowHigh,2),round(ProbLowVHigh,2)],
              [round(ProbHighVLow,2),round(ProbHighLow,2),round(ProbHighHigh,2),round(ProbHighVHigh,2)],
              [round(ProbVHighVLow,2),round(ProbVHighLow,2),round(ProbVHighHigh,2),round(ProbVHighVHigh,2)]]) 

# create MarkovChainDraw object
mc = MarkovChainDraw(P, ['Very Low', 'Low', 'High', 'Very High'])
mc.draw()