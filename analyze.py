import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import multivariate_normal
from sklearn.cluster import DBSCAN
from mpl_toolkits.mplot3d import Axes3D

def label_str_lut(dim):
    if dim == 0:
        return 'x'
    elif dim == 1:
        return 'y'
    elif dim == 2:
        return 'z'
    else:
        return "unknown"

class Analyze:
    def __init__(self, data):
        self.data = data

    def __str__(self):
        return self.notes

    def compute_statistics(self):
        x = self.data[:, 0]
        y = self.data[:, 1]
        z = self.data[:, 2]
        ncpu = self.data[:, 3]
        frame_number = self.data[:, 4]

        mean = []
        mean.append(np.mean(x))
        mean.append(np.mean(y))
        mean.append(np.mean(z))

        sigma = np.cov(np.vstack([x, y, z]))

        return (mean, sigma)

    def fit_gaussian(self, fig_num, dim_x, dim_y):
        '''
        dimension canonly be 0, 1, 2
        :param fig_num:
        :param dim_x:
        :param dim_y:
        :return:
        '''
        X = np.linspace(-5, 5, 1001)
        Y = np.linspace(-5, 5, 1001)
        X, Y = np.meshgrid(X, Y)
        pos = np.empty(X.shape + (2,))
        pos[:, :, 0] = X;
        pos[:, :, 1] = Y

        x = self.data[:, dim_x]
        y = self.data[:, dim_y]
        sigma = np.cov(np.stack((x, y), axis=0))
        mean = np.array([.0, .0], dtype=np.float64)

        mean[0] = np.mean(self.data[:, dim_x])
        mean[1] = np.mean(self.data[:, dim_y])

        g = multivariate_normal(mean, sigma)

        if fig_num == -1:
            return (mean, sigma, g)
        fig = plt.figure(fig_num)
        ax = fig.gca(projection='3d')
        ax.plot_surface(X, Y, g.pdf(pos), cmap='viridis', linewidth=0)
        ax.set_xlabel(label_str_lut(dim_x))
        ax.set_ylabel(label_str_lut(dim_y))
        ax.set_zlabel("pdf")
        ax.set_title(label_str_lut(dim_x) + label_str_lut(dim_y) + " plane")

        plt.show()

        return (mean, sigma, g)

    def clustering(self, eps = 0.2, min_samples = 10):
        db = DBSCAN(eps = eps, min_samples = min_samples).fit(self.data[:, 0:3])
        core_samples_mask = np.zeros_like(db.labels_, dtype=bool)
        core_samples_mask[db.core_sample_indices_] = True
        labels = db.labels_

        # Number of clusters in labels, ignoring noise if present.
        n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)
        n_noise_ = list(labels).count(-1)

        cluster = [np.zeros((1, 5)) for t in range(n_clusters_+ 1)]
        for i in range(len(self.data)):
            if(labels[i] == -1):
                cluster[labels[i]]=np.append(cluster[labels[i]],[self.data[i]],axis=0)
            else:
                cluster[labels[i]]=np.append(cluster[labels[i]],[self.data[i]],axis=0)

        return (n_clusters_, n_noise_, cluster, float(n_noise_ / len(self.data)))


