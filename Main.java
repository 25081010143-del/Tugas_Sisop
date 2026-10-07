import java.util.Scanner;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    // Shared resource & Lock untuk mendemonstrasikan sinkronisasi
    private static int globalCounter = 0;
    private static final ReentrantLock lock = new ReentrantLock();

    // Task 1: Faktorial
    static class FactorialTask implements Runnable {
        private final int n;

        public FactorialTask(int n) {
            this.n = n;
        }

        @Override
        public void run() {
            long fact = 1;
            for (int i = 1; i <= n; i++) {
                fact *= i;
            }
            System.out.println("[Thread 1 - Faktorial] Hasil dari " + n + "! = " + fact);

            // Mencegah Race Condition pada shared counter
            lock.lock();
            try {
                globalCounter++;
            } finally {
                lock.unlock();
            }
        }
    }

    // Task 2: Fibonacci
    static class FibonacciTask implements Runnable {
        private final int n;

        public FibonacciTask(int n) {
            this.n = n;
        }

        @Override
        public void run() {
            StringBuilder sb = new StringBuilder();
            sb.append("[Thread 2 - Fibonacci] ").append(n).append(" suku pertama: ");
            
            long a = 0, b = 1, next;
            for (int i = 0; i < n; i++) {
                if (i == 0) {
                    sb.append(a);
                } else if (i == 1) {
                    sb.append(", ").append(b);
                } else {
                    next = a + b;
                    a = b;
                    b = next;
                    sb.append(", ").append(next);
                }
            }
            System.out.println(sb.toString());

            // Mencegah Race Condition pada shared counter
            lock.lock();
            try {
                globalCounter++;
            } finally {
                lock.unlock();
            }
        }
    }

    // Task 3: Menghitung Panjang Kalimat
    static class TextCountTask implements Runnable {
        private final String text;

        public TextCountTask(String text) {
            this.text = text;
        }

        @Override
        public void run() {
            int charCount = text.length();
            System.out.println("[Thread 3 - Teks] Jumlah karakter dalam teks: " + charCount);

            // Mencegah Race Condition pada shared counter
            lock.lock();
            try {
                globalCounter++;
            } finally {
                lock.unlock();
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Scanner scanner = new Scanner(System.in);

        // Input Manual dari User
        System.out.println("=== INPUT PROGRAM (Java) ===");
        System.out.print("Masukkan angka untuk Faktorial: ");
        int nFact = scanner.nextInt();

        System.out.print("Masukkan jumlah suku Fibonacci: ");
        int nFibo = scanner.nextInt();

        scanner.nextLine(); // Membersihkan newline buffer
        System.out.print("Masukkan teks/kalimat: ");
        String textInput = scanner.nextLine();

        System.out.println("\n=== MEMULAI THREAD ===");

        // Inisialisasi Thread
        Thread t1 = new Thread(new FactorialTask(nFact));
        Thread t2 = new Thread(new FibonacciTask(nFibo));
        Thread t3 = new Thread(new TextCountTask(textInput));

        // Memulai ketiga thread
        t1.start();
        t2.start();
        t3.start();

        // Menunggu seluruh thread selesai (Join)
        t1.join();
        t2.join();
        t3.join();

        System.out.println("\n[Main Thread] Total task selesai: " + globalCounter);
        scanner.close();
    }
}