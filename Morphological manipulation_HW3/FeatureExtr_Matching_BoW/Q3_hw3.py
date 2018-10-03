#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 26 22:33:51 2018

@author: rickerish_nah
"""

import numpy as np
import cv2
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

############################____FUNCTION_____#################################
def match_image(img1,des1,kp1,img2,des2,kp2):
    # BFMatcher with default params
    bf = cv2.BFMatcher()    
    
    # Match descriptors.
    matches = bf.knnMatch(des1,des2, k=2)

    # Apply ratio test
    good = []
    for m,n in matches:
        if m.distance < 0.75*n.distance:
            good.append([m])
        
    img5 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,good,None,flags=2)
    return img5

######################____MAIN_____#################################
#Image Read
image_bum = cv2.imread('bumblebee.jpg')         
image_prime = cv2.imread('Optimus_Prime.jpg')
image_fer1 = cv2.imread('ferrari1.jpg') 
image_fer_2 = cv2.imread('ferrari_2.jpg') 


# Initiate SIFT detector
sift = cv2.xfeatures2d.SIFT_create()#322)
# Initiate SURF detector
surf = cv2.xfeatures2d.SURF_create()#841)

# find the keypoints and descriptors with SIFT
kp_b, des1 = sift.detectAndCompute(image_bum,None)
kp_p, des2 = sift.detectAndCompute(image_prime,None)
kp_f1, des3 = sift.detectAndCompute(image_fer1,None)
kp_f_2, des4 = sift.detectAndCompute(image_fer_2,None)

# find the keypoints and descriptors with SURF
kp_b_su, des1_su = surf.detectAndCompute(image_bum,None)
kp_p_su, des2_su = surf.detectAndCompute(image_prime,None)
kp_f1_su, des3_su = surf.detectAndCompute(image_fer1,None)
kp_f_2_su, des4_su = surf.detectAndCompute(image_fer_2,None)

########################################################### Q 3c
##Initializing K-Means                                    #
kmeans= KMeans(n_clusters=8)                              #
##Data fitting
x_data=np.vstack((des1,des2,des3))
kmeans=kmeans.fit(x_data)
centers = kmeans.cluster_centers_
labels = kmeans.labels_

#predict
fer1=kmeans.predict(des3[0:322,:])
fer2=kmeans.predict(des4[0:322,:])
bum=kmeans.predict(des1[0:322,:])
prime=kmeans.predict(des2[0:322,:])

plt.figure(1)
plt.hist(fer1,bins=50)
plt.xlabel("Clusters 'K'")
plt.title("Ferrari 1")

plt.figure(2)
plt.hist(fer2,bins=50)
plt.xlabel("Clusters 'K'")
plt.title("Ferrari 2")

plt.figure(3)
plt.hist(bum,bins=50)
plt.xlabel("Clusters 'K'")
plt.title("BumbleBee")

plt.figure(4)
plt.hist(prime,bins=50)
plt.xlabel("Clusters 'K'")                                #
plt.title("Optimus Prime")                                #
###########################################################
'''
# define criteria, number of clusters(K) and apply kmeans()
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 200, 1.0)
K = 8
ret,label,center=cv2.kmeans(x_data,K,None,criteria,10,cv2.KMEANS_RANDOM_CENTERS)
'''

#########################_____QUESTION___1_________#################
#BumbleBEE
out_image_bum1=cv2.drawKeypoints(image_bum,kp_b,None) #SIFT
cv2.imwrite('BumbleBee_SIFT.jpg',out_image_bum1)

out_image_bum2=cv2.drawKeypoints(image_bum,kp_b_su,None) #SURF
cv2.imwrite('BumbleBee_SURF.jpg',out_image_bum2)
#O_Prime
out_image_prime1=cv2.drawKeypoints(image_prime,kp_p,None) #SIFT
cv2.imwrite('OptimusPrime_SIFT.jpg',out_image_prime1)

out_image_prime2=cv2.drawKeypoints(image_prime,kp_p_su,None) #SURF
cv2.imwrite('OptimusPrime_SURF.jpg',out_image_prime2)
#F1
out_image_fer11=cv2.drawKeypoints(image_fer1,kp_f1,None) #SIFT
cv2.imwrite('Ferrari1_SIFT.jpg',out_image_fer11)

out_image_fer12=cv2.drawKeypoints(image_fer1,kp_f1_su,None) #SURF
cv2.imwrite('Ferrari1_SURF.jpg',out_image_fer12)
#F_2
out_image_fer_21=cv2.drawKeypoints(image_fer_2,kp_f_2,None) #SIFT
cv2.imwrite('Ferrari_2_SIFT.jpg',out_image_fer_21)

out_image_fer_22=cv2.drawKeypoints(image_fer_2,kp_f_2_su,None) #SIFT
cv2.imwrite('Ferrari_2_SURF.jpg',out_image_fer_22)


#########################_____QUESTION___2__MATCHING_________#################
#####1
# SIFT match Fer1 and Fer2
img1 = match_image(image_fer1,des3,kp_f1,image_fer_2,des4,kp_f_2)
cv2.imwrite('SIFT_Match_FerS.jpg',img1)

#SURF match Fer1 and Fer2
img2 = match_image(image_fer1,des3_su,kp_f1_su,image_fer_2,des4_su,kp_f_2_su) 
cv2.imwrite('SURF_Match_FerS.jpg',img2)

#####2
#SIFT match Fer1 and Optimus Prime
img3 = match_image(image_fer1,des3,kp_f1,image_prime,des2,kp_p) 
cv2.imwrite('SIFT Match Fer & Prime.jpg',img3)

#SURF match Fer1 and Optimus Prime
img4 = match_image(image_fer1,des3_su,kp_f1_su,image_prime,des2_su,kp_p_su) 
cv2.imwrite('SURF Match Fer & Prime.jpg',img4)

#####3
#SIFT match Fer1 and Bum
img5 = match_image(image_fer1,des3,kp_f1,image_bum,des1,kp_b) 
cv2.imwrite('SIFT Match Fer & BBee.jpg',img5)

#SURF match Fer1 and Bum
img6 = match_image(image_fer1,des3_su,kp_f1_su,image_bum,des1_su,kp_b_su) 
cv2.imwrite('SURF Match Fer & BBee.jpg',img6)