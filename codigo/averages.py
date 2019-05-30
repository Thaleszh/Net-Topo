import numpy as np

charm_start = [
	 0.026, 0.024, 0.025, 0.031, 0.021, 0.024, 0.023, 0.022, 0.023, 0.022, 0.032, 0.022, 
	 0.020, 0.025, 0.035, 0.022, 0.026, 0.022, 0.023, 0.024, 0.023, 0.028, 0.008, 0.023, 
	 0.026, 0.030, 0.022, 0.023, 0.022, 0.025]

start_topology = [
	0.000639, 0.002586, 0.000911, 0.000924, 0.000922, 0.000686, 0.000909, 0.000940, 0.000644, 
	0.000931, 0.000905, 0.000912, 0.000647, 0.000649, 0.000978, 0.000659, 0.000960, 0.000672, 
	0.000908, 0.000939, 0.000910, 0.000898, 0.000639, 0.000908, 0.000656, 0.000904, 0.000962, 
	0.000906, 0.000642, 0.000918]

load_topology = [
	0.000715, 0.000738, 0.000740, 0.000732, 0.000733, 0.000732, 0.000737, 0.000742, 0.000569, 
	0.000736, 0.000734, 0.000736, 0.000722, 0.000716, 0.000739, 0.000733, 0.000741, 0.000731, 
	0.000975, 0.000736, 0.000727, 0.000732, 0.000573, 0.000577, 0.000746, 0.000739, 0.000749, 
	0.000743, 0.000733, 0.000735, 0.000769, 0.000735, 0.000650, 0.000732, 0.000730]

neigh_finish = [
	60.601631, 63.807528, 67.241083, 64.234690, 60.147702, 68.689111, 57.683148, 66.796355, 
	70.452773, 64.600163, 72.524272, 72.665128, 70.658160, 69.230606, 67.440739, 69.784635, 
	66.780380, 71.215472, 57.273604, 62.515716]

myneigh_finish = [
	49.884660, 64.735555, 64.252653, 73.763889, 55.965011, 66.270413, 68.721576, 66.440813, 
	61.297315, 64.251111, 62.217247, 76.111961, 73.595112, 69.683825, 62.834316, 59.453180, 
	69.981297, 63.113140, 71.169736, 60.032455]

lb = [
	0.019121, 0.018851, 0.018476, 0.019606, 0.023475, 0.018357, 0.018608, 0.018594, 0.018363, 
	0.018615, 0.018612, 0.022367, 0.018611, 0.022296, 0.018634, 0.018617, 0.019152, 0.014148, 
	0.018625, 0.018578, 0.023405, 0.023885, 0.025439, 0.018356, 0.018679, 0.018923, 0.018612, 
	0.018596, 0.018454, 0.018424, 0.019280, 0.020793, 0.024584, 0.018619, 0.018589, 0.018363, 
	0.014080, 0.020059, 0.016338, 0.018610, 0.021128, 0.018380, 0.029521, 0.016339, 0.018444, 
	0.018626, 0.023304, 0.018374, 0.018534, 0.018618, 0.018605, 0.025322, 0.018860, 0.018620, 
	0.018594, 0.018545, 0.016326, 0.028993, 0.019217, 0.018431]

my_lb = [
	0.018616, 0.019060, 0.018388, 0.018610, 0.018576, 0.018626, 0.019307, 0.024955, 0.018366, 
	0.018429, 0.018573, 0.018441, 0.020486, 0.019457, 0.018980, 0.018624, 0.018612, 0.018609, 
	0.018436, 0.019959, 0.019364, 0.014036, 0.025052, 0.016347, 0.018609, 0.018437, 0.018431, 
	0.022836, 0.018796, 0.018622, 0.018624, 0.019049, 0.018370, 0.018877, 0.018628, 0.018616, 
	0.025822, 0.021395, 0.018628, 0.018551, 0.018611, 0.019550, 0.018617, 0.020941, 0.017851, 
	0.018601, 0.021133, 0.018361, 0.018546, 0.018353, 0.018885, 0.019192, 0.018442, 0.019059, 
	0.019966, 0.018374, 0.018373, 0.018367, 0.035867, 0.018374]

big_myneigh_finish = [
	64.232757, 90.152693, 81.795563, 89.973774, 97.531444, 89.087955, 86.416950, 105.543833, 
	90.378668, 93.396736, 100.834980, 97.207210, 98.036300, 91.181876, 92.530091]

big_neigh_finish = [
	84.958305, 94.931647, 92.796417, 98.304628, 79.204636, 97.976507, 97.051286, 97.961783, 
	102.904683,	 99.921830, 81.486996, 87.175542, 101.637023, 87.388648, 87.823909]

