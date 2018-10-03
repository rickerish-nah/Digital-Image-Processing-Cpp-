#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 18 23:54:17 2018

@author: rickerish_nah
"""

"""
Final Project for EE 569 Spring 2018
Program    : LeNet 5 on MNIST dataset
@authors   : Harikrishna Prabhu     
USC ID     : 3333077042         
Email      : hprabhu@usc.edu   
Dataset    : MNIST dataset
Instructor : Professor C.C Kuo
"""

import numpy as np
import tensorflow as tf
import time
import matplotlib.pyplot as plt
from tensorflow.examples.tutorials.mnist import input_data
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten, Reshape
from keras.layers import Conv2D, MaxPooling2D, AveragePooling2D
from keras import optimizers
from time import localtime, strftime
from keras.callbacks import TensorBoard

#################################################################

#______________________FUNCTIONS USED IN THE PROGRAM _______________________________#
# Defining the CNN architecture (for part (b) LeNet5 model)
def base_model():    
    # creating model
    model = Sequential()
    model.add(Reshape((28,28,1), input_shape=(784,)))
    # [4] strictly 6 for LeNet 5 (changed and used for part (c))
    # [5] filter size 5x5
    # [6] Activation Function: sigmoid, ReLU
    # [7] Padding = 'SAME' for zero padding, 'VALID' for actual padding that reduces the dimension
    # [8] Kernel_initializer = RandomNormal, TruncatedNormal, GlorotNormal
    
    #convolution layer 1
    model.add(Conv2D(6, kernel_size=(5, 5), strides=(1, 1),activation='sigmoid',padding='SAME', use_bias=True, kernel_initializer='RandomNormal'))
    # [9] Pooling layer = MaxPooling or Average Pooling
    model.add(MaxPooling2D(pool_size=(2, 2)))
    # convolution layer 2. Can have the same number of parameters as C1
    model.add(Conv2D(16, (5, 5), activation='sigmoid', use_bias=True))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    # [10] to be used only in general CNN and shoud be discarded while performing for LeNet5 architecture
    model.add(Dropout(0.20))
    # convolution layer 3. Can have the same number of parameters as C1. After this stage only we obtain pure spectrum of the image
    model.add(Conv2D(120, kernel_size=(5, 5), strides=(1, 1),activation='sigmoid', use_bias=True))
    model.add(Flatten())
    
    model.add(Dense(84, activation='sigmoid', use_bias=True))
    model.add(Dense(num_classes, activation='softmax'))
    
    # [11] optimizer = SGD, Adam
    sgd = optimizers.SGD(lr=0.1, momentum=0.5)
    # Compile model
    # [12] Loss function = categorical_crossentropy
    model.compile(loss='categorical_crossentropy',optimizer=sgd, metrics=['accuracy'])
    return model

# Ploting the accuracy and loss
def plot_model_history(model_history):
    fig, axs = plt.subplots(1,2,figsize=(12,5))
    # summarize history for accuracy
    axs[0].plot(range(1,len(model_history.history['acc'])+1),model_history.history['acc'])
    axs[0].plot(range(1,len(model_history.history['val_acc'])+1),model_history.history['val_acc'])
    axs[0].set_title('Model Accuracy')
    axs[0].set_ylabel('Accuracy')
    axs[0].set_xlabel('Epoch')
    axs[0].legend(['Training', 'Validation'], loc='best')
    
    # summarize history for loss
    axs[1].plot(range(1,len(model_history.history['loss'])+1),model_history.history['loss'])
    axs[1].plot(range(1,len(model_history.history['val_loss'])+1),model_history.history['val_loss'])
    axs[1].set_title('Model Loss')
    axs[1].set_ylabel('Loss')
    axs[1].set_xlabel('Epoch')
    axs[1].legend(['Training', 'Validation'], loc='best')
    plt.show()

def PlotHistory(train_value, test_value, value_is_loss_or_acc):
    f, ax = plt.subplots()
    ax.plot([None] + train_value, '^-')
    ax.plot([None] + test_value, 'x-')
    # Plot legend and use the best location automatically: loc = 0.
    ax.legend(['Train ' + value_is_loss_or_acc, 'Validation ' + value_is_loss_or_acc], loc = 'best')
    ax.set_title('Training/Validation ' + value_is_loss_or_acc + ' per Epoch') #  change to Training/Test
    ax.set_xlabel('Epoch')
    ax.set_ylabel(value_is_loss_or_acc)
#_______________________________MAIN PROGRAM_______________________________________#
# Parameters for the convolutional Layers:
batch_size = 128 #[1] 64,128,196,256
num_classes = 10 #[2] constant for the data as class = 10
epochs = 200 #[3] 100, 150, 200

# Loading the training, validation and testing data
# Load training and eval data
mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)
X_train = mnist.train.images # Returns np.array
y_train = np.asarray(mnist.train.labels, dtype=np.int32)
X_test = mnist.test.images # Returns np.array
y_test = np.asarray(mnist.test.labels, dtype=np.int32)
X_valid = mnist.validation.images # Returns np.array
y_valid = np.asarray(mnist.validation.labels, dtype=np.int32)
# build the model
model = base_model()
# print model information
print(model.summary())
print('Training the LeNet 5 Model')
# Fit the model
start = time.time()
ts = localtime() # start time
history = model.fit(X_train, y_train, validation_data=(X_valid, y_valid),epochs=epochs, batch_size=batch_size, verbose=2)
end = time.time() # end time
print ("\nModel took %0.2f seconds to train\n"%(end - start))

# Final evaluation of the model
scores = model.evaluate(X_test, y_test, verbose=2) # test
score_train = model.evaluate(X_train, y_train, verbose=2) # train data
'''
model.save_weights("model_4.h5", overwrite=True)
model.save('my_model_4.h5', overwrite=True)
# save as JSON
json_string = model.to_json()
'''
# printing the Error, Loss and the Classification accuracy
'''
print("Train Data:")
print('LeNet 5 Test Error: %.4f%%' % (100-score_train[1]*100))
print('LeNet 5 Test Loss: %.4f%%' % score_train[0])
print('LeNet 5 Test Accuracy: %.4f%%' % (score_train[1]*100))
'''
print("Test Data:")
print('LeNet 5 Test Error: %.4f%%' % (100-scores[1]*100))
print('LeNet 5 Test Loss: %.4f%%' % scores[0])
print('LeNet 5 Test Accuracy: %.4f%%' % (scores[1]*100))

# Plotting the graphs for Accuracy and Loss
plot_model_history(history)
PlotHistory(history.history['loss'], history.history['val_loss'], 'Loss')
PlotHistory(history.history['acc'], history.history['val_acc'], 'Accuracy')
