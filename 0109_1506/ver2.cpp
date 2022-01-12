#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<cstdlib>
#include<cmath>
using namespace std;

struct coin{
    bool ifAppear;
    sf::Sprite sprite2_coin;
};
//待改，那個時候沒教template所以我是亂寫的
//template<typename sf::Sprite,typename sf::Sprite>
bool check_collision(const sf::Sprite &a, const sf::Sprite &b){ //碰撞檢測，矩形
    sf::FloatRect box1 = a.getGlobalBounds();
    sf::FloatRect box2 = b.getGlobalBounds();
    return box1.intersects(box2);
}

bool detectWall(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_up(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_down(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_left(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_right(int x, int y, sf::Color colorWall, sf::Image maze1);

void drawCoins(coin* &arrCoin1, int n, sf::RenderWindow &window);
bool touchCoins(sf::Sprite sprite2_chart1, coin* &arrCoin1, int n);

void openBox(sf::Sprite &sprite, float &v, int &score);

void drawRects(sf::RectangleShape** arrRect, int cntRect, sf::RenderWindow &window, int x, int y);
float dist(int x1, int x2, int y1, int y2);
bool show_detectcoin_words(sf::Time time_detect,sf::Time time_now);
int main()
{
    int winW = 1680;
    int winL = 1680;
    sf::RenderWindow window(sf::VideoMode(winW, winL), "hello window");

    //畫面一要用到的物件(texture, sprite)
    sf::Texture texture1_start;
    if (!texture1_start.loadFromFile("p1_start.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite1_start(texture1_start);
    sprite1_start.setScale(1.3, 1.3);

    sf::Texture texture1_startButtom;
    if (!texture1_startButtom.loadFromFile("p1_start1Buttom.png"))
        return EXIT_FAILURE;
    sf::Texture texture1_startButtom_red;
    if (!texture1_startButtom_red.loadFromFile("p1_start1Buttom_click.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite1_startButtom(texture1_startButtom);
    sprite1_startButtom.setPosition(sf::Vector2f(500.f, 1000.f));
    sprite1_startButtom.setScale(0.5, 0.5);

    //畫面二要用到的物件(迷宮、人物、錢幣、寶箱、返回鍵)
    sf::Texture texture2_maze0;
    if (!texture2_maze0.loadFromFile("p2_mikong.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_maze1(texture2_maze0);
    sprite2_maze1.setScale(0.8, 0.8);
    sf::Color colorOrigin = sprite2_maze1.getColor();

    //chart1: airplane
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
    sprite2_chart1.setPosition(sf::Vector2f(808.f, 944.f));//set pic2
    sprite2_chart1.setScale(0.01, 0.01);
    int X = sprite2_chart1.getPosition().x;
    int Y = sprite2_chart1.getPosition().y;

    //採集牆壁顏色，maze1是採集地圖中的牆壁
    sf::Image maze1;
    if (!maze1.loadFromFile("p2_mikong.jpg")){
        return EXIT_FAILURE;
    }
    sf::Color colorWall = maze1.getPixel(804, 1026);

    //coins
    sf::Texture texture2_coin1;
    if (!texture2_coin1.loadFromFile("p2_coin.png"))
        return EXIT_FAILURE;
    //還沒弄delete, nullptr
    int cntCoin = 5;/*-----設置錢幣數量----*/
    coin* arrCoin1 = new coin[cntCoin];
    for(int i = 0; i < cntCoin; i++){
        arrCoin1[i].ifAppear = true;
        arrCoin1[i].sprite2_coin.setTexture(texture2_coin1);
        arrCoin1[i].sprite2_coin.setScale(0.01, 0.01);
    }
    arrCoin1[0].sprite2_coin.setPosition(sf::Vector2f(920.f, 978.f));
    arrCoin1[1].sprite2_coin.setPosition(sf::Vector2f(896.f, 1018.f));
    arrCoin1[2].sprite2_coin.setPosition(sf::Vector2f(822.f, 1062.f));
    arrCoin1[3].sprite2_coin.setPosition(sf::Vector2f(20.f, 100.f));
    arrCoin1[4].sprite2_coin.setPosition(sf::Vector2f(20.f, 120.f));

    //boxes
    sf::Texture texture2_box;
    if (!texture2_box.loadFromFile("p2_box.png"))
        return EXIT_FAILURE;
    int cntBox = 5;/*-----設置寶箱數量----*/
    //還沒弄delete, nullptr
    coin *arrBox = new coin[cntBox];
    for(int i = 0; i < cntBox; i++){
        arrBox[i].ifAppear = true;
        arrBox[i].sprite2_coin.setTexture(texture2_box);
        arrBox[i].sprite2_coin.setScale(0.02, 0.02);
    }
    arrBox[0].sprite2_coin.setPosition(sf::Vector2f(796.f, 966.f));
    arrBox[1].sprite2_coin.setPosition(sf::Vector2f(844.f, 928.f));
    arrBox[2].sprite2_coin.setPosition(sf::Vector2f(802.f, 978.f));
    arrBox[3].sprite2_coin.setPosition(sf::Vector2f(838.f, 974.f));
    arrBox[4].sprite2_coin.setPosition(sf::Vector2f(776.f, 962.f));

    //返回鍵 我先隨便放左下角
    sf::Texture texture2_backButtom;
    if (!texture2_backButtom.loadFromFile("p2_backButtom.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_backButtom_red;
    if (!texture2_backButtom_red.loadFromFile("p2_backButtom_click.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_backButtom(texture2_backButtom);
    sprite2_backButtom.setPosition(sf::Vector2f(200.f, 1500.f));
    sprite2_backButtom.setScale(0.4, 0.4);


    //視野限制框框，漂亮版
    int lenRect = 20;/*------setting-----*/
    int cntRect = (winL/lenRect) + 1;
    auto** arrRect = new sf::RectangleShape*[cntRect];
    for(int i = 0; i < cntRect; i++) {
        arrRect[i] = new sf::RectangleShape[cntRect];
        for(int j = 0; j < cntRect; j++){
            arrRect[i][j].setSize(sf::Vector2f(lenRect, lenRect));
            arrRect[i][j].setPosition(sf::Vector2f(i*lenRect, j*lenRect));
            arrRect[i][j].setFillColor(sf::Color(0, 0, 0, 220));
        }
    }



    //sf::RectangleShape rect(sf::Vector2f(50.f, 50.f)); // 矩形
    //rect.setPosition(sf::Vector2f(100.f, 100.f));
    // rect.setFillColor(sf::Color(0, 0, 0, 200));


    //target(heaven)
    //先借用char1的圖片當終點，最後把圖片設為透明
    sf::Sprite sprite2_target(texture2_chart1_right);
    //sprite2_target.setColor(sf::Color::Transparent);
    sprite2_target.setScale(0.05, 0.05);
    sprite2_target.setPosition(sf::Vector2f(808.f, 258.f));

    //bad target(hell)
    sf::Sprite sprite2_badtarget(texture2_chart1_right);
    //sprite2_badtarget.setColor(sf::Color::Transparent);
    sprite2_badtarget.setScale(0.05, 0.05);
    sprite2_badtarget.setPosition(sf::Vector2f(386.f, 1486.f));

    //碰到target後的恭喜畫面
    sf::Texture texture2_congra;
    if (!texture2_congra.loadFromFile("ph_congra.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_congra(texture2_congra);
    sprite2_congra.setScale(2, 2);

    sf::Texture texture2_homeButtom;
    if (!texture2_homeButtom.loadFromFile("ph_homeButtom.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_homeButtom_red;
    if (!texture2_homeButtom_red.loadFromFile("ph_homeButtom_click.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_homeButtom(texture2_homeButtom);
    sprite2_homeButtom.setScale(0.3, 0.3);
    sprite2_homeButtom.setPosition(sf::Vector2f(1000.f, 600.f));

    //碰到bad target後的失敗畫面
    sf::Texture texture2_failed;
    if (!texture2_failed.loadFromFile("ph_failed.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_failed(texture2_failed);
    //sprite2_failed.setScale(2, 2);

    /*--------------計時器------------*/
    //計時器設定
    sf::Color origin;
    sf::Clock time;
    sf::Time elapsed1 ;
    sf::Time elapsed2 ;
    //觸碰事件時間控制
    sf::Time detect_object_t;
    //計時長度
    int limit = 20;

    bool first = true;
    bool second = false;
    bool heaven = false;
    bool hell = false;
    //int v = 1;
    float v = 0.9; //比較細的地圖需要跑比較慢 /*--global INIT_V---*/
    int score = 0; //coin的加分機制，要跟真正的分數機制合併
    bool ifLight = 0;
    bool detected_object = false;
    //以下為操作
    while (window.isOpen())
    {
        //sprite2.setColor(sf::Color::Green);//
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


        //第一個畫面
        if(first){
            window.clear();

            window.draw(sprite1_start);
            window.draw(sprite1_startButtom);
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if(sprite1_startButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                sprite1_startButtom.setTexture(texture1_startButtom_red);
            }else{
                sprite1_startButtom.setTexture(texture1_startButtom);
            }
            if(sprite1_startButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                first = false;
                second = true;
                window.clear();
                window.setSize(sf::Vector2u(1680, 1680));
                window.setTitle("play window");
                //預備讓人在正確的地方開始
                sprite2_chart1.setPosition(sf::Vector2f(808.f, 944.f));
                //調整視窗大小會讓圖片物件失真
                //要配合圖片調整(window.setSize 跟 spriteName.setScale要一起調整)
                //window.setSize(sf::Vector2u(1680, 1065));

                //還原錢幣、寶箱初始化
                for(int i = 0; i < cntCoin; i++){
                    arrCoin1[i].ifAppear = true;
                }
                for(int i = 0; i < cntBox; i++){
                    arrBox[i].ifAppear = true;
                }
                //
                //int v = 1;
                v = 0.9; //比較細的地圖需要跑比較慢
                score = 0; //coin的加分機制，要跟真正的分數機制合併
                ifLight = false;

            }
            //剛剛用到的東西
            //sf::Mouse::isButtonPressed(sf::Mouse::Left)
            //spriteNAME.getGlobalBounds().contains(mousePos.x, mousePos.y)

        }


        //第二個畫面
        if(second){
            window.clear();
            window.draw(sprite2_maze1);
            window.draw(sprite2_chart1);//chart1: emoji

            window.draw(sprite2_target);
            window.draw(sprite2_badtarget);


            //畫上錢幣
            drawCoins(arrCoin1, cntCoin, window);
            //碰到錢幣
            if(touchCoins(sprite2_chart1, arrCoin1, cntCoin)){
                score++;
                cout << "new score: " << score << endl;
                detected_object = true;
                detect_object_t = time.getElapsedTime();
            }




            //畫上寶箱
            drawCoins(arrBox, cntBox, window);
            if(touchCoins(sprite2_chart1, arrBox, cntBox)){
                openBox(sprite2_chart1, v, score);
                detected_object = true;
                detect_object_t = time.getElapsedTime();
            }

            if(show_detectcoin_words(detect_object_t,time.getElapsedTime())){
                //window.draw(text);
                cout << "draw text" << endl;
            }

            //取得座標
            int x = sprite2_chart1.getPosition().x;
            int y = sprite2_chart1.getPosition().y;

            //一直cout chart1移動後座標
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

            //返回鍵
            window.draw(sprite2_backButtom);

            //開關燈由空白鍵處理
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
                second = false;
                first = true;
                window.clear();
                window.setTitle("hello window");

                continue;

            }

            //走到 heaven
            if(check_collision(sprite2_chart1, sprite2_target)){
                second = false;
                heaven = true;
                window.clear();
                window.setTitle("congratulations!");
            }

            //走到 hell /*-----倒數到0時可能也要合併成失敗畫面-----*/
            if(check_collision(sprite2_chart1, sprite2_badtarget)){
                second = false;
                hell = true;
                window.clear();
                window.setTitle("FAILED");
            }



            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                sprite2_chart1.setTexture(texture2_chart1_up);
                if((y <= 2) || detectWall_up(x, y, colorWall, maze1)){
                    //我把邊界條件的警告（紅色背景）註解掉了，眼睛好痛xd
                    //sprite2_maze1.setColor(sf::Color::Red); //碰到牆壁背景就變紅色
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(0, -v);//move chart1
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                sprite2_chart1.setTexture(texture2_chart1_down);
                if((y >= 1670) || detectWall_down(x, y, colorWall, maze1)){
                    //sprite2_maze1.setColor(sf::Color::Red);
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(0, v);//move chart1
                }

            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                sprite2_chart1.setTexture(texture2_chart1_left);
                if((x <= 2) || detectWall_left(x, y, colorWall, maze1)){
                    //sprite2_maze1.setColor(sf::Color::Red);
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(-v, 0);//move chart1
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                sprite2_chart1.setTexture(texture2_chart1_right);
                if((x >= 1670) || detectWall_right(x, y, colorWall, maze1)){
                    //sprite2_maze1.setColor(sf::Color::Red);
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(v, 0);//move chart1
                }
            }



        }
        if(heaven){
            window.clear();
            window.draw(sprite2_congra);
            window.draw(sprite2_homeButtom);
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                sprite2_homeButtom.setTexture(texture2_homeButtom_red);
            }else{
                sprite2_homeButtom.setTexture(texture2_homeButtom);
            }
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                heaven = false;
                first = true;
                window.clear();
                window.setTitle("hello window");
                continue;
            }


        }
        if(hell){
            window.clear();
            window.draw(sprite2_failed);
            window.draw(sprite2_homeButtom);
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                sprite2_homeButtom.setTexture(texture2_homeButtom_red);
            }else{
                sprite2_homeButtom.setTexture(texture2_homeButtom);
            }
            if(sprite2_homeButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                hell = false;
                first = true;
                window.clear();

                window.setTitle("hello window");
                continue;
            }

        }
        /*
        elapsed2 = time.getElapsedTime();
        if(second){
            cout << (int)(limit-(elapsed2.asSeconds()-elapsed1.asSeconds()))<< endl;
        }
        */
        window.display();
    }




    return EXIT_SUCCESS;
}

bool detectWall(int x, int y, sf::Color colorWall, sf::Image maze1){
    //x = x / 1.3; 如果有調整背景大小
    //y = y / 1.3;
    //寬度19, 長度19
    int w = 20, l = 20;
    sf::Color color_at_airplane = maze1.getPixel(x, y);
    for(int i = 0; i < w; i++){
        color_at_airplane = maze1.getPixel(x + i, y);
        if (color_at_airplane == colorWall) {
            return true;
        }
        color_at_airplane = maze1.getPixel(x + i, y + l);
        if (color_at_airplane == colorWall) {
            return true;
        }
    }
    for(int j = 0; j < l; j++){
        color_at_airplane = maze1.getPixel(x, y + j);
        if (color_at_airplane == colorWall) {
            return true;
        }
        color_at_airplane = maze1.getPixel(x + w, y + j);
        if (color_at_airplane == colorWall) {
            return true;
        }
    }
    return false;
}

//一定要分成上下左右偵測，上下左右是四個案例
bool detectWall_up(int x, int y, sf::Color colorWall, sf::Image maze1){
    x = x / 0.8; //如果有調整背景大小
    y = y / 0.8;
    //寬度10, 長度10
    int w = 10, l = 10;
    w = w / 0.8;
    l = l / 0.8;
    sf::Color color_at_airplane = maze1.getPixel(x, y);
    for(int i = 0; i < w; i++){
        color_at_airplane = maze1.getPixel(x + i, y); //up
        if (color_at_airplane == colorWall) {
            return true;
        }
    }
    return false;
}

bool detectWall_down(int x, int y, sf::Color colorWall, sf::Image maze1){
    x = x / 0.8; //如果有調整背景大小
    y = y / 0.8;
    int w = 10, l = 10;
    w = w / 0.8;
    l = l / 0.8;
    sf::Color color_at_airplane = maze1.getPixel(x, y);
    for(int i = 0; i < w; i++){
        color_at_airplane = maze1.getPixel(x + i, y + l); //down
        if (color_at_airplane == colorWall) {
            return true;
        }
    }
    return false;
}

bool detectWall_left(int x, int y, sf::Color colorWall, sf::Image maze1){
    x = x / 0.8; //如果有調整背景大小
    y = y / 0.8;
    int w = 10, l = 10;
    w = w / 0.8;
    l = l / 0.8;
    sf::Color color_at_airplane = maze1.getPixel(x, y);
    for(int j = 0; j < l; j++){
        color_at_airplane = maze1.getPixel(x, y + j); //left
        if (color_at_airplane == colorWall) {
            return true;
        }
    }
    return false;
}

bool detectWall_right(int x, int y, sf::Color colorWall, sf::Image maze1){
    x = x / 0.8; //如果有調整背景大小
    y = y / 0.8;
    int w = 10, l = 10;
    w = w / 0.8;
    l = l / 0.8;
    sf::Color color_at_airplane = maze1.getPixel(x, y);
    for(int j = 0; j < l; j++){
        color_at_airplane = maze1.getPixel(x + w, y + j); //right
        if (color_at_airplane == colorWall) {
            return true;
        }
    }
    return false;
}

void drawCoins(coin* &arrCoin1, int n, sf::RenderWindow &window){
    for(int i = 0; i < n; i++){
        if(arrCoin1[i].ifAppear == true){
            window.draw(arrCoin1[i].sprite2_coin);
        }
    }
}

bool touchCoins(sf::Sprite sprite2_chart1, coin* &arrCoin1, int n){
    for(int i = 0; i < n; i++){
        if((arrCoin1[i].ifAppear == true) && (check_collision(sprite2_chart1, arrCoin1[i].sprite2_coin))){
            arrCoin1[i].ifAppear = false;
            return true;
        }
    }
    return false;
}

//開啟寶箱功能的函數，可以加點音效（？
//不知道能不能做出加時間、扣時間的case
void openBox(sf::Sprite &sprite, float &v, int &score){
    int rn = rand();
    switch(rn % 5) {
        case 0:
            cout << "考試睡過頭，掰啦" << endl; //到時候要變成顯示的文字
            sprite.setPosition(sf::Vector2f(808.f, 944.f));//set chart1 to 原點
            break;
        case 1:
            cout << "熬夜奮戰好像獲得了些什麼，啊，那不是分數，那是我省下的暑修金" << endl;
            score += 10;
            cout << "new score: " << score << endl; //到時候要跟真正的得分機制合併
            break;
        case 2:
            cout << "積 不 出 來 ಥ_ಥ 分數也 積 不 出 來" << endl;
            score -= 10;
            cout << "new score: " << score << endl; //到時候要跟真正的得分機制合併
            break;
        case 3:
            cout << "考前突然通靈，速度up up" << endl;
            v = v * 2;
            break;
        case 4:
            cout << "唸書唸到ㄎㄧㄤ，逐漸進入夢鄉，速度跟著進度下去啦" << endl;
            v = v / 2;
            break;
        default:
            cout << "nothing happen" << endl; //預防萬一
            break;
    }

    return;

}

void drawRects(sf::RectangleShape** arrRect, int cntRect, sf::RenderWindow &window, int x, int y){
    int lenRect = 20; /*------要改------*/
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
bool show_detectcoin_words(sf::Time detect_time,sf::Time now){
    if( now.asSeconds() - detect_time.asSeconds() < 2){
        return true;
    }else{
        return  false;
    }
}
