#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_t thread_1, thread_2;

typedef struct {
    char hoTen [50],
         namSinh [10],
         sdt [20],
         queQuan [50];
} thr_data_t;

static void *thr_handle (void *args) {

    pthread_t tid = pthread_self (); // lấy pthread id băng hàm self
    thr_data_t *human = (thr_data_t *)args;

    if (pthread_equal (tid, thread_1)) { //ktra nếu thread hiện tại là thread 1
        printf ("Thread 1 dayyy\n");
    } else { //thread 2
        printf ("%s \t %s \t %s \t %s \n", human -> hoTen, human -> namSinh, human -> sdt, human -> queQuan);
    }
}

int main (int argc, char const *argv[]) {
    int ret;
    thr_data_t human = {0};

    //truyền thông tin vào struct
    strncpy (human.hoTen,   "Nguyen Van Khanh", sizeof (human.hoTen));
    strncpy (human.namSinh, "1999",             sizeof (human.namSinh));
    strncpy (human.sdt,     "123456",           sizeof (human.sdt));
    strncpy (human.queQuan, "QN",               sizeof (human.queQuan));

    if (ret = pthread_create (&thread_1, NULL, &thr_handle, NULL)) { 
        //tạo thread trỏ tới handle
        printf("create () error number = %d\n", ret);
        return -1;
    }
    if (ret = pthread_create (&thread_2, NULL, &thr_handle, &human)) { 
        //tạo thread trỏ tới handle
        printf("create () error number = %d\n", ret);
        return -1;    
    }
    sleep (5); //tại sao ko có cái này thì lại không in được ra màn hình nhỉ
    return 0;
}