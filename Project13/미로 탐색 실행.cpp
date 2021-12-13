#define _CRT_SECURE_NO_WARNINGS
#include "LinkedStack.h" //미로 탐색 실행.cpp
#include "LinkedQueue.h"
#include "RandomMaze.h"
#define MAXCAP 100

static string map[MAXCAP];
static string savedStackMap[MAXCAP];
static int MAZE_HEIGHT;
static int MAZE_WIDTH;
static Node* entry = nullptr;
//큐를 이용해 모든 경로 저장
static LinkedQueue *stackPath = nullptr;
static LinkedQueue* queuePath = nullptr;

//함수 선언. 정의는 main 뒤에 있음.
bool isValidLoc(int r, int c);
void printMap();
void GetMap(int ran);
void StackMaze(int ran);
void QueueMaze(int ran);
void StackMapSave();
void PrintSavedStackMap();

void main()
{
    int a=0, ran=0;
    string str;
    do
    {
        printf("1. 파일 입출력 이용, 2. 랜덤 미로, 3. 종료\n");
        printf("입력 : ");
        cin >> str;
        system("cls");
        try {
            a = stoi(str);
        }
        catch (exception e)
        {
            printf("올바른 숫자를 입력해주세요\n");
            continue;
        }
        if (a == 1) ran = 1;
        else if (a == 2) 
        {
            ran = 2;
            MakeRandom();
        }
        else if (a == 3) break;
        do
        {
            printf("1. 스택&큐 미로탐색, 2. 스택 미로탐색(깊이 우선), 3. 큐 미로탐색(너비 우선), 4. 뒤로\n");
            printf("입력 : ");
            cin >> str;
            system("cls");
            try {
                a = stoi(str);
            }
            catch (exception e)
            {
                printf("올바른 숫자를 입력해주세요\n");
            }
            switch (a)
            {
            case 1:
                StackMaze(ran);
                QueueMaze(ran);
                PrintSavedStackMap();
                printf("\n스택 미로 탐색 정보\n");
                printf("\n스택 모든 경로: "); stackPath->display();
                printf("\n");
                delete stackPath;
                delete queuePath;
                stackPath = nullptr;
                queuePath = nullptr;
                break;
            case 2:
                StackMaze(ran); 
                delete stackPath;
                stackPath = nullptr;
                break;
            case 3:
                QueueMaze(ran);
                delete queuePath;
                queuePath = nullptr;
                break;
            case 4:
                break;
            default:
                cout << "올바른 숫자를 입력해주세요" << endl;
            }
        } while (a != 4);
    } while (1);
}

//함수 정의 부분
void GetMap(int ran)
{
    FILE* getting;
    if(ran == 1)
        getting = fopen("input.txt", "r"); // input.txt로 부터 getting 변수에 읽어들임.
    else
        getting = fopen("randomMaze.txt", "r");
    char temp[100];// 받아온 한줄을 임시로 받는 변수
    for (int i = 0;i < MAXCAP;i++)      // 최대용량까지 반복문 루프
    {
        if (!fgets(temp, 100, getting))     // 받아온 줄이 공백이면,
        {
            MAZE_HEIGHT = i;                // i 가 미로의 세로길이
            break;                          // 루프 탈출.
        }
        else map[i] = temp;                // 받아온 줄이 내용이 있다면, map의 i줄에 저장.
    }
        MAZE_WIDTH = map[0].size();         // 루프를 탈출하면, 첫번째 스트링의 크기가 미로의 가로길이.(미로는 사각형 이므로 몇번째 스트링으로 할진 무관함.)
        fclose(getting);
}

bool isValidLoc(int r, int c)
{
    if (r < 0 || c < 0 || r >= MAZE_HEIGHT || c >= MAZE_WIDTH) return false; //배열 범위 밖의 미로 걸러냄. false 반환
    else return map[r][c] == '0' || map[r][c] == 'x'; // 길이나 출구일 때 true 반환. 벽이면 false 반환
}

void printMap()
{
    printf("엔터를 입력해주세요.");
    if (cin.get() == '\n')
        system("cls");
    for (int i = 0; i < MAZE_HEIGHT; i++)
    {
        for (int j = 0; j < MAZE_WIDTH; j++)
        {
            if (map[i][j] == '1') printf("■"); // 벽
            else if (map[i][j] == '0') printf("□"); // 길
            else if (map[i][j] == 'e' || (i == entry->getRow() && j == entry->getCol())) // 입구
            {
                printf("♨");
                // 입구 위치 저장
                if (map[i][j] == 'e')
                {
                    entry->setRow(i);
                    entry->setCol(j);
                }               
            }
            else if (map[i][j] == 'x') printf("♬"); // 출구
            else if (map[i][j] == '.') printf("♩"); // 지나온 경로
        }
        printf("\n");
    } 
}

