# 量化行情转发服务器

## 项目描述
- 基于Epoll ET + std::atomic无锁环形缓冲区实现UDP组播行情转发服务器，支持多线程消费
- 采用memory_order_release/acquire保证无锁安全，单机可处理10万QPS行情包（本地测试通过）
- 已支持生产消费分离，为后续策略引擎提供低延迟数据通道

## 项目亮点
- Epoll ET + Lock-Free Ring Buffer（std::atomic + memory_order）
- UDP组播实时接收行情 + 多线程消费
- 零锁、零GC、单机十万QPS
- 已支持无锁队列生产消费分离

## 编译运行
```bash
make epoll_server 
./epoll_server
