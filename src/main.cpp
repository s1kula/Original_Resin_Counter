#include "app-window.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <string>

// int sstoi(slint::SharedString in){
//     int k = 0;
//     for(int i = 0; i < in.)
// }

// void work(auto ui){
//     int r;
//     slint::invoke_from_event_loop([&ui, &r]() {
//         r = ui->get_counter(); 
//     });
//     std::cout << r << std::endl;
//     std::this_thread::sleep_for(
//                 std::chrono::milliseconds(100)
//             );
// }

int main(/*int argc, char **argv*/)
{

    std::string data = std::getenv("HOME");
    data += "/.local/share/OriResCou.data";

    auto ui = AppWindow::create();

    ui->on_edit_resin([&](int plusValue){
        // std::string s = plusValue;
        // int value = std::stoi(s);
        if((ui->get_counter()>=abs(plusValue) && plusValue < 0) || ( plusValue > 0)){
            ui->set_counter(ui->get_counter()+plusValue);
        }
        ui->invoke_edit_time(ui->get_seconds(), ui->get_modsec(), ui->get_counter());
    });

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    

   std::ifstream check(data);

    if (check) {
        check.close();

        std::ifstream dtime(data);

        int sec;
        int modsec;
        int countRes;
        int open;

        dtime >> sec >> modsec >> countRes >> open;

        if(open == 1) return 1;

        std::ofstream checkOpen(data, std::ios::in);
        checkOpen.seekp(-2, std::ios::end);
        checkOpen << 1 << '\n';
        checkOpen.close();

        ui->set_seconds(time);
        ui->set_modsec(modsec);
        int plus = ((((sec-modsec)%480)+(time-sec))/480);
        if(countRes+plus>200){
            plus = 200 - countRes;
            if(plus<0) plus = 0;
        }
        ui->set_counter(countRes+plus);

    } else {
        check.close();
        std::ofstream dtime(data);

        dtime << time << '\n';
        dtime << time % 480 << '\n';
        dtime << 200 << '\n';
        dtime << 1 << '\n';

        ui->set_seconds(time);
        ui->set_modsec(time % 480);
        ui->set_counter(200);
    }

    int sec;
    int modsec;
    int countRes;

    ui->on_edit_time([&](int a, int b, int c){
        sec = a;
        modsec = b;
        countRes = c;
        now = std::chrono::system_clock::now();
        time = std::chrono::system_clock::to_time_t(now);
        int plus = ((((sec-modsec)%480)+(time-sec))/480);
        if(countRes+plus>200){
            plus = 200 - countRes;
            if(plus<0) plus = 0;
        }
        ui->set_counter(countRes+plus);
        if(countRes>=200) ui->set_modsec(time%480);
        ui->set_seconds(time);
        int ost = (200-countRes-1)*480+(480-(sec-modsec)%480);
        if(countRes>=200){ost=0;}
        ui->set_hours(ost/3600);
        ui->set_minutes(ost/60%60);
    });

    ui->invoke_edit_time(ui->get_seconds(), ui->get_modsec(), ui->get_counter());

    ui->run();

    std::ofstream dtime(data);
    dtime << sec << '\n';
    dtime << modsec << '\n';
    dtime << countRes << '\n';
    dtime << 0 << '\n';

}
