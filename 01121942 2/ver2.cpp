#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
using namespace std;

const float MAZE_SCALE = 0.5;
const int LEN_RECT = 20; /*----setting-----*/

struct prop{
    bool ifAppear;
    sf::Sprite sprite2_coin;
};

//碰撞檢測，矩形
bool check_collision(const sf::Sprite &a, const sf::Sprite &b){
    sf::FloatRect box1 = a.getGlobalBounds();
    sf::FloatRect box2 = b.getGlobalBounds();
    return box1.intersects(box2);
}

bool detectWall(int x, int y, sf::Color colorWall, sf::Image maze1, int key);

void drawProps(prop* &arrProp, int n, sf::RenderWindow &window);
bool touchCoins(sf::Sprite sprite2_chart1,prop* &arrCoin1, int n);
bool touchBoxes(sf::Sprite sprite2_chart1, prop* &arrBox, int n, int &x, int &y);
void openBox(sf::Sprite &sprite, float &v, int &score, sf::Text &openBoxMsg);

void drawRects(sf::RectangleShape** arrRect, int cntRect, sf::RenderWindow &window, int x, int y);
float dist(int x1, int x2, int y1, int y2);

void setBoxes(prop* arrBox);
void setCoins(prop* arrCoin1);
int main()
{
    int winW = 1050;
    int winL = 1050;
    sf::RenderWindow window(sf::VideoMode(winW, winL), "Final Hell");
    
    //畫面一要用到的物件(開場背景、開始按鈕)
    sf::Texture texture1_start;
    if (!texture1_start.loadFromFile("p1_start.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite1_start(texture1_start);
    sprite1_start.setScale(1.44, 1.44);
    
    sf::Texture texture1_startButtom;
    if (!texture1_startButtom.loadFromFile("p1_start1Buttom.png"))
        return EXIT_FAILURE;
    sf::Texture texture1_startButtom_red;
    if (!texture1_startButtom_red.loadFromFile("p1_start1Buttom_click.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite1_startButtom(texture1_startButtom);
    sprite1_startButtom.setPosition(sf::Vector2f(360.f, 700.f));
    sprite1_startButtom.setScale(0.5, 0.5);
    
    //畫面二要用到的物件(迷宮、人物、錢幣、寶箱、返回鍵、終點們)
    sf::Texture texture2_maze0;
    if (!texture2_maze0.loadFromFile("p2_mikong.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_maze1(texture2_maze0);
    sprite2_maze1.setScale(MAZE_SCALE, MAZE_SCALE); // maze scale: 0.5
    sf::Color colorOrigin = sprite2_maze1.getColor();
    
    //採集牆壁顏色
    sf::Image maze1;
    if (!maze1.loadFromFile("p2_mikong.jpg")){
        return EXIT_FAILURE;
    }
    sf::Color colorWall = maze1.getPixel(804, 1026);
    
    //chart1: dot
    sf::Texture texture2_chart1_up;
    if (!texture2_chart1_up.loadFromFile("p2_chart1_up.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_chart1_down;
    if (!texture2_chart1_down.loadFromFile("p2_chart1_down.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_chart1_left;
    if (!texture2_chart1_left.loadFromFile("p2_chart1_left.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_chart1_right;
    if (!texture2_chart1_right.loadFromFile("p2_chart1_right.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_chart1(texture2_chart1_right);
    sprite2_chart1.setPosition(sf::Vector2f(506.f, 588.f));//set pic2
    sprite2_chart1.setScale(0.007, 0.007);
    
    //找座標用
    /*
    int X = sprite2_chart1.getPosition().x;
    int Y = sprite2_chart1.getPosition().y;
    */
    
    //coins
    sf::Texture texture2_coin1;
    if (!texture2_coin1.loadFromFile("p2_coin.png"))
        return EXIT_FAILURE;
   
    int cntCoin = 55;/*-----設置錢幣數量----*/
    prop* arrCoin1 = new prop[cntCoin];
    for(int i = 0; i < cntCoin; i++){
        arrCoin1[i].ifAppear = true;
        arrCoin1[i].sprite2_coin.setTexture(texture2_coin1);
        arrCoin1[i].sprite2_coin.setScale(0.006, 0.006);
    }
    setCoins(arrCoin1);
    
    //boxes
    sf::Texture texture2_box;
    if (!texture2_box.loadFromFile("p2_box.png"))
        return EXIT_FAILURE;
    int cntBox = 13;/*-----設置寶箱數量----*/
    
    prop *arrBox = new prop[cntBox];
    for(int i = 0; i < cntBox; i++){
        arrBox[i].ifAppear = true;
        arrBox[i].sprite2_coin.setTexture(texture2_box);
        arrBox[i].sprite2_coin.setScale(0.06, 0.06);
    }
    setBoxes(arrBox);
    
    //返回鍵 右下角
    sf::Texture texture2_backButtom;
    if (!texture2_backButtom.loadFromFile("p2_backButtom.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_backButtom_red;
    if (!texture2_backButtom_red.loadFromFile("p2_backButtom_click.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_backButtom(texture2_backButtom);
    sprite2_backButtom.setPosition(sf::Vector2f(786.f, 952.f));
    sprite2_backButtom.setScale(0.3, 0.3);
    
    //視野正方形
    int lenRect = LEN_RECT;
    int cntRect = (winL/lenRect) + 1;
    sf::RectangleShape** arrRect = new sf::RectangleShape*[cntRect];
    for(int i = 0; i < cntRect; i++) {
        arrRect[i] = new sf::RectangleShape[cntRect];
        for(int j = 0; j < cntRect; j++){
            arrRect[i][j].setSize(sf::Vector2f(lenRect, lenRect));
            arrRect[i][j].setPosition(sf::Vector2f(i*lenRect, j*lenRect));
            arrRect[i][j].setFillColor(sf::Color(0, 0, 0, 220));
        }
    }
    
    //target(heaven)
    //先借用char1的圖片當終點，最後把圖片設為透明
    sf::Sprite sprite2_target(texture2_chart1_right);
    sprite2_target.setColor(sf::Color::Transparent);
    sprite2_target.setScale(0.05, 0.05);
    sprite2_target.setPosition(sf::Vector2f(492.f, 138.f));
    
    //bad target(hell)
    sf::Sprite sprite2_badtarget(texture2_chart1_right);
    sprite2_badtarget.setColor(sf::Color::Transparent);
    sprite2_badtarget.setScale(0.05, 0.05);
    sprite2_badtarget.setPosition(sf::Vector2f(224.f, 926.f));
    
    //成功畫面（碰到target後）
    sf::Texture texture2_congra;
    if (!texture2_congra.loadFromFile("ph_congra.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_congra(texture2_congra);
    sprite2_congra.setScale(0.36, 0.36);
    
    //home按鈕
    sf::Texture texture2_homeButtom;
    if (!texture2_homeButtom.loadFromFile("ph_homeButtom.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_homeButtom_red;
    if (!texture2_homeButtom_red.loadFromFile("ph_homeButtom_click.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_homeButtom(texture2_homeButtom);
    sprite2_homeButtom.setScale(0.3, 0.3);
    sprite2_homeButtom.setPosition(sf::Vector2f(434.f, 846.f));
    
    //失敗畫面（碰到bad target後 or 時間到）
    sf::Texture texture2_failed;
    if (!texture2_failed.loadFromFile("ph_failed.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_failed(texture2_failed);
    sprite2_failed.setScale(0.53, 0.53);
    
    
    /*--------------sounds------------*/
	sf::SoundBuffer getCoinBf;
	getCoinBf.loadFromFile("getcoin.wav");
	sf::Sound getCoin;
	getCoin.setBuffer(getCoinBf);
	
	sf::SoundBuffer punchBf;
	punchBf.loadFromFile("punch.wav");
	sf::Sound punch;
	punch.setBuffer(punchBf);
	
	sf::SoundBuffer openboxBf;
	openboxBf.loadFromFile("openbox.wav");
	sf::Sound openbox;
	openbox.setBuffer(openboxBf);
	
	sf::SoundBuffer pressBtnBf;
	pressBtnBf.loadFromFile("pressbtn.wav");
	sf::Sound pressBtn;
	pressBtn.setBuffer(pressBtnBf);
	
    /*--------------texts------------*/
	sf::Font sitkaB;
	sf::Font msjh;
	sitkaB.loadFromFile("SitkaB.ttc");
	msjh.loadFromFile("msjh.ttc");
	sf::Text scoreHint;
	sf::Text scoreDsp;
	sf::Text timeHint;
	sf::Text restTime;
	sf::Text openBoxMsg;
	
	scoreHint.setFont(sitkaB);
	scoreDsp.setFont(sitkaB);
	timeHint.setFont(sitkaB);
	restTime.setFont(sitkaB);
	openBoxMsg.setFont(msjh);
	
	scoreHint.setString("Score: ");
	scoreDsp.setString("0");
	timeHint.setString("time: ");
	restTime.setString("20");
	openBoxMsg.setString("");
	
	scoreHint.setPosition(10.f, 10.f);
	scoreDsp.setPosition(200.f, 10.f);
	timeHint.setPosition(770.f, 10.f);
	restTime.setPosition(970.f, 10.f);
	openBoxMsg.setPosition(500.f, 500.f);
	
	scoreHint.setCharacterSize(48);
	scoreDsp.setCharacterSize(48);
	timeHint.setCharacterSize(48);
	restTime.setCharacterSize(48);
	openBoxMsg.setCharacterSize(24);
	
	scoreHint.setFillColor(sf::Color::Blue);
	scoreDsp.setFillColor(sf::Color::Blue);
	timeHint.setFillColor(sf::Color::Blue);
	restTime.setFillColor(sf::Color::Blue);
	openBoxMsg.setFillColor(sf::Color::Red);
	
	scoreHint.setStyle(sf::Text::Bold);
	scoreDsp.setStyle(sf::Text::Bold);
	timeHint.setStyle(sf::Text::Bold);
	restTime.setStyle(sf::Text::Bold);
	openBoxMsg.setStyle(sf::Text::Regular);
    
    /*--------------music------------*/
<<<<<<< HEAD
    
	sf::Music hellMusic;
	sf::Music successMusic;
	sf::Music failMusic;
	hellMusic.openFromFile("hellMusic.ogg"); 
	successMusic.openFromFile("successMusic.ogg");
	failMusic.openFromFile("failMusic.ogg");
=======

	//sf::Music hellMusic;
	//sf::Music successMusic;
	//sf::Music failMusic;
	//hellMusic.openFromFile("hellMusic.ogg");
	//successMusic.openFromFile("successMusic.ogg");
	//failMusic.openFromFile("failMusic.ogg");
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
    
    /*--------------計時器------------*/
    //計時器設定
    sf::Color origin;
    sf::Clock time;
    sf::Time elapsed1 ;
    sf::Time elapsed2 ;
    int limit = 100;
    
    
    /*--------------init------------*/
    bool first = true;
    bool second = false;
    bool heaven = false;
    bool hell = false;
    
    float v = 0.9; //比較細的地圖需要跑比較慢
    int score = 0; //coin的加分機制，要跟真正的分數機制合併
    string scoreStr;
    string restTimeStr;
    bool ifLight = 0;
    int timeRec = elapsed2.asSeconds();
    int windowOpen = 0;
    
    
    /*------------------------以下為操作----------------------*/
    /*------------------------以下為操作----------------------*/
    /*------------------------以下為操作----------------------*/
    
    while (window.isOpen())
    {
<<<<<<< HEAD
    	//cout << windowOpen  << " " << hell << endl;
=======
    	/*
        //cout << windowOpen  << " " << hell << endl;
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
    	if(second == false)
    		hellMusic.stop();
    	if(heaven == false)
    		successMusic.stop();
    	if(hell == false)
    		failMusic.stop();
    		
    	if(windowOpen == 0 && second == true)
    	{
			hellMusic.play();
			windowOpen++;
		}
		
		if(windowOpen == 0 && heaven == true)
		{
			successMusic.play();
			windowOpen++;
		}
		
		if(windowOpen == 0 && hell == true)
		{
			//cout <<  << endl;
			failMusic.play();
			//cout << failMusic.getStatus() << endl;
			windowOpen++;
		}
<<<<<<< HEAD
		
=======
		*/
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //輸出輸入的字元
            /*
            if (event.type == sf::Event::TextEntered){
                if (event.text.unicode < 128)
                    std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
            }
             */
            
            //找座標用(很好用): 滑鼠移動的座標偵測
            /*
            if (event.type == sf::Event::MouseMoved){
                cout << "mouse (x: " << event.mouseMove.x << ", y: " << event.mouseMove.y << ")" << endl;
            }
            */
            
        }
        
        
        /*------------------第一個畫面 == 開場畫面-------------------*/
        if(first){
            window.clear();
            window.draw(sprite1_start);
            window.draw(sprite1_startButtom);
            
            //按鈕處理
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if(sprite1_startButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                sprite1_startButtom.setTexture(texture1_startButtom_red);
            }else{
                sprite1_startButtom.setTexture(texture1_startButtom);
            }
            if(sprite1_startButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                pressBtn.play();
				first = false;
                second = true;
                
                //reset(init)
                windowOpen = 0;
                window.clear();
                window.setTitle("play window");
                //reset to origin
                sprite2_chart1.setPosition(sf::Vector2f(506.f, 588.f));
                //還原錢幣、寶箱初始化
                for(int i = 0; i < cntCoin; i++){
                    arrCoin1[i].ifAppear = true;
                }
                for(int i = 0; i < cntBox; i++){
                    arrBox[i].ifAppear = true;
                }
                v = 0.9;
                score = 0;
                ifLight = false;
                elapsed1 = time.getElapsedTime();//重新計時
                continue;
            }
        }
        
        /*------------------第二個畫面 == 遊戲畫面-------------------*/
        if(second){
            window.clear();
            window.draw(sprite2_maze1);
            window.draw(sprite2_chart1);//chart1: dot
            
            window.draw(sprite2_target);
            window.draw(sprite2_badtarget);
            
            scoreStr = to_string(score); //重玩時的第一次更新(score: 0)
            scoreDsp.setString(scoreStr);
            
            //畫上錢幣
            drawProps(arrCoin1, cntCoin, window);
            //碰到錢幣
            if(touchCoins(sprite2_chart1, arrCoin1, cntCoin)){
            	getCoin.play();
                score++;
                scoreStr = to_string(score);
                scoreDsp.setString(scoreStr);
                cout << "new score: " << score << endl;
            }
            
            //畫上寶箱
            drawProps(arrBox, cntBox, window);
            int boxX = 0, boxY = 0;
            if(touchBoxes(sprite2_chart1, arrBox, cntBox, boxX, boxY)){
            	openbox.play();
                openBox(sprite2_chart1, v, score, openBoxMsg);
                scoreStr = to_string(score);
                scoreDsp.setString(scoreStr);
                openBoxMsg.setPosition(boxX, boxY);
                timeRec = elapsed2.asSeconds();
            }
            //寶箱文字最多兩秒
            if(timeRec - elapsed2.asSeconds() <= -2)
            {
            	openBoxMsg.setString("");
            	timeRec = elapsed1.asSeconds();
			}
			
            //取得dot 位置
            int x = sprite2_chart1.getPosition().x;
            int y = sprite2_chart1.getPosition().y;
            //一直cout dot移動後座標
            /*
            if((x != X) || (y != Y)){
                cout << "(" << x  << ", " << y << ")" << endl;
                X = x;
                Y = y;
            }
            */
            
            if(!ifLight){
                //畫上正方形
                drawRects(arrRect, cntRect, window, x, y);
            }
            
            //以下為黑色正方形之上方塗層-->不可以被視野限制覆蓋掉
            
            window.draw(sprite2_backButtom);
            
            window.draw(scoreHint);
            window.draw(scoreDsp);
            window.draw(timeHint);
            window.draw(restTime);
            window.draw(openBoxMsg);
            
            //按著空白鍵就開燈
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                ifLight = true;
            }else{
                ifLight = false;
            }
            
            //按下返回鍵
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if(sprite2_backButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                sprite2_backButtom.setTexture(texture2_backButtom_red);
            }else{
                sprite2_backButtom.setTexture(texture2_backButtom);
            }
            if(sprite2_backButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                pressBtn.play();
				second = false;
                first = true;
                windowOpen = 0;
                window.clear();
                window.setTitle("Final Hell");
                continue;
                
            }
            
            //走到 heaven
            if(check_collision(sprite2_chart1, sprite2_target)){
                second = false;
                heaven = true;
                windowOpen = 0;
                window.clear();
                window.setTitle("congratulations!");
            }
            
            //走到 hell
            if(check_collision(sprite2_chart1, sprite2_badtarget)){
                second = false;
                hell = true;
                windowOpen = 0;
                window.clear();
                window.setTitle("FAILED");
            }
            
            //上下左右控制
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                sprite2_chart1.setTexture(texture2_chart1_up);
                if((y <= 2) || detectWall(x, y, colorWall, maze1, 1) )
                {
                    //希望撞牆聲可以完整播放，避免重疊播放
                    if(punch.getStatus() != sf::Sound::Status::Playing) {
                        punch.play();
                    }
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(0, -v);//move chart1
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                sprite2_chart1.setTexture(texture2_chart1_down);
                if((y >= 1040) || detectWall(x, y, colorWall, maze1, 2)){
<<<<<<< HEAD
                    punch.play();
=======
                    if(punch.getStatus() != sf::Sound::Status::Playing) {
                        punch.play();
                    }
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(0, v);//move chart1
                }
                
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                sprite2_chart1.setTexture(texture2_chart1_left);
                if((x <= 2) || detectWall(x, y, colorWall, maze1, 3)){
                    if(punch.getStatus() != sf::Sound::Status::Playing){
                        punch.play();
                    }
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(-v, 0);//move chart1
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                sprite2_chart1.setTexture(texture2_chart1_right);
                if((x >= 1040) || detectWall(x, y, colorWall, maze1, 4)){
<<<<<<< HEAD
                    punch.play();
=======
                    if(punch.getStatus() != sf::Sound::Status::Playing){
                        punch.play();
                    }
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(v, 0);//move chart1
                }
            }
            
            
        }
        
        /*------------------------成功畫面-------------------------*/
        if(heaven){
<<<<<<< HEAD
        	successMusic.play();
=======
        	//successMusic.play();
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
            window.clear();
            window.draw(sprite2_congra);
            window.draw(sprite2_homeButtom);
            scoreHint.setPosition(500.f, 500.f);
            scoreDsp.setPosition(700.f, 500.f);
            window.draw(scoreHint);
            window.draw(scoreDsp);
            
            //回主畫面按鈕
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                sprite2_homeButtom.setTexture(texture2_homeButtom_red);
            }else{
                sprite2_homeButtom.setTexture(texture2_homeButtom);
            }
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                heaven = false;
                first = true;
                windowOpen = 0;
                window.clear();
                window.setTitle("Final Hell");
                scoreHint.setPosition(10.f, 10.f);
				scoreDsp.setPosition(200.f, 10.f);
                continue;
            }
            
            
        }
        
        /*------------------------失敗畫面-------------------------*/
        if(hell){
<<<<<<< HEAD
        	failMusic.play();
=======
        	//failMusic.play();
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
            window.clear();
            window.draw(sprite2_failed);
            window.draw(sprite2_homeButtom);
            
            //按鈕處理
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                sprite2_homeButtom.setTexture(texture2_homeButtom_red);
            }else{
                sprite2_homeButtom.setTexture(texture2_homeButtom);
            }
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                //pressBtn.play();
				hell = false;
                first = true;
                windowOpen = 0;
                window.clear();
                window.setTitle("Final Hell");
                continue;
            }
            
        }
        
        elapsed2 = time.getElapsedTime();
        if(second){
            //cout << (int)(limit-(elapsed2.asSeconds()-elapsed1.asSeconds())) << endl;
            restTimeStr = to_string((int)(limit - (elapsed2.asSeconds() - elapsed1.asSeconds())));
            restTime.setString(restTimeStr);
            if((int)(limit - (elapsed2.asSeconds() - elapsed1.asSeconds())) == 0)
        	{
        		second = false;
				hell = true;
				windowOpen = 0;
				window.clear();
				window.setTitle("FAILED");	
			}
        }
        window.display();
    }
    
    //delete
    delete [] arrCoin1;
    arrCoin1 = nullptr;
    delete [] arrBox;
    arrBox = nullptr;
    for(int i = 0; i < cntRect; i++){
        delete [] arrRect[i];
        arrRect[i] = nullptr;
    }
    delete [] arrRect;
    arrRect = nullptr;
    
    return EXIT_SUCCESS;
}

bool detectWall(int x, int y, sf::Color colorWall, sf::Image maze1, int key){
    x = x / MAZE_SCALE; //如果有調整背景大小
    y = y / MAZE_SCALE;
    //寬度7, 長度7
    int w = 7, l = 7;
    w = w / MAZE_SCALE;
    l = l / MAZE_SCALE;
    sf::Color color_at_chart = maze1.getPixel(x, y);
    switch(key){
        case 1:
            for(int i = 2; i < w; i++){
                color_at_chart = maze1.getPixel(x + i, y); //up
                if (color_at_chart == colorWall) {
                    return true;
                }
            }
            return false;
            break;
        case 2:
            for(int i = 2; i < w; i++){
                color_at_chart = maze1.getPixel(x + i, y + l); //down
                if (color_at_chart == colorWall) {
                    return true;
                }
            }
            return false;
            break;
        case 3:
            for(int j = 2; j < l; j++){
                color_at_chart = maze1.getPixel(x, y + j); //left
                if (color_at_chart == colorWall) {
                    return true;
                }
            }
            return false;
            break;
        case 4:
            for(int j = 2; j < l; j++){
                color_at_chart = maze1.getPixel(x + w, y + j); //right
                if (color_at_chart == colorWall) {
                    return true;
                }
            }
            return false;
            break;
        default:
            cout << "keyboard broken" << endl;
            break;
    }
    return false;
}

//畫寶箱、錢幣
void drawProps(prop* &arrProp, int n, sf::RenderWindow &window){
    for(int i = 0; i < n; i++){
        if(arrProp[i].ifAppear == true){
            window.draw(arrProp[i].sprite2_coin);
        }
    }
}

bool touchCoins(sf::Sprite sprite2_chart1, prop* &arrCoin1, int n){
    for(int i = 0; i < n; i++){
        if((arrCoin1[i].ifAppear == true) && (check_collision(sprite2_chart1, arrCoin1[i].sprite2_coin))){
            arrCoin1[i].ifAppear = false;
            return true;
        }
    }
    return false;
}

bool touchBoxes(sf::Sprite sprite2_chart1, prop* &arrBox, int n, int &x, int &y){
    for(int i = 0; i < n; i++){
        if((arrBox[i].ifAppear == true) && (check_collision(sprite2_chart1, arrBox[i].sprite2_coin))){
            x = arrBox[i].sprite2_coin.getPosition().x;
            y = arrBox[i].sprite2_coin.getPosition().y;
            arrBox[i].ifAppear = false;
            return true;
        }
    }
    return false;
}


void openBox(sf::Sprite &sprite, float &v, int &score, sf::Text &openBoxMsg){
    int rn = rand();
    switch(rn % 5) {
        case 0:
<<<<<<< HEAD
            openBoxMsg.setString(L"考試睡過頭，掰啦");
=======
           	openBoxMsg.setString(L"考試睡過頭，掰啦");
>>>>>>> 0ba152e439f1ed06534a50d35aeb5c8003dfa5a8
            sprite.setPosition(sf::Vector2f(506.f, 588.f));//set chart1 to 原點
            break;
        case 1:
            //加分
            openBoxMsg.setString(L"熬夜爆肝 分數++");
			score += 10;
            cout << "new score: " << score << endl;
            break;
        case 2:
            //減分
            openBoxMsg.setString(L"積不出來qq 分數--");
			score -= 10;
            cout << "new score: " << score << endl;
            break;
        case 3:
            //加速
            openBoxMsg.setString(L"考前通靈 速度up up");
			v = v * 2;
            break;
        case 4:
            //減速
            openBoxMsg.setString(L"唸到ㄎㄧㄤ掉 速度--");
			v = v / 2;
            break;
        default:
            cout << "nothing happen" << endl; //預防萬一
            break;
        }
    
    return;
    
}

//畫正方形
void drawRects(sf::RectangleShape** arrRect, int cntRect, sf::RenderWindow &window, int x, int y){
    int lenRect = LEN_RECT;
    for(int i = 0; i < cntRect; i++){
        for(int j = 0; j < cntRect; j++){
            if(dist(x, (i*lenRect + (lenRect/2)), y, (j*lenRect + (lenRect/2))) < 75.0){
                arrRect[i][j].setFillColor(sf::Color(0, 0, 0, 0));
            }else if(dist(x, (i*lenRect + (lenRect/2)), y, (j*lenRect + (lenRect/2))) < 100.0){
                arrRect[i][j].setFillColor(sf::Color(0, 0, 0, 100));
            }else if(dist(x, (i*lenRect + (lenRect/2)), y, (j*lenRect + (lenRect/2))) < 125.0){
                arrRect[i][j].setFillColor(sf::Color(0, 0, 0, 150));
            }else{
                arrRect[i][j].setFillColor(sf::Color(0, 0, 0, 245));
            }
            window.draw(arrRect[i][j]);
        }
    }
    return;
}

float dist(int x1, int x2, int y1, int y2){
    int dist = pow((x1 - x2), 2) + pow((y1 - y2), 2);
    dist = pow(dist, 0.5);
    return dist;
    
}

//訂定箱子、錢幣點位
void setBoxes(prop* arrBox){
    arrBox[0].sprite2_coin.setPosition(sf::Vector2f(276.f, 888.f));
    arrBox[1].sprite2_coin.setPosition(sf::Vector2f(140.f, 656.f));
    arrBox[2].sprite2_coin.setPosition(sf::Vector2f(252.f, 806.f));
    arrBox[3].sprite2_coin.setPosition(sf::Vector2f(354.f, 832.f));
    arrBox[4].sprite2_coin.setPosition(sf::Vector2f(310.f, 614.f));
    arrBox[5].sprite2_coin.setPosition(sf::Vector2f(616.f, 756.f));
    arrBox[6].sprite2_coin.setPosition(sf::Vector2f(740.f, 556.f));
    arrBox[7].sprite2_coin.setPosition(sf::Vector2f(590.f, 454.f));
    arrBox[8].sprite2_coin.setPosition(sf::Vector2f(644.f, 658.f));
    arrBox[9].sprite2_coin.setPosition(sf::Vector2f(456.f, 690.f));
    arrBox[10].sprite2_coin.setPosition(sf::Vector2f(830.f, 578.f));
    arrBox[11].sprite2_coin.setPosition(sf::Vector2f(676.f, 298.f));
    arrBox[12].sprite2_coin.setPosition(sf::Vector2f(512.f, 212.f));
    
    return;
}
void setCoins(prop* arrCoin1){
    arrCoin1[0].sprite2_coin.setPosition(sf::Vector2f(204.f, 806.f));
    arrCoin1[1].sprite2_coin.setPosition(sf::Vector2f(174.f, 760.f));
    arrCoin1[2].sprite2_coin.setPosition(sf::Vector2f(152.f, 708.f));
    arrCoin1[3].sprite2_coin.setPosition(sf::Vector2f(196.f, 680.f));
    arrCoin1[4].sprite2_coin.setPosition(sf::Vector2f(208.f, 730.f));
    
    arrCoin1[5].sprite2_coin.setPosition(sf::Vector2f(224.f, 762.f));
    arrCoin1[6].sprite2_coin.setPosition(sf::Vector2f(322.f, 816.f));
    arrCoin1[7].sprite2_coin.setPosition(sf::Vector2f(368.f, 794.f));
    arrCoin1[8].sprite2_coin.setPosition(sf::Vector2f(308.f, 726.f));
    arrCoin1[9].sprite2_coin.setPosition(sf::Vector2f(272.f, 630.f));
    
    arrCoin1[10].sprite2_coin.setPosition(sf::Vector2f(336.f, 682.f));
    arrCoin1[11].sprite2_coin.setPosition(sf::Vector2f(374.f, 740.f));
    arrCoin1[12].sprite2_coin.setPosition(sf::Vector2f(410.f, 764.f));
    arrCoin1[13].sprite2_coin.setPosition(sf::Vector2f(452.f, 782.f));
    arrCoin1[14].sprite2_coin.setPosition(sf::Vector2f(508.f, 790.f));
    
    arrCoin1[15].sprite2_coin.setPosition(sf::Vector2f(644.f, 780.f));
    arrCoin1[16].sprite2_coin.setPosition(sf::Vector2f(696.f, 742.f));
    arrCoin1[17].sprite2_coin.setPosition(sf::Vector2f(732.f, 680.f));
    arrCoin1[18].sprite2_coin.setPosition(sf::Vector2f(768.f, 696.f));
    arrCoin1[19].sprite2_coin.setPosition(sf::Vector2f(786.f, 638.f));
    
    arrCoin1[20].sprite2_coin.setPosition(sf::Vector2f(824.f, 510.f));
    arrCoin1[21].sprite2_coin.setPosition(sf::Vector2f(784.f, 416.f));
    arrCoin1[22].sprite2_coin.setPosition(sf::Vector2f(772.f, 392.f));
    arrCoin1[23].sprite2_coin.setPosition(sf::Vector2f(690.f, 258.f));
    arrCoin1[24].sprite2_coin.setPosition(sf::Vector2f(616.f, 230.f));
    
    arrCoin1[25].sprite2_coin.setPosition(sf::Vector2f(558.f, 218.f));
    arrCoin1[26].sprite2_coin.setPosition(sf::Vector2f(706.f, 564.f));
    arrCoin1[27].sprite2_coin.setPosition(sf::Vector2f(688.f, 508.f));
    arrCoin1[28].sprite2_coin.setPosition(sf::Vector2f(648.f, 444.f));
    arrCoin1[29].sprite2_coin.setPosition(sf::Vector2f(606.f, 434.f));
    
    arrCoin1[30].sprite2_coin.setPosition(sf::Vector2f(750.f, 620.f));
    arrCoin1[31].sprite2_coin.setPosition(sf::Vector2f(634.f, 504.f));
    arrCoin1[32].sprite2_coin.setPosition(sf::Vector2f(660.f, 576.f));
    arrCoin1[33].sprite2_coin.setPosition(sf::Vector2f(598.f, 716.f));
    arrCoin1[34].sprite2_coin.setPosition(sf::Vector2f(538.f, 744.f));
    
    arrCoin1[35].sprite2_coin.setPosition(sf::Vector2f(520.f, 704.f));
    arrCoin1[36].sprite2_coin.setPosition(sf::Vector2f(484.f, 702.f));
    arrCoin1[37].sprite2_coin.setPosition(sf::Vector2f(464.f, 644.f));
    arrCoin1[38].sprite2_coin.setPosition(sf::Vector2f(440.f, 604.f));
    arrCoin1[39].sprite2_coin.setPosition(sf::Vector2f(616.f, 278.f));
    
    arrCoin1[40].sprite2_coin.setPosition(sf::Vector2f(424.f, 288.f));
    arrCoin1[41].sprite2_coin.setPosition(sf::Vector2f(454.f, 350.f));
    arrCoin1[42].sprite2_coin.setPosition(sf::Vector2f(278.f, 354.f));
    arrCoin1[43].sprite2_coin.setPosition(sf::Vector2f(304.f, 432.f));
    arrCoin1[44].sprite2_coin.setPosition(sf::Vector2f(366.f, 456.f));
    
    arrCoin1[45].sprite2_coin.setPosition(sf::Vector2f(346.f, 536.f));
    arrCoin1[46].sprite2_coin.setPosition(sf::Vector2f(356.f, 590.f));
    arrCoin1[47].sprite2_coin.setPosition(sf::Vector2f(408.f, 640.f));
    arrCoin1[48].sprite2_coin.setPosition(sf::Vector2f(218.f, 492.f));
    arrCoin1[49].sprite2_coin.setPosition(sf::Vector2f(208.f, 634.f));
    
    arrCoin1[50].sprite2_coin.setPosition(sf::Vector2f(456.f, 900.f));
    arrCoin1[51].sprite2_coin.setPosition(sf::Vector2f(452.f, 890.f));
    arrCoin1[52].sprite2_coin.setPosition(sf::Vector2f(690.f, 864.f));
    arrCoin1[53].sprite2_coin.setPosition(sf::Vector2f(744.f, 736.f));
    arrCoin1[54].sprite2_coin.setPosition(sf::Vector2f(874.f, 608.f));
    
    return;
}
