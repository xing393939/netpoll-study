---
marp: true
size: 16:9
style: |
  section {
    background-color: #ccc;
    justify-content: normal;
  }
  table {
    width: 100%;
    display:table;
  }
  td { 
    text-align:center;
  }
  section.center {
    justify-content: center;
    text-align: center;
  }

---
<!-- _class: center -->
# 贝叶斯分类

---
# 内容目录
* 买西瓜的例子
* 相关公式
* 伯努利分布和高斯分布
* 手写数字识别

---
# 买西瓜的例子
如何鉴别西瓜熟了？
- 先验概率：瓜摊上60%的瓜都是熟瓜
- 后验概率：瓜蒂脱落的瓜就是熟瓜

![bg right:50% w:85%](../images/watermelon.jpg)

---
求后验概率：
```
P(瓜熟|瓜蒂脱落) = P(瓜熟) * P(瓜蒂脱落|瓜熟) / P(瓜蒂脱落)
```

根据以往的买瓜经验：
```
P(瓜熟)=0.6
P(瓜蒂脱落|瓜熟)=0.8
P(瓜蒂脱落)=0.64
```

可得：
```
P(瓜熟|瓜蒂脱落) = 0.6*0.8 / 0.64 = 0.75
```

---
通过3个特征来判断西瓜熟了
|瓜蒂脱落|颜色|形状|结果|
|---|---|---|---|
|是|浅绿|圆|__瓜熟__|
|否|深绿|尖|__瓜生__|
|...|

<br/>
给定一个向量X(包含3个特征)，分别求出：

```
P(瓜熟|X) = P(瓜熟) * P(X|瓜熟) / P(X)
P(瓜生|X) = P(瓜生) * P(X|瓜生) / P(X)
```

---
根据以往的买瓜经验：
```
P(瓜蒂脱落|瓜熟)=2/3        P(瓜蒂脱落|瓜生)=0.25
P(浅绿|瓜熟)=1/3　　        P(浅绿|瓜生)=0.25
P(圆形|瓜熟)=2/3　　        P(圆形|瓜生)=0.25
```

可得：
```
P(瓜熟|X) = 0.6 * 2/3 * 1/3 * 2/3 = 4/45
P(瓜生|X) = 0.4 * 0.25 * 0.25 * 0.25 = 1/160 
```

---
# 相关公式
### 1.贝叶斯公式
$P(Y|X) = \frac{P(Y) * P(X|Y)}{P(X)}$

### 2.贝叶斯分类
$P(Y=c_k|X=x) = \frac{P(Y=c_k) * P(X=x|Y=c_k)}{P(X=x)}$

$y = arg \max\limits_{c_k} \frac{P(Y=c_k) * P(X=x|Y=c_k)}{P(X=x)}$

$\ \, = arg \max\limits_{c_k} P(Y=c_k) * P(X=x|Y=c_k)$
$\ \, = arg \max\limits_{c_k} P(Y=c_k) * \prod\limits_{j=1}^nP(X^{(j)}=x^{(j)}|Y=c_k)$

---
# 伯努利分布和高斯分布
### 伯努利分布
$$
\begin{flalign}
\qquad \quad \ \ \ p_{X}(x) = \begin{cases} 
p,  & \text{若x=1} \\
1-p, & \text{若x=0}  \end{cases} &&
\end{flalign}
$$

### 高斯分布
$\qquad\;\;\;\;\; f_X(x) = \frac{1}{\sqrt{2\pi}σ}e^{-(x-μ)^2/(2σ^2)}$

---
<style>
strong {color: green}
em {color: red}
</style>
3个特征的特征值变成0~255
|瓜蒂脱落|颜色|形状|结果|
|---|---|---|---|
|251|1|240|__瓜熟__|
|3|155|16|_瓜生_|
|...|

<br/>
P(瓜熟|X) = 0.6 * 0.06 * 0.04 * 0.01
P(瓜生|X) = 0.4 * 0.03 * 0.11 * 0.02

![bg right:50% w:90%](../images/gaussian.jpg)

---
如何求出高斯概率分布函数？

$f_X(x) = \frac{1}{\sqrt{2\pi}σ}e^{-(x-μ)^2/(2σ^2)}$



---
<!-- _class: center -->
# Thanks
