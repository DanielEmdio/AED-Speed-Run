import numpy as np
import math
import matplotlib.pyplot as plt

# sample data for tests
# y = np.array([6.065e-06,1.014e-06,1.074e-06,1.134e-06,1.439e-06,5.960e-07,1.381e-06,1.600e-06,1.103e-06,1.342e-06,3.370e-06,2.307e-06,5.159e-06,1.122e-05,3.161e-06,2.782e-06,2.040e-07,1.584e-06,1.715e-06,4.237e-06,5.458e-06,5.299e-06,4.065e-05,2.172e-05,2.119e-05,2.204e-05,2.257e-05,2.284e-05,2.478e-05,2.420e-05,2.505e-05,2.626e-05,2.629e-05])
# x = np.array([1,2,3,4,6,8,10,21,22,23,55,60,110,220,320,440,460,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800])

# n_mec = 108882 with plain recursion
x = np.array([1,2,3,4,5,6,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50])
y = np.array([1.462e-06,3.180e-07,3.570e-07,4.280e-07,4.870e-07,5.870e-07,9.840e-07,2.237e-06,2.137e-06,3.162e-06,1.406e-06,8.013e-06,1.031e-05,2.180e-05,3.651e-05,6.009e-05,9.720e-05,1.579e-04,2.589e-04,4.291e-04,7.090e-04,1.246e-03,2.119e-03,3.799e-03,7.024e-03,1.191e-02,1.967e-02,3.323e-02,5.481e-02,9.164e-02,1.514e-01,2.517e-01,4.250e-01,7.087e-01,1.187e+00,1.959e+00,3.274e+00,5.428e+00,8.980e+00,1.490e+01,2.440e+01,4.016e+01,6.683e+01,1.115e+02,1.860e+02,3.104e+02,5.169e+02])

const_array = np.polyfit(x, np.log(y), 1, w=np.sqrt(y)) # gets constants for the exp function
print("base equation: a*e^bx")
print(f"constants: {const_array[1]}, {const_array[0]}")
print(f"final exponencial fit equation: e^{const_array[1]} * e^{const_array[0]}*x")
print(f"prediction, in seconds, to solve the problem with length 800: {math.e ** const_array[1] * math.e ** (const_array[0] * 800)}")

def f(a):
   return math.e ** const_array[1] * math.e ** (const_array[0] * a)

plt.title("Plain recursion")  
plt.xlabel(f"Final position\nPredicted time for final position 800: {math.e ** const_array[1] * math.e ** (const_array[0] * 800)} s")
plt.ylabel("Time [s]")
plt.plot(x, y, 'bo', label='Plain recursion results')
plt.plot(x, f(x), 'r', label='Exponencial fit equation')
plt.legend()
plt.show()