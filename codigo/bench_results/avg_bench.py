import numpy as np
import pandas as pd
# <name>, <init time>, <save time>, <load time>, <prox time>, <hops 1>, <hops 2>, 
# <dist 1>, <dist 2>, <fill>, <dist 1>, <dist 2>

dist_1_memoi = {}
dist_2_memoi = {}
dist_1_nomemoi = {}
dist_2_nomemoi = {}
dist_1_fill = {}
dist_2_fill = {}
# <init time> <save time> <load time>
init_memoi = {}
init_nomemoi = {}
load = {}
load_nomemoi = {}
hops_1 = {}
hops_2 = {}
prox = {}
fill = {}

# get them all into arrays and dicts

for name in ["mesh2D_16", "mesh3D_8", "tree_7", "tree_10"]:
	temp = pd.read_csv("dist.1." + name + ".no_memoi.txt", sep=" ", header=None)
	temp.drop(temp.tail(1).index,inplace=True)
	dist_1_nomemoi[name] = temp.to_numpy().astype(int)

	temp = pd.read_csv("dist.2." + name + ".no_memoi.txt", sep=" ", header=None)
	temp.drop(temp.tail(1).index,inplace=True)
	dist_2_nomemoi[name] = temp.to_numpy().astype(int)

	temp = pd.read_csv("init." + name + ".no_memoi.txt", sep=" ", header=None)
	temp.drop(temp.tail(1).index,inplace=True)
	init_nomemoi[name] = np.array(temp.to_numpy().astype(int))

	temp = pd.read_csv("load." + name + ".no_memoi.txt", sep=" ", header=None)
	temp.drop(temp.tail(1).index,inplace=True)
	load_nomemoi[name] = temp.to_numpy().astype(int)

	temp = pd.read_csv("dist.1." + name + ".with_memoi.txt", sep=" ", header=None)
	temp.drop(temp.tail(1).index,inplace=True)
	dist_1_memoi[name] = temp.to_numpy().astype(int)

	temp = pd.read_csv("dist.2." + name + ".with_memoi.txt", sep=" ", header=None)
	temp.drop(temp.tail(1).index,inplace=True)
	dist_2_memoi[name] = temp.to_numpy().astype(int)

	dist_1_fill[name] = (pd.read_csv("dist.1." + name + ".with_fill.txt", sep=" ", header=None)).to_numpy().astype(int)
	dist_2_fill[name] = (pd.read_csv("dist.2." + name + ".with_fill.txt", sep=" ", header=None)).to_numpy().astype(int)
	prox[name] = (pd.read_csv("prox." + name + ".txt", sep=" ", header=None)).to_numpy().astype(int)
	hops_1[name] = (pd.read_csv("hop.1." + name + ".txt", sep=" ", header=None)).to_numpy().astype(int)
	hops_2[name] = (pd.read_csv("hop.2." + name + ".txt", sep=" ", header=None)).to_numpy().astype(int)
	fill[name] = (pd.read_csv("fill." + name + ".txt", sep=" ", header=None)).to_numpy().astype(int)

	init_memoi[name] = (pd.read_csv("init." + name + ".with_memoi.txt", sep=" ", header=None)).to_numpy().astype(int)
	load[name] = (pd.read_csv("load." + name + ".with_memoi.txt", sep=" ", header=None)).to_numpy().astype(int)


repetitions=25
smallrep=100
giantrep=200

# find averages and means
avg_dist_1_memoi = {}
avg_dist_2_memoi = {}
avg_dist_1_nomemoi = {}
avg_dist_2_nomemoi = {}
avg_dist_1_fill = {}
avg_dist_2_fill = {}

#dist 1 = small rep
#dist 2 = giant rep
avg_dist_1_grouped_memoi = {}
avg_dist_2_grouped_memoi = {}
avg_dist_1_grouped_nomemoi = {}
avg_dist_2_grouped_nomemoi = {}
#both filles are giant
avg_dist_1_grouped_fill = {}
avg_dist_2_grouped_fill = {}
# <init time> <save time> <load time>
avg_init_memoi = {}
avg_save_memoi = {}
avg_init_save_memoi = {}
avg_init_nomemoi = {}
avg_save_nomemoi = {}
avg_init_save_nomemoi = {}
avg_load = {}
avg_load_nomemoi = {}
avg_hops_1 = {}
avg_hops_2 = {}
# small and giant reps
avg_hops_1_grouped = {}
avg_hops_2_grouped = {}
avg_prox = {}
avg_fill = {}

std_dist_1_memoi = {}
std_dist_2_memoi = {}
std_dist_1_nomemoi = {}
std_dist_2_nomemoi = {}
std_dist_1_fill = {}
std_dist_2_fill = {}

# <init time> <save time> <load time>
std_init_memoi = {}
std_save_memoi = {}
std_init_save_memoi = {}
std_init_nomemoi = {}
std_save_nomemoi = {}
std_init_save_nomemoi = {}
std_load = {}
std_load_nomemoi = {}
std_hops_1 = {}
std_hops_2 = {}
std_prox = {}
std_fill = {}
# exclusive init and dist no memoi from these 2

