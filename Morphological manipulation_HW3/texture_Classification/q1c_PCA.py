
#########################################
#     Harikrishna Prabhu               ##
#     3333077042                       ##
#########################################
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA


x_data=np.genfromtxt("Question1c.csv", delimiter=',') 


pca=PCA(n_components=25)

pca.fit(x_data)
y=pca.transform(x_data)

np.savetxt("PCA_Question1c_1.csv",y, delimiter=',') 