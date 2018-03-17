# NOT used

import numpy as np
import matplotlib.pyplot as plt
import csv

with open('mean_queue_length.csv') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')

	x = []
	y = []

	for row in csv_reader:
		x_val = row[0]
		y_val = row[1]
		x.append(x_val)
		y.append(y_val)

	plt.scatter(x, y)
	plt.xlabel('Lambda')
	plt.ylabel('Mean Queue Length')
	plt.title('Mean Queue Length vs Lambda')
	plt.autoscale(enable=True, axis='both', tight=None)
	plt.show()
