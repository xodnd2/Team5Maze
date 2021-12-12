#define _CRT_SECURE_NO_WARNINGS
#include "LinkedStack.h" //�̷� Ž�� ����.cpp
#include "LinkedQueue.h"

class MazeEscape {
	string Map[100];
	int x_size, y_size;
	int x, y = 0;
	int Counting;
public:
	void GetMap() 
	{
		FILE* in = fopen("input.txt", "r");
		char temp2[20];
		for (int i = 0;i < 100;i++)
		{
			if (fgets(temp2, 20, in) == 0)break;
			else Map[i] = temp2;
			x_size = Map[i].size();
			y_size = i + 1;
		}
	}
	void PrintMap()
	{
		for (int i = 0; i < y_size; i++)
		{
			for (int j = 0; j < x_size; j++)
			{
				if (Map[i][j] == '1') printf("��"); // ��
				else if (Map[i][j] == '0') printf("��"); // ��
				else if (Map[i][j] == 'e') // �Ա�
				{
					printf("��");
				}
				else if (Map[i][j] == 'x') printf("��"); // �ⱸ
				else if (Map[i][j] > 1) 
				{
					printf("%2d", Map[i][j] - 1);
					Counting = Map[i][j] - 1;
				}
			}
			printf("\n");
		}
	}
	Node* FindEnter()
	{
		Node* entry = new Node(0, 0);
		for (int i = 0;i < y_size;i++)
		{
			if (i == 0 && i == y_size - 1)
			{
				for (int j = 0;j < x_size;j++)
				{
					if (Map[i][j] == 'e')return entry = new Node(i, j);
				}
			}
			else
			{
				if (Map[i][0] == 'e')return entry = new Node(i, 0);
				if (Map[i][x_size - 1] == 'e')return entry = new Node(i, x_size - 1);
			}
		}
	}
	bool isValidLoc(int r, int c)
	{
		if (r < 0 || c < 0 || r >= y_size || c >= x_size) return false; //�迭 ���� ���� �̷� �ɷ���. false ��ȯ
		else return Map[r][c] == '0' || Map[r][c] == 'x'; // ���̳� �ⱸ�� �� true ��ȯ. ���̸� false ��ȯ
	}
	void EscByDfs()
	{
		int cnt = 2;
		LinkedStack Ls;
		Ls.push(FindEnter());
		while (!Ls.isEmpty())
		{
			Node* Pos = Ls.pop();
			int x = Pos->getCol();
			int y = Pos->getRow();
			printf("(%d,%d) ", x, y);
		
			if (Map[y][x] == 'x') // ���� ��ġ�� �ⱸ�̸� ����-����
			{
				printf("\n���� �̷� Ž�� ����\n\n");
				Map[y][x] = cnt++;
				break;
			}
			else {
				Map[y][x] = cnt++;
				// ���� ��ġ���� �����¿� �� �� �� �ִ� ��� ��θ� ���ÿ� �߰�
				if (isValidLoc(y - 1, x)) Ls.push(new Node(y - 1, x)); // ��
				if (isValidLoc(y + 1, x)) Ls.push(new Node(y + 1, x)); // ��
				if (isValidLoc(y, x - 1)) Ls.push(new Node(y, x - 1)); // ��
				if (isValidLoc(y, x + 1)) Ls.push(new Node(y, x + 1)); // ��
			}
		}
	}
	void EscByBfs()
	{
		int cnt = 2;
		LinkedQueue Qs;
		Qs.enqueue(FindEnter());
		while (!Qs.isEmpty())
		{
			Node* Pos = Qs.dequeue();
			int x = Pos->getCol();
			int y = Pos->getRow();
			printf("(%d,%d) ", x, y);

			if (Map[y][x] == 'x') // ���� ��ġ�� �ⱸ�̸� ����-����
			{
				printf("\nť �̷� Ž�� ����\n\n");
				Map[y][x] = cnt++;
				break;
			}
			else {
				Map[y][x] = cnt++;
				// ���� ��ġ���� �����¿� �� �� �� �ִ� ��� ��θ� ���ÿ� �߰�
				if (isValidLoc(y - 1, x)) Qs.enqueue(new Node(y - 1, x)); // ��
				if (isValidLoc(y + 1, x)) Qs.enqueue(new Node(y + 1, x)); // ��
				if (isValidLoc(y, x - 1)) Qs.enqueue(new Node(y, x - 1)); // ��
				if (isValidLoc(y, x + 1)) Qs.enqueue(new Node(y, x + 1)); // ��
			}
		}
	}
	int GetCounting()
	{
		return Counting;
	}
};
int main(void)
{
	MazeEscape st1,Q1;
	st1.GetMap();
	st1.EscByDfs();
	st1.PrintMap();

	Q1.GetMap();
	Q1.EscByBfs();
	Q1.PrintMap();
	if (st1.GetCounting() < Q1.GetCounting())
	{
		printf("\nDFS(STACK)�� �� ȿ�����̴�.\n");
	}
	else
	{
		printf("\nBFS(QUEUE)�� �� ȿ�����̴�.");
	}
}