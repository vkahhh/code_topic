#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

pthread_cond_t  cond = PTHREAD_COND_INITIALIZER ;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_1, thread_2, thread_3;

typedef struct {
    char hoTen    [30];
    char ngaySinh [30];
    char queQuan  [30];
} thr_data_t;

thr_data_t thr = {0};

static void *thr_handle1 (void *args) {
    
    printf("----- Nhap thong tin sinh vien -----\n");
    //for (int i = 0; i < 3; i++) {
        pthread_mutex_lock (&lock);

        printf("Nhap ten cho sinh vien: ");
        scanf ("%s", thr.hoTen);
        printf("Nhap ngay sinh cho sinh vien: ");
        scanf ("%s", thr.ngaySinh);
        printf("Nhap que quan cho sinh vien: ");
        scanf ("%s", thr.queQuan);

        pthread_cond_signal (&cond);
        pthread_mutex_unlock (&lock);
        pthread_exit (NULL);

//}
}

static void *thr_handle2 (void *args) {
    pthread_mutex_lock(&lock);  
    pthread_cond_wait (&cond, &lock);
   // pthread_join (thread_1, NULL);    

    char *str2 = "\n", *str1 = "\t";
    printf("----- Dang luu thong tin sinh vien -----\n");
    //for (int i = 0; i < 3; i++) {
        int fd = open ("./sv.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
        write (fd, str2, strlen (str2));
        write (fd, thr.hoTen   , strlen (thr.hoTen)   );
        write (fd, str1, strlen (str1));
        write (fd, thr.ngaySinh, strlen (thr.ngaySinh));
        write (fd, str1, strlen (str1));
        write (fd, thr.queQuan , strlen (thr.queQuan) );
        write (fd, str2, strlen (str2));
        pthread_mutex_unlock(&lock); 
        pthread_exit (NULL);
   // }
}

static void *thr_handle3 (void *args) {
    pthread_join (thread_2, NULL);
    //pthread_cond_wait (&cond, &lock);
    printf("----- Dang in thong tin sinh vien -----\n");
    //for (int i = 0; i < 3; i++) {
        execl ("/bin/cat", "cat", "./sv.txt", NULL);
   // }
}

int main (int argc, char const *argv[]) {
    int count = 1;
    pthread_create (&thread_1, NULL, thr_handle1, NULL);
    pthread_create (&thread_2, NULL, thr_handle2, NULL);
    pthread_create (&thread_3, NULL, thr_handle3, NULL);
    pthread_mutex_lock (&lock);
    while (count <= 3) {
        pthread_cond_wait (&cond, &lock);
        pthread_create (&thread_1, NULL, thr_handle1, NULL);
        sleep (1);
        pthread_create (&thread_2, NULL, thr_handle2, NULL);
        //sleep (2);
        pthread_create (&thread_3, NULL, thr_handle3, NULL);
        //sleep (3);
        count ++;
    } 
    pthread_mutex_unlock (&lock);
    return 0;   
    
}