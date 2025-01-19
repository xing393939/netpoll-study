---
marp: true
size: 16:9
style: |
  section {
    background-color: #ccc;
    justify-content: normal;
    font-family: 'Microsoft YaHei', 'SimHei', sans-serif;
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
# 2024年终工作总结
陈兴

---
# 目录
* 工作内容
* 工作总结
* 新年计划

---
# 工作内容
### 活动组：
1. llm活动：参与机构数1300家，参与馆员1700家，宣传了公司大模型平台
2. 健康之屋：医疗行业的升级版微刊，负责其中的商城部分开发
3. 公共组件：红包、抽奖
4. 元宵元宇宙V2：在V1的基础上增加了多种游戏和NPC，趣味性和可玩性更高。
5. 老项目维护：答题V4、趣味答题、虚拟空间入馆培训。

---
### 数据组：
1. RAG流程：大段落切分
2. Flink技术预演：sql任务、python任务、java任务、CDC同步
3. 索引向量化：对比分词搜索，语义相关的检索能力更强
4. 消费mq入库es：实时订阅业务表变更实现实时向量化检索

---
# 工作总结
### 做的好的：
1，robot_qa项目索引别名配置改成使用es别名机制，并无缝替换原有逻辑，使其不再依赖redis服务。
2，Flink项目的依赖包dolphinscheduler-datasource-clickhouse的实现。

### 做的不好的：
做外部对接需要提前规划好对接的接口文档，否则会影响后续的业务流程。

---
# 新年计划
1. 保证所负责项目的稳定运行和需求迭代；
2. 向量化检索，实现百万级向量的快速检索；
3. 学习基于大模型的应用层开发，例如代码生成，claude的Model Context Protocol等等。

---
<!-- _class: center -->
# 谢谢大家
