import numpy as np

import matplotlib.pyplot as plt

# Import the Linear regression module from scikit-learn library
from sklearn.linear_model import LinearRegression

# myModel function
def myModel(x):
    return x*slope + intercept

x = np.array([0, 50, 100, 150, 200, 255])
y = np.array([0, 0.6, 1.13, 1.7, 2.27, 3.03])

# Linear Regression instantiate
lin_reg = LinearRegression()

# Training the model
# The fit method already add the linear bias column (a0)
lin_reg.fit(x.reshape(-1,1), y.reshape(-1,1))

slope = lin_reg.coef_[0][0]
intercept = lin_reg.intercept_[0]

# Getting predict values
myModel = myModel(x)

# Printing values of coefficients
print('a0: %.4f' % (lin_reg.intercept_[0]))
print('a1: %.4f' % (lin_reg.coef_[0][0]))

plt.plot(x, y, '.', color='red')
plt.plot(x, myModel, color='black')
plt.grid(True)
plt.title('Linear Regression for Voltage x PWM')
plt.xlabel('PWM')
plt.ylabel('Voltage [V]')
plt.legend(['Real', 'Estimated'])
plt.savefig('assets/voltage_vs_pwm.png', dpi=400)
plt.show()