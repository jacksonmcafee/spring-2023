import pandas as pd
import numpy as np
from neo4j import GraphDatabase
file = r"shooting-1982-2023.csv"
df = pd.read_csv(file)

# Creates 3 different types of nodes (examples: Product ... Customer)

# Reading through your raw data file create an appropriate node for each type
# Note: each node must have at least 2 labels and 2 properties

# Creates at least 1 index for each node

# Creates at least 2 relationships that connect your nodes to each other
# Reading through your raw data file create the appropriate relationships between nodes
# Note: each relationship must have at least 1 property

# Note: You may create other Python scripts and call these from MainCreateGraphDB.py.  