for name in ["mesh2D_16", "mesh3D_8", "tree_7", "tree_10"]:
	avg_dist_1_nomemoi[name] = np.mean(dist_1_nomemoi[name])
	avg_dist_2_nomemoi[name] = np.mean(dist_2_nomemoi[name])
	avg_dist_1_grouped_nomemoi[name] = np.sum(dist_1_nomemoi[name]) / giantrep
	avg_dist_2_grouped_nomemoi[name] = np.sum(dist_2_nomemoi[name]) / smallrep
	avg_init_nomemoi[name] = [np.mean(init_nomemoi[name][:, 0]) , np.mean(init_nomemoi[name][:, 1]), np.mean(init_nomemoi[name][:, 2])]
	avg_load_nomemoi[name] = np.mean(load_nomemoi[name])
	std_dist_1_nomemoi[name] = np.std(dist_1_nomemoi[name])
	std_dist_2_nomemoi[name] = np.std(dist_2_nomemoi[name])
	std_init_nomemoi[name] = [np.std(init_nomemoi[name][:, 0]) , np.std(init_nomemoi[name][:, 1]), np.std(init_nomemoi[name][:, 2])]
	std_load_nomemoi[name] = np.std(load_nomemoi[name])
	avg_dist_1_memoi[name] = np.mean(dist_1_memoi[name])
	avg_dist_2_memoi[name] = np.mean(dist_2_memoi[name])
	avg_dist_1_grouped_memoi[name] = np.sum(dist_1_memoi[name]) / giantrep
	avg_dist_2_grouped_memoi[name] = np.sum(dist_2_memoi[name]) / smallrep
	avg_dist_1_fill[name] = np.mean(dist_1_fill[name])
	avg_dist_2_fill[name] = np.mean(dist_2_fill[name])
	avg_dist_1_grouped_fill[name] = np.sum(dist_1_fill[name]) / giantrep
	avg_dist_2_grouped_fill[name] = np.sum(dist_2_fill[name]) / giantrep
	avg_init_memoi[name] = [np.mean(init_memoi[name][:, 0]) , np.mean(init_memoi[name][:, 1]), np.mean(init_memoi[name][:, 2])]
	avg_load[name] = np.mean(load[name])
	avg_hops_1[name] = np.mean(hops_1[name])
	avg_hops_2[name] = np.mean(hops_2[name])
	avg_hops_1_grouped[name] = np.sum(hops_1[name]) / giantrep
	avg_hops_2_grouped[name] = np.sum(hops_2[name]) / smallrep
	avg_prox[name] = np.mean(prox[name])
	avg_fill[name] = np.mean(fill[name])
	std_dist_1_memoi[name] = np.std(dist_1_memoi[name])
	std_dist_2_memoi[name] = np.std(dist_2_memoi[name])
	std_dist_1_fill[name] = np.std(dist_1_fill[name])
	std_dist_2_fill[name] = np.std(dist_2_fill[name])
	std_init_memoi[name] = [np.std(init_memoi[name][:, 0]) , np.std(init_memoi[name][:, 1]), np.std(init_memoi[name][:, 2])]
	std_load[name] = np.std(load[name])
	std_hops_1[name] = np.std(hops_1[name])
	std_hops_2[name] = np.std(hops_2[name])
	std_prox[name] = np.std(prox[name])
	std_fill[name] = np.std(fill[name])

