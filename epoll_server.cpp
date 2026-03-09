#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>
#include <atomic>
#include <thread>

#define MAX_EVENTS 128
#define PORT 8080
#define RING_SIZE 1024 // 无锁环形缓冲区大小

//Lock-free Ring Buffer
struct RingBuffer
{
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};
    char buffer[RING_SIZE];

    bool push(const char* data,size_t len){
        size_t h = head.load(std::memory_order_relaxed);
        size_t t = tail.load(std::memory_order_acquire);
        if((h + len) %RING_SIZE == t) return false;
        std::memcpy(buffer + h,data,len);
        head.store((h + len) % RING_SIZE,std::memory_order_release);
        return true;
    }

    size_t pop(char* out_data) {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire); // 核心：必须看见生产者最新的数据

        if (h == t) return 0; // 队列为空

        size_t len = (h > t) ? (h - t) : (1024 - t); 
        std::memcpy(out_data, buffer + t, len);

        tail.store((t + len) % 1024, std::memory_order_release); // 确保读完数据才更新tail
        return len;
    }
};

RingBuffer rb;



int main(){
    std::cout << "== 量化行情转发服务器启动！（UDP组播 + LOCK—Free Ring Buffer）==\n";

    std::thread consumer([&]() {
        char local_buf[2048];
        std::cout << "[Consumer] 消费者线程已就绪，等待行情..." << std::endl;
        while (true) {
            size_t n = rb.pop(local_buf);
            if (n > 0) {
                // 刺破真相：这里就是你的策略逻辑入口
                std::cout << ">>> 消费到行情包 " << n << " 字节" << std::endl;
            } else {
                // 避免 100% 占用 CPU，实际 HFT 会用忙等或 yield
                std::this_thread::yield(); 
            }
        }
    });

    //UDP socket （接受行情组播）
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock,(struct sockaddr*)&addr,sizeof(addr));

    //epoll
    int epfd = epoll_create1(0);
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sock;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev);

    std::cout << "行情接收端口 " << PORT << " 已就绪，等待组播数据...\n";

        

    while(true){

        int nfds = epoll_wait(epfd,events,MAX_EVENTS,-1);

        for(int i = 0;i < nfds;++i) {
            if(events[i].data.fd == sock){
                char buf[2048];
                ssize_t n = read(sock,buf,sizeof(buf));
                if(n>0){
                    if(rb.push(buf,n)){
                        std::cout << "收到行情包 " << n << " 字节，已放入无锁队列\n";
                    
                    } else {
                        std::cout << "队列满！丢包！\n";
                    }
                }
            }
        }
        
    
    
    
    }

   consumer.join(); 
    

return 0;
}

