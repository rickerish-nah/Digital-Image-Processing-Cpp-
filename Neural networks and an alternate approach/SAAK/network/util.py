from __future__ import print_function
from itertools import product
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
import cv2
import scipy.misc
import _pickle as cPickle
from sklearn.decomposition import PCA
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import confusion_matrix
from sklearn.cluster import KMeans
#__________________FUNCTIONS__________________________________#
# classification using SVM
def classify_svm(train_feature, train_label, test_feature, test_label): #_______1
    svc = SVC()
    svc.fit(train_feature, train_label)
    accuracy = svc.score(test_feature, test_label)
    return accuracy

def _extract_batches(images, ks): #_____________________________________________2
    (num_img, height, width, channel) = images.shape #image dimensions
    
    idx = range(0, height - ks + 1, ks) # in case index of overflow
    idy = range(0, width - ks + 1, ks)
    id_iter = [(x, y) for x in idx for y in idy]

    batches = np.array([images[:,i:i+ks,j:j+ks,:] for (i, j) in id_iter])
    print("Extracted image batch shape: " + str(batches.shape))

    batches = np.reshape(batches, [-1, ks*ks*channel])
    print("Processed image batch shape: " + str(batches.shape))

    return batches

def _fit_anchor_vectors(i,batches, ks, channel_in, lossy_rate=1, augment=True): #_3
    
    # removing mean from the image
    # print("Image mean: ",(np.mean(batches, axis=0)))
    batches = batches - np.mean(batches, axis=0)
    # print("Image mean after removal: ",(np.mean(batches, axis=0)))
    
    # fit anchor vectors
    pca = PCA()
    pca.fit(batches)
    # get number of anchor vectors to keep based on lossy rate
    # to get 2048 features:
    #if so run the SAAK_PCA; as for 2048 tensor cannot be created for greater than 2 GB
    # for the normal one - 1536; this code works well
    '''
    score = pca.explained_variance_ratio_
    #determines the number of components to hold after each SAAK layer
    components_to_keep = np.searchsorted(score, lossy_rate)
    '''
    
    # to get 1536 features
    if i==0:
        components_to_keep = 3
    elif i==1:
        components_to_keep = 4
    elif i==2:
        components_to_keep = 7
    elif i==3:
        components_to_keep = 6
    else:
        components_to_keep = 8
    print("Number of anchors to keep: " + str(components_to_keep))
    # get anchor vectors
    components = pca.components_[:components_to_keep,:]
    print("Anchor vector shape: " + str(components.shape))
    assert ks * ks * channel_in == components.shape[1]
   
    # performing the S/P conversion
    if augment:
        components = np.concatenate([components, -components], axis=0)
        components_to_keep = components_to_keep * 2
        print("Augmented anchor vector shape: " + str(components.shape))

    # reshape anchor vectors
    components = np.reshape(components, [-1, ks, ks, channel_in])
    print("Reshaped anchor shape: " + str(components.shape))

    # transpose anchor vectors to tensorflow format 
    # [ks, ks, channel_in, channel_out]
    components = components.transpose(1, 2, 3, 0)
    print("Tensorflow formated anchor shape: " + str(components.shape))

    # augment anchors
    return components, components_to_keep

def conv_and_relu(images, anchors, sess, ks): #_____________________________4
    #Convolution layer with ReLU activation function
    kernel = tf.constant(anchors)
    out = tf.nn.conv2d(images, kernel, strides=[1, 2, 2, 1], padding='SAME')
    out = tf.nn.relu(out)
    result = sess.run(out)
    print("Saak coefficients shape: " + str(result.shape))
    return result

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
    print("Confusion Matrix:\n",c) #confusion matrix
    
    print("Using RF")
    print("using 128 D reduced features\nTrain accuracy:",a1)
    print("Test Accuracy :",acc1)
    print("using 64 D reduced features\nTrain accuracy:",a2)
    print("Test Accuracy:",acc2)
    print("using 32 D reduced features\nTrain accuracy:",a3)
    print("Test Accuracy:",acc3)



#_________________MAIN FUNCTION HERE_____________________#
def get_saak_anchors(images, _sess=None, ks=2, max_layer=5, vis=None):
    if _sess is None:
        sess = tf.Session()
    else:
        sess = _sess

    if images.dtype == 'uint8':
        images = images.astype(np.float32)
        images = images / 255.  #normalizing

    anchors = []
    channel_in = images.shape[3]

    rf_size = []
    n = min(images.shape[1], images.shape[2])
    while n >= ks and len(rf_size) < max_layer:
        n = n // ks
        rf_size.append(n)

    print("Start to extract Saak anchors:\n")

    for i, _ in enumerate(rf_size):
        print("Stage %d start:" % (i + 1, ))
        batches = _extract_batches(images, ks)
        anchor, channel_out = _fit_anchor_vectors(i,batches, ks, channel_in)
        anchors.append(anchor)
        images = conv_and_relu(images, anchor, sess, ks)
        channel_in = channel_out

        if vis is not None:
            ind = range(len(batches))
            np.random.shuffle(ind)
            ind = ind[:1000]
            projection = np.matmul([batches[k,:] for k in ind], np.reshape(anchor[:,:,:,:2], [-1,2]))
            # np.save('p.npy', projection)
            # np.save('p2.npy', anchor)
            # np.save('p3.npy', batches)
            print("projection shape " + str(projection.shape))
            # plt.plot(projection[:,0], projection[:,1], 'o')
            plt.scatter(projection[:,0], projection[:,1])
            # plt.show()
            plt.savefig('images/' + vis)

        print("Stage %d end\n" % (i + 1, )) 

    if _sess is None:
        sess.close()

    return anchors

 