# not printing tree 10 and mesh3D 8
for name in ["mesh2D_16", "tree_7"]:
	print("\n\n No memoi - " + name)
	print("Count: " + str(len(dist_1_nomemoi[name])) + " dist_1_nomemoi " + name + " " + str(avg_dist_1_nomemoi[name]) + " avrg call time: " + str(avg_dist_1_grouped_nomemoi[name]))
	print("Count: " + str(len(dist_2_nomemoi[name])) + " dist_2_nomemoi " + name + " " + str(avg_dist_2_nomemoi[name]) + " avrg call time: " + str(avg_dist_2_grouped_nomemoi[name]))
	print("Count: " + str(len(init_nomemoi[name][:, 0])) + " init_nomemoi " + name + " " + str(avg_init_nomemoi[name][0]))
	print("Count: " + str(len(init_nomemoi[name][:, 1])) + " save_nomemoi " + name + " " + str(avg_init_nomemoi[name][1]))
	print("Count: " + str(len(init_nomemoi[name][:, 2])) + " init_total_nomemoi " + name + " " + str(avg_init_nomemoi[name][2]))
	print("Count: " + str(len(load_nomemoi[name])) + " load_nomemoi " + name + " " + str(avg_load_nomemoi[name]))
	print("std_dist_1_nomemoi (%) " + str(100* std_dist_1_nomemoi[name] / avg_dist_1_nomemoi[name]) + " avrg call deviation: " + str(100* std_dist_1_nomemoi[name] / avg_dist_1_grouped_nomemoi[name]))
	print("std_dist_2_nomemoi (%) " + str(100* std_dist_2_nomemoi[name] / avg_dist_2_nomemoi[name]) + " avrg call deviation: " + str(100* std_dist_2_nomemoi[name] / avg_dist_2_grouped_nomemoi[name]))
	print("std_init_nomemoi (%) " + str(100* std_init_nomemoi[name][0] / avg_init_nomemoi[name][0]))
	print("std_save_nomemoi (%) " + str(100* std_init_nomemoi[name][1] / avg_init_nomemoi[name][1]))
	print("std_init_total_nomemoi (%) " + str(100* std_init_nomemoi[name][2] / avg_init_nomemoi[name][2]))
	print("std_load_nomemoi (%) " + str(100* std_load_nomemoi[name] / avg_load_nomemoi[name]))
	# print("\n\n Memoi - " + name)
	# print("Count: " + str(len(dist_1_memoi[name])) + " dist_1_memoi " + name + " " + str(avg_dist_1_memoi[name]) + " avrg call time: " + str(avg_dist_1_grouped_memoi[name]))
	# print("Count: " + str(len(dist_2_memoi[name])) + " dist_2_memoi " + name + " " + str(avg_dist_2_memoi[name]) + " avrg call time: " + str(avg_dist_2_grouped_memoi[name]))
	# print("Count: " + str(len(dist_1_fill[name])) + " dist_1_fill " + name + " " + str(avg_dist_1_fill[name]) + " avrg call time: " + str(avg_dist_1_grouped_fill[name]))
	# print("Count: " + str(len(dist_2_fill[name])) + " dist_2_fill " + name + " " + str(avg_dist_2_fill[name]) + " avrg call time: " + str(avg_dist_2_grouped_fill[name]))
	# print("Count: " + str(len(init_memoi[name][:, 0])) + " init_memoi " + name + " " + str(avg_init_memoi[name][0]))
	# print("Count: " + str(len(init_memoi[name][:, 1])) + " save_memoi " + name + " " + str(avg_init_memoi[name][1]))
	# print("Count: " + str(len(init_memoi[name][:, 2])) + " init_total_memoi " + name + " " + str(avg_init_memoi[name][2]))	
	# print("Count: " + str(len(load[name])) + " load " + name + " " + str(avg_load[name]))
	# print("Count: " + str(len(hops_1[name])) + " hops_1 " + name + " " + str(avg_hops_1[name]) + " avrg call time: " + str(avg_hops_1_grouped[name]))
	# print("Count: " + str(len(hops_2[name])) + " hops_2 " + name + " " + str(avg_hops_2[name]) + " avrg call time: " + str(avg_hops_2_grouped[name]))
	# print("Count: " + str(len(prox[name])) + " prox " + name + " " + str(avg_prox[name]))
	# print("Count: " + str(len(fill[name])) + " fill " + name + " " + str(avg_fill[name]))
	# print("std_dist_1_memoi (%) " + str(100* std_dist_1_memoi[name] / avg_dist_1_memoi[name]) + " avrg call deviation: " + str(100* std_dist_1_memoi[name] / avg_dist_1_grouped_memoi[name]))
	# print("std_dist_2_memoi (%) " + str(100* std_dist_2_memoi[name] / avg_dist_2_memoi[name]) + " avrg call deviation: " + str(100* std_dist_2_memoi[name] / avg_dist_2_grouped_memoi[name]))
	# print("std_dist_1_fill (%) " + str(100* std_dist_1_fill[name] / avg_dist_1_fill[name]) + " avrg call deviation: " + str(100* std_dist_1_fill[name] / avg_dist_1_grouped_fill[name]))
	# print("std_dist_2_fill (%) " + str(100* std_dist_2_fill[name] / avg_dist_2_fill[name]) + " avrg call deviation: " + str(100* std_dist_2_fill[name] / avg_dist_2_grouped_fill[name]))
	# print("std_init_memoi (%) " + str(100* std_init_memoi[name][0] / avg_init_memoi[name][0]))
	# print("std_save_memoi (%) " + str(100* std_init_memoi[name][1] / avg_init_memoi[name][1]))
	# print("std_init_total_memoi (%) " + str(100* std_init_memoi[name][2] / avg_init_memoi[name][:2]))
	# print("std_load (%) " + str(100* std_load[name] / avg_load[name]))
	# print("std_hops_1 (%) " + str(100* std_hops_1[name] / avg_hops_1[name]) + " avrg call deviation: " + str(100* std_hops_1[name] / avg_hops_1_grouped[name]))
	# print("std_hops_2 (%) " + str(100* std_hops_2[name] / avg_hops_2[name]) + " avrg call deviation: " + str(100* std_hops_2[name] / avg_hops_2_grouped[name]))
	# print("std_prox (%) " + str(100* std_prox[name] / avg_prox[name]))
	# print("std_fill (%) " + str(100* std_fill[name] / avg_fill[name]))