my_big_lb = [
	0.039805, 0.028279, 0.028022, 0.027931, 0.027172, 0.027362, 0.028645, 0.035102, 0.027559,
	0.040218, 0.028048, 0.028327, 0.028237, 0.036760, 0.027554, 0.028058, 0.027814, 0.030954, 
	0.029725, 0.020612, 0.026920, 0.027299, 0.028315, 0.033976, 0.027341, 0.024071, 0.027156, 
	0.039722, 0.027146, 0.024128, 0.027804, 0.027551, 0.028384, 0.028051, 0.027535, 0.028047, 
	0.025870, 0.030632, 0.027814, 0.027600, 0.028146, 0.028375, 0.028237, 0.027927, 0.028054]

big_lb = [
	0.028560, 0.029042, 0.028396, 0.027297, 0.038019, 0.028302, 0.032525, 0.027783, 0.028588, 
	0.027631, 0.027535, 0.028299, 0.043253, 0.027914, 0.028074, 0.027797, 0.041245, 0.027877, 
	0.027144, 0.027678, 0.024100, 0.027365, 0.026905, 0.028024, 0.027236, 0.027865, 0.036109, 
	0.027177, 0.027121, 0.027159, 0.026917, 0.024110, 0.027544, 0.027418, 0.027102, 0.027171, 
	0.027173, 0.020856, 0.028057, 0.035988, 0.028003, 0.032223, 0.027631, 0.036935, 0.031596]

avg_charm_start = np.mean(charm_start)
avg_start_topology = np.mean(start_topology)
avg_load_topology = np.mean(load_topology)
avg_neigh_finish = np.mean(neigh_finish)
avg_myneigh_finish = np.mean(myneigh_finish)
avg_lb = np.mean(lb)
avg_my_lb = np.mean(my_lb)
avg_big_myneigh_finish = np.mean(big_myneigh_finish)
avg_big_neigh_finish = np.mean(big_neigh_finish)
avg_my_big_lb = np.mean(my_big_lb)
avg_big_lb = np.mean(big_lb)

print("avg_charm_start:")
print(avg_charm_start)
print()
print("avg_start_topology:")
print(avg_start_topology)
print()
print("avg_load_topology:")
print(avg_load_topology)
print()
print("avg_neigh_finish:")
print(avg_neigh_finish)
print()
print("avg_myneigh_finish:")
print(avg_myneigh_finish)
print()
print("avg_lb:")
print(avg_lb)
print()
print("avg_my_lb:")
print(avg_my_lb)
print()
print("avg_big_myneigh_finish:")
print(avg_big_myneigh_finish)
print()
print("avg_big_neigh_finish:")
print(avg_big_neigh_finish)
print()
print("avg_my_big_lb:")
print(avg_my_big_lb)
print()
print("avg_big_lb:")
print(avg_big_lb)
print()

std_charm_start = np.std(charm_start)
std_start_topology = np.std(start_topology)
std_load_topology = np.std(load_topology)
std_neigh_finish = np.std(neigh_finish)
std_myneigh_finish = np.std(myneigh_finish)
std_lb = np.std(lb)
std_my_lb = np.std(my_lb)
std_big_myneigh_finish = np.std(big_myneigh_finish)
std_big_neigh_finish = np.std(big_neigh_finish)
std_my_big_lb = np.std(my_big_lb)
std_big_lb = np.std(big_lb)

print("std_charm_start (%):")
print( 100* std_charm_start / avg_charm_start)
print()
print("std_start_topology (%):")
print( 100* std_start_topology / avg_start_topology)
print()
print("std_load_topology (%):")
print( 100* std_load_topology / avg_load_topology)
print()
print("std_neigh_finish (%):")
print( 100* std_neigh_finish / avg_neigh_finish)
print()
print("std_myneigh_finish (%):")
print( 100* std_myneigh_finish / avg_myneigh_finish)
print()
print("std_lb (%):")
print( 100* std_lb / avg_lb)
print()
print("std_my_lb (%):")
print( 100* std_my_lb / avg_my_lb)
print()
print("std_big_myneigh_finish (%):")
print( 100* std_big_myneigh_finish / avg_big_myneigh_finish)
print()
print("std_big_neigh_finish (%):")
print( 100* std_big_neigh_finish / avg_big_neigh_finish)
print()
print("std_my_big_lb (%):")
print( 100* std_my_big_lb / avg_my_big_lb)
print()
print("std_big_lb (%):")
print( 100* std_big_lb / avg_big_lb)