#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include<iostream>
using namespace std;

//待改，那個時候沒教template所以我是亂寫的
//template<typename sf::Sprite,typename sf::Sprite>
bool check_collision(const sf::Sprite &a,const sf::Sprite &b){ //碰撞檢測，矩形
    sf::FloatRect box1 = a.getGlobalBounds();
    sf::FloatRect box2 = b.getGlobalBounds();
    return box1.intersects(box2);
}

bool detectWall(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_up(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_down(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_left(int x, int y, sf::Color colorWall, sf::Image maze1);
bool detectWall_right(int x, int y, sf::Color colorWall, sf::Image maze1);
int main()
{
    
    sf::RenderWindow window(sf::VideoMode(2386, 1491), "hello window");
    
    //畫面一要用到的物件(texture, sprite)
    sf::Texture texture1_start;
    if (!texture1_start.loadFromFile("p1_start.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite1_start(texture1_start);
    //sprite1_start.setScale(3, 3);
    
    sf::Texture texture1_startButtom;
    if (!texture1_startButtom.loadFromFile("p1_start1Buttom.png"))
        return EXIT_FAILURE;
    sf::Texture texture1_startButtom_red;
    if (!texture1_startButtom_red.loadFromFile("p1_strat1Buttom_click.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite1_startButtom(texture1_startButtom);
    sprite1_startButtom.setPosition(sf::Vector2f(500.f, 1000.f));
    sprite1_startButtom.setScale(0.1, 0.1);
    
    //畫面二要用到的物件(迷宮、人物、牆壁)
    sf::Texture texture2_maze0;
    if (!texture2_maze0.loadFromFile("p2_mikong.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_maze1(texture2_maze0);
    //sprite2_maze1.setScale(1.3, 1.3);
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
    sprite2_chart1.setPosition(sf::Vector2f(4.f, 800.f));//set pic2
    sprite2_chart1.setScale(0.05, 0.05);
    int X = sprite2_chart1.getPosition().x;
    int Y = sprite2_chart1.getPosition().y;
    
    //採集牆壁顏色，maze1是採集地圖中的牆壁
    //maze2是採集色塊圖案的顏色，但是maze2失敗了，好像取到背景顏色了
    sf::Image maze2;
    if (!maze2.loadFromFile("p2_wallColor.png")){
        return EXIT_FAILURE;
    }
    sf::Color colorWall2 = maze2.getPixel(400, 400);
    sf::Image maze1;
    if (!maze1.loadFromFile("p2_mikong.jpg")){
        return EXIT_FAILURE;
    }
    sf::Color colorWall = maze1.getPixel(279, 240);
    
    if(colorWall == colorWall2){
        cout << "there are same" << endl;
    }
    
    //target(cheese)
    //先借用char1的圖片當終點，最後把圖片設為透明
    sf::Sprite sprite2_target(texture2_chart1_right);
    //sprite2_target.setColor(sf::Color::Transparent);
    sprite2_target.setScale(0.1, 0.1);
    sprite2_target.setPosition(sf::Vector2f(2266.f, 70.f));
    
    //碰到target後的恭喜畫面
    sf::Texture texture2_congra;
    if (!texture2_congra.loadFromFile("p2_congra.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_congra(texture2_congra);
    sprite2_congra.setScale(2, 2);
    
    sf::Texture texture2_cheeseButtom;
    if (!texture2_cheeseButtom.loadFromFile("p2_cheeseButtom.png"))
        return EXIT_FAILURE;
    sf::Texture texture2_cheeseButtom_red;
    if (!texture2_cheeseButtom_red.loadFromFile("p2_cheeseButtom_red.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite2_cheeseButtom(texture2_cheeseButtom);
    sprite2_cheeseButtom.setScale(0.6, 0.6);
    sprite2_cheeseButtom.setPosition(sf::Vector2f(1000.f, 600.f));

    //計時器設定
    sf::Color origin;
    sf::Clock time;
    sf::Time elapsed1 ;
    sf::Time elapsed2 ;
    int limit = 20;


    bool first = true;
    bool second = false;
    int v = 1;
    float dv = 0.1;//沒用了
    //以下為操作
    while (window.isOpen())
    {
        //sprite2.setColor(sf::Color::Green);//
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered){
                if (event.text.unicode < 128)
                    std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
            }
            //滑鼠移動的座標偵測
            
            if (event.type == sf::Event::MouseMoved){
                cout << "mouse (x: " << event.mouseMove.x << ", y: " << event.mouseMove.y << ")" << endl;
            }
            
            
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
                window.setTitle("play window");
                //預備讓人在正確的地方開始
                sprite2_chart1.setPosition(sf::Vector2f(4.f, 800.f));
                
                //調整視窗大小會讓圖片物件失真
                //要配合圖片調整(window.setSize 跟 spriteName.setScale要一起調整)
                //window.setSize(sf::Vector2u(1680, 1065));


                elapsed1 = time.getElapsedTime();//得到起始時間
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
            
            if(check_collision(sprite2_chart1, sprite2_target)){
                //window.clear();
                window.draw(sprite2_congra);
                window.draw(sprite2_cheeseButtom);
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if(sprite2_cheeseButtom.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                    sprite2_cheeseButtom.setTexture(texture2_cheeseButtom_red);
                }else{
                    sprite2_cheeseButtom.setTexture(texture2_cheeseButtom);
                }
                if(sprite2_cheeseButtom.getGlobalBounds().contains(mousePos.x, mousePos.y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    second = false;
                    first = true;
                    window.clear();
                    window.setTitle("hello window");
                    continue;
                }
            }
            
            //一直cout chart1移動後座標
            int x = sprite2_chart1.getPosition().x;
            int y = sprite2_chart1.getPosition().y;
            if((x != X) || (y != Y)){
                cout << "(" << x  << ", " << y << ")" << endl;
                X = x;
                Y = y;
            }
            
            //detect wall(x, y, colorWall, maze1)自動偵測顏色，與牆壁顏色對照
            if (detectWall(x, y, colorWall, maze1)) {
                cout << "detect wall" << endl;
            }
            
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                sprite2_chart1.setTexture(texture2_chart1_up);
                if((y <= 2) || detectWall_up(x, y, colorWall, maze1)){
                    sprite2_maze1.setColor(sf::Color::Red);
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(0, -v);//move chart1
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                sprite2_chart1.setTexture(texture2_chart1_down);
                if((y >= 1400) || detectWall_down(x, y, colorWall, maze1)){
                    sprite2_maze1.setColor(sf::Color::Red);
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(0, v);//move chart1
                }
                
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                sprite2_chart1.setTexture(texture2_chart1_left);
                if((x <= 2) || detectWall_left(x, y, colorWall, maze1)){
                    sprite2_maze1.setColor(sf::Color::Red);
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(-v, 0);//move chart1
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                sprite2_chart1.setTexture(texture2_chart1_right);
                if((x >= 2380) || detectWall_right(x, y, colorWall, maze1)){
                    sprite2_maze1.setColor(sf::Color::Red);
                }else{
                    sprite2_maze1.setColor(colorOrigin);
                    sprite2_chart1.move(v, 0);//move chart1
                }
            }
            
            
        }
        elapsed2 = time.getElapsedTime();
        if(secon){
            cout << (int)(limit-(elapsed2.asSeconds()-elapsed1.asSeconds()))<< endl;
        }
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
    //寬度19, 長度19, 保險起見先弄20
    int w = 20, l = 20;
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
    int w = 20, l = 20;
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
    int w = 20, l = 20;
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
    int w = 20, l = 20;
    sf::Color color_at_airplane = maze1.getPixel(x, y);
    for(int j = 0; j < l; j++){
        color_at_airplane = maze1.getPixel(x + w, y + j); //right
        if (color_at_airplane == colorWall) {
            return true;
        }
    }
    return false;
}
