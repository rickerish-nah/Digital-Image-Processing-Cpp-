#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 24 21:12:34 2018

@author: rickerish_nah
"""

import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import confusion_matrix

def classify_accuracy(train, train_label, test, test_label):
    print("Size:\nTRAIN:",train.shape[0],"\nTEST:",test.shape[0])

    pca1 = PCA(n_components=128) 
    pca2 = PCA(n_components=64)
    pca3 = PCA(n_components=32)
    tra1 = pca1.fit_transform(train)
    tra2 = pca2.fit_transform(train)
    tra3 = pca3.fit_transform(train)
    tst1 = pca1.transform(test)
    tst2 = pca2.transform(test)
    tst3 = pca3.transform(test)
    #SVM
    svc1 = SVC()
    svc1.fit(tra1, train_label)
    #print(svc1)
    accuracy1 = svc1.score(tst1, test_label)
    ac1 = svc1.score(tra1, train_label)
    
    #s1 = svc1.evaluate(test, test_label, verbose=2)
    #RF
    rf1 = RandomForestClassifier()
    rf1.fit(tra1, train_label)
    acc1 = rf1.score(tst1, test_label)
    a1 = rf1.score(tra1, train_label)
    
    
    #SVM 2
    svc2 = SVC()
    svc2.fit(tra2, train_label)
    accuracy2 = svc2.score(tst2, test_label)
    ac2 = svc2.score(tra2, train_label)
    #RF 2
    rf2 = RandomForestClassifier()
    rf2.fit(tra2, train_label)
    acc2 = rf2.score(tst2, test_label)
    a2 = rf2.score(tra2, train_label)
    
    #SVM 3
    svc3 = SVC()
    svc3.fit(tra3, train_label)
    y = svc3.predict(tst3)
    accuracy3 = svc3.score(tst3, test_label)
    ac3 = svc3.score(tra3, train_label)
    c = confusion_matrix(y, test_label)
    # RF 3
    rf3 = RandomForestClassifier()
    rf3.fit(tra3, train_label)
    acc3 = rf3.score(tst3, test_label)
    a3 = rf3.score(tra3, train_label)
    
    
    
    print("Using SVM")
    print("using 128 D reduced features\nTrain accuracy:",ac1)
    print("Test Accuracy :",accuracy1)

    
    print("using 64 D reduced features\nTrain accuracy:",ac2)
    print("Test Accuracy:",accuracy2)
    print("using 32 D reduced features\nTrain accuracy:",ac3)
    print("Test Accuracy:",accuracy3)
    print("Confusion Matrix:\n",c)
    
    print("Using RF")
    print("using 128 D reduced features\nTrain accuracy:",a1)
    print("Test Accuracy :",acc1)
    print("using 64 D reduced features\nTrain accuracy:",a2)
    print("Test Accuracy:",acc2)
    print("using 32 D reduced features\nTrain accuracy:",a3)
    print("Test Accuracy:",acc3)
    
    
#reading all CSV files
tr1=np.genfromtxt("/Users/rickerish_nah/Downloads/Set1.csv", delimiter=',') #training 10000
tr2=np.genfromtxt("/Users/rickerish_nah/Downloads/Set2.csv", delimiter=',') #10000
tr3=np.genfromtxt("/Users/rickerish_nah/Downloads/Set3.csv", delimiter=',') #10000
tr4=np.genfromtxt("/Users/rickerish_nah/Downloads/Set4.csv", delimiter=',') #10000
tr5=np.genfromtxt("/Users/rickerish_nah/Downloads/Set5.csv", delimiter=',') #10000
tr6=np.genfromtxt("/Users/rickerish_nah/Downloads/Set6.csv", delimiter=',') #9000
train_label=np.genfromtxt("/Users/rickerish_nah/Downloads/Trainlabels.csv", delimiter=',') # train label
test=np.genfromtxt("/Users/rickerish_nah/Downloads/Testdata.csv", delimiter=',') #test data
test_label=np.genfromtxt("/Users/rickerish_nah/Downloads/Testlabels.csv", delimiter=',') #test label
#print("Shapes",tr1.shape,tr2.shape,tr3.shape,tr4.shape,tr5.shape,tr6.shape,train_label.shape,test.shape,test_label.shape)

#1_________________________________________________________Train = 59000 Test = 10000
train = np.concatenate((tr1,tr2,tr3,tr4,tr5,tr6),axis = 0)  #appending => training: 10+10+10+10+10+9 k = 59000
#print("Size of:\nTrain_Data:",train.shape,"\nTrain_label:",train_label.shape)
#print("Test_Data:",test.shape,"\nTest_label:",test_label.shape)
print("Size:\nTRAIN:",train.shape[0],"\nTEST:",test.shape[0])

#1
classify_accuracy(train, train_label, test, test_label)
#2
test2 = np.concatenate((test,train[50000:,:]),axis=0)  #making 10000 to 19000 (not sure if test data can be used for more than 10000[default], check with nive)
test_label2 = np.concatenate((test_label,train_label[50000:]),axis=0)

train2 = train[:50000,:]  #making 59000 to 50000
train_label2 = train_label[:50000]

classify_accuracy(train2, train_label2, test2, test_label2)

#3
test3 = np.concatenate((test2,train2[40000:,:]),axis=0) #making 19000 to 29000
test_label3 = np.concatenate((test_label2,train_label2[40000:]),axis=0)

train3 = train2[:40000,:] #making 50000 to 40000
train_label3 = train_label2[:40000]

classify_accuracy(train3, train_label3, test3, test_label3)

'''
pca1 = PCA(n_components=128) 
pca2 = PCA(n_components=64)
pca3 = PCA(n_components=32)
tra1 = pca1.fit_transform(train)
tra2 = pca2.fit_transform(train)
tra3 = pca3.fit_transform(train)
tst1 = pca1.transform(test)
tst2 = pca2.transform(test)
tst3 = pca3.transform(test)
#SVM
svc1 = SVC()
svc1.fit(tra1, train_label)
accuracy1 = svc1.score(tst1, test_label)
ac1 = svc1.score(tra1, train_label)
#RF
rf1 = RandomForestClassifier()
rf1.fit(tra1, train_label)
acc1 = rf1.score(tst1, test_label)
a1 = rf1.score(tra1, train_label)


#SVM 2
svc2 = SVC()
svc2.fit(tra2, train_label)
accuracy2 = svc2.score(tst2, test_label)
ac2 = svc2.score(tra2, train_label)
#RF 2
rf2 = RandomForestClassifier()
rf2.fit(tra2, train_label)
acc2 = rf2.score(tst2, test_label)
a2 = rf2.score(tra2, train_label)

#SVM 3
svc3 = SVC()
svc3.fit(tra3, train_label)
accuracy3 = svc3.score(tst3, test_label)
ac3 = svc3.score(tra3, train_label)
# RF 3
rf3 = RandomForestClassifier()
rf3.fit(tra3, train_label)
acc3 = rf3.score(tst3, test_label)
a3 = rf3.score(tra3, train_label)



print("Using SVM")
print("using 128 D reduced features\nTrain accuracy:",ac1)
print("Test Accuracy :",accuracy1)
print("using 64 D reduced features\nTrain accuracy:",ac2)
print("Test Accuracy:",accuracy2)
print("using 32 D reduced features\nTrain accuracy:",ac3)
print("Test Accuracy:",accuracy3)

print("Using RF")
print("using 128 D reduced features\nTrain accuracy:",a1)
print("Test Accuracy :",acc1)
print("using 64 D reduced features\nTrain accuracy:",a2)
print("Test Accuracy:",acc2)
print("using 32 D reduced features\nTrain accuracy:",a3)
print("Test Accuracy:",acc3)

#error and loss
scores = rf1.evaluate(X_test, y_test, verbose=2)
'''

'''
# 2______________________________________________Train = 50000 Test = 19000 [data size]

test2 = np.concatenate((test,train[50000:,:]),axis=0)  #making 10000 to 19000 (not sure if test data can be used for more than 10000[default], check with nive)
test_label2 = np.concatenate((test_label,train_label[50000:]),axis=0)

train2 = train[:50000,:]  #making 59000 to 50000
train_label2 = train_label[:50000]

#print("Size of:\nTrain_Data:",train2.shape,"\nTrain_label:",train_label2.shape)
#print("Test_Data\n:",test2.shape,"\nTest_label:",test_label2.shape)
print("Size:\nTRAIN:",train2.shape[0],"\nTEST:",test2.shape[0])
tra1 = pca1.fit_transform(train2)  #128
tra2 = pca2.fit_transform(train2)  #64
tra3 = pca3.fit_transform(train2)  #32
tst1 = pca1.transform(test2)  #128
tst2 = pca2.transform(test2)  #64
tst3 = pca3.transform(test2)  #32
#SVM
svc1 = SVC()
svc1.fit(tra1, train_label2)
accuracy1 = svc1.score(tst1, test_label2)
ac1 = svc1.score(tra1, train_label2)
#RF
rf1 = RandomForestClassifier()
rf1.fit(tra1, train_label2)
acc1 = rf1.score(tst1, test_label2)
a1 = rf1.score(tra1, train_label2)
#SVM 2
svc2 = SVC()
svc2.fit(tra2, train_label2)
accuracy2 = svc2.score(tst2, test_label2)
ac2 = svc2.score(tra2, train_label2)
#RF 2
rf2 = RandomForestClassifier()
rf2.fit(tra2, train_label2)
acc2 = rf2.score(tst2, test_label2)
a2 = rf2.score(tra2, train_label2)
#SVM3
svc3 = SVC()
svc3.fit(tra3, train_label2)
accuracy3 = svc3.score(tst3, test_label2)
ac3= svc3.score(tra3, train_label2)
# RF 3
rf3 = RandomForestClassifier()
rf3.fit(tra3, train_label2)
acc3 = rf3.score(tst3, test_label2)
a3 = rf3.score(tra3, train_label2)
print("Using SVM")
print("using 128 D reduced features\nTrain accuracy:",ac1)
print("Test Accuracy :",accuracy1)
print("using 64 D reduced features\nTrain accuracy:",ac2)
print("Test Accuracy:",accuracy2)
print("using 32 D reduced features\nTrain accuracy:",ac3)
print("Test Accuracy:",accuracy3)

print("Using RF")
print("using 128 D reduced features\nTrain accuracy:",a1)
print("Test Accuracy :",acc1)
print("using 64 D reduced features\nTrain accuracy:",a2)
print("Test Accuracy:",acc2)
print("using 32 D reduced features\nTrain accuracy:",a3)
print("Test Accuracy:",acc3)

# 3______________________________________________Train = 40000 Test = 29000

test3 = np.concatenate((test2,train2[40000:,:]),axis=0) #making 19000 to 29000
test_label3 = np.concatenate((test_label2,train_label2[40000:]),axis=0)

train3 = train2[:40000,:] #making 50000 to 40000
train_label3 = train_label2[:40000]

#print("Size of:\nTrain_Data:",train3.shape,"\nTrain_label:",train_label3.shape)
#print("Test_Data\n:",test3.shape,"\nTest_label:",test_label3.shape)
print("Size:\n TRAIN:",train3.shape[0],"\nTEST:",test3.shape[0])
tra1 = pca1.fit_transform(train3)
tra2 = pca2.fit_transform(train3)
tra3 = pca3.fit_transform(train3)
tst1 = pca1.transform(test3)
tst2 = pca2.transform(test3)
tst3 = pca3.transform(test3)
#SVM
svc1 = SVC()
svc1.fit(tra1, train_label3)
accuracy1 = svc1.score(tst1, test_label3)
ac1 = svc1.score(tra1, train_label3)
#RF
rf1 = RandomForestClassifier()
rf1.fit(tra1, train_label3)
acc1 = rf1.score(tst1, test_label3)
a1 = rf1.score(tra1, train_label3)
#SVM 2
svc2 = SVC()
svc2.fit(tra2, train_label3)
accuracy2 = svc2.score(tst2, test_label3)
ac2 = svc2.score(tra2, train_label3)
#RF 2
rf2 = RandomForestClassifier()
rf2.fit(tra2, train_label3)
acc2 = rf2.score(tst2, test_label3)
a2 = rf2.score(tra2, train_label3)

#SVM3

svc3 = SVC()
svc3.fit(tra3, train_label3)
accuracy3 = svc3.score(tst3, test_label3)
ac3 = svc3.score(tra3, train_label3)
# RF 3
rf3 = RandomForestClassifier()
rf3.fit(tra3, train_label3)
acc3 = rf3.score(tst3, test_label3)
a3 = rf3.score(tra3, train_label3)
print("Using SVM")
print("using 128 D reduced features\nTrain accuracy:",ac1)
print("Test Accuracy :",accuracy1)
print("using 64 D reduced features\nTrain accuracy:",ac2)
print("Test Accuracy:",accuracy2)
print("using 32 D reduced features\nTrain accuracy:",ac3)
print("Test Accuracy:",accuracy3)

print("Using RF")
print("using 128 D reduced features\nTrain accuracy:",a1)
print("Test Accuracy :",acc1)
print("using 64 D reduced features\nTrain accuracy:",a2)
print("Test Accuracy:",acc2)
print("using 32 D reduced features\nTrain accuracy:",a3)
print("Test Accuracy:",acc3)

'''