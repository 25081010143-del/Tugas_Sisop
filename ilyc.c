#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Struct untuk mengirim beberapa argumen ke Thread 3
typedef struct {
    char text[256];
    int char_count;
} TextData;

// Shared resource & Mutex untuk mendemonstrasikan sinkronisasi
int global_counter = 0;
pthread_mutex_t lock;

// Thread 1: Menhitung Faktorial
void* task_factorial(void* arg) {
    int n = *(int*)arg;
    long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    printf("[Thread 1 - Faktorial] Hasil dari %d! = %lld\n", n, fact);

    // Mencegah Race Condition pada shared counter
    pthread_mutex_lock(&lock);
    global_counter++;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

// Thread 2: Menampilkan Deret Fibonacci
void* task_fibonacci(void* arg) {
    int n = *(int*)arg;
    printf("[Thread 2 - Fibonacci] %d suku pertama: ", n);
    
    long long a = 0, b = 1, next;
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            printf("%lld", a);
        } else if (i == 1) {
            printf(", %lld", b);
        } else {
            next = a + b;
            a = b;
            b = next;
            printf(", %lld", next);
        }
    }
    printf("\n");

    // Mencegah Race Condition pada shared counter
    pthread_mutex_lock(&lock);
    global_counter++;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

// Thread 3: Menghitung Panjang Kalimat
void* task_count_text(void* arg) {
    TextData* data = (TextData*)arg;
    data->char_count = strlen(data->text);
    printf("[Thread 3 - Teks] Jumlah karakter dalam teks: %d\n", data->char_count);

    // Mencegah Race Condition pada shared counter
    pthread_mutex_lock(&lock);
    global_counter++;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3;
    int n_fact, n_fibo;
    TextData text_data;

    // Inisialisasi Mutex
    pthread_mutex_init(&lock, NULL);

    // Input Manual dari User
    printf("=== INPUT PROGRAM (C) ===\n");
    printf("Masukkan angka untuk Faktorial: ");
    scanf("%d", &n_fact);

    printf("Masukkan jumlah suku Fibonacci: ");
    scanf("%d", &n_fibo);

    getchar(); // Membersihkan newline buffer dari scanf
    printf("Masukkan teks/kalimat: ");
    fgets(text_data.text, sizeof(text_data.text), stdin);
    text_data.text[strcspn(text_data.text, "\n")] = 0; // Hapus newline

    printf("\n=== MEMULAI THREAD ===\n");

    // Membuat ketiga thread
    pthread_create(&t1, NULL, task_factorial, &n_fact);
    pthread_create(&t2, NULL, task_fibonacci, &n_fibo);
    pthread_create(&t3, NULL, task_count_text, &text_data);

    // Menunggu seluruh thread selesai (Join)
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("\n[Main Thread] Total task selesai: %d\n", global_counter);

    // Destruksi Mutex
    pthread_mutex_destroy(&lock);

    return 0;
}