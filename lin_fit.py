import numpy as np
import matplotlib.pyplot as plt

# Clever Bruteforcing (solution 2)
# solve_title = 'Clever Bruteforcing'
# x = np.array([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,55,60,65,70,75,80,85,90,95,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800])
# y = np.array([1.844e-06,3.910e-07,3.700e-07,3.500e-07,4.010e-07,3.610e-07,3.810e-07,3.910e-07,3.500e-07,4.410e-07,1.613e-06,6.010e-07,5.410e-07,6.110e-07,9.020e-07,5.310e-07,5.210e-07,5.720e-07,6.310e-07,6.620e-07,1.803e-06,8.720e-07,8.510e-07,8.310e-07,7.410e-07,7.710e-07,8.720e-07,6.910e-07,7.810e-07,6.820e-07,5.810e-07,6.620e-07,6.910e-07,7.020e-07,5.910e-07,6.120e-07,6.010e-07,5.710e-07,6.110e-07,6.210e-07,6.310e-07,6.210e-07,5.910e-07,6.310e-07,6.710e-07,6.210e-07,6.410e-07,6.610e-07,7.220e-07,6.910e-07,1.994e-06,9.620e-07,8.410e-07,1.082e-06,1.373e-06,1.192e-06,1.593e-06,1.362e-06,1.392e-06,1.062e-06,2.835e-06,1.373e-06,1.373e-06,1.433e-06,1.894e-06,1.463e-06,1.703e-06,1.543e-06,1.793e-06,1.753e-06,3.026e-06,2.385e-06,1.282e-06,1.883e-06,2.174e-06,2.384e-06,4.558e-06,2.555e-06,2.715e-06,2.254e-06,3.537e-06,3.156e-06,2.906e-06,1.433e-06,2.615e-06,1.503e-06,1.834e-06,1.694e-06,1.703e-06,1.673e-06,1.694e-06,1.834e-06,3.116e-06,1.903e-06,1.984e-06,2.014e-06,2.054e-06,2.254e-06,2.344e-06,2.304e-06])

# Dynamic programming (solution 3)
solve_title = 'Dynamic programming'
x = np.array([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,55,60,65,70,75,80,85,90,95,100,110,120,130,140,150,160,170,180,190,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,680,700,720,740,760,780,800])
y = np.array([2.495e-06,4.110e-07,4.210e-07,4.400e-07,4.910e-07,5.910e-07,5.410e-07,5.310e-07,5.910e-07,7.110e-07,1.142e-06,5.810e-07,6.310e-07,6.910e-07,7.910e-07,7.520e-07,7.610e-07,7.410e-07,7.710e-07,8.920e-07,1.843e-06,1.182e-06,1.383e-06,1.613e-06,1.993e-06,2.014e-06,2.104e-06,1.693e-06,1.302e-06,1.273e-06,1.202e-06,1.253e-06,1.312e-06,1.282e-06,1.292e-06,1.253e-06,1.293e-06,1.273e-06,1.333e-06,1.303e-06,1.353e-06,1.403e-06,1.413e-06,1.462e-06,1.473e-06,1.503e-06,1.473e-06,1.503e-06,1.603e-06,1.713e-06,3.386e-06,2.385e-06,2.054e-06,2.245e-06,2.475e-06,2.755e-06,3.186e-06,3.777e-06,3.257e-06,3.146e-06,5.099e-06,4.008e-06,4.538e-06,4.839e-06,5.981e-06,6.943e-06,5.490e-06,5.911e-06,6.712e-06,7.644e-06,1.102e-05,8.937e-06,1.148e-05,9.768e-06,1.004e-05,1.153e-05,1.140e-05,1.227e-05,1.355e-05,1.385e-05,2.028e-05,1.652e-05,1.568e-05,1.641e-05,1.707e-05,1.733e-05,1.841e-05,1.929e-05,1.942e-05,2.117e-05,2.196e-05,2.151e-05,2.249e-05,2.291e-05,2.611e-05,3.225e-05,4.608e-05,4.697e-05,4.592e-05,2.733e-05])

m, b = np.polyfit(x, y, 1) # gets constants for the exp function
print("Base equation: mx + b")
print(f"Constants: {m}, {b}")
print(f"Final linear fit equation: {m}*x + {b}")
print(f"Prediction, in seconds, to solve the problem with length 800: {m * 800 + b}")

def f(a):
    return m*a+b

plt.title(f"{solve_title}")  
plt.xlabel(f"Final position\nPredicted time for final position 800: {m * 800 + b} s")
plt.ylabel("Time [s]")
plt.plot(x, y, 'bo', label=f'{solve_title} results')
plt.plot(x, f(x), 'r', label='Linear fit equation')
plt.legend()
plt.show()