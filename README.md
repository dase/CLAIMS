**CLAIMS** (CLuster-Aware In-Memory Sql query engine) is a parallel in-memory database prototype, which runs on clusters of commodity servers and provides real-time data analytics on relational dataset. 

#### Highlights

##### 1. Massively parallel execution engine.

CLAIMS relies on highly parallel query processing engine to dramatically accelerate data analysis speed. Query evaluations are not only distributed across the cluster to leverage the computation power of the cluster, but are also executed in a multi-threaded fashion to unleash the power of modern multi-core hardware.


##### 2. Smart intra-node parallelism. 

Pipelining the query execution among nodes in the cluster effectively reduces the response time and dramatically saves storage space for intermediate query results. However, its benefits degrade tremendously when the workloads are imbalanced among execution partitions due to the improperly generated query execution plan. To tackle this problem, a novel elastic query processing framework, i.e., *elastic pipelining*, is proposed in CLAIMS, which adjusts the intra-node parallelism according to the runtime workload based on elaborate performance model. Beneficial from elastic pipelining query processing, the parallelism of different execution fragments in a pipelined is self-adaptive, resulting in an optimal intra-node parallelism assignment. Please refer to our SIGMOD paper for more details about the elastic pipelining framework.

![asdf](https://i1.piimg.com/1949/99a94a4e18e6fc21.jpg)




##### 3. Efficient in-memory data processing.

CLAIMS employs a large set of optimization techniques to achieve efficient in-memory data processing, including batch-at-a-time processing, cache-sensitive operators, SIMD-based optimization, code generation, lock-free and concurrent processing structures. These optimizations work collaborately and enable CLAIMS to process up to gigabytes data per second on a single thread.

##### 4. Network communication optimization. 
Parallel query processing imposes high burdens on network communication, which usually becomes performance bottleneck of the in-memory parallel databases due to the relatively slow network bandwidth. When compiling a user query into an execution plan, CLAIMS’s query optimizer leverages a sophisticated selectivity propagation system and cost model to generate physical query plans with minimized network communication cost. Furthermore, CLAIMS deploys a new data exchange implementation, which offers efficient, scalable and skew-resilient network data transfer among CLAIMS instances. These optimizations greatly reduce the response time for a large variety of queries.

#### Performance
Beneficial from the smart and massively parallelism and the in-memory data processing optimizations, CLAIMS is up to 5X faster than Shark and Impala, two state-of-the-art systems in the open source community, in the queries against TPCH dataset and Shanghai Stock Exchange dataset.

![asdf](https://i1.piimg.com/1949/de04caa268f1215f.jpg)


#### Publications

1. Li Wang, Minqi Zhou, Zhenjie Zhang, Yin Yang, Aoying Zhou, Dina Bitton. Elastic Pipelining in a In-Memory Database Cluster. ACM SIGMOD 2016, pp. 1279-1294. [[pdf](http://dase.ecnu.edu.cn/liwang/papers/li-elastic.pdf), [cite](http://dase.ecnu.edu.cn/liwang/papers/li-elastic.bib)]
2. Li Wang, Minqi Zhou, Zhenjie Zhang, Ming-chien Shan, Aoying Zhou. NUMA-aware Scalable and Efficient Aggregation on Large Domains. IEEE TKDE 2015:4. pp.1071-1084. [[pdf](http://dase.ecnu.edu.cn/liwang/papers/li-tkde2014.pdf), [cite](http://dase.ecnu.edu.cn/liwang/papers/li-tkde2014.bib)]
3. Li Wang, Lei Zhang, Chengcheng Yu, Aoying Zhou. Optimizing Pipelined Execution for Distributed In-memory OLAY System. In: DaMen 2014. Springer. 2014. pp. 35-56.
4. Lan Huang, Ke Xun, Xiaozhou Chen, Minqi Zhou, In-memory Cluster Computing: Interactive Data Analysis, Journal of East China Normal University, 2014
5. Shaochan Dong, Minqi Zhou, Rong Zhang，Aoying Zhou, In-Memory Index：Performance Enhancement Leveraging on Processors, Journal of East China Normal University,2014
6. Xinzhou Zhang, Minqi Zhou,A Survey of Fault Tolerance and Fault Recovery Technique in Large Distributed Parallel Computing System, Journal of East China Normal University,2014
7. Lei Zhang, Minqi Zhou, Li Wang, LCDJ: Locality Conscious Join Alogrithm for In-memory Cluster Computing, Journal of East China Normal University, 2014
8. Lei Zhang, Zhuhe Fang, Minqi Zhou，Lan Huang, Join Algorithms Towards In-memory Computing, Journal of East China Normal University, 2014
9. Yongfeng Li, Minqi Zhou, Hualiang Hu, Survey of resource uniform management and scheduling in cluster, Journal of East China Normal University, 2014

#### Quick Start
Try our CLAIMS, please follow [Quick Start](https://github.com/dase/CLAIMS/wiki/Installation-steps).

#### More 
Learn more information, please go to [Wiki](https://github.com/dase/CLAIMS/wiki).

