from __future__ import print_function

import argparse
import os
import sys
import time

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt


from tensorflow.contrib.learn.python.learn.datasets.mnist import read_data_sets
# from scipy.misc import imresize
from cv2 import resize
from sklearn.svm import SVC

from network import *

# from network.model import SaakModel
# from network.util import get_saak_anchors

DATA_DIR = './data/'
MODEL_DIR = './model-mnist-train-32x32.npy'
RESTORE_MODEL_FROM = None
SAAK_COEF_DIR = './coef-mnist-test-32x32.npy'
RESTORE_COEF_FROM = None

def get_argument():
    parser = argparse.ArgumentParser(description="Compute Saak Coefficient for MNIST")
    parser.add_argument("--data-dir", type=str, default=DATA_DIR,
                        help="directory to store mnist dataset")
    parser.add_argument("--model-dir", type=str, default=MODEL_DIR,
                        help="directory to store extracted Saak anchor vectors")
    parser.add_argument("--restore-model-from", type=str, default=RESTORE_MODEL_FROM,
                        help="stored saak model file (there will be no training if this parameter is provided)")
    parser.add_argument("--restore-coef-from", type=str, default=RESTORE_COEF_FROM,
                        help="stored saak coefficients file (there will be no computation if this parameter is provided)")
    parser.add_argument("--saak-coef-dir", type=str, default=SAAK_COEF_DIR,
                        help="di")
    return parser.parse_args()

def main():
    args = get_argument()
    
    # initialize tf session
    sess = tf.Session()
    
    # load MNIST data
    mnist = read_data_sets(args.data_dir, reshape=False, validation_size=1000)
    print("Input MNIST image shape: " + str(mnist.train.images.shape))
    
    # resize MNIST images to 32x32
    train_images = [resize(img,(32,32)) for img in mnist.train.images]
    train_images = np.expand_dims(train_images, axis=3)
    print("Resized MNIST images: " + str(train_images.shape))
    #return train_images
    
    # extract saak anchors
    
    if args.restore_model_from is None:
        anchors = get_saak_anchors(train_images, sess)
        np.save(args.model_dir, {'anchors': anchors})
    else:
        print("\nRestore from existing model:")
        data = np.load(args.restore_model_from).item()
        anchors = data['anchors']
        print("Restoration succeed!\n")
    
    # build up saak model
    print("Build up Saak model")
    model = SaakModel()
    model.load(anchors)

    # prepare testing images
    print("Prepare testing images")
    input_data = tf.placeholder(tf.float32)
    test_images = [resize(img,(32,32)) for img in mnist.test.images]
    test_images = np.expand_dims(test_images, axis=3)
    
    # compute saak coefficients for testing images
    if args.restore_coef_from is None:
        print("Compute saak coefficients")
        out = model.inference(input_data, layer=0)
        test_coef = sess.run(out, feed_dict={input_data: test_images})
        train_coef = sess.run(out, feed_dict={input_data: train_images})
        # save saak coefficients
        print("Save saak coefficients")
        np.save(args.saak_coef_dir, {'train': train_coef, 'test': test_coef})
    else:
        print("Restore saak coefficients from existing file")
        data = np.load(args.restore_coef_from).item()
        train_coef = data['train']
        test_coef = data['test']
    
    ##Changes after this part to be taken under consideration. (can run for validation = 50000 and higher only, check the SAAK_PCA for further improvement)
    #np.savetxt("train_image_1.csv", a, delimiter=",")
    
    #variable initialization
    train = np.reshape(train_coef, [train_coef.shape[0], -1])
    train_label=np.genfromtxt("/Users/rickerish_nah/Downloads/Trainlabels.csv", delimiter=',') # train label
    test = np.reshape(test_coef, [test_coef.shape[0], -1])
    test_label=np.genfromtxt("/Users/rickerish_nah/Downloads/Testlabels.csv", delimiter=',') #test label
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

if __name__ == "__main__":
    main()

