#include<iostream>
#include<easyx.h>  //图形库
#include<vector>	//顺序表
#include<ctime>
/*精灵类(父类)*/
class Sprite
{
public:
	Sprite():Sprite(270,190) {};
	Sprite(int x,int y):m_x(x),m_y(y),m_color(RED) {};

	virtual void draw()  //虚函数，为了多态而产生的一个机制
	{
		//设置颜色
		setfillcolor(m_color);
		//绘制矩形
		fillrectangle(m_x,m_y,m_x+10,m_y+10);
	}//绘制精灵
	void moveBy(int dx, int dy)
	{
		m_x += dx;
		m_y += dy;
	}//移动
	bool collision(const Sprite& other)
	{
		return m_x == other.m_x && m_y == other.m_y;
	}


protected:
	int m_x;
	int m_y;
	COLORREF m_color;  //颜色
};
/*蛇类(继承精灵类)*/     
class Snake :public Sprite
{
public:
	Snake() :Snake(0, 0) {}
	Snake(int x, int y) :Sprite(x, y),dir(VK_RIGHT)
	{
		
		nodes.push_back(Sprite(20,0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}//初始话蛇的身体
	bool collision(const Sprite& other)
	{
		return nodes[0].collision(other);
	}//检测头是否与其他进行碰撞
	void draw() override
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		}
	}	//重启父类的虚函数  
	void bodyMove()
	{
		for (size_t i = nodes.size() - 1 ; i > 0; i-- )		//i  == 3-1 是索引的意思 不是数量的意思 在这里3是节点的数量
		{
			nodes[i] = nodes[i - 1]; //身体的每一节节点等于后一节节点
		}
		switch (dir) 
		{
		case VK_UP:
			nodes[0].moveBy(0, -10);//蛇头向上移动
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);//蛇头向下移动
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0); //蛇头向左移动
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0); //蛇头向右移动
			break;
		}
	}/*蛇的身体的移动*/
	void bodyadd()
	{
		nodes.push_back(Sprite());
	}//蛇的身体增加
	bool collisionself()
	{
		for (size_t i = 1; i < nodes.size(); ++i)
		{
			if (nodes[0].collision(nodes[i]))
				return true; // 蛇头撞到身体
		}
		return false;
	}//检测蛇头是否撞到自己的身体
private:
	//蛇有多节身体
	std::vector<Sprite> nodes; //蛇的所有节点
public:
	int dir;//蛇的方向
};
/*食物类(继承精灵类)*/

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
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);  //画椭圆，食物
	}
	void chagePos()
	{
		m_x = rand() % 64 * 10;//随机生成坐标	
		m_y = rand() % 48 * 10;
	}//改变食物的坐标
};
/*游戏的场景*/
class GameScense 
{
public:
	GameScense() {};
	void run() 
	{
		
		BeginBatchDraw();//双缓冲绘图
		cleardevice();//清屏，防止生成的蛇一直停留在原地，然后继续生成新的蛇头
		snake.draw();//画蛇
		food.draw();//画食物
		eatFood();//吃食物
		EndBatchDraw();//双缓冲绘图
		snake.bodyMove();//移动蛇，改变蛇的坐标
		gameover();//游戏结束
		
		ExMessage msg = { 0 };
		while (peekmessage(&msg, PP_SYM_KEYSIZE))//获取消息
		{
			onMsg(msg);
		}
	}; 
	void onMsg(const ExMessage& msg)		//移动蛇的方向 获取键盘按键 ——getch（）
	{
	//有无按键按下
		if (msg.message == WM_KEYDOWN)
		{
			//判断按下的是哪个键  vkcode  == virtual key code  虚拟键码
			
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
	}//响应消息：获取当前键盘消息
	void eatFood()
	{
		if (snake.collision(food))//判断是否吃到了食物
		{	//蛇的节数增长，并且食物再次随机生成
			snake.bodyadd();
			food.chagePos();
		}
	}
	void gameover()
	{
		if (snake.collisionself()==true)
		{
			//游戏结束
			std::cout << "gameover" << std::endl;
			exit(0);
		}
	}//检测是否撞到自己
private:
	Snake snake;
	Food food;
};
/*主函数运行*/
int main()
{
	initgraph(640, 480, EX_SHOWCONSOLE);  //初始化绘图窗口，输出打印信息
	
	srand(time(nullptr));//设置随机数种子
	GameScense scene;
	while (true) 
	{ 
		scene.run();
		Sleep(100);  //延时
	}
	getchar();
	return 0;
}