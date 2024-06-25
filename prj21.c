#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    // 점의 총 개수와 범위 내부에 찍힌 점의 수를 저장할 수 있는 메모리 공간 할당
    double *count_circle = (double*)malloc(2 * sizeof(double));
    
    // 메모리 할당 실패 시 예외처리
    if (count_circle == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }

    // 메모리 할당이 성공했으면, 값을 할당
    count_circle[0] = 0;  // 전체 점의 수
    count_circle[1] = 0;  // 원 안에 있는 점의 수

    int i = 0;
    unsigned int seed = time(NULL)+(unsigned int)pthread_self(); 
    double x = 0;
    double y = 0;
    
    // 1,000,000번 반복하며 랜덤 좌표 생성 및 원 안에 있는지 확인
     for(i = 0; i <1000000; i++) {
            x = (double)rand_r(&seed)/(double)RAND_MAX;
            y = (double)rand_r(&seed)/(double)RAND_MAX;
            count_circle[0]++; // 전체 점의 수 증가
            if((x*x) + (y*y) <= 1){
                count_circle[1]++; // 원 안에 있는 점의 수 증가
            }
        }

    // 결과 출력
    printf("n = %0.0f, pi = %0.15f\n",count_circle[0],(count_circle[1]/count_circle[0]) *4);
    // 동적 메모리 해제
    free(count_circle);
    printf("finish single thread processing\n");
    return 0;
}
