import numpy as np
import maxflow
import cv2 as cv

from utils.GMM import Mul_Gaussian_model
from matplotlib import pyplot as plt

class My_GrabCut:

    # Basic settings align with main grabcut.py
    BLUE = [255,0,0]        # rectangle color
    RED = [0,0,255]         # PR BG
    GREEN = [0,255,0]       # PR FG
    BLACK = [0,0,0]         # sure BG
    WHITE = [255,255,255]   # sure FG
    
    DRAW_BG = {'color' : BLACK, 'val' : 0}
    DRAW_FG = {'color' : WHITE, 'val' : 1}
    DRAW_PR_BG = {'color' : RED, 'val' : 2}
    DRAW_PR_FG = {'color' : GREEN, 'val' : 3}

    # init (align with grabcut func)
    def __init__(self, img, mask, rect, iterCount=1, mode=cv.GC_INIT_WITH_RECT, gmm_components=5):
        # Basic statements
        self.img = np.asarray(img, dtype=np.float64)
        self.rows, self.cols, _ = img.shape
        self.iterCount = iterCount
        self.mode = mode

        self.mask = mask
        if mode == cv.GC_INIT_WITH_RECT:        # only done when mode == cv.GC_INIT_WITH_RECT
            self.mask[rect[1]:rect[1] + rect[3],
                      rect[0]:rect[0] + rect[2]] = self.DRAW_PR_FG['val']
        self.classify_pixels()

        self.bgd_gmm = None
        self.fgd_gmm = None
        self.comp_idxs = np.empty((self.rows, self.cols), dtype=np.uint32)
        self.gmm_components = gmm_components    # choose 5 for num_of_Gaussian_model
        self.gamma = 50                         # choose 50 for gamma
        self.beta = 0

        self.left_V = np.empty((self.rows, self.cols - 1))
        self.upleft_V = np.empty((self.rows - 1, self.cols - 1))
        self.up_V = np.empty((self.rows - 1, self.cols))
        self.upright_V = np.empty((self.rows - 1, self.cols - 1))

    def exec(self):
        # Basic funtions of computing GrabCut algorithm
        self.init_GMMs()
        num_iters = self.iterCount
        for _ in range(num_iters):
            self.assign_GMMs_components()
            self.learn_new_GMMs()
            self.graph_construction()
        return self.mask

    def classify_pixels(self):
        self.bgd_indexes = np.where(np.logical_or(
            self.mask == self.DRAW_BG['val'], self.mask == self.DRAW_PR_BG['val']))
        self.fgd_indexes = np.where(np.logical_or(
            self.mask == self.DRAW_FG['val'], self.mask == self.DRAW_PR_FG['val']))

        assert self.bgd_indexes[0].size > 0
        assert self.fgd_indexes[0].size > 0

        # print('(pr_)bgd count: %d, (pr_)fgd count: %d' % (
        #     self.bgd_indexes[0].size, self.fgd_indexes[0].size))

    def init_GMMs(self):
        d = self.img[self.bgd_indexes].shape[1]
        self.bgd_gmm = Mul_Gaussian_model(
            num_of_models=self.gmm_components, 
            dimension=d).initModel(self.img[self.bgd_indexes])
        self.fgd_gmm = Mul_Gaussian_model(
            num_of_models=self.gmm_components, 
            dimension=d).initModel(self.img[self.fgd_indexes])
        print("GMM Initialisation Done.")

    def assign_GMMs_components(self):
        """Step 1 in Loop: Assign GMM components to pixels"""
        self.comp_idxs[self.bgd_indexes] = self.bgd_gmm.cal_component_prob(self.img[self.bgd_indexes])
        self.comp_idxs[self.fgd_indexes] = self.fgd_gmm.cal_component_prob(self.img[self.fgd_indexes])

    def learn_new_GMMs(self):
        """Step 2 in Loop: Learn new GMM parameters from data z"""
        self.bgd_gmm.fit(self.img[self.bgd_indexes], self.comp_idxs[self.bgd_indexes])
        self.fgd_gmm.fit(self.img[self.fgd_indexes], self.comp_idxs[self.fgd_indexes])
        print("Update GMM Model Done.")

    def calc_beta_smoothness(self):
        _left_diff = self.img[:, 1:] - self.img[:, :-1]
        _upleft_diff = self.img[1:, 1:] - self.img[:-1, :-1]
        _up_diff = self.img[1:, :] - self.img[:-1, :]
        _upright_diff = self.img[1:, :-1] - self.img[:-1, 1:]

        self.beta = np.sum(np.square(_left_diff)) + np.sum(np.square(_upleft_diff)) + \
            np.sum(np.square(_up_diff)) + \
            np.sum(np.square(_upright_diff))
        self.beta = 1 / (2 * self.beta / (
            # Each pixel has 4 neighbors (left, upleft, up, upright)
            4 * self.cols * self.rows
            # The 1st column doesn't have left, upleft and the last column doesn't have upright
            - 3 * self.cols
            - 3 * self.rows  # The first row doesn't have upleft, up and upright
            + 2))  # The first and last pixels in the 1st row are removed twice
        # print('Beta:', self.beta)

        # Smoothness term V computing formula
        self.left_V = self.gamma * np.exp(-self.beta * np.sum(
            np.square(_left_diff), axis=2))
        self.upleft_V = self.gamma / np.sqrt(2) * np.exp(-self.beta * np.sum(
            np.square(_upleft_diff), axis=2))
        self.up_V = self.gamma * np.exp(-self.beta * np.sum(
            np.square(_up_diff), axis=2))
        self.upright_V = self.gamma / np.sqrt(2) * np.exp(-self.beta * np.sum(
            np.square(_upright_diff), axis=2))

    def graph_construction(self):

        # init graph
        self.g = maxflow.Graph[float]()
        nodeids = self.g.add_grid_nodes(self.img.shape[:-1])

        # add Smoothness term V edges
        self.calc_beta_smoothness()
        # Edges pointing left
        structure = np.zeros((3, 3))
        structure[1, 0] = 1
        V = np.column_stack((np.zeros(self.left_V.shape[0]), self.left_V))
        self.g.add_grid_edges(nodeids, structure=structure, weights=V, symmetric=True)

        # Edges pointing up
        structure = np.zeros((3, 3))
        structure[0, 1] = 1
        V = np.row_stack((np.zeros(self.up_V.shape[1]), self.up_V))
        self.g.add_grid_edges(nodeids, structure=structure, weights=V, symmetric=True)

        # Edges pointing up-left
        structure = np.zeros((3, 3))
        structure[0, 0] = 1
        V = np.column_stack((np.zeros(self.upleft_V.shape[0]), self.upleft_V))
        V = np.row_stack((np.zeros(V.shape[1]), V))
        self.g.add_grid_edges(nodeids, structure=structure, weights=V, symmetric=True)

        # Edges pointing up-right
        structure = np.zeros((3, 3))
        structure[0, 2] = 1
        V = np.column_stack((self.upright_V, np.zeros(self.upright_V.shape[0])))
        V = np.row_stack((np.zeros(V.shape[1]), V))
        self.g.add_grid_edges(nodeids, structure=structure, weights=V, symmetric=True)

        # add Data term U tedges
        # For true foreground, put tedges to 0(source) or inf(sink)
        U_fgd = -np.log(self.fgd_gmm.cal_pic_probs(self.img))
        U_fgd[np.where(self.mask==self.DRAW_FG['val'])] = 0         # definitely link to source
        U_fgd[np.where(self.mask==self.DRAW_BG['val'])] = np.inf    # definitely not link to source

        U_bgd = -np.log(self.bgd_gmm.cal_pic_probs(self.img))
        U_bgd[np.where(self.mask==self.DRAW_FG['val'])] = np.inf    # definitely not link to sink
        U_bgd[np.where(self.mask==self.DRAW_BG['val'])] = 0         # definitely link to sink

        self.g.add_grid_tedges(nodeids, U_fgd, U_bgd)
        print("Creating Graph Done.")
        self.g.maxflow()
        sgm = self.g.get_grid_segments(nodeids)
        # print(np.where(sgm==False))
        sgm = np.int_(sgm) + 2      # '+2' for probable background/foreground
        sgm[np.where(self.mask==self.DRAW_FG['val'])] = 1
        sgm[np.where(self.mask==self.DRAW_BG['val'])] = 0
        # print("----------------")
        # print(np.where(sgm==0))
        # print(np.where(sgm==1))
        # print(np.where(sgm==2))
        # print(np.where(sgm==3))
    
        self.mask = sgm
        print("Computing Mask Done.")
