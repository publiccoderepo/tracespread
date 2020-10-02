# tracespread
This repository contains the code used in the paper "Modeling Disease Spread and Detecting Outbreaks in Spatio-temporal Networks".

# Inputdata format
N E \
n1 n2\
n3 n4\
...\
where N is the number of nodes, E denotes the number of edges connected in graph G,\
and 'n1 n2' represents the edge connection between node n1 and node n2.

# Run experiment
Example: ./dtinfmax para1 para2 para3 para4 para5 para6\
para1: Input data file path\
para2: Number of time windows\
para3: k (Number of nodes selection)\
para4: p (propogation probability)\
para5: dataset name\
para6: Number of nets


# Comment
The output value is the original results over different measures.\
Each of these results needs to be further normalized across methods.



