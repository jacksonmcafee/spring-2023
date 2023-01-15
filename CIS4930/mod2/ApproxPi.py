"""
Name: Jackson McAfee
Date: 2023-01-15
Assignment: Module 2 Project - Python Basics
Due Date: 2023-01-22
About this project: This program should be able to compute pi to n terms as provided by the user.  
Assumptions: N/A
All work below was performed by Jackson McAfee
"""

def calc_pi(n):
    # store a persistent value that starts at zero
    val = 0
    # store the value of the denominator, which is incremented as the function runs
    # starts at 1 because 1/1 = 1 for the first step
    denom = 1

    # iterate through n and add/sub from val based on if a i is even or odd
    for i in range(n):
        if i % 2 == 0:
            val += 1/denom
        else:
            val -= 1/denom
        # increment value of the denominator by 2
        denom += 2
    
    # return value * 4 because this formula computes pi/4
    return 4*val

if __name__ == "__main__":
    # get user input
    while(True):
        # until user input is a value greater than zero, do not continue
        n = int(input("Input the number of digits you want computed here: "))
        if (n >= 1):
            break
    # calculate pi and print it
    print(f'The value of pi to {n} digits is {calc_pi(n)}')