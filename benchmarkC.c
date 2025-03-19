#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>

// Message box updates
LPCWSTR benchmarkUpdate = L"Benchmark Update";
LPCWSTR arithFinished = L"Finished arithmetic operations benchmark!";
LPCWSTR logicFinished = L"Finished logic operations benchmark!";
LPCWSTR floatingPointFinished = L"Finished floating-point operations benchmark!";
LPCWSTR dataTransferFinished = L"Finished data transfer speed benchmark!";
LPCWSTR ramBenchmarkFinished = L"Finished RAM speed benchmark!";
static int benchmarkNumber; // Track benchmark runs
int FloatScore;
int DataScore;
int RAMScore;

// Sieve of Eratosthenes algorithm
void sieveOfEratosthenes(int limit, int* primeCount) {
    bool* isPrime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!isPrime) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i <= limit; i++) {
        isPrime[i] = true;
    }
    isPrime[0] = isPrime[1] = false;

    for (int p = 2; p * p <= limit; ++p) {
        if (isPrime[p]) {
            for (int i = p * p; i <= limit; i += p) {
                isPrime[i] = false;
            }
        }
    }

    *primeCount = 0;
    for (int i = 0; i <= limit; ++i) {
        if (isPrime[i]) {
            (*primeCount)++;
        }
    }

    free(isPrime);
}

// XOR-based cipher for encryption and decryption
void xorCipher(const char* inputFile, const char* outputFile, const char* key) {
    FILE* inFile = fopen(inputFile, "rb"); // Open in binary mode to handle potential binary data
    FILE* outFile = fopen(outputFile, "wb");

    if (!inFile || !outFile) {
        perror("Error opening files");
        exit(1);
    }

    size_t keyLength = strlen(key);
    int c;
    size_t keyIndex = 0;

    while ((c = fgetc(inFile)) != EOF) {
        c ^= key[keyIndex % keyLength];
        fputc(c, outFile);
        keyIndex++;
    }

    fclose(inFile);
    fclose(outFile);
}

int increment_number_in_file(const char* filename) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return -1; 
    }

    // Read the number
    int number;
    if (fscanf(file, "%d", &number) != 1) {
        perror("Error reading number from file");
        fclose(file);
        return -1; 
    }

    // Increment the number
    number++;
    benchmarkNumber = number;

    rewind(file);

    // Write the incremented number back to the file
    if (fprintf(file, "%d", number) < 0) {
        perror("Error writing number to file");
        fclose(file);
        return -1; 
    }

    fclose(file);
    return 0; 
}



void floatingPointOperations(FILE* outputFile, int iterations, double baselineTimeSeconds, int baselineScore) {
    double totalTime = 0.0; // Total average time across all tests

    fprintf(outputFile, "Floating Point Operations Benchmark:\n");
    printf("Starting Floating Point Operations Benchmark...\n");

    for (int i = 1; i <= iterations; i++) {
        double additionTime = 0, subtractionTime = 0, multiplicationTime = 0, divisionTime = 0;

        // Addition Benchmark
        clock_t start = clock();
        double a = 0.0;
        for (int j = 0; j < 100000000; j++) {
            a += 1.1;
        }
        clock_t end = clock();
        additionTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Subtraction Benchmark
        start = clock();
        for (int j = 0; j < 100000000; j++) {
            a -= 1.1;
        }
        end = clock();
        subtractionTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Multiplication Benchmark
        start = clock();
        for (int j = 0; j < 100000000; j++) {
            a *= 1.1;
        }
        end = clock();
        multiplicationTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Division Benchmark
        start = clock();
        for (int j = 0; j < 100000000; j++) {
            a /= 1.1;
        }
        end = clock();
        divisionTime = (double)(end - start) / CLOCKS_PER_SEC;

        // Average time for this test
        double averageTime = (additionTime + subtractionTime + multiplicationTime + divisionTime) / 4.0;
        totalTime += averageTime;

        // Log results for this test
        fprintf(outputFile, "Test %d\n", i);
        fprintf(outputFile, "Addition: %.2f seconds\n", additionTime);
        fprintf(outputFile, "Subtraction: %.2f seconds\n", subtractionTime);
        fprintf(outputFile, "Multiplication: %.2f seconds\n", multiplicationTime);
        fprintf(outputFile, "Division: %.2f seconds\n", divisionTime);
        fprintf(outputFile, "Test %d Average: %.2f seconds\n", i, averageTime);
        printf("Floating Point Test %d completed.\n", i);
    }

    // Calculate the final score for the Floating Point Benchmark
    double finalAverageTime = totalTime / iterations;
    double performanceRatio = baselineTimeSeconds / finalAverageTime;
    int floatingPointScore = (int)(performanceRatio * baselineScore);
    FloatScore = floatingPointScore;

    // Log final results
    fprintf(outputFile, "Average Time Across All Tests: %.2f seconds\n", finalAverageTime);
    fprintf(outputFile, "Floating Point Benchmark Score: %d\n", floatingPointScore);

    printf("Floating Point Operations Benchmark Finished!\n");
    printf("Final Average Time: %.2f seconds\n", finalAverageTime);
    printf("Final Score: %d\n", floatingPointScore);

    // Notify user of completion
    MessageBox(0, floatingPointFinished, benchmarkUpdate, MB_ICONINFORMATION);
}

