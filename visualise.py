import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D

import matplotlib
matplotlib.use('TkAgg')

N = 1000
T = 50000
D = 3


#
# sim_data = np.genfromtxt("NBody_BH_data.txt", delimiter=",")
# sim_data = np.loadtxt("NBody_BH_data.txt", delimiter=",")

# print(sim_data[0].shape)
# print(sim_data)

# positions_of_bodies_for_all_times = np.zeros((T, N, D))
# for i in range(T):
#     for j in range(N):
#         for k in range(D):
#             positions_of_bodies_for_all_times[i][j][k] = sim_data[i][(k * N) + j]


# sim_data_np_arr = np.asarray(positions_of_bodies_for_all_times)
# print(positions_of_bodies_for_all_times.shape)


def load_data(filename):
    data = np.genfromtxt(filename, delimiter=",")
    return data


def proc_data(dat, X, Y, Z):
    # order of input is X = T, Y = N, Z = D
    # so transpose in (1, 2, 0)
    procd_data = np.zeros((X, Y, Z))
    for i in range(X):
        for j in range(Y):
            for k in range(Z):
                procd_data[i][j][k] = dat[i][(k * Y) + j]
    procd_data = procd_data.transpose(1, 2, 0)
    return procd_data


def plot_data(data):
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1, projection='3d')

    max_range = 0
    for i in range(np.size(data, 0)):
        max_dim = max(max(data[i][0]), max(data[i][1]), max(data[i][2]))
        if max_dim > max_range:
            max_range = max_dim
        ax.plot(data[i][0], data[i][1], data[i][2])

    ax.set_xlim([-max_range, max_range])
    ax.set_ylim([-max_range, max_range])
    ax.set_zlim([-max_range, max_range])
    plt.show()


def animate_data(data):
    max_range = 0
    for i in range(np.size(data, 0)):
        max_dim = max(max(data[i][0]), max(data[i][1]), max(data[i][2]))
        if max_dim > max_range:
            max_range = max_dim
    time = np.arange(data.shape[2])
    fig1 = plt.figure()  # facecolor='black'
    fig1.set_size_inches(12.8, 7.2, True)  # 21 inches width, 15 inches height
    ax1 = plt.subplot(111, projection='3d')  # Axes3D(fig1) plt.subplot(111,projection='3d')
    ax1.grid(False)
    ax1.set_facecolor('black')
    ax1.w_xaxis.set_pane_color((0.0, 0.0, 0.0, 0.0))
    ax1.w_yaxis.set_pane_color((0.0, 0.0, 0.0, 0.0))
    ax1.w_zaxis.set_pane_color((0.0, 0.0, 0.0, 0.0))
    paths = [ax1.plot(body[0, 0:1], body[1, 0:1], body[2, 0:1], marker='o', color='w', markersize=2, alpha=1)[0] for
             body in data]

    def update(i, bodies, trajectories):
        for trajectory, body in zip(trajectories, bodies):
            trajectory.set_data(body[0:2, i - 1:i])
            trajectory.set_3d_properties(body[2, i - 1:i])

        return trajectories

    ax1.set_xlim([-max_range, max_range])
    ax1.set_ylim([-max_range, max_range])
    ax1.set_zlim([-max_range, max_range])
    ani = animation.FuncAnimation(fig1, update, frames=time[::25], fargs=(data, paths), interval=1, blit=True)  # [::25]

    return ani


sim_data = load_data("NBody_BH_data.txt")
procd_data = proc_data(sim_data, T, N, D)
p = plot_data(procd_data)

# Writer = animation.writers['ffmpeg']
# writer = Writer(fps=60, bitrate=1000)
# a = animate_data(procd_data)
# a.save('NbodyBH.mp4', writer=writer,dpi=150)

