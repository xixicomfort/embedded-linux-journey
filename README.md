# 量化行情转发服务器

## 项目亮点
- Epoll ET + Lock-Free Ring Buffer（std：：atomic + memory_order）
- UDP组播实时接收行情 + 多线程消费
- 零锁、零GC、单机十万QPS

## 编译运行
```bash
make epoll_server 
./epoll_server