// Data Transfer Speed Benchmark Function
void dataTransferBenchmark(FILE* outputFile, int iterations, double baselineTimeSeconds, int baselineScore, size_t readFileSize, size_t writeFileSize) {
    double totalReadTime = 0.0, totalWriteTime = 0.0;
    double totalReadSpeed = 0.0, totalWriteSpeed = 0.0;

    const char* readFile = "read_benchmark.txt";
    const char* writeFile = "write_benchmark.txt";

    fprintf(outputFile, "Data Transfer Speed Benchmark:\n");
    printf("Starting Data Transfer Speed Benchmark...\n");


    for (int i = 1; i <= iterations; i++) {
        char* buffer = (char*)malloc(readFileSize);
        if (!buffer) {
            fprintf(stderr, "Error: Unable to allocate memory for buffer.\n");
            exit(1);
        }

        // Read Benchmark
        FILE* readFilePtr = fopen(readFile, "rb");
        if (!readFilePtr) {
            fprintf(stderr, "Error: Unable to open %s for reading.\n", readFile);
            free(buffer);
            exit(1);
        }

        clock_t start = clock();
        fread(buffer, 1, readFileSize, readFilePtr);
        clock_t end = clock();
        fclose(readFilePtr);

        double readTime = (double)(end - start) / CLOCKS_PER_SEC;
        double readSpeed = readFileSize / readTime;
        totalReadTime += readTime;
        totalReadSpeed += readSpeed;

        // Write Benchmark
        FILE* writeFilePtr = fopen(writeFile, "wb");
        if (!writeFilePtr) {
            fprintf(stderr, "Error: Unable to open %s for writing.\n", writeFile);
            free(buffer);
            exit(1);
        }

        start = clock();
        fwrite(buffer, 1, writeFileSize, writeFilePtr);
        end = clock();
        fclose(writeFilePtr);

        double writeTime = (double)(end - start) / CLOCKS_PER_SEC;
        double writeSpeed = writeFileSize / writeTime;
        totalWriteTime += writeTime;
        totalWriteSpeed += writeSpeed;

        // Log the results for this test
        fprintf(outputFile, "Test %d\n", i);
        fprintf(outputFile, "Read Time: %.2f seconds\n", readTime);
        fprintf(outputFile, "Read Speed: %.2f bytes/second\n", readSpeed);
        fprintf(outputFile, "Write Time: %.2f seconds\n", writeTime);
        fprintf(outputFile, "Write Speed: %.2f bytes/second\n", writeSpeed);
        printf("Data Transfer Test %d completed.\n", i);

        free(buffer);
    }

    // Calculate final averages
    double avgReadTime = totalReadTime / iterations;
    double avgReadSpeed = totalReadSpeed / iterations;
    double avgWriteTime = totalWriteTime / iterations;
    double avgWriteSpeed = totalWriteSpeed / iterations;

    // Calculate the final score for the Data Transfer Benchmark
    double performanceRatio = baselineTimeSeconds / (avgReadTime + avgWriteTime);
    int dataTransferScore = (int)(performanceRatio * baselineScore);
    DataScore = dataTransferScore;

    // Log final results
    fprintf(outputFile, "Average Read Time: %.2f seconds\n", avgReadTime);
    fprintf(outputFile, "Average Read Speed: %.2f bytes/second\n", avgReadSpeed);
    fprintf(outputFile, "Average Write Time: %.2f seconds\n", avgWriteTime);
    fprintf(outputFile, "Average Write Speed: %.2f bytes/second\n", avgWriteSpeed);
    fprintf(outputFile, "Data Transfer Benchmark Score: %d\n", dataTransferScore);

    printf("Data Transfer Speed Benchmark Finished!\n");
    printf("Average Read Time: %.2f seconds\n", avgReadTime);
    printf("Average Read Speed: %.2f bytes/second\n", avgReadSpeed);
    printf("Average Write Time: %.2f seconds\n", avgWriteTime);
    printf("Average Write Speed: %.2f bytes/second\n", avgWriteSpeed);
    printf("Final Score: %d\n", dataTransferScore);

    // Notify user of completion
    MessageBox(0, dataTransferFinished, benchmarkUpdate, MB_ICONINFORMATION);
}






