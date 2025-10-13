#include<iostream>
#include<easyx.h>  //ͼ�ο�
#include<vector>	//˳���
#include<ctime>
/*������(����)*/
class Sprite
{
public:
	Sprite():Sprite(270,190) {};
	Sprite(int x,int y):m_x(x),m_y(y),m_color(RED) {};

	virtual void draw()  //�麯����Ϊ�˶�̬��������һ������
	{
		//������ɫ
		setfillcolor(m_color);
		//���ƾ���
		fillrectangle(m_x,m_y,m_x+10,m_y+10);
	}//���ƾ���
	void moveBy(int dx, int dy)
	{
		m_x += dx;
		m_y += dy;
	}//�ƶ�
	bool collision(const Sprite& other)
	{
		return m_x == other.m_x && m_y == other.m_y;
	}


protected:
	int m_x;
	int m_y;
	COLORREF m_color;  //��ɫ
};
/*����(�̳о�����)*/     
class Snake :public Sprite
{
public:
	Snake() :Snake(0, 0) {}
	Snake(int x, int y) :Sprite(x, y),dir(VK_RIGHT)
	{
		
		nodes.push_back(Sprite(20,0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}//��ʼ���ߵ�����
	bool collision(const Sprite& other)
	{
		return nodes[0].collision(other);
	}//���ͷ�Ƿ�������������ײ
	void draw() override
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		}
	}	//����������麯��  
	void bodyMove()
	{
		for (size_t i = nodes.size() - 1 ; i > 0; i-- )		//i  == 3-1 ����������˼ ������������˼ ������3�ǽڵ������
		{
			nodes[i] = nodes[i - 1]; //�����ÿһ�ڽڵ���ں�һ�ڽڵ�
		}
		switch (dir) 
		{
		case VK_UP:
			nodes[0].moveBy(0, -10);//��ͷ�����ƶ�
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);//��ͷ�����ƶ�
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0); //��ͷ�����ƶ�
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0); //��ͷ�����ƶ�
			break;
		}
	}/*�ߵ�������ƶ�*/
	void bodyadd()
	{
		nodes.push_back(Sprite());
	}//�ߵ���������
	bool collisionself()
	{
		for (size_t i = 1; i < nodes.size(); ++i)
		{
			if (nodes[0].collision(nodes[i]))
				return true; // ��ͷײ������
		}
		return false;
	}//�����ͷ�Ƿ�ײ���Լ�������
private:
	//���ж������
	std::vector<Sprite> nodes; //�ߵ����нڵ�
public:
	int dir;//�ߵķ���
};
/*ʳ����(�̳о�����)*/

class Food : public Sprite
{
public:
	Food() :Sprite(0, 0)
	{
		chagePos();
	}
	void draw() override
	{
		setfillcolor(GREEN);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);  //����Բ��ʳ��
	}
	void chagePos()
	{
		m_x = rand() % 64 * 10;//�����������	
		m_y = rand() % 48 * 10;
	}//�ı�ʳ�������
};
/*��Ϸ�ĳ���*/
class GameScense 
{
public:
	GameScense() {};
	void run() 
	{
		
		BeginBatchDraw();//˫�����ͼ
		cleardevice();//��������ֹ���ɵ���һֱͣ����ԭ�أ�Ȼ����������µ���ͷ
		snake.draw();//����
		food.draw();//��ʳ��
		eatFood();//��ʳ��
		EndBatchDraw();//˫�����ͼ
		snake.bodyMove();//�ƶ��ߣ��ı��ߵ�����
		gameover();//��Ϸ����
		
		ExMessage msg = { 0 };
		while (peekmessage(&msg, PP_SYM_KEYSIZE))//��ȡ��Ϣ
		{
			onMsg(msg);
		}
	}; 
	void onMsg(const ExMessage& msg)		//�ƶ��ߵķ��� ��ȡ���̰��� ����getch����
	{
	//���ް�������
		if (msg.message == WM_KEYDOWN)
		{
			//�жϰ��µ����ĸ���  vkcode  == virtual key code  �������
			
			switch (msg.vkcode)
			{
			case VK_UP:
				if (snake.dir != VK_DOWN)
					snake.dir = msg.vkcode;
				break;
			case VK_DOWN:
				if (snake.dir != VK_UP)
					snake.dir = msg.vkcode;
				break;
			case VK_LEFT:
				if (snake.dir != VK_RIGHT)
					snake.dir = msg.vkcode;
				break;
			case VK_RIGHT:
				if (snake.dir != VK_LEFT)
					snake.dir = msg.vkcode;
				break;
			}
		std::cout << (int)msg.vkcode << std::endl;
		}
	}//��Ӧ��Ϣ����ȡ��ǰ������Ϣ
	void eatFood()
	{
		if (snake.collision(food))//�ж��Ƿ�Ե���ʳ��
		{	//�ߵĽ�������������ʳ���ٴ��������
			snake.bodyadd();
			food.chagePos();
		}
	}
	void gameover()
	{
		if (snake.collisionself()==true)
		{
			//��Ϸ����
			std::cout << "gameover" << std::endl;
			exit(0);
		}
	}//����Ƿ�ײ���Լ�
private:
	Snake snake;
	Food food;
};
/*����������*/
int main()
{
	initgraph(640, 480, EX_SHOWCONSOLE);  //��ʼ����ͼ���ڣ������ӡ��Ϣ
	
	srand(time(nullptr));//�������������
	GameScense scene;
	while (true) 
	{ 
		scene.run();
		Sleep(100);  //��ʱ
	}
	getchar();
	return 0;
}