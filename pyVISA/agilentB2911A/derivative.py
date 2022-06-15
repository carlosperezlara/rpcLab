import matplotlib.pyplot as plt
import numpy as np

def get_numerical_ln_derivative(X, Y, middle_approx=False):
    if (middle_approx==False):
        return get_numerical_derivative_no_middle_approx(X, Y)

def get_numerical_derivative_no_middle_approx(X, Y):
    sec = []
    for i in range(1, len(X)):
        sec_value = (Y[i]-Y[i-1])/(X[i]-X[i-1])
        sec_value = (1.0/(Y[i])) * sec_value

        sec.append([X[i], sec_value])
    return sec


def get_inflection_point(firstDeriv):
    slopes = []
    x = []

    for i in range(len(firstDeriv)):
        slopes.append(firstDeriv[i][1])
        x.append(firstDeriv[i][0])

    ind = slopes.index(np.amax(slopes))

    return x[ind]