// RAM Benchmark Function
void ramSpeedBenchmark(FILE* outputFile, int iterations, double baselineTimeSeconds, int baselineScore) {
    const size_t ramSize = 1 * 1024 * 1024 * 1024; // 1 GB of RAM
    char targetChar = 'A';
    
    double totalWriteTime = 0.0, totalReadTime = 0.0;

    fprintf(outputFile, "RAM Speed Benchmark:\n");
    printf("Starting RAM Speed Benchmark...\n");

    for (int i = 1; i <= iterations; i++) {
        char* memoryBlock = (char*)malloc(ramSize);
        if (!memoryBlock) {
            fprintf(stderr, "Error: Unable to allocate memory for RAM benchmark.\n");
            exit(1);
        }

        // Fill memory with memset
        clock_t start = clock();
        memset(memoryBlock, targetChar, ramSize);
       
        // Search memory with memchr
        volatile void* result = memchr(memoryBlock, targetChar, ramSize);
        clock_t end = clock();

        double writeTime = (double)(end - start) / CLOCKS_PER_SEC;
        totalWriteTime += writeTime;

        free(memoryBlock);

        // Log results for each test
        fprintf(outputFile, "Test %d\n", i);
        fprintf(outputFile, "Taken Time: %.2f seconds\n", writeTime);
        printf("RAM Test %d completed: Taken Time = %.2f s\n", i, writeTime);
    }

    // Calculate averages
    double avgWriteTime = totalWriteTime / iterations;

    // Calculate the final score for RAM Benchmark
    double performanceRatio = baselineTimeSeconds / (avgWriteTime);
    int ramScore = (int)(performanceRatio * baselineScore);
    RAMScore = ramScore;

    // Log final results
    fprintf(outputFile, "Average Taken Time: %.2f seconds\n", avgWriteTime);
    fprintf(outputFile, "RAM Benchmark Score: %d\n", ramScore);

    printf("RAM Speed Benchmark Finished!\n");
    printf("Average Write Time: %.2f seconds\n", avgWriteTime);
    printf("Final Score: %d\n", ramScore);

    // Notify user of completion
    MessageBox(0, ramBenchmarkFinished, benchmarkUpdate, MB_ICONINFORMATION);
}



