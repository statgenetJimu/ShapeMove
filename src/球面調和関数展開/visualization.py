#---------------
# 複素数版
#---------------
import numpy as np
from scipy.special import sph_harm as _sph_harm
def sph_harm(l, m, theta, phi):
    return _sph_harm(m, l, phi, theta)
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import matplotlib.cm as cm

# R^2(u=θ, v=Φ)
u = np.linspace(0, np.pi, 80)
v = np.linspace(0, 2 * np.pi, 80)

# R^3
x = np.outer(np.sin(u), np.cos(v))
y = np.outer(np.sin(u), np.sin(v))
z = np.outer(np.cos(u), np.ones(v.size))

fig = plt.figure()
Lmax = 3
for l in range(Lmax + 1):
    for m in range(-l, l+1):
        # 球面調和関数の値を色で表す
        sph = np.abs(sph_harm(l, m, np.array(u, ndmin=2).T, np.array(v)))
# 正規化
#        if sph.max() == sph.min():
#            sph.fill(0.5)
#        else:
#            sph = (sph - sph.min()) / (sph.max() - sph.min())
        colors = cm.jet(sph) #cm.bwr
        ax = fig.add_subplot(Lmax + 1, 2*Lmax+1, l*(2*Lmax+1)+m+Lmax+1, projection="3d")
        ax.set_axis_off()
        surf = ax.plot_surface(x, y, z, facecolors=colors,
                rstride=2, cstride=2, linewidth=10, shade=False)

plt.savefig("sh_c.png", transparent=False)

#---------------
# 実数版
#---------------
import numpy as np
from scipy.special import sph_harm as _sph_harm
def sph_harm(l, m, theta, phi):
    if m >= 0:
        return np.sqrt(2) * _sph_harm(m, l, phi, theta).real
    else:
        return np.sqrt(2) * _sph_harm(-m, l, phi, theta).imag
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import matplotlib.cm as cm

# R^2
u = np.linspace(0, np.pi, 80)
v = np.linspace(0, 2 * np.pi, 80)

# lの最大値
Lmax = 2
# rの初期値
r = np.zeros([u.size, v.size])
# 係数
c = np.concatenate([[10], [1, 2, 0], [1, 5, 3, 4, 0]])

for l in range(Lmax + 1):
    for m in range(-l, l + 1):
        # 球面調和関数を足し合わせる
        r += c[l**2 + l + m] * sph_harm(l, m, np.array(u, ndmin=2).T, v).real

# R^3
x = r * np.outer(np.sin(u), np.cos(v))
y = r * np.outer(np.sin(u), np.sin(v))
z = r * np.outer(np.cos(u), np.ones(v.size))

fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")
ax.set_axis_off()
surf = ax.plot_surface(x, y, z, color='b',
        rstride=2, cstride=2, linewidth=1, shade=False)
plt.savefig("shape.png", transparent=False)