void StackMaze(int ran) // 스택 미로 탐색 시작
{
    stackPath = new LinkedQueue();
    entry = new Node(0, 0);
    GetMap(ran); // input.txt로부터 미로 불러오기
    
    int stackSuccess = 0; // 스택 미로 탐색 성공 여부 확인용
    printMap(); // 맵 출력, 동시에 입구 위치 기억
    LinkedStack locStack;
    locStack.push(entry); // 스택에 입구 위치 집어넣음.
    
    while (locStack.isEmpty() == false)
    {
        Node* here = locStack.pop(); // 현재 위치 노드 불러옴

        int r = here->getRow();
        int c = here->getCol();
        stackPath->enqueue(new Node(r,c)); // 현재 위치 저장
        if (map[r][c] == 'x') // 현재 위치가 출구이면 성공
        {
            printMap();
            printf("\n스택 미로 탐색 성공\n\n");
            printf("스택 모든 경로: "); stackPath->display();
            printf("\n");
            stackSuccess = 1;
            break;
        }
        else
        {
            map[r][c] = '.'; // 현재 위치를 "지나옴" 처리
            // 현재 위치에서 상하좌우 중 갈 수 있는 모든 경로를 스택에 추가
            if (isValidLoc(r - 1, c)) locStack.push(new Node(r - 1, c)); // 상
            if (isValidLoc(r + 1, c)) locStack.push(new Node(r + 1, c)); // 하
            if (isValidLoc(r, c - 1)) locStack.push(new Node(r, c - 1)); // 좌
            if (isValidLoc(r, c + 1)) locStack.push(new Node(r, c + 1)); // 우
        }
            printMap();
    }        
    if (stackSuccess == 0)
        printf("\n스택 미로 탐색 실패\n\n");
    // 스택 & 큐 동시 진행할 때, 큐가 끝나고 스택 맵을 다시 출력해주기 위해 저장
    StackMapSave();
    savedStackMap[entry->getRow()][entry->getCol()] = 'e'; // entry 노드가 삭제되므로 맵 직접 변경
    delete entry;
}

void QueueMaze(int ran) // 큐 미로 탐색 시작
{
    queuePath = new LinkedQueue();
    entry = new Node(0, 0);
    GetMap(ran);
    int queueSuccess = 0; // 큐 미로 탐색 성공 여부 확인용

    printMap();
    LinkedQueue locQueue;
    locQueue.enqueue(entry);

    while (locQueue.isEmpty() == false)
    {
        Node* here = locQueue.dequeue(); // 현재 위치 노드 불러옴

        int r = here->getRow();
        int c = here->getCol();
        queuePath->enqueue(new Node(r, c)); // 현재 위치 저장
        if (map[r][c] == 'x') // 현재 위치가 출구이면 성공 
        {
            printMap();
            printf("\n큐 미로 탐색 성공\n\n");
            printf("큐 모든 경로: "); queuePath->display();
            printf("\n");
            queueSuccess = 1;
            break;
        }
        else {
            map[r][c] = '.'; // 현재 위치를 "지나옴" 처리
            // 현재 위치에서 상하좌우 중 갈 수 있는 모든 경로를 큐에 추가
            if (isValidLoc(r - 1, c)) locQueue.enqueue(new Node(r - 1, c)); // 상
            if (isValidLoc(r + 1, c)) locQueue.enqueue(new Node(r + 1, c)); // 하
            if (isValidLoc(r, c - 1)) locQueue.enqueue(new Node(r, c - 1)); // 좌
            if (isValidLoc(r, c + 1)) locQueue.enqueue(new Node(r, c + 1)); // 우
        }
            printMap();
    }
    if (queueSuccess == 0)
        printf("\n큐 미로 탐색 실패\n\n");
    delete entry;
}

// 스택 맵 저장

void StackMapSave()
{
    for (int i = 0; i < MAXCAP; i++)
    {
        if(map[i] != "")
            savedStackMap[i] = map[i];
    }
}

void PrintSavedStackMap()
{
    for (int i = 0; i < MAZE_HEIGHT; i++)
    {
        for (int j = 0; j < MAZE_WIDTH; j++)
        {
            if (savedStackMap[i][j] == '1') printf("■"); // 벽
            else if (savedStackMap[i][j] == '0') printf("□"); // 길
            else if (savedStackMap[i][j] == 'e') // 입구
            {
                printf("♨");
            }
            else if (savedStackMap[i][j] == 'x') printf("♬"); // 출구
            else if (savedStackMap[i][j] == '.') printf("♩"); // 지나온 경로
        }
        printf("\n");
    }
}

