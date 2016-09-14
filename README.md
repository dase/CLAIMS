**CLAIMS** (CLuster-Aware In-Memory Sql query engin) is a parallel in-memory database prototype, which runs on clusters of commodity servers and aims to provide real-time data analytics on relational dataset. 

#### Highlights

##### 1. Massively parallel execution engine.

CLAIMS relies on highly parallel query processing engine to dramatically accelerate data analysis speed. Query evaluations are distributed across the cluster and executed in parallel. Query evaluations are not only distirbuted across the cluster to leverage the computation power of the cluster, but are also executed in a multi-threaded fashion to unleash the power of modern multi-core hardware.


##### 2. Smart intra-node parallelism. 

Pipelining the query execution among nodes in the cluster effectively reduces the response latency and dramatically saves storage space for intermediate query results. However, its benefits degrade tremendously when the workloads are imbalanced among execution partitions due to the improperly generated query execution plan. To tackle this problem, a novel elastic pipelining query processing model is proposed in CLAIMS, which adapts the intra-node parallelism to the runtime workload. Beneficial from elastic pipelining query processing, the parallelism of different execution fragments in a pipelined is self-adaptive with each other and results in an optimal intra-node parallelism assignment. Please refer to our SIGMOD paper for more details about elastic pipelining.
![asdf](http://dase.ecnu.edu.cn/liwang/images/elastic_pipeline.jpg)



##### 3. Efficient in-memory data processing.

CLAIMS employs a large set of optimization techniques to achieve efficient in-memory data processing, including batch-at-a-time processing, cache-sensitive operators, SIMD-based optimization, code generation, lock-free and concurrent processing structures. These optimizations work collaborately and enable CLAIMS to process up to gigabytes data per second within a single thread.

##### 4. Network communication optimization. 
Parallel query processing imposes high burdens on network communication, which becomes the performance bottleneck for in-memory parallel databases due to the relatively slow network bandwidth. When compiling a user query into an execution plan, CLAIMS’s query optimizer leverages a sophisticated selectivity propagation system and cost model to generate physical query plans with minimized network communication cost. Furthermore, CLAIMS deploys a new data exchange implementation, which offers efficient, scalable and skew-resilient network data communication among CLAIMS instances. These optimizations greatly reduce the response time of the queries that require network data communication.

#### Performance
Beneficial from the smart and massively parallelism and the in-memory data processing optimizations, CLAIMS is up to 5X faster than Shark and Impala, two state-of-the-art systems in the open source community, in the queries against TPCH dataset and Shanghai Stock Exchange dataset.

![asdf](http://dase.ecnu.edu.cn/liwang/images/compare.jpg)

#### Team members

[Aoying Zhou](http://dase.ecnu.edu.cn/index/shows/catid/14/id/131), Professor in East China Normal University, is the person in charge of this project.

[Minqi Zhou](https://github.com/polpo1980), Associate Professor in East China Normal University, is the person in charge of this project.

[Chuliang Weng](http://dase.ecnu.edu.cn/index/shows/catid/14/id/144), Professor in East China Normal University, is the person in charge of this project.

[Zhuhe Fang](https://github.com/fzhedu) is mainly responsible for designing and implementing SQL query optimization.

[Kai Yu](https://github.com/yukai2014) is mainly responsible for designing and implementing SQL DDL and data injection.

[Han Zhang](https://github.com/Jackson1992) is mainly responsible for storage layer.

[Yang Yu](https://github.com/FishYoung) is mainly responsible for SQL parser and optimization.

[Zhifang Li](https://github.com/yestodaylee) is mainly responsible for data injection.

[Zhenhui Zhao](https://github.com/zhejiangxiaomai) is mainly responsible for improving system stability.

[Tingting Sun]() is mainly responsible for data format.

#### Old members

[Li Wang](https://github.com/wangli1426), Ph.D. student in East China Normal University, manages the master students in this team and is responsible for designing and implementing the key components of CLAIMS, including query optimizer, catalog, physical operators, distributed communication infrastructure, storage layout, etc.

[Lei Zhang](https://github.com/egraldlo) is responsible for designing and implementing the key components of CLAIMS, including query optimizer, physical operators, persistent data exchange, storage management, etc.

[Shaochan Dong](https://github.com/scdong) is responsible for designing and implementing in-memory index and index management, data types, as well as data loading and importing.

[Xinzhou Zhang]() is mainly responsible for web UI design and implementing data importing model.

[Yongfeng Li](https://github.com/NagamineLee) was a formal member of CLAIMS, who participated in designing and implementing catalog model.

[Lin Gu]() is responsible for designing the demo cases of CLAIMS.

#### Publications

1. Li Wang, Minqi Zhou, Zhenjie Zhang, Yin Yang, Aoying Zhou, Dina Bitton. Elastic Pipelining in In-Memory Database Cluster. To appear in Sigmod 2016.
2. Li Wang, Minqi Zhou, Zhenjie Zhang, Ming-chien Shan, Aoying Zhou. NUMA-aware Scalable and Efficient Aggregation on Large Domains. IEEE TKDE 2015:4. pp.1071-1084 .
3. Li Wang, Lei Zhang, Chengcheng Yu, Aoying Zhou. Optimizing Pipelined Execution for Distributed In-memory OLAY System. In: DaMen 2014. Springer. 2014. pp. 35-56.
4. Lan Huang, Ke Xun, Xiaozhou Chen, Minqi Zhou, In-memory Cluster Computing: Interactive Data Analysis, Journal of East China Normal University, 2014
5. Shaochan Dong, Minqi Zhou, Rong Zhang，Aoying Zhou, In-Memory Index：Performance Enhancement Leveraging on Processors, Journal of East China Normal University,2014
6. Xinzhou Zhang, Minqi Zhou,A Survey of Fault Tolerance and Fault Recovery Technique in Large Distributed Parallel Computing System, Journal of East China Normal University,2014
7. Lei Zhang, Minqi Zhou, Li Wang, LCDJ: Locality Conscious Join Alogrithm for In-memory Cluster Computing, Journal of East China Normal University, 2014
8. Lei Zhang, Zhuhe Fang, Minqi Zhou，Lan Huang, Join Algorithms Towards In-memory Computing, Journal of East China Normal University, 2014
9. Yongfeng Li, Minqi Zhou, Hualiang Hu, Survey of resource uniform management and scheduling in cluster, Journal of East China Normal University, 2014

#### Quick Start
Try our CLAIMS, following [Quick Start](https://github.com/dase/CLAIMS/wiki).