int main() {
    const int limit = 100000000; // Interval from 1 to 100,000,000
    const int baselineTimeSeconds = 180; // Placeholder until tests are done
    const double baseTimeArith = 3.95f;
    const double baseTimeLogic = 7.73f;
    const double baseTimeFloat = 0.75f;
    const double baseTimeDataT = 0.29f;
    const double baseTimeRAM = 0.09f;
    const int baselineScore =1000;
    const int iterations = 50; 
    const char* inputFile = "logic_benchmark.txt";
    const char* encryptedFile = "encrypted.txt";
    const char* key = "ah27dh5jln3b46fgdl97hasfhjkhjkwrqwhjkerfitr"; // 25-character key
    int aux = increment_number_in_file("benchmark_number.txt");
    const size_t readFileSize = 39000000; 
    const size_t writeFileSize = 39000000; 

    printf("Starting Arithmetic Operations Benchmark...\n");

    FILE* outputFile = fopen("benchmark_results.txt", "a");
    if (!outputFile) {
        fprintf(stderr, "Error: Could not open the results file.\n");
        return 1;
    }

    
    fprintf(outputFile, "#%d\n", benchmarkNumber++);
    fprintf(outputFile, "Arithmetic Operations Benchmark:\n");

    double totalTime = 0.0; // To calculate average time
    for (int i = 1; i <= iterations; i++) {
        clock_t start = clock();

        int primeCount = 0;
        sieveOfEratosthenes(limit, &primeCount);

        clock_t end = clock();
        double durationSeconds = (double)(end - start) / CLOCKS_PER_SEC;
        totalTime += durationSeconds;

        // Log the time for each run
        fprintf(outputFile, "Test %d: %.2f seconds\n", i, durationSeconds);
        printf("Test %d completed: Time Taken: %.2f seconds\n", i, durationSeconds);
    }

    // Calculate average time and final score for Arithmetic Benchmark
    double averageTime = totalTime / iterations;
    double performanceRatio = baseTimeArith / averageTime;
    int arithmeticScore = (int)(performanceRatio * baselineScore);

    // Log final results for Arithmetic Benchmark
    fprintf(outputFile, "Average Time: %.2f seconds\n", averageTime);
    fprintf(outputFile, "Arithmetic Benchmark Score: %d\n", arithmeticScore);

    printf("Arithmetic Operations Benchmark Finished!\n");
    printf("Average Time Taken: %.2f seconds\n", averageTime);
    printf("Final Score: %d\n", arithmeticScore);

    // Notify user of completion
    MessageBox(0, arithFinished, benchmarkUpdate, MB_ICONINFORMATION);

    // Logic Operations Benchmark
    printf("Starting Logic Operations Benchmark...\n");
    fprintf(outputFile, "Logic Operations Benchmark:\n");

    totalTime = 0.0; // Reset total time for logic benchmark
    for (int i = 1; i <= iterations; i++) {
        clock_t start = clock();

        xorCipher(inputFile, encryptedFile, key);

        clock_t end = clock();
        double durationSeconds = (double)(end - start) / CLOCKS_PER_SEC;
        totalTime += durationSeconds;

        //Log the time for each run
        fprintf(outputFile, "Test %d: %.2f seconds\n", i, durationSeconds);
        printf("Logic Test %d completed: Time Taken: %.2f seconds\n", i, durationSeconds);
    }

    // Calculate average time and final score for Logic Benchmark
    averageTime = totalTime / iterations;
    performanceRatio = baseTimeLogic / averageTime;
    int logicScore = (int)(performanceRatio * baselineScore);

    // Log final results for Logic Benchmark
    fprintf(outputFile, "Average Time: %.2f seconds\n", averageTime);
    fprintf(outputFile, "Logic Benchmark Score: %d\n", logicScore);

    printf("Logic Operations Benchmark Finished!\n");
    printf("Average Time Taken: %.2f seconds\n", averageTime);
    printf("Final Score: %d\n", logicScore);

    // Notify user of completion
    MessageBox(0, logicFinished, benchmarkUpdate, MB_ICONINFORMATION);

    //Floating Point Benchmark
    floatingPointOperations(outputFile, iterations, baseTimeFloat, baselineScore);

    // Data Transfer Speed Benchmark
    dataTransferBenchmark(outputFile, iterations, baseTimeDataT, baselineScore, readFileSize, writeFileSize);

    // RAM Benchmark
    ramSpeedBenchmark(outputFile, iterations, baseTimeRAM, baselineScore);

    //Compute final score using geometric mean
    int FINAL_SCORE;
    double auxScore = 1.0f;
    auxScore *= arithmeticScore;
    auxScore *= logicScore;
    auxScore *= FloatScore;
    auxScore *= DataScore;
    auxScore *= RAMScore;

    FINAL_SCORE = (int)round(pow(auxScore, 1.0 / 5));


    fprintf(outputFile, "Final score: %d\n",FINAL_SCORE);
    fclose(outputFile);

    return 0;
}
