#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/epoll.h>

#define SEM_NAME "/semi"
#define FILE_NAME "output.txt"

int main() {
    sem_t *sem;
    int fd;

    // Создание/открытие именованного семафора
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Создание/открытие файла
    fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("open");
        sem_close(sem);
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }

    // Создание экземпляра epoll
    int epoll_fd = epoll_create(1);
    if (epoll_fd == -1) {
        perror("epoll_create");
        close(fd);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        close(epoll_fd);
        close(fd);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }

    // Основной цикл программы
    while (1) {
        if (sem_wait(sem)) {
            perror("sem_wait");
            break;
        }

        // запись в файл и вывод на экран
        for (int i = 0; i < 10; i++) {
            write(fd, "1", 1);
            printf("1");
            fflush(stdout); 
            sleep(1);
        }

        if (sem_post(sem)) {
            perror("sem_post");
            break;
        }
        sleep(1);

        // Проверка нажатия Enter
        struct epoll_event events[1];
        int num_events = epoll_wait(epoll_fd, events, 1, 0);  
        if (num_events == -1) {
            perror("epoll_wait");
            break;
        } else if (num_events > 0) {
            if (events[0].data.fd == STDIN_FILENO) {
                break;  // Выход из цикла
            }
        }
    }

    close(fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    close(epoll_fd);

    return 0;
}
