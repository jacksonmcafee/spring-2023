import mdptoolbox.example as mdpExamp
import mdptoolbox
import numpy as np

# number of possible states
STATES = 4

# number of possible actions (fish, not_to_fish)
ACTIONS = 2
ACTION_FISH = 1
ACTION_NOT_FISH = 0

P = np.array([
    # P[0] = Fish
    [[0, 1, 0, 0],
     [0.75, 0.25, 0, 0],
     [0, 0.75, 0.25, 0],
     [0, 0, 0.6, 0.4]],
    # P[1] = Not Fish
    [[0, 0, 0, 0],
     [0, 0.3, 0.7, 0],
     [0, 0, 0.25, 0.75],
     [0, 0, 0.05, 0.95]]
])

R = np.array(
    [[0, -200], # Empty
     [10, 0],   # Low
     [10, 0],   # Medium
     [50, 0]]   # High 
)     

'''
               #[0] = slow
               #[1] = fast
R = np.array([[ 1, 2],
              [ 1,-10],
              [-10,-10]])

print("P=",P)
print("R=",R)
'''


Discount = 0.9
NumPeriods = 10

##########################
print("Value Iteration")
vi = mdptoolbox.mdp.ValueIteration(P, R, Discount, NumPeriods)
vi.setVerbose()
vi.run()
print("optimal value function=",vi.V)
print("optimal policy=",vi.policy)
##########################

##########################
print("Policy Iteration")
pi = mdptoolbox.mdp.PolicyIteration(P, R, Discount)
pi.setVerbose()
pi.run()
print("optimal value function=",pi.V)
print("optimal policy=",pi.policy)
##########################
