# 这部分代码是在拷贝实验四中对高斯混合模型的定义的基础上略加修改
import numpy as np
from scipy.stats import multivariate_normal
from sklearn.cluster import KMeans
from tqdm import tqdm

class Mul_Gaussian_model():
    def __init__(self, num_of_models, dimension) -> None:
        self.num_of_models = num_of_models
        self.dimension = dimension
        self.mu = np.zeros([num_of_models, dimension])
        self.sigma = np.ones([num_of_models, dimension, dimension])
        self.mix_coeffi = np.ones([num_of_models]) / num_of_models

    # Init model with K-means
    def initModel(self, trainData):
        # assert input matrix: [num_samples*dimension] every row represents a sample
        kmeans = KMeans(n_clusters=self.num_of_models).fit(trainData)
        # calculate each set of parameters of each Gaussian Model
        for i in range(self.num_of_models):
            data = trainData[kmeans.labels_==i]
            self.mu[i] = np.mean(data, axis=0)                              # calculate mean (followed by row)
            self.sigma[i] = np.cov(data.T)                                  # calculate varience (need transpose to get [dimension*dimension] result)
            self.mix_coeffi[i] = np.sum(kmeans.labels_==i) / len(trainData)    # update the mixing coefficents

        return self

    # Train model with EM algorithm
    def EM_trainModel(self, trainData, num_of_iter=100):

        # train model based on EM algorithm
        for _ in tqdm(range(num_of_iter)):
            
            # calculate density
            density = np.empty((len(trainData), self.num_of_models))
            for i in range(self.num_of_models):
                norm = multivariate_normal(self.mu[i], self.sigma[i])
                density[:,i] = norm.pdf(trainData)
            # calculate N_k based on the formular
            posterior = density * self.mix_coeffi
            posterior = posterior / posterior.sum(axis=1, keepdims=True)
            N_k = posterior.sum(axis=0)
            # caculate mu, sigma
            mu_hat = np.tensordot(posterior, trainData, axes=[0, 0])
            sigma_hat = np.empty(self.sigma.shape)
            for i in range(self.num_of_models):
                tmp = trainData - self.mu[i]
                sigma_hat[i] = np.dot(tmp.T*posterior[:,i], tmp) / N_k[i]
            # update all the parameters
            self.sigma = sigma_hat
            self.mu = mu_hat / N_k.reshape(-1,1)
            self.mix_coeffi = N_k / len(trainData)

        return self

    # calculate prob Matrix given pixels
    def cal_pic_probs(self, X):
        # Pic shape: [row*col*dimension]
        # Expected shape of input: [num_samples*dimension] (every row represents a sample)
        # Expected output: [row*col]
        row = X.shape[0]
        X = X.reshape(-1, 3)
        probMatrix = np.zeros([self.num_of_models, X.shape[0]])
        for i in range(self.num_of_models):
            norm = multivariate_normal(self.mu[i], self.sigma[i])
            probMatrix[i] = norm.pdf(X)
        meanProb = np.dot(self.mix_coeffi, probMatrix).reshape(row, -1)
        return meanProb
            
    # calculate the most possible Gaussina component of each pixel
    def cal_component_prob(self, X):
        # shape of input: [num_samples*dimension] (every row represents a sample)
        probMatrix = np.zeros([self.num_of_models, X.shape[0]])
        for i in range(self.num_of_models):
            diff = X - self.mu[i]
            norm = multivariate_normal(self.mu[i], self.sigma[i])
            probMatrix[i] = norm.pdf(diff)
        # print(np.argmax(probMatrix.T, axis=1))
        return np.argmax(probMatrix.T, axis=1)

    # Fix GMM model based on current pixel sets
    def fit(self, X, labels):
        assert self.dimension == X.shape[1]

        n_samples = np.zeros(self.num_of_models)
        self.mix_coeffi[:] = 0
        uni_labels, count = np.unique(labels, return_counts=True)
        n_samples[uni_labels] = count

        variance = 0.01
        for ci in uni_labels:
            n = n_samples[ci]
            self.mix_coeffi[ci] = n / np.sum(n_samples)
            self.mu[ci] = np.mean(X[ci == labels], axis=0)
            self.sigma[ci] = 0 if n_samples[ci] <= 1 else np.cov(X[ci == labels].T)

            det = np.linalg.det(self.sigma[ci])
            if det <= 0:
                # Adds the white noise to avoid singular covariance matrix.
                self.sigma[ci] += np.eye(self.dimension) * variance
                det = np.linalg.det(self.sigma[ci])

    def forward(self, input):
        p = np.zeros([self.num_of_models])
        for i in range(self.num_of_models):
            norm = multivariate_normal(self.mu[i], self.sigma[i])
            p[i] = norm.pdf(input)
        return p.mean()