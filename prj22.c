#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

sem_t sem; // 세마포어 선언
void *multi_thread(void *count_circle) 
{
    double *args = (double*)count_circle;
    int i = 0;
    unsigned int seed = time(NULL)+(unsigned int)pthread_self(); 
    double x = 0;
    double y = 0;
    double circle=0,count=0;
    // 각 스레드마다 250,000번 반복하며 랜덤 좌표 생성 및 원 안에 있는지 확인
     for(i = 0; i <250000; i++) {
            x = (double)rand_r(&seed)/(double)RAND_MAX;
            y = (double)rand_r(&seed)/(double)RAND_MAX;
            count++; // 전체 점의 수 증가
            if((x*x) + (y*y) <= 1){
                circle++; // 원 안에 있는 점의 수 증가
            }
        }

    // 세마포어를 사용하여 공유 데이터에 접근
    sem_wait(&sem);
    args[0] += count;
    args[1] += circle;
    sem_post(&sem);
    
        return NULL;
}


int main()
{
    pthread_t pthread[3]; // 스레드 배열
    int thr_id;
    char *p[] = {"thread_1","thread_2","thread_3","main_thread"};

    // 전체 결과를 집계하기 위한(점의 개수와 범위 내부에 찍힌 점의 수)를 저장할 수 있는 메모리 공간 할당
    double *count_circle = (double*)malloc(2 * sizeof(double));
    
    // 메모리 할당 실패 시 예외처리
    if (count_circle == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    // 메모리 할당이 성공했으면, 값을 할당
    count_circle[0] = 0; // 전체 점의 수
    count_circle[1] = 0; // 원 안에 있는 점의 수

    // 세마포어 초기화
    if (sem_init(&sem, 0, 1) < 0) {
                return -1;
        }

    // 스레드 생성
    for(int i = 0; i < (sizeof(pthread)/sizeof(pthread[0])); i++){
    thr_id = pthread_create(&pthread[i], NULL, multi_thread, (void*)count_circle); //2
    if(thr_id < 0){
        printf("Thread create Error, Thread id: %s",p[i]);
        exit(EXIT_FAILURE);
        }
    }

    // 메인 스레드에서도 동일한 작업 수행
    multi_thread((void *)count_circle);

    // 모든 스레드가 종료될 때까지 대기
    for(int i = 0; i < (sizeof(pthread)/sizeof(pthread[0])); i++) {
        pthread_join(pthread[i], NULL);
    }

    // 결과 출력
    printf("n = %0.0f, pi = %0.15f\n",count_circle[0],(count_circle[1]/count_circle[0]) *4);
    free(count_circle); //동적 메모리 해제
    printf("finish multi thread processing\n");
    sem_destroy(&sem); // 세마포어 제거
    return 0;
}
