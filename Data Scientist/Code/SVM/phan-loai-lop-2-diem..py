# Import các thư viện cần thiết
import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm, datasets

# Lấy tập dữ liệu iris với 2 features (để giống bài toán phân lớp 2 điểm)
iris = datasets.load_iris()
X = iris.data[:, :2]
y = iris.target

# Thực hiện SVM với kernel là linear
svc = svm.SVC(kernel='linear', C=1.0).fit(X, y)

# Tạo một ma trận 2D từ các giá trị tối đa và tối thiểu của 2 features để tạo ra các điểm dữ liệu ảo
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.02), np.arange(y_min, y_max, 0.02))

# Dự đoán lớp của các điểm dữ liệu ảo với mô hình đã được thực hiện
Z = svc.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

# Vẽ đường phân chia giữa 2 lớp và các điểm dữ liệu đã được phân lớp
plt.contourf(xx, yy, Z, cmap=plt.cm.coolwarm, alpha=0.8)
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=plt.cm.coolwarm)
plt.xlabel('Feature 1')
plt.ylabel('Feature 2')
plt.title('SVM Classification')

plt.show()
