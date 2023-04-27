'''
Name: Jackson McAfee
Date: April 27, 2023
Assignment: Module 14 Project
Due Date: April 30, 2023
About this project: This program creates an MDP based on the example provided in the project details.
Assumptions: N/A
All work below was performed by Jackson McAfee
'''


import mdptoolbox.example as mdpExamp
import mdptoolbox
import numpy as np

# number of possible states
STATES = 4

# number of possible actions (fish, not_to_fish)
ACTIONS = 2
ACTION_FISH = 0
ACTION_NOT_FISH = 1

P = np.array([
    # P[0] = Fish
    [[0, 1, 0, 0],          # empty state
     [0.75, 0.25, 0, 0],    # low state
     [0, 0.75, 0.25, 0],    # medium state
     [0, 0, 0.6, 0.4]],     # high state
    # P[1] = Not Fish
    [[1, 0, 0, 0],          # empty state (is unreachable, value 1 assigned to make matrix stochastic)
     [0, 0.3, 0.7, 0],      # low state
     [0, 0, 0.25, 0.75],    # medium state  
     [0, 0, 0.05, 0.95]]    # high state
])

R = np.array(
    [[0, -200], # Empty State
     [10, 0],   # Low State 
     [10, 0],   # Medium State 
     [50, 0]]   # High State
)     

# set discount and numperiods values based on project guidelines
Discount = 0.9
NumPeriods = 10

# compute and display the optimal value function and policy
# use 0.9 discount and 10 periods
print("Value Iteration")
vi = mdptoolbox.mdp.ValueIteration(P, R, Discount, NumPeriods)
vi.setVerbose()
vi.run()
print("optimal value function=",vi.V)
print("optimal policy=",vi.policy)

# compute and display the optimal value function and policy
# use 0.9 discount
print("Policy Iteration")
pi = mdptoolbox.mdp.PolicyIteration(P, R, Discount)
pi.setVerbose()
pi.run()
print("optimal value function=",pi.V)
print("optimal policy=",pi.policy)