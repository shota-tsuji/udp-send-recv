#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sock1;
    struct sockaddr_in addr1;
    struct pollfd fds[1];
    char buf[2048];

    // socket の設定をする
    sock1 = socket(AF_INET, SOCK_DGRAM, 0);

    // ポートとアドレスの設定をする
    addr1.sin_family = AF_INET;
    
    inet_pton(AF_INET, "127.0.0.1", &addr1.sin_addr.s_addr);

    addr1.sin_port = htons(12345);

    // 「ポートとアドレスの設定」と socket の紐付けをする
    bind(sock1, (struct sockaddr *)&addr1, sizeof(addr1));

    memset(&fds, 0, sizeof(fds));

    // poll の対象に含めるソケットを追加する
    fds[0].fd = sock1;
    fds[0].events = POLLIN | POLLERR;

    // 読み込み可能になるまで待つ、poll を用いた処理をループさせる
    while (1) {
        // block until an event occures.
        //poll(fds, 1, -1);
        poll(fds, 1, 500);

        if (fds[0].revents & POLLIN) {
            // 受け取ったデータを表示する
            memset(buf, 0, sizeof(buf));
            recv(sock1, buf, sizeof(buf), 0);
            printf("%s\n", buf);
        }

        puts("next poll loop");
    }

    // Does not reach here.
    close(sock1);

    return 0;
}